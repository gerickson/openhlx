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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a collection of HLX zones.
 *
 */

#ifndef OPENHLXCLIENTZONESCONTROLLER_HPP
#define OPENHLXCLIENTZONESCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/ZonesControllerBasis.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Common/ZonesControllerBasis.hpp>
#include <OpenHLX/Model/BalanceModel.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX zones.
 *
 *  @ingroup client
 *  @ingroup zone
 *
 */
class ZonesController :
    public Common::ZonesControllerBasis,
    public Client::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status QueryMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status QuerySource(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status QueryVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aZoneIdentifier) const;

    // Mutator Methods

    Common::Status IncreaseBalanceLeft(const IdentifierType &aZoneIdentifier);
    Common::Status IncreaseBalanceRight(const IdentifierType &aZoneIdentifier);
    Common::Status SetBalance(const IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance);

    Common::Status SetEqualizerBand(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);
    Common::Status IncreaseEqualizerBand(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status DecreaseEqualizerBand(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

    Common::Status SetEqualizerPreset(const IdentifierType &aZoneIdentifier, const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    Common::Status DecreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status DecreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status IncreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status IncreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status SetBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass);
    Common::Status SetTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aTreble);

    Common::Status SetHighpassCrossover(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);
    Common::Status SetLowpassCrossover(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);

    Common::Status ClearMute(const IdentifierType &aZoneIdentifier);
    Common::Status SetMute(const IdentifierType &aZoneIdentifier);
    Common::Status SetMute(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status ToggleMute(const IdentifierType &aZoneIdentifier);

    Common::Status SetName(const IdentifierType &aZoneIdentifier, const char *aName);

    Common::Status SetSoundMode(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode);

    Common::Status SetSource(const IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status SetSourceAll(const Model::SourceModel::IdentifierType &aSourceIdentifier);

    Common::Status SetVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status SetVolumeAll(const Model::VolumeModel::LevelType &aLevel);
    Common::Status IncreaseVolume(const IdentifierType &aZoneIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aZoneIdentifier);

    Common::Status SetVolumeLocked(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);

private:
    // Implementation

    Common::Status SetTone(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, const Model::ToneModel::LevelType &aTreble);
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTZONESCONTROLLER_HPP
