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

#include "EqualizerPresetsController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
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
EqualizerPresetsController :: EqualizerPresetsController(void) :
    Proxy::ControllerBasis(),
    Common::EqualizerPresetsControllerBasis(),
    Client::EqualizerPresetsControllerBasis(),
    Server::EqualizerPresetsControllerBasis(Common::EqualizerPresetsControllerBasis::mEqualizerPresets,
                                            Common::EqualizerPresetsControllerBasis::kEqualizerPresetsMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
EqualizerPresetsController :: ~EqualizerPresetsController(void)
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
EqualizerPresetsController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kEqualizerBandResponse,
            EqualizerPresetsController::EqualizerBandNotificationReceivedHandler
        },

        {
            kNameResponse,
            EqualizerPresetsController::NameNotificationReceivedHandler
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
EqualizerPresetsController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kDecreaseBandRequest,
            EqualizerPresetsController::DecreaseBandRequestReceivedHandler
        },

        {
            kIncreaseBandRequest,
            EqualizerPresetsController::IncreaseBandRequestReceivedHandler
        },

        {
            kQueryRequest,
            EqualizerPresetsController::QueryRequestReceivedHandler
        },

        {
            kSetBandRequest,
            EqualizerPresetsController::SetBandRequestReceivedHandler
        },

        {
            kSetNameRequest,
            EqualizerPresetsController::SetNameRequestReceivedHandler
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
EqualizerPresetsController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::EqualizerPresetsControllerBasis::Init();
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
 *  Presently, this controller does so by executing a "query
 *  equalizer preset [QEPn]" command with the peer server for each equalizer
 *  preset.
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
EqualizerPresetsController :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mEqualizerPresetsDidRefreshCount = 0;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query equalizer preset request for each equalizer preset.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
EqualizerPresetsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
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
 *    Query the current state of all equalizer presets.
 *
 *  This queries the current state of all HLX server equalizer
 *  presets.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (IdentifierType lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin; lEqualizerPresetIdentifier <= kEqualizerPresetsMax; lEqualizerPresetIdentifier++)
    {
        lRetval = Query(lEqualizerPresetIdentifier);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of an equalizer preset.
 *
 *  This queries the current state of the specified HLX server
 *  equalizer preset.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of
 *                                          the equalizer preset
 *                                          to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset identifier
 *                                        is smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: Query(const IdentifierType &aEqualizerPresetIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                                lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::EqualizerPresets::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::EqualizerPresets::Query>(lCommand)->Init(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::QueryCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query equalizer preset client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the query
 *  equalizer preset command request.
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
EqualizerPresetsController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    IdentifierType                 lEqualizerPresetIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mEqualizerPresetsDidRefreshCount++;

    MaybeUpdateRefreshIfRefreshWasRequested(static_cast<uint8_t>(mEqualizerPresetsDidRefreshCount),
                                            static_cast<uint8_t>(kEqualizerPresetsMax));

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous equalizer preset set/decrease/increase equalizer
 *    band level client command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  equalizer preset set/decrease/increase equalizer band level
 *  command request.
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
EqualizerPresetsController :: SetEqualizerBandCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    EqualizerBandNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous equalizer preset set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  equalizer preset set name command request.
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
EqualizerPresetsController :: SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous equalizer presets controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client equalizer presets controller
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
EqualizerPresetsController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Equalizer Preset Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query equalizer preset client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query equalizer preset command request.
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
EqualizerPresetsController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous equalizer preset set/decrease/increase equalizer
 *    band level client command response completion handler
 *    trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the equalizer preset set/decrease/increase equalizer
 *  band level command request.
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
EqualizerPresetsController :: SetEqualizerBandCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerBandCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous equalizer preset set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the equalizer preset set name command request.
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
EqualizerPresetsController :: SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous equalizer presets controller client command request
 *    error handler trampoline.
 *
 *  This invokes the handler for any asynchronous client equalizer presets
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
EqualizerPresetsController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Equalizer preset equalizer band level client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  an equalizer preset equalizer band level changed notification.
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
EqualizerPresetsController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer,
                                                                       const size_t &aSize,
                                                                       const RegularExpression::Matches &aMatches)
{
    EqualizerPresetModel::IdentifierType             lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType               lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType                    lLevel;
    EqualizerPresetModel *                           lEqualizerPresetModel;
    EqualizerBandModel *                             lEqualizerBandModel;
    StateChange::EqualizerPresetsBandNotification    lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size()== Client::Command::EqualizerPresets::EqualizerBandResponse::kExpectedMatches, done);

    // Match 2/4: Equalizer Preset Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Preset Band Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Preset Band Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mEqualizerPresets.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lEqualizerPresetModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the level is unchanged, SetLevel will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lEqualizerBandModel->SetLevel(lLevel);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lEqualizerPresetIdentifier, lEqualizerBandIdentifier, lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Equalizer preset name changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the equalizer preset name changed notification.
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
EqualizerPresetsController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                   lEqualizerPresetIdentifier;
    const char *                                     lName;
    size_t                                           lNameSize;
    EqualizerPresetModel *                           lEqualizerPresetModel;
    StateChange::EqualizerPresetsNameNotification    lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size()== Client::Command::EqualizerPresets::NameResponse::kExpectedMatches, done);

    // Match 2/3: Equalizer Preset Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mEqualizerPresets.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lEqualizerPresetModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lEqualizerPresetIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Equalizer preset equalizer band level  changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the equalizer preset equalizer band level changed
 *  notification.
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
EqualizerPresetsController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->EqualizerBandNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Equalizer preset name changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the equalizer preset name changed notification.
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
EqualizerPresetsController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

void EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = -1;
    IdentifierType                              lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::DecreaseBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBandReceived(aConnection, lEqualizerPresetIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = 1;
    IdentifierType                              lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::IncreaseBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBandReceived(aConnection, lEqualizerPresetIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                    lEqualizerPresetIdentifier;
    Server::Command::EqualizerPresets::QueryResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer           lResponseBuffer;
    Status                                            lStatus;
    const uint8_t *                                   lBuffer;
    size_t                                            lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lEqualizerPresetIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lStatus = lResponse.Init(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
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
                                          EqualizerPresetsController::QueryCompleteHandler,
                                          EqualizerPresetsController::CommandErrorHandler,
                                          EqualizerPresetsController::QueryRequestReceivedHandler,
                                          this);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }

 exit:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                           lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType       lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType            lBandLevel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::SetBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleSetBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleSetBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level
    //
    // The validity of the equalizer band level will be range checked
    // at HandleSetBandReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBandLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetBandReceived(lEqualizerPresetIdentifier, lEqualizerBandIdentifier, lBandLevel, lResponseBuffer);
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

void EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                   lEqualizerPresetIdentifier;
    const char *                                     lName;
    size_t                                           lNameSize;
    EqualizerPresetModel *                           lEqualizerPresetModel;
    Server::Command::EqualizerPresets::NameResponse  lNameResponse;
    ConnectionBuffer::MutableCountedPointer          lResponseBuffer;
    Status                                           lStatus;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range checked at
    // GetEqualizerPreset below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the equalizer preset model associated with the parsed
    // equalizer preset identifier. This will include a range check on
    // the equalizer preset identifier.

    lStatus = mEqualizerPresets.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lEqualizerPresetModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = lNameResponse.Init(lEqualizerPresetIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
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

void EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Proxy Handlers

// MARK: Proxy Handler Trampolines

// MARK: Server-facing Client Implementation

// MARK: Client-facing Server Implementation

Status EqualizerPresetsController :: GetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel)
{
    EqualizerPresetModel *             lEqualizerPresetModel;
    Status                             lRetval;


    // Attempt to get the equalizer preset model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Attempt to get the equalizer band model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = lEqualizerPresetModel->GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(Server::ConnectionBasis &aConnection,
                                                       const IdentifierType &aEqualizerPresetIdentifier,
                                                       const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                       const Model::EqualizerBandModel::LevelType &aBandAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleAdjustBandReceived(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(const IdentifierType &aEqualizerPresetIdentifier,
                                                       const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                       const Model::EqualizerBandModel::LevelType &aBandAdjustment,
                                                       Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    EqualizerBandModel::LevelType      lBandLevel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Increase or decrease, per the adjustment, the equalizer band
    // level.

    if (aBandAdjustment > 0)
    {
        lRetval = lEqualizerBandModel->IncreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aBandAdjustment < 0)
    {
        lRetval = lEqualizerBandModel->DecreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: HandleSetBandReceived(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lEqualizerBandModel->SetLevel(aBandLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Proxy

}; // namespace HLX
