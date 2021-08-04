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

#include "ZonesControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Request data

Server::Command::Zones::AdjustBalanceRequest          ZonesControllerBasis::kAdjustBalanceRequest;
Server::Command::Zones::DecreaseBassRequest           ZonesControllerBasis::kDecreaseBassRequest;
Server::Command::Zones::IncreaseBassRequest           ZonesControllerBasis::kIncreaseBassRequest;
Server::Command::Zones::DecreaseTrebleRequest         ZonesControllerBasis::kDecreaseTrebleRequest;
Server::Command::Zones::IncreaseTrebleRequest         ZonesControllerBasis::kIncreaseTrebleRequest;
Server::Command::Zones::DecreaseEqualizerBandRequest  ZonesControllerBasis::kDecreaseEqualizerBandRequest;
Server::Command::Zones::IncreaseEqualizerBandRequest  ZonesControllerBasis::kIncreaseEqualizerBandRequest;
Server::Command::Zones::DecreaseVolumeRequest         ZonesControllerBasis::kDecreaseVolumeRequest;
Server::Command::Zones::IncreaseVolumeRequest         ZonesControllerBasis::kIncreaseVolumeRequest;
Server::Command::Zones::MuteRequest                   ZonesControllerBasis::kMuteRequest;
Server::Command::Zones::QueryRequest                  ZonesControllerBasis::kQueryRequest;
Server::Command::Zones::QueryMuteRequest              ZonesControllerBasis::kQueryMuteRequest;
Server::Command::Zones::QuerySourceRequest            ZonesControllerBasis::kQuerySourceRequest;
Server::Command::Zones::QueryVolumeRequest            ZonesControllerBasis::kQueryVolumeRequest;
Server::Command::Zones::SetBalanceRequest             ZonesControllerBasis::kSetBalanceRequest;
Server::Command::Zones::SetEqualizerBandRequest       ZonesControllerBasis::kSetEqualizerBandRequest;
Server::Command::Zones::SetEqualizerPresetRequest     ZonesControllerBasis::kSetEqualizerPresetRequest;
Server::Command::Zones::SetHighpassCrossoverRequest   ZonesControllerBasis::kSetHighpassCrossoverRequest;
Server::Command::Zones::SetLowpassCrossoverRequest    ZonesControllerBasis::kSetLowpassCrossoverRequest;
Server::Command::Zones::SetNameRequest                ZonesControllerBasis::kSetNameRequest;
Server::Command::Zones::SetSoundModeRequest           ZonesControllerBasis::kSetSoundModeRequest;
Server::Command::Zones::SetSourceRequest              ZonesControllerBasis::kSetSourceRequest;
Server::Command::Zones::SetSourceAllRequest           ZonesControllerBasis::kSetSourceAllRequest;
Server::Command::Zones::SetToneRequest                ZonesControllerBasis::kSetToneRequest;
Server::Command::Zones::SetVolumeRequest              ZonesControllerBasis::kSetVolumeRequest;
Server::Command::Zones::SetVolumeAllRequest           ZonesControllerBasis::kSetVolumeAllRequest;
Server::Command::Zones::SetVolumeFixedRequest         ZonesControllerBasis::kSetVolumeFixedRequest;
Server::Command::Zones::ToggleMuteRequest             ZonesControllerBasis::kToggleMuteRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesControllerBasis :: ZonesControllerBasis(void)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesControllerBasis :: ~ZonesControllerBasis(void)
{
    return;
}

Status
ZonesControllerBasis :: Init(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ZonesControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = kAdjustBalanceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseBassRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseBassRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseTrebleRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseTrebleRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQuerySourceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetBalanceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetEqualizerPresetRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetHighpassCrossoverRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetLowpassCrossoverRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSoundModeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSourceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSourceAllRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetToneRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeAllRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeFixedRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kToggleMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
