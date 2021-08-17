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

#include "InfraredController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/InfraredControllerCommands.hpp>
#include <OpenHLX/Client/InfraredStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


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
InfraredController :: InfraredController(void) :
    Proxy::ControllerBasis(),
    Common::InfraredControllerBasis(),
    Client::InfraredControllerBasis(),
    Server::InfraredControllerBasis(Common::InfraredControllerBasis::mInfraredModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
InfraredController :: ~InfraredController(void)
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
InfraredController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kDisabledResponse,
            InfraredController::DisabledNotificationReceivedHandler
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
InfraredController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            InfraredController::QueryRequestReceivedHandler
        },

        {
            kSetDisabledRequest,
            InfraredController::SetDisabledRequestReceivedHandler
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
InfraredController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::InfraredControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::InfraredControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::InfraredControllerBasis::Init();
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
 *  Presently, this controller does so by executing a "query infrared
 *  disabled/locked [QIRL]" command with the peer server.
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
InfraredController :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query infrared disabled/locked request.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
InfraredController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status                                lRetval = kStatus_Success;


    (void)aConnection;

    lRetval = HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Observer Methods

/**
 *  @brief
 *    Query the infrared disabled/locked state.
 *
 *  This queries the current HLX server infrared disabled/locked state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
InfraredController :: Query(void)
{
    Client::Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;

    lCommand.reset(new Client::Command::Infrared::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Infrared::Query>(lCommand)->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          InfraredController::QueryCompleteHandler,
                          InfraredController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query infrared disabled/locked client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the query
 *  infrared disabled/locked command request.
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
InfraredController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();


    /*
     * There is a bug in either the documentation or in the implementation
     * of the HLX such that the response to [QIRL] is not "(IRL#)(QIRL)"
     * but rather just "(IRL#)", the same response as to a disabled command.
     */
    DisabledNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);

    MaybeUpdateRefreshIfRefreshWasRequested();

    return;
}

/**
 *  @brief
 *    Asynchronous infrared set disabled/locked client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  infrared set disabled/locked command request.
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
InfraredController :: SetDisabledCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    DisabledNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous infrared controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client infrared controller
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
InfraredController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Infrared Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query infrared disabled/locked client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query infrared disabled/locked command request.
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
InfraredController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous infrared set disabled/locked client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the infrared set disabled/locked command request.
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
InfraredController :: SetDisabledCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetDisabledCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous infrared controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client infrared
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
InfraredController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Infrared disabled/locked changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the infrared disabled/locked changed notification.
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
InfraredController :: DisabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    InfraredModel::DisabledType                    lDisabled;
    StateChange::InfraredDisabledNotification      lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Infrared::DisabledResponse::kExpectedMatches, done);

    // Match 2/2: Brightness

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the disable state is unchanged, SetDisabled will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = GetModel().SetDisabled(lDisabled);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Infrared disabled/locked changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the infrared disabled/locked changed notification.
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
InfraredController :: DisabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->DisabledNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers


void InfraredController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Infrared::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleQueryReceived(lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }
    else if (lStatus == kError_NotInitialized)
    {
        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          kQueryResponse,
                                          InfraredController::QueryCompleteHandler,
                                          InfraredController::CommandErrorHandler,
                                          InfraredController::QueryRequestReceivedHandler,
                                          this);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }

 exit:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }
}

void InfraredController :: SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    InfraredModel::DisabledType                  lDisabled;
    Server::Command::Infrared::DisabledResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Infrared::SetDisabledRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Disabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetDisabled(lDisabled);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleDisabledResponse(lDisabled, lResponseBuffer);
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

void InfraredController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void InfraredController :: SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetDisabledRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Proxy Handlers

// MARK: Proxy Handler Trampolines

// MARK: Server-facing Client Implementation

// MARK: Client-facing Server Implementation

}; // namespace Proxy

}; // namespace HLX
