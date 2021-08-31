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
    Common::ZonesControllerBasis(),
    Client::ZonesControllerBasis(Common::ZonesControllerBasis::mZones,
                                 Common::ZonesControllerBasis::kZonesMax)
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
ZonesController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::ZonesControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods


Status
ZonesController :: QueryMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                                lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::QueryMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::QueryMute>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::ZonesControllerBasis::SetMuteCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ZonesController :: QuerySource(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                                lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::QuerySource());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::QuerySource>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::ZonesControllerBasis::SetSourceCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ZonesController :: QueryVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                                lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::QueryVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::QueryVolume>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
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
                          Client::ZonesControllerBasis::SetBalanceCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetBalanceCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetBalanceCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetEqualizerPresetCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetToneCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetToneCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetToneCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetToneCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetToneCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetHighpassCrossoverCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetLowpassCrossoverCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetMuteCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetMuteCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetMuteCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetNameCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetSoundModeCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetSourceCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set all zones source (input) state to the specified source.
 *
 *  This attempts to set all zone source (input) state to the
 *  specified source on the peer HLX server controller.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the source (input) identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
ZonesController :: SetSourceAll(const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = SourcesController::ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::SetSourceAll());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetSourceAll>(lCommand)->Init(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::ZonesControllerBasis::SetSourceAllCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set all zones volume level.
 *
 *  This attempts to set all zone volume level on the peer HLX server
 *  controller.
 *
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetVolumeAll(const Model::VolumeModel::LevelType &aLevel)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lCommand.reset(new Command::Zones::SetVolumeAll());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::SetVolumeAll>(lCommand)->Init(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::ZonesControllerBasis::SetVolumeAllCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
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
                          Client::ZonesControllerBasis::SetVolumeFixedCompleteHandler,
                          Client::ZonesControllerBasis::CommandErrorHandler,
                          static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
