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

namespace Client
{

// Notification response data

Command::Zones::BalanceResponse            ZonesControllerBasis::kBalanceResponse;
Command::Zones::EqualizerBandResponse      ZonesControllerBasis::kEqualizerBandResponse;
Command::Zones::EqualizerPresetResponse    ZonesControllerBasis::kEqualizerPresetResponse;
Command::Zones::HighpassCrossoverResponse  ZonesControllerBasis::kHighpassCrossoverResponse;
Command::Zones::LowpassCrossoverResponse   ZonesControllerBasis::kLowpassCrossoverResponse;
Command::Zones::MuteResponse               ZonesControllerBasis::kMuteResponse;
Command::Zones::NameResponse               ZonesControllerBasis::kNameResponse;
Command::Zones::QueryResponse              ZonesControllerBasis::kQueryResponse;
Command::Zones::SoundModeResponse          ZonesControllerBasis::kSoundModeResponse;
Command::Zones::SourceResponse             ZonesControllerBasis::kSourceResponse;
Command::Zones::SourceAllResponse          ZonesControllerBasis::kSourceAllResponse;
Command::Zones::ToneResponse               ZonesControllerBasis::kToneResponse;
Command::Zones::VolumeResponse             ZonesControllerBasis::kVolumeResponse;
Command::Zones::VolumeAllResponse          ZonesControllerBasis::kVolumeAllResponse;
Command::Zones::VolumeFixedResponse        ZonesControllerBasis::kVolumeFixedResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesControllerBasis :: ZonesControllerBasis(void) :
    mZonesDidRefreshCount(0)
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


    lRetval = ResponseInit();
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
ZonesControllerBasis :: ResponseInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    // Initialize static notification response data.

    lRetval = kBalanceResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kEqualizerBandResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kEqualizerPresetResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kHighpassCrossoverResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kLowpassCrossoverResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kMuteResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kNameResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSoundModeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSourceResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSourceAllResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kToneResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeAllResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeFixedResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
