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
 *      This file implements an object for...
 *
 */

#include "EqualizerPresetsControllerBasis.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
#include <OpenHLX/Common/EqualizerPresetsControllerBasis.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

// Notification response data

Command::EqualizerPresets::EqualizerBandResponse  EqualizerPresetsControllerBasis::kEqualizerBandResponse;
Command::EqualizerPresets::NameResponse           EqualizerPresetsControllerBasis::kNameResponse;
Command::EqualizerPresets::QueryResponse          EqualizerPresetsControllerBasis::kQueryResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerPresetsControllerBasis :: EqualizerPresetsControllerBasis(Model::EqualizerPresetsModel &aEqualizerPresetsModel,
                                                                   const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetsMax) :
    Client::ObjectControllerBasis(),
    mEqualizerPresetsModel(aEqualizerPresetsModel),
    mEqualizerPresetsMax(aEqualizerPresetsMax),
    mEqualizerPresetsDidRefreshCount(0)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
EqualizerPresetsControllerBasis :: ~EqualizerPresetsControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

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
EqualizerPresetsControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status          lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager, aTimeout);
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
 *  Presently, this controller does so by executing a "query equalizer
 *  preset [QEPn]" command with the peer server for each equalizer preset.
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
EqualizerPresetsControllerBasis :: Refresh(const Timeout &aTimeout)
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

// MARK: Implementation

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
EqualizerPresetsControllerBasis :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kEqualizerBandResponse,
            EqualizerPresetsControllerBasis::EqualizerBandNotificationReceivedHandler
        },

        {
            kNameResponse,
            EqualizerPresetsControllerBasis::NameNotificationReceivedHandler
        }
    };
    static constexpr size_t  lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Client::ObjectControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
                                                                    &lNotificationHandlers[lNotificationHandlerCount],
                                                                    this,
                                                                    aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Initialize client command response regular expression patterns.
 *
 *  This initializes solicited and unsolicited client command
 *  responses that this controller would like to register to handle.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsControllerBasis :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response regular expression pattern data.

    lRetval = kEqualizerBandResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kNameResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

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
EqualizerPresetsControllerBasis :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (auto lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin; lEqualizerPresetIdentifier <= mEqualizerPresetsMax; lEqualizerPresetIdentifier++)
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
EqualizerPresetsControllerBasis :: Query(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = Common::EqualizerPresetsControllerBasis::ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::Query>(lCommand)->Init(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsControllerBasis::QueryCompleteHandler,
                          EqualizerPresetsControllerBasis::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Completion Handlers

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
EqualizerPresetsControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis *        lResponse = aExchange->GetResponse();
    const size_t                          lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                       lBuffer = lResponse->GetBuffer()->GetHead();
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mEqualizerPresetsDidRefreshCount++;

    MaybeUpdateRefreshIfRefreshWasRequested(static_cast<uint8_t>(mEqualizerPresetsDidRefreshCount),
                                            static_cast<uint8_t>(mEqualizerPresetsMax));

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
EqualizerPresetsControllerBasis :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
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
EqualizerPresetsControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
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
EqualizerPresetsControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Equalizer Preset Command", aError);
}

// MARK: Command Completion Handler Trampolines

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
EqualizerPresetsControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

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
EqualizerPresetsControllerBasis :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

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
EqualizerPresetsControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

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
EqualizerPresetsControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

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
EqualizerPresetsControllerBasis :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer,
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

    nlREQUIRE(aMatches.size() == Command::EqualizerPresets::EqualizerBandResponse::kExpectedMatches, done);

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

    lStatus = mEqualizerPresetsModel.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
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
EqualizerPresetsControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    EqualizerPresetModel::IdentifierType             lEqualizerPresetIdentifier;
    const char *                                     lName;
    size_t                                           lNameSize;
    EqualizerPresetModel *                           lEqualizerPresetModel;
    StateChange::EqualizerPresetsNameNotification    lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::EqualizerPresets::NameResponse::kExpectedMatches, done);

    // Match 2/3: Equalizer Preset Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mEqualizerPresetsModel.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
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

// MARK: Unsolicited Notification Handler Trampolines

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
EqualizerPresetsControllerBasis :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

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
EqualizerPresetsControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsControllerBasis *lController = static_cast<EqualizerPresetsControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
