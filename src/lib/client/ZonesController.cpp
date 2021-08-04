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
 *      observation and mutation of a collection of HLX zones.
 *
 */

#include "ZonesController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Client/EqualizerPresetsController.hpp>
#include <OpenHLX/Client/SourcesController.hpp>
#include <OpenHLX/Model/BalanceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
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
ZonesController :: ZonesController(void) :
    Client::ControllerBasis(),
    Common::ZonesControllerBasis(),
    Client::ZonesControllerBasis(),
    mZonesDidRefreshCount(0)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesController :: ~ZonesController(void)
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
ZonesController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kBalanceResponse,
            ZonesController::BalanceNotificationReceivedHandler
        },

        {
            kEqualizerBandResponse,
            ZonesController::EqualizerBandNotificationReceivedHandler
        },

        {
            kEqualizerPresetResponse,
            ZonesController::EqualizerPresetNotificationReceivedHandler
        },

        {
            kHighpassCrossoverResponse,
            ZonesController::HighpassCrossoverNotificationReceivedHandler
        },

        {
            kLowpassCrossoverResponse,
            ZonesController::LowpassCrossoverNotificationReceivedHandler
        },

        {
            kMuteResponse,
            ZonesController::MuteNotificationReceivedHandler
        },

        {
            kNameResponse,
            ZonesController::NameNotificationReceivedHandler
        },

        {
            kSoundModeResponse,
            ZonesController::SoundModeNotificationReceivedHandler
        },

        {
            kSourceResponse,
            ZonesController::SourceNotificationReceivedHandler
        },

        {
            kSourceAllResponse,
            ZonesController::SourceAllNotificationReceivedHandler
        },

        {
            kToneResponse,
            ZonesController::ToneNotificationReceivedHandler
        },

        {
            kVolumeResponse,
            ZonesController::VolumeNotificationReceivedHandler
        },

        {
            kVolumeAllResponse,
            ZonesController::VolumeAllNotificationReceivedHandler
        },

        {
            kVolumeFixedResponse,
            ZonesController::VolumeFixedNotificationReceivedHandler
        }
    };
    static constexpr size_t                lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                                 lRetval = kStatus_Success;

    lRetval = Client::ControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
                                                              &lNotificationHandlers[lNotificationHandlerCount],
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
ZonesController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Client::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init(kZonesMax);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
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
 *  Presently, this controller does so by executing a "query zone
 *  [QOn]" command with the peer server for each zone.
 *
 *  @param[in]  aTimeout  The timeout to use for the refresh operation
 *                        with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mZonesDidRefreshCount = 0;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query zone request for each zone.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Query the current state of all zones.
 *
 *  This queries the current state of all HLX server zones.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (IdentifierType lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        lRetval = Query(lZoneIdentifier);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of a zone.
 *
 *  This queries the current state of the specified HLX server
 *  zone.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the identifier
 *                               of the zone to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: Query(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::Query>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::QueryCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX zones.
 *
 *  @param[out]  aZones  The maximum number of HLX zones on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
ZonesController :: GetZonesMax(size_t &aZones) const
{
    Status  lRetval = kStatus_Success;

    aZones = kZonesMax;

    return (lRetval);
}

/**
 *  @brief
 *    Get the zone model associated with specified zone
 *    identifier.
 *
 *  @param[in]   aIdentifier  An immutable reference to the zone
 *                            model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the zone model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is smaller
 *                            or larger than supported.
 *
 */
Status
ZonesController :: GetZone(const IdentifierType &aIdentifier, const ZoneModel *&aModel) const
{
    Status  lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.GetZone(aIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the zone identifier with the specified name.
 *
 *  This attempts to lookup the zone identifier for the zone
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    zone to find an identifier
 *                                    for.
 *  @param[out]  aZoneIdentifier      A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No zone could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
ZonesController :: LookupIdentifier(const char *aName, IdentifierType &aZoneIdentifier) const
{
    const ZoneModel * lZoneModel;
    Status            lRetval;


    lRetval = mZones.GetZone(aName, lZoneModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lZoneModel->GetIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Balance Mutator Commands

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the left channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the left channel on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
ZonesController :: IncreaseBalanceLeft(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseBalanceLeft());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseBalanceLeft>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetBalanceCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the right channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the right channel on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
ZonesController :: IncreaseBalanceRight(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseBalanceRight());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseBalanceRight>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetBalanceCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}


/**
 *  @brief
 *    Set the zone stereophonic channel balance state.
 *
 *  This attempts to set the zone stereophonic channel balance state
 *  on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the
 *                                stereophonic channel balance state.
 *  @param[in]  aBalance          An immutable reference to the
 *                                balance state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
ZonesController :: SetBalance(const IdentifierType &aZoneIdentifier,
                              const BalanceModel::BalanceType &aBalance)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetBalance());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetBalance>(lCommand)->Init(aZoneIdentifier, aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetBalanceCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Equalizer Band Mutator Commands

/**
 *  @brief
 *    Set the level of an equalizer band of an zone equalizer.
 *
 *  This attempts to set the level of the specified equalizer band of
 *  the specified zone equalizer to the provided value on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetEqualizerBand(const IdentifierType &aZoneIdentifier,
                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                    const EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetEqualizerBand>(lCommand)->Init(aZoneIdentifier,
                                                                                         aEqualizerBandIdentifier,
                                                                                         aEqualizerBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetEqualizerBandCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the level of an equalizer band of an zone equalizer
 *    by one (1) unit.
 *
 *  This attempts to increase the level of the specified equalizer
 *  band of the specified zone equalizer by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: IncreaseEqualizerBand(const IdentifierType &aZoneIdentifier,
                                         const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseEqualizerBand>(lCommand)->Init(aZoneIdentifier,
                                                                                              aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetEqualizerBandCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the level of an equalizer band of an zone equalizer
 *    by one (1) unit.
 *
 *  This attempts to decrease the level of the specified equalizer
 *  band of the specified zone equalizer by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: DecreaseEqualizerBand(const IdentifierType &aZoneIdentifier,
                                         const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::DecreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::DecreaseEqualizerBand>(lCommand)->Init(aZoneIdentifier,
                                                                                              aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetEqualizerBandCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Equalizer Preset Mutator Commands


/**
 *  @brief
 *    Set the zone equalizer preset state.
 *
 *  This attempts to set the zone equalizer preset state on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          for the zone for which
 *                                          to set the equalizer
 *                                          preset.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone or equalizer preset
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
ZonesController :: SetEqualizerPreset(const IdentifierType &aZoneIdentifier,
                                      const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerPresetsController::ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetEqualizerPreset());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetEqualizerPreset>(lCommand)->Init(aZoneIdentifier, aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetEqualizerPresetCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Tone Mutator Commands

/**
 *  @brief
 *    Decrease the bass level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the bass level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: DecreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::DecreaseBass());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::DecreaseBass>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetToneCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the treble level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the treble level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: DecreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::DecreaseTreble());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::DecreaseTreble>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetToneCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the bass level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the bass level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: IncreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseBass());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseBass>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetToneCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the treble level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the treble level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: IncreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseTreble());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseTreble>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetToneCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the bass level of the zone tone equalizer to the specified
 *    level.
 *
 *  This attempts to set the bass level of the zone tone equalizer to
 *  the specified level on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the bass level.
 *  @param[in]  aBass                     An immutable reference to the
 *                                        bass level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetBass(const ZoneModel::IdentifierType &aZoneIdentifier,
                           const ToneModel::LevelType &aBass)
{
    const ZoneModel *     lZoneModel;
    ToneModel::LevelType  lTreble;
    Status                lRetval;


    // The HLX server controller only supports setting tone as one
    // combined request including both bass and treble. Consequently,
    // we need to get the current treble value and send it along with
    // the requested bass value.

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // If the zone for this HLX server controller is not in the tone
    // sound mode, then the initial tone values for both bass and
    // treble are unknown and the following GetTreble call will fail
    // with 'kError_NotInitialized'.
    //
    // The best client course of action would be to ensure that the
    // zone is first in tone sound mode before attempting to set the
    // bass or treble by value. However, since the HLX controller does
    // support setting the tone outside of that mode (which will force
    // the controller into that mode), the best we can do is to
    // default the treble to some sane value and proceed.

    lRetval = lZoneModel->GetTreble(lTreble);
    nlREQUIRE(lRetval >= kStatus_Success || lRetval == kError_NotInitialized, done);

    if (lRetval == kError_NotInitialized)
    {
        lTreble = ToneModel::kLevelFlat;
    }

    // Send the combined requested and locally-retrieved tone request.

    lRetval = SetTone(aZoneIdentifier, aBass, lTreble);
    nlREQUIRE(lRetval >= kStatus_Success, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the treble level of the zone tone equalizer to the specified
 *    level.
 *
 *  This attempts to set the treble level of the zone tone equalizer to
 *  the specified level on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the treble level.
 *  @param[in]  aTreble                   An immutable reference to the
 *                                        treble level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetTreble(const ZoneModel::IdentifierType &aZoneIdentifier, const ToneModel::LevelType &aTreble)
{
    const ZoneModel *     lZoneModel;
    ToneModel::LevelType  lBass;
    Status                lRetval;


    // The HLX server controller only supports setting tone as one
    // combined request including both bass and treble. Consequently,
    // we need to get the current bass value and send it along with
    // the requested treble value.

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // If the zone for this HLX server controller is not in the tone
    // sound mode, then the initial tone values for both bass and
    // treble are unknown and the following GetBass call will fail
    // with 'kError_NotInitialized'.
    //
    // The best client course of action would be to ensure that the
    // zone is first in tone sound mode before attempting to set the
    // bass or treble by value. However, since the HLX controller does
    // support setting the tone outside of that mode (which will force
    // the controller into that mode), the best we can do is to
    // default the bass to some sane value and proceed.

    lRetval = lZoneModel->GetBass(lBass);
    nlREQUIRE(lRetval >= kStatus_Success || lRetval == kError_NotInitialized, done);

    if (lRetval == kError_NotInitialized)
    {
        lBass = ToneModel::kLevelFlat;
    }

    // Send the combined requested and locally-retrieved tone request.

    lRetval = SetTone(aZoneIdentifier, lBass, aTreble);
    nlREQUIRE(lRetval >= kStatus_Success, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the bass and treble levels of the zone tone equalizer to the
 *    specified levels.
 *
 *  This attempts to set the bass and treble levels of the zone tone
 *  equalizer to the specified levels on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the bass and
 *                                        treble levels.
 *  @param[in]  aBass                     An immutable reference to the
 *                                        bass level to set.
 *  @param[in]  aTreble                   An immutable reference to the
 *                                        treble level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetTone(const ZoneModel::IdentifierType &aZoneIdentifier,
                           const ToneModel::LevelType &aBass,
                           const ToneModel::LevelType &aTreble)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lCommand.reset(new Command::Zones::SetTone());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetTone>(lCommand)->Init(aZoneIdentifier, aBass, aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetToneCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: High- and Low-pass Crossover Frequency Mutator Commands

/**
 *  @brief
 *    Set the crossover frequency of the zone highpass filter to the
 *    specified frequency.
 *
 *  This attempts to set the crossover frequency of the zone highpass
 *  filter to the specified frequency on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the highpass
 *                                        crossover frequency.
 *  @param[in]  aHighpassFrequency        An immutable reference to the
 *                                        highpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetHighpassCrossover(const IdentifierType &aZoneIdentifier,
                                        const CrossoverModel::FrequencyType &aHighpassFrequency)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetHighpassCrossover());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetHighpassCrossover>(lCommand)->Init(aZoneIdentifier, aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetHighpassCrossoverCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the crossover frequency of the zone lowpass filter to the
 *    specified frequency.
 *
 *  This attempts to set the crossover frequency of the zone lowpass
 *  filter to the specified frequency on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the lowpass
 *                                        crossover frequency.
 *  @param[in]  aLowpassFrequency         An immutable reference to the
 *                                        lowpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetLowpassCrossover(const IdentifierType &aZoneIdentifier,
                                       const CrossoverModel::FrequencyType &aLowpassFrequency)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetLowpassCrossover());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetLowpassCrossover>(lCommand)->Init(aZoneIdentifier, aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetLowpassCrossoverCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mute Mutator Commands

/**
 *  @brief
 *    Clear (deassert) the zone volume mute state.
 *
 *  This attempts to clear (deassert) the zone volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to clear (deassert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: ClearMute(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::ClearMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::ClearMute>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetMuteCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set (assert) the zone volume mute state.
 *
 *  This attempts to set (assert) the zone volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set (assert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetMute(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetMute>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetMuteCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the zone volume mute state to the specified state.
 *
 *  This attempts to set the zone volume mute state to the specified
 *  state on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                               zone for which to set the
 *                               volume mute state.
 *  @param[in]  aMute            An immutable reference to the
 *                               volume mute state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetMute(const IdentifierType &aZoneIdentifier,
                           const Model::VolumeModel::MuteType &aMute)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ((aMute) ? SetMute(aZoneIdentifier) : ClearMute(aZoneIdentifier));
    nlEXPECT(lRetval >= kStatus_Success, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Toggle (flip) the zone volume mute state.
 *
 *  This attempts to toggle (flip) the zone volume mute state on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to toggle (flip)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: ToggleMute(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::ToggleMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::ToggleMute>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetMuteCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Name Mutator Commands

/**
 *  @brief
 *    Set the zone to the specified name.
 *
 *  This attempts to set the zone with the provided identifier to the
 *  specified name on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                               zone for which to set the
 *                               name.
 *  @param[in]  aName            A pointer to the null-terminated
 *                               C string to set the zone name
 *                               to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
ZonesController :: SetName(const IdentifierType &aZoneIdentifier,
                           const char *aName)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetName());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetName>(lCommand)->Init(aZoneIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetNameCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Sound Mode Mutator Commands

/**
 *  @brief
 *    Set the zone equalizer to the specified sound mode.
 *
 *  This attempts to set the zone equalizer to the specified sound
 *  mode on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the equalizer
 *                                        sound mode.
 *  @param[in]  aSoundMode                An immutable reference to the
 *                                        equalizer sound mode to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesController :: SetSoundMode(const IdentifierType &aZoneIdentifier,
                                const SoundModel::SoundMode &aSoundMode)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetSoundMode());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetSoundMode>(lCommand)->Init(aZoneIdentifier, aSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetSoundModeCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Source Mutator Commands

/**
 *  @brief
 *    Set the zone source (input) state to the specified source.
 *
 *  This attempts to set the zone source (input) state to the
 *  specified source on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone or source (input)
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
ZonesController :: SetSource(const IdentifierType &aZoneIdentifier,
                             const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SourcesController::ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetSource());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetSource>(lCommand)->Init(aZoneIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetSourceCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Volume Mutator Commands

/**
 *  @brief
 *    Set the zone volume level.
 *
 *  This attempts to set the zone volume level on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetVolume(const IdentifierType &aZoneIdentifier,
                             const Model::VolumeModel::LevelType &aLevel)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetVolume>(lCommand)->Init(aZoneIdentifier, aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetVolumeCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone volume level by one (1) unit.
 *
 *  This attempts to increase the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                                zone for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: IncreaseVolume(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::IncreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::IncreaseVolume>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetVolumeCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the zone volume level by one (1) unit.
 *
 *  This attempts to decrease the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: DecreaseVolume(const IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::DecreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::DecreaseVolume>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetVolumeCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the zone volume locked state.
 *
 *  This attempts to set the zone volume locked state to the specified
 *  value on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                locked state.
 *  @param[in]  aLocked           An immutable reference to the volume
 *                                locked state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetVolumeLocked(const IdentifierType &aZoneIdentifier,
                                   const Model::VolumeModel::FixedType &aLocked)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetVolumeFixed());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetVolumeFixed>(lCommand)->Init(aZoneIdentifier, aLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::SetVolumeFixedCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query zone client command response completion
 *    handler.
 *
 *  This handles an asynchronous client command response for the query
 *  zone command request.
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
ZonesController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    IdentifierType                 lZoneIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mZonesDidRefreshCount++;

    if (WasRefreshRequested())
    {
        const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mZonesDidRefreshCount),
                                                                static_cast<uint8_t>(kZonesMax));

        OnIsRefreshing(lPercentComplete);

        if (lPercentComplete == 100)
        {
            OnDidRefresh();
        }
    }

done:
    return;
}

/**
 *  @brief
 *    Asynchronous zone set stereophonic channel balance client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set stereophonic channel balance command request.
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
ZonesController :: SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    BalanceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone equalizer set/decrease/increase equalizer
 *    band level client command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone equalizer set/decrease/increase equalizer band level
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
ZonesController :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    EqualizerBandNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set equalizer preset client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set equalizer preset command request.
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
ZonesController :: SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    EqualizerPresetNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set highpass filter crossover frequency client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set highpass filter crossover frequency command request.
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
ZonesController :: SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    HighpassCrossoverNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set lowpass filter crossover frequency client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set lowpass filter crossover frequency command request.
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
ZonesController :: SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    LowpassCrossoverNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set volume mute client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume mute command request.
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
ZonesController :: SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    MuteNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set name command request.
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
ZonesController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set equalizer sound mode client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set equalizer sound mode command request.
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
ZonesController :: SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SoundModeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set source (input) client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set source (input) command request.
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
ZonesController :: SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SourceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone decrease/increase/set tone equalizer client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone decrease/increase/set tone equalizer command request.
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
ZonesController :: SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    ToneNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume level command request.
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
ZonesController :: SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set volume fixed/locked client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume fixed/locked command request.
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
ZonesController :: SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeFixedNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zones controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client zones controller
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
ZonesController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Zone Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query zone client command response completion
 *    handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query zone command request.
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
ZonesController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set stereophonic channel balance client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set stereophonic channel balance command
 *  request.
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
ZonesController :: SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                             const RegularExpression::Matches &aMatches,
                                             void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBalanceCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone equalizer set/decrease/increase equalizer
 *    band level client command response completion handler
 *    trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone equalizer set/decrease/increase equalizer
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
ZonesController :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerBandCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set equalizer preset client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set equalizer preset command request.
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
ZonesController :: SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerPresetCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set highpass filter crossover frequency client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set highpass filter crossover frequency
 *  command request.
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
ZonesController :: SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetHighpassCrossoverCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set lowpass filter crossover frequency client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set lowpass filter crossover frequency
 *  command request.
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
ZonesController :: SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLowpassCrossoverCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume mute client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume mute command request.
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
ZonesController :: SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetMuteCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set name command request.
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
ZonesController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set equalizer sound mode client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set equalizer sound mode command request.
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
ZonesController :: SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSoundModeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set source (input) client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone` set source (input) mute command request.
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
ZonesController :: SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone decrease/increase/set tone equalizer client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone decrease/increase/set tone equalizer command
 *  request.
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
ZonesController :: SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetToneCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume level command request.
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
ZonesController :: SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume fixed/locked client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume fixed/locked command request.
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
ZonesController :: SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeFixedCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zones controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client zones
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
ZonesController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

/**
 *  @brief
 *    Zone stereophonic channel balance client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone stereophonic channel balance changed notification.
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
 *  @ingroup balance
 *
 */
void
ZonesController :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    BalanceModel::ChannelType              lChannel;
    BalanceModel::BalanceType              lBalance;
    ZoneModel *                            lZoneModel;
    StateChange::ZonesBalanceNotification  lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::BalanceResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Channel

    lChannel = *((const char *)(aBuffer) + aMatches.at(2).rm_so);

    // Match 4/4: Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Adjust the balance from the HLX's L:{80, 0} to {0, 80}:R tagged
    // discontinuous model to a non-tagged, continuous L:{-80, 80}:R
    // model.

    if (lChannel == BalanceModel::kChannelLeft)
    {
        lBalance = -lBalance;
    }

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the balance is unchanged, SetBalance will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetBalance(lBalance);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone equalizer band level client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone equalizer band level changed notification.
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
ZonesController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer,
                                                            const size_t &aSize,
                                                            const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    EqualizerBandModel::IdentifierType               lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType                    lLevel;
    ZoneModel *                                      lZoneModel;
    EqualizerBandModel *                             lEqualizerBandModel;
    StateChange::ZonesEqualizerBandNotification      lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::EqualizerBandResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the level is unchanged, SetLevel will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lEqualizerBandModel->SetLevel(lLevel);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lEqualizerBandIdentifier, lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone equalizer preset client unsolicited notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone equalizer preset changed notification.
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
ZonesController :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    EqualizerPresetModel::IdentifierType             lEqualizerPresetIdentifier;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesEqualizerPresetNotification    lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::EqualizerPresetResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Equalizer Preset Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the equalizer preset is unchanged, SetEqualizerPreset will
    // return kStatus_ValueAlreadySet and there will be no need to
    // send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = lZoneModel->SetEqualizerPreset(lEqualizerPresetIdentifier);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone tone equalizer state client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone tone equalizer state changed notification.
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
ZonesController :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType           lZoneIdentifier;
    ToneModel::LevelType                lBass;
    ToneModel::LevelType                lTreble;
    ZoneModel *                         lZoneModel;
    StateChange::ZonesToneNotification  lStateChangeNotification;
    Status                              lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::ToneResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Bass Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lBass);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Treble Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the tone is unchanged, SetTone will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetTone(lBass, lTreble);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lBass, lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone highpass filter crossover frequency client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone highpass filter crossover frequency changed notification.
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
ZonesController :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    CrossoverModel::FrequencyType                    lHighpassFrequency;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesHighpassCrossoverNotification  lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::HighpassCrossoverResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Highpass Frequency

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the highpass crossover frequency is unchanged,
    // SetHighpassFrequency will return kStatus_ValueAlreadySet and
    // there will be no need to send a state change notification. If
    // we receive kStatus_Success, it is the first time set or a
    // change and state change notification needs to be sent.

    lStatus = lZoneModel->SetHighpassFrequency(lHighpassFrequency);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone lowpass filter crossover frequency client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone lowpass filter crossover frequency changed notification.
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
ZonesController :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    CrossoverModel::FrequencyType                    lLowpassFrequency;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesLowpassCrossoverNotification   lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::LowpassCrossoverResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Lowpass Frequency

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the lowpass crossover frequency is unchanged,
    // SetLowpassFrequency will return kStatus_ValueAlreadySet and
    // there will be no need to send a state change notification. If
    // we receive kStatus_Success, it is the first time set or a
    // change and state change notification needs to be sent.

    lStatus = lZoneModel->SetLowpassFrequency(lLowpassFrequency);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone volume mute state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume mute state changed notification.
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
ZonesController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    const char *                           lMutep;
    IdentifierType                         lZoneIdentifier;
    VolumeModel::MuteType                  lMute;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::MuteResponse::kExpectedMatches, done);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleMuteChange(lZoneIdentifier, lMute);

done:
    return;
}

/**
 *  @brief
 *    Zone name changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone name changed notification.
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
ZonesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    const char *                           lName;
    size_t                                 lNameSize;
    ZoneModel *                            lZoneModel;
    StateChange::ZonesNameNotification     lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::NameResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone equalizer sound mode changed client unsolicited
 *    notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone equalizer sound mode changed notification.
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
ZonesController :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                           lZoneIdentifier;
    SoundModel::SoundMode                    lSoundMode;
    ZoneModel *                              lZoneModel;
    StateChange::ZonesSoundModeNotification  lStateChangeNotification;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SoundModeResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Sound Mode

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the sound mode is unchanged, SetSoundMode will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetSoundMode(lSoundMode);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

/**
 *  @brief
 *    Zone source (input) changed client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification
 *  for the zone source (input) changed notification.
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
ZonesController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SourceResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleSourceChange(lZoneIdentifier, lSourceIdentifier);

done:
    return;
}

/**
 *  @brief
 *    All zones source (input) changed client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  the all zones source (input) changed notification.
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
ZonesController :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SourceAllResponse::kExpectedMatches, done);

    // Match 2/2: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        HandleSourceChange(lZoneIdentifier, lSourceIdentifier);
    }

done:
    return;
}

/**
 *  @brief
 *    Zone volume level state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume level state changed notification.
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
ZonesController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    VolumeModel::LevelType                lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleVolumeChange(lZoneIdentifier, lVolume);

done:
    return;
}

/**
 *  @brief
 *    All zones volume level state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the all zones volume level state changed notification.
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
ZonesController :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lZoneIdentifier;
    VolumeModel::LevelType                lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeAllResponse::kExpectedMatches, done);


    // Match 2/2: Volume

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        HandleVolumeChange(lZoneIdentifier, lVolume);
    }

done:
    return;
}

/**
 *  @brief
 *    Zone volume fixed/locked state changed client unsolicited
 *    notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume fixed/locked state changed notification.
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
ZonesController :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                              lZoneIdentifier;
    VolumeModel::FixedType                      lLocked;
    ZoneModel *                                 lZoneModel;
    StateChange::ZonesVolumeLockedNotification  lStateChangeNotification;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeFixedResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Fixed

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the volume lock (fixed) is unchanged, SetVolumeFixed will
    // return kStatus_ValueAlreadySet and there will be no need to
    // send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = lZoneModel->SetVolumeFixed(lLocked);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Zone stereophonic channel balance changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone stereophonic channel balance changed
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
ZonesController :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->BalanceNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer band level changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer band level changed
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
ZonesController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->EqualizerBandNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer preset changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer preset changed notification.
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
ZonesController :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->EqualizerPresetNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone tone equalizer state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone tone equalizer state changed
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
ZonesController :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToneNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone highpass filter crossover frequency changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone highpass filter crossover frequency
 *  changed notification.
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
ZonesController :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->HighpassCrossoverNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone lowpass filter crossover frequency changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone lowpass filter crossover frequency
 *  changed notification.
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
ZonesController :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->LowpassCrossoverNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume mute state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume mute state changed notification.
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
ZonesController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone name changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone name changed notification.
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
ZonesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer sound mode changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer sound mode changed
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
ZonesController :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SoundModeNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone source (input) changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone source (input) changed notification.
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
ZonesController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SourceNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    All zones source (input) changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the all zones source (input) changed
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
ZonesController :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SourceAllNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume level state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume level state changed notification.
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
ZonesController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    All zones volume level state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the all zones volume level state changed
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
ZonesController :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeAllNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume fixed/locked state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume fixed/locked state changed
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
ZonesController :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeFixedNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

void
ZonesController :: HandleMuteChange(const IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesMuteNotification     lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the mute status is unchanged, SetMute will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetMute(aMute);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aMute);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

void
ZonesController :: HandleVolumeChange(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesVolumeNotification   lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the volume is unchanged, SetVolume will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetVolume(aVolume);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

void
ZonesController :: HandleSourceChange(const IdentifierType &aZoneIdentifier, const SourceModel::IdentifierType &aSourceIdentifier)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesSourceNotification   lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the source is unchanged, SetSource will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetSource(aSourceIdentifier);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

}; // namespace Client

}; // namespace HLX
