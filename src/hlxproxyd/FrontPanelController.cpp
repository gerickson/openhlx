/*
 *    Copyright (c) 2021 Grant Erickson
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an "AS
 *    IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *    express or implied.  See the License for the specific language
 *    governing permissions and limitations under the License.
 *
 */

/**
 *    @file
 *      This file implements an object for....
 *
 */

#include "FrontPanelController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/FrontPanelControllerCommands.hpp>
#include <OpenHLX/Client/FrontPanelStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "ProxyCommand.hpp"


using namespace HLX::Client;
using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FrontPanelController :: FrontPanelController(void) :
    Proxy::ControllerBasis(),
    Common::FrontPanelControllerBasis(),
    Client::FrontPanelControllerBasis(),
    Server::FrontPanelControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
FrontPanelController :: ~FrontPanelController(void)
{
    return;
}

/**
 *  @brief
 *    Register or unregister notification handlers.
 *
 *  This registers or unregisters the solicited and unsolicited client
 *  command response notification handlers that this controller is
 *  interested in and will handle on behalf of the client.
 *
 *  @param[in]  aRegister  Indicates whether to register (true) or
 *                         unregister (false) the handlers.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If either of the handler iterators
 *                                        was null.
 *  @retval  -EEXIST                      If a registration already exists.
 *  @retval  -ENOENT                      If there was no such handler
 *                                        registration to unregister.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FrontPanelController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kBrightnessResponse,
            FrontPanelController::BrightnessNotificationReceivedHandler
        },

        {
            kLockedResponse,
            FrontPanelController::LockedNotificationReceivedHandler
        }
    };
    static constexpr size_t  lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Client::ControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
                                                              &lNotificationHandlers[lNotificationHandlerCount],
                                                              this,
                                                              aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
FrontPanelController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            FrontPanelController::QueryRequestReceivedHandler
        },

        {
            kSetBrightnessRequest,
            FrontPanelController::SetBrightnessRequestReceivedHandler
        },

        {
            kSetLockedRequest,
            FrontPanelController::SetLockedRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                         &lRequestHandlers[lRequestHandlerCount],
                                                         this,
                                                         aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FrontPanelController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::FrontPanelControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::FrontPanelControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::FrontPanelControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Proxy::ControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // These MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Refresh or obtain an up-to-date view of the server peer state.
 *
 *  This attempts to refresh or obtain an up-to-date view of the
 *  server peer state with the specified timeout.
 *
 *  Presently, this controller does so by executing a "query front panel
 *  disabled/locked [QFPL]" command with the peer server.
 *
 *  @param[in]  aTimeout  The timeout to use for the refresh operation
 *                        with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FrontPanelController :: Refresh(const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query front panel disabled/locked request.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
FrontPanelController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval = kStatus_Success;


    (void)aConnection;

    lRetval = HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Observer Methods

/**
 *  @brief
 *    Query the front panel disabled/locked state.
 *
 *  This queries the current HLX server front panel disabled/locked state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FrontPanelController :: Query(void)
{
    Client::Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;

    lCommand.reset(new Client::Command::FrontPanel::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::FrontPanel::Query>(lCommand)->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          FrontPanelController::QueryCompleteHandler,
                          FrontPanelController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query front panel disabled/locked client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the query
 *  front panel disabled/locked command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
FrontPanelController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();


    /*
     * There is a bug in either the documentation or in the implementation
     * of the HLX such that the response to [QFPL] is not "(FPL#)(QFPL)"
     * but rather just "(FPL#)", the same response as to a locked command.
     */
    LockedNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);

    MaybeUpdateRefreshIfRefreshWasRequested();

    return;
}

/**
 *  @brief
 *    Asynchronous front panel set brightness client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  front panel set brightness command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
FrontPanelController :: SetBrightnessCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    BrightnessNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous front panel set disabled/locked client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  front panel set disabled/locked command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
FrontPanelController :: SetLockedCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    LockedNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous front panel controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client front panel controller
 *  command request that results in an error response from the HLX
 *  peer server.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command error
 *                         and its original request.
 *  @param[in]  aError     An immutable reference to the error
 *                         associated with the failed client command
 *                         request.
 *
 */
void
FrontPanelController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Front Panel Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query front panel disabled/locked client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query front panel disabled/locked command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous front panel set brightness client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the front panel set brightness command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: SetBrightnessCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBrightnessCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous front panel set disabled/locked client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the front panel set disabled/locked command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: SetLockedCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLockedCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous front panel controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client front panel
 *  controller command request that results in an error response from
 *  the HLX peer server.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             error response and its original
 *                             request.
 *  @param[in]      aError     An immutable reference to the error
 *                             associated with the failed command
 *                             request.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Front panel brightness changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the front panel brightness changed notification.
 *
 *  @param[in]  aBuffer   An immutable pointer to the start of the
 *                        buffer extent containing the notification.
 *  @param[in]  aSize     An immutable reference to the size of the
 *                        buffer extent containing the notification.
 *  @param[in]  aMatches  An immutable reference to the regular
 *                        expression substring matches associated
 *                        with the client command response that
 *                        triggered this handler.
 *
 */
void
FrontPanelController :: BrightnessNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    FrontPanelModel::BrightnessType                lBrightness;
    StateChange::FrontPanelBrightnessNotification  lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::FrontPanel::BrightnessResponse::kExpectedMatches, done);

    // Match 2/2: Brightness

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the brightness is unchanged, SetBrightness will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = mFrontPanelModel.SetBrightness(lBrightness);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Front panel disabled/locked changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the front panel disabled/locked changed notification.
 *
 *  @param[in]  aBuffer   An immutable pointer to the start of the
 *                        buffer extent containing the notification.
 *  @param[in]  aSize     An immutable reference to the size of the
 *                        buffer extent containing the notification.
 *  @param[in]  aMatches  An immutable reference to the regular
 *                        expression substring matches associated
 *                        with the client command response that
 *                        triggered this handler.
 *
 */
void
FrontPanelController :: LockedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    FrontPanelModel::LockedType                    lLocked;
    StateChange::FrontPanelLockedNotification      lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::FrontPanel::LockedResponse::kExpectedMatches, done);

    // Match 2/2: Locked

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the lock state is unchanged, SetLocked will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = mFrontPanelModel.SetLocked(lLocked);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Front panel brightness changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the front panel brightness changed notification.
 *
 *  @param[in]      aBuffer    An immutable pointer to the start of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aSize      An immutable reference to the size of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: BrightnessNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->BrightnessNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Front panel disabled/locked changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the front panel disabled/locked changed notification.
 *
 *  @param[in]      aBuffer    An immutable pointer to the start of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aSize      An immutable reference to the size of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
FrontPanelController :: LockedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->LockedNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

void FrontPanelController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::LockedType              lLocked;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mFrontPanelModel.GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleLockedResponse(lLocked, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void FrontPanelController :: SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::BrightnessType          lBrightness;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::SetBrightnessRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Brightness

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mFrontPanelModel.SetBrightness(lBrightness);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleBrightnessResponse(lBrightness, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void FrontPanelController :: SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::LockedType                  lLocked;
    Server::Command::FrontPanel::LockedResponse  lLockedResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::SetLockedRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Locked

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mFrontPanelModel.SetLocked(lLocked);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleLockedResponse(lLocked, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Client-facing Server Command Request Handler Trampolines

void FrontPanelController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void FrontPanelController :: SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBrightnessRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void FrontPanelController :: SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLockedRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Proxy Handlers

// MARK: Proxy Handler Trampolines

// MARK: Server-facing Client Implementation

// MARK: Client-facing Server Implementation

Status
FrontPanelController :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    FrontPanelModel::BrightnessType          lBrightness;
    FrontPanelModel::LockedType              lLocked;
    Status                                   lRetval;


    lRetval = mFrontPanelModel.GetBrightness(lBrightness);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleBrightnessResponse(lBrightness, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mFrontPanelModel.GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleLockedResponse(lLocked, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status FrontPanelController :: HandleBrightnessResponse(const FrontPanelModel::BrightnessType &aBrightness, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::BrightnessResponse  lBrightnessResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lBrightnessResponse.Init(aBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lBrightnessResponse.GetBuffer();
    lSize = lBrightnessResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

Status FrontPanelController :: HandleLockedResponse(const FrontPanelModel::LockedType &aLocked, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::LockedResponse  lLockedResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    lStatus = lLockedResponse.Init(aLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lLockedResponse.GetBuffer();
    lSize = lLockedResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

}; // namespace Proxy

}; // namespace HLX
