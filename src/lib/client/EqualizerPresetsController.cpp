/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a collection of HLX equalizer
 *      presets.
 *
 */

#include "EqualizerPresetsController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
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

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerPresetsController :: EqualizerPresetsController(void) :
    Client::ControllerBasis(),
    Common::EqualizerPresetsControllerBasis(),
    Client::EqualizerPresetsControllerBasis(),
    mEqualizerPresetsDidRefreshCount(0)
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
    static constexpr size_t                lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                                 lRetval = kStatus_Success;

    lRetval = Client::ControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
                                                              &lNotificationHandlers[lNotificationHandlerCount],
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
EqualizerPresetsController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = Common::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(lRegister);
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
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::Query>(lCommand)->Init(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::QueryCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX equalizer presets.
 *
 *  @param[out]  aEqualizerPresets  The maximum number of HLX equalizer
 *                                  presets on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
EqualizerPresetsController :: GetEqualizerPresetsMax(size_t &aEqualizerPresets) const
{
    Status  lRetval = kStatus_Success;

    aEqualizerPresets = kEqualizerPresetsMax;

    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset model associated with specified
 *    equalizer preset identifier.
 *
 *  @param[in]   aIdentifier  An immutable reference to the equalizer
 *                            preset model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the equalizer preset
 *                            model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than supported.
 *
 */
Status
EqualizerPresetsController :: GetEqualizerPreset(const IdentifierType &aIdentifier, const EqualizerPresetModel *&aModel) const
{
    Status  lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerPresets.GetEqualizerPreset(aIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset identifier with the specified name.
 *
 *  This attempts to lookup the equalizer preset identifier for the
 *  equalizer preset with the specified name.
 *
 *  @param[in]   aName                       A pointer to a null-
 *                                           terminated C string of
 *                                           the name of the equalizer
 *                                           preset to find an
 *                                           identifier for.
 *  @param[out]  aEqualizerPresetIdentifier  A reference to storage by
 *                                           which to return the
 *                                           identifier if successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No equalizer preset could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetsController :: LookupIdentifier(const char *aName, IdentifierType &aEqualizerPresetIdentifier) const
{
    const EqualizerPresetModel * lEqualizerPresetModel;
    Status                       lRetval;


    lRetval = mEqualizerPresets.GetEqualizerPreset(aName, lEqualizerPresetModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lEqualizerPresetModel->GetIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Equalizer Band Mutator Commands

/**
 *  @brief
 *    Set the level of an equalizer band of an equalizer preset.
 *
 *  This attempts to set the level of the specified equalizer band of
 *  the specified equalizer preset to the provided value on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aLevel                      An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: SetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                               const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                               const EqualizerBandModel::LevelType &aLevel)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::SetEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::SetEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                    aEqualizerBandIdentifier,
                                                                                                    aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::SetEqualizerBandCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to increase the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: IncreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::IncreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::IncreaseEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                         aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::SetEqualizerBandCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to decrease the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: DecreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::DecreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::DecreaseEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                         aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::SetEqualizerBandCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Name Mutator Commands

/**
 *  @brief
 *    Set the equalizer preset to the specified name.
 *
 *  This attempts to set the equalizer preset with the provided identifier
 *  to the specified name on the peer HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          for which to set the name.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string to set
 *                                          the equalizer preset name
 *                                          to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetsController :: SetName(const IdentifierType &aEqualizerPresetIdentifier,
                                      const char *aName)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::SetName());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::SetName>(lCommand)->Init(aEqualizerPresetIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          EqualizerPresetsController::SetNameCompleteHandler,
                          EqualizerPresetsController::CommandErrorHandler,
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
EqualizerPresetsController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
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
EqualizerPresetsController :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
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
EqualizerPresetsController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
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
EqualizerPresetsController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
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
EqualizerPresetsController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
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
EqualizerPresetsController :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
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
EqualizerPresetsController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
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
EqualizerPresetsController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

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

    nlREQUIRE(aMatches.size() == Command::EqualizerPresets::NameResponse::kExpectedMatches, done);

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

}; // namespace Client

}; // namespace HLX
