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

#ifndef OPENHLXCLIENTZONESCONTROLLERBASIS_HPP
#define OPENHLXCLIENTZONESCONTROLLERBASIS_HPP

#include <OpenHLX/Client/ZonesControllerCommands.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup zone
 *
 */
class ZonesControllerBasis
{
public:
    virtual ~ZonesControllerBasis(void);

protected:
    ZonesControllerBasis(void);

    Common::Status Init(void);

private:
    Common::Status ResponseInit(void);

protected:
    size_t                                            mZonesDidRefreshCount;

protected:
    static Command::Zones::BalanceResponse            kBalanceResponse;
    static Command::Zones::EqualizerBandResponse      kEqualizerBandResponse;
    static Command::Zones::EqualizerPresetResponse    kEqualizerPresetResponse;
    static Command::Zones::HighpassCrossoverResponse  kHighpassCrossoverResponse;
    static Command::Zones::LowpassCrossoverResponse   kLowpassCrossoverResponse;
    static Command::Zones::MuteResponse               kMuteResponse;
    static Command::Zones::NameResponse               kNameResponse;
    static Command::Zones::QueryResponse              kQueryResponse;
    static Command::Zones::SoundModeResponse          kSoundModeResponse;
    static Command::Zones::SourceResponse             kSourceResponse;
    static Command::Zones::SourceAllResponse          kSourceAllResponse;
    static Command::Zones::ToneResponse               kToneResponse;
    static Command::Zones::VolumeResponse             kVolumeResponse;
    static Command::Zones::VolumeAllResponse          kVolumeAllResponse;
    static Command::Zones::VolumeFixedResponse        kVolumeFixedResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTZONESCONTROLLERBASIS_HPP
