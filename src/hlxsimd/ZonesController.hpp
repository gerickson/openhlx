/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines an object for managing the server-side
 *      observation and mutation of a collection of HLX zones.
 *
 */

#ifndef OPENHLXSIMULATORZONESCONTROLLER_HPP
#define OPENHLXSIMULATORZONESCONTROLLER_HPP

#include <stddef.h>

#include <OpenHLX/Common/ZonesControllerBasis.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ToneModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>
#include <OpenHLX/Server/ZonesControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a collection of HLX zones.
 *
 *  @ingroup server
 *  @ingroup zone
 *
 */
class ZonesController :
    public Simulator::ControllerBasis,
    public Server::ContainerControllerBasis,
    public Common::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    Common::Status Init(Server::CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    // Data Model State Mutation Methods
	//
	// Exposed to the parent controller for group zone membership delegation for requests
	// to decrease, increase, or set the volume; set or toggle the muting; or set the source.

    friend class Controller;

    Common::Status AdjustVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Model::VolumeModel::LevelType &aVolume);
    Common::Status SetMute(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status SetVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);
    Common::Status SetSource(const IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status ToggleMute(const IdentifierType &aZoneIdentifier, Model::VolumeModel::MuteType &aMute);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status ZoneEqualizerLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneEqualizerPresetLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneHighpassCrossoverLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneLowpassCrossoverLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneToneLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneSoundModeLoadFromBackupConfiguration(CFDictionaryRef aZoneDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ZoneVolumeLoadFromBackupConfiguration(CFDictionaryRef aZoneDictionary, Model::ZoneModel &aZoneModel);
    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aZonesDictionary, const IdentifierType &aZoneIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aZonesDictionary, const IdentifierType &aZoneIdentifier) const final;
    static Common::Status ZoneEqualizerSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneEqualizerPresetSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneHighpassCrossoverSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneLowpassCrossoverSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneToneSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneSoundModeSaveToBackupConfiguration(CFMutableDictionaryRef aZoneDictionary, const Model::ZoneModel &aZoneModel);
    static Common::Status ZoneVolumeSaveToBackupConfiguration(CFMutableDictionaryRef aZoneDictionary, const Model::ZoneModel &aZoneModel);

    Common::Status GetEqualizerBand(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel);

    Common::Status HandleQueryReceived(const bool &aIsConfiguration, const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer) const;

    Common::Status HandleQueryMuteReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQueryMuteReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleQuerySoundMode(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryEqualizerPreset(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryHighpassCrossover(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryLowpassCrossover(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryTone(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryZoneEqualizer(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleQuerySourceReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQuerySourceReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryVolumeFixed(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustBalanceReceived(const IdentifierType &aZoneIdentifier, const Model::BalanceModel::ChannelType &aChannel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetBalanceReceived(const IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustBassReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment);
    Common::Status HandleAdjustBassReceived(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustTrebleReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment);
    Common::Status HandleAdjustTrebleReceived(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleSetMute(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMuteConditionally(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMuteUnconditionally(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleSetSoundMode(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetSoundModeConditionally(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetSoundModeUnconditionally(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustEqualizerBandReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment);
    Common::Status HandleAdjustEqualizerBandReceived(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetEqualizerBandReceived(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleEqualizerBandResponse(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleEqualizerPresetResponse(const IdentifierType &aZoneIdentifier, const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleHighpassCrossoverResponse(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleLowpassCrossoverResponse(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleMuteResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleSoundModeResponse(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleToneResponse(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, Model::ToneModel::LevelType &aTreble, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeFixedResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aVolumeFixed, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    // Command Request Completion Handlers

    void AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    static Server::Command::Zones::AdjustBalanceRequest          kAdjustBalanceRequest;
    static Server::Command::Zones::DecreaseBassRequest           kDecreaseBassRequest;
    static Server::Command::Zones::IncreaseBassRequest           kIncreaseBassRequest;
    static Server::Command::Zones::DecreaseTrebleRequest         kDecreaseTrebleRequest;
    static Server::Command::Zones::IncreaseTrebleRequest         kIncreaseTrebleRequest;
    static Server::Command::Zones::DecreaseEqualizerBandRequest  kDecreaseEqualizerBandRequest;
    static Server::Command::Zones::IncreaseEqualizerBandRequest  kIncreaseEqualizerBandRequest;
    static Server::Command::Zones::DecreaseVolumeRequest         kDecreaseVolumeRequest;
    static Server::Command::Zones::IncreaseVolumeRequest         kIncreaseVolumeRequest;
    static Server::Command::Zones::MuteRequest                   kMuteRequest;
    static Server::Command::Zones::QueryRequest                  kQueryRequest;
    static Server::Command::Zones::QueryMuteRequest              kQueryMuteRequest;
    static Server::Command::Zones::QuerySourceRequest            kQuerySourceRequest;
    static Server::Command::Zones::QueryVolumeRequest            kQueryVolumeRequest;
    static Server::Command::Zones::SetBalanceRequest             kSetBalanceRequest;
    static Server::Command::Zones::SetEqualizerBandRequest       kSetEqualizerBandRequest;
    static Server::Command::Zones::SetEqualizerPresetRequest     kSetEqualizerPresetRequest;
    static Server::Command::Zones::SetHighpassCrossoverRequest   kSetHighpassCrossoverRequest;
    static Server::Command::Zones::SetLowpassCrossoverRequest    kSetLowpassCrossoverRequest;
    static Server::Command::Zones::SetNameRequest                kSetNameRequest;
    static Server::Command::Zones::SetSoundModeRequest           kSetSoundModeRequest;
    static Server::Command::Zones::SetSourceRequest              kSetSourceRequest;
    static Server::Command::Zones::SetSourceAllRequest           kSetSourceAllRequest;
    static Server::Command::Zones::SetToneRequest                kSetToneRequest;
    static Server::Command::Zones::SetVolumeRequest              kSetVolumeRequest;
    static Server::Command::Zones::SetVolumeAllRequest           kSetVolumeAllRequest;
    static Server::Command::Zones::SetVolumeFixedRequest         kSetVolumeFixedRequest;
    static Server::Command::Zones::ToggleMuteRequest             kToggleMuteRequest;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORZONESCONTROLLER_HPP
