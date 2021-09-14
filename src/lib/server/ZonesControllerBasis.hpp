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
 *      This file defines a derivable object for realizing a HLX
 *      zones controller, in a server.
 *
 */

#ifndef OPENHLXSERVERZONESCONTROLLERBASIS_HPP
#define OPENHLXSERVERZONESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>
#include <OpenHLX/Server/ZonesControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A derivable object for realizing a HLX zones controller, in a
 *    server.
 *
 *  @ingroup server
 *  @ingroup zones
 *
 */
class ZonesControllerBasis :
    public Server::ObjectControllerBasis
{
public:
    virtual ~ZonesControllerBasis(void);

protected:
    ZonesControllerBasis(Model::ZonesModel &aZonesModel,
                         const Model::ZoneModel::IdentifierType &aZonesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status        HandleQueryReceived(const bool &aIsConfiguration,
                                              Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status        HandleQueryReceived(const bool &aIsConfiguration,
                                              const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                              Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status        HandleQueryMuteReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status        HandleQuerySourceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status        HandleQueryVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

protected:
    // Observation (Query) Command Request Class (Static) Handlers

    static Common::Status HandleQueryEqualizerPreset(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                     const Model::ZoneModel &aZoneModel,
                                                     Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryHighpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                       const Model::ZoneModel &aZoneModel,
                                                       Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryLowpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                      const Model::ZoneModel &aZoneModel,
                                                      Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryMuteReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  const Model::ZoneModel &aZoneModel,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQuerySoundMode(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                               const Model::ZoneModel &aZoneModel,
                                               Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQuerySourceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    const Model::ZoneModel &aZoneModel,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryTone(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                          const Model::ZoneModel &aZoneModel,
                                          Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    const Model::ZoneModel &aZoneModel,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryVolumeFixed(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                 const Model::ZoneModel &aZoneModel,
                                                 Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryZoneEqualizer(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                   const Model::ZoneModel &aZoneModel,
                                                   Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

    static Common::Status HandleEqualizerBandResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                      const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                      const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel,
                                                      Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleEqualizerPresetResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                        const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                                        Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleHighpassCrossoverResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                          const Model::CrossoverModel::FrequencyType &aHighpassFrequency,
                                                          Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleLowpassCrossoverResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                         const Model::CrossoverModel::FrequencyType &aLowpassFrequency,
                                                         Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleMuteResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                             const Model::VolumeModel::MuteType &aMute,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleSoundModeResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  const Model::SoundModel::SoundMode &aSoundMode,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleToneResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                             const Model::ToneModel::LevelType &aBass,
                                             Model::ToneModel::LevelType &aTreble,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeFixedResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    const Model::VolumeModel::FixedType &aVolumeFixed,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                               const Model::VolumeModel::LevelType &aVolume,
                                               Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

private:
    Model::ZonesModel &                               mZonesModel;
    const Model::ZoneModel::IdentifierType &          mZonesMax;

protected:
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

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERZONESCONTROLLERBASIS_HPP
