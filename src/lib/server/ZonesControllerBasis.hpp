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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXSERVERZONESCONTROLLERBASIS_HPP
#define OPENHLXSERVERZONESCONTROLLERBASIS_HPP

#include <OpenHLX/Server/ZonesControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup zones
 *
 */
class ZonesControllerBasis
{
public:
    virtual ~ZonesControllerBasis(void);

protected:
    ZonesControllerBasis(void);

    Common::Status Init(void);

protected:
    Common::Status RequestInit(void);

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
