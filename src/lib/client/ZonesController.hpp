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

#ifndef HLXCLIENTZONESCONTROLLER_HPP
#define HLXCLIENTZONESCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/ControllerBasis.hpp>
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
    public Client::ControllerBasis,
    public Common::ZonesControllerBasis,
    public Client::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const IdentifierType &aZoneIdentifier);

    Common::Status GetZonesMax(size_t &aZones) const;

    Common::Status GetZone(const IdentifierType &aIdentifier, const Model::ZoneModel *&aModel) const;

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

    Common::Status SetVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status IncreaseVolume(const IdentifierType &aZoneIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aZoneIdentifier);

    Common::Status SetVolumeLocked(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);

    Common::Status SetTone(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, const Model::ToneModel::LevelType &aTreble);

    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    //!< Ensure the base class can access the Handle*Change methods
    //!< for cross zone-to-group and group-to-zone state synthesis.

    friend class Controller;

    void HandleMuteChange(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    void HandleSourceChange(const IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleVolumeChange(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTZONESCONTROLLER_HPP
