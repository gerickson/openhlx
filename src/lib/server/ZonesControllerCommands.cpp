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
 *      This file implements objects for HLX server zone data model
 *      commands and their constituent requests and responses.
 *
 */

#include "ZonesControllerCommands.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Zones
{

static const char * const kZoneObject     = "O";

const char * const AdjustBalanceRequest::kRequestRegexp         = "BO([[:digit:]]+)([LR])U";
const char * const DecreaseBassRequest::kRequestRegexp          = "TO([[:digit:]]+)B(D)";
const char * const IncreaseBassRequest::kRequestRegexp          = "TO([[:digit:]]+)B(U)";
const char * const DecreaseTrebleRequest::kRequestRegexp        = "TO([[:digit:]]+)T(D)";
const char * const IncreaseTrebleRequest::kRequestRegexp        = "TO([[:digit:]]+)T(U)";
const char * const DecreaseEqualizerBandRequest::kRequestRegexp = "EO([[:digit:]]+)B([[:digit:]]+)(D)";
const char * const IncreaseEqualizerBandRequest::kRequestRegexp = "EO([[:digit:]]+)B([[:digit:]]+)(U)";
const char * const DecreaseVolumeRequest::kRequestRegexp        = "VO([[:digit:]]+)(D)";
const char * const IncreaseVolumeRequest::kRequestRegexp        = "VO([[:digit:]]+)(U)";
const char * const QueryMuteRequest::kRequestRegexp             = "QVMO([[:digit:]]+)";
const char * const QuerySourceRequest::kRequestRegexp           = "QCO([[:digit:]]+)";
const char * const QueryVolumeRequest::kRequestRegexp           = "QVO([[:digit:]]+)";

const size_t AdjustBalanceRequest::kExpectedMatches             = 3;
const size_t DecreaseBassRequest::kExpectedMatches              = 3;
const size_t IncreaseBassRequest::kExpectedMatches              = 3;
const size_t DecreaseTrebleRequest::kExpectedMatches            = 3;
const size_t IncreaseTrebleRequest::kExpectedMatches            = 3;
const size_t DecreaseEqualizerBandRequest::kExpectedMatches     = 4;
const size_t IncreaseEqualizerBandRequest::kExpectedMatches     = 4;
const size_t DecreaseVolumeRequest::kExpectedMatches            = 3;
const size_t IncreaseVolumeRequest::kExpectedMatches            = 3;
const size_t QueryMuteRequest::kExpectedMatches                 = 2;
const size_t QuerySourceRequest::kExpectedMatches               = 2;
const size_t QueryVolumeRequest::kExpectedMatches               = 2;

// MARK: Observer Requests, Responses, and Commands

Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

Status
QueryResponse :: Init(const Model::ZoneModel::IdentifierType &aZone)
{
    return (QueryResponseBasis::Init(kZoneObject, aZone));
}

Status
QueryMuteRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
QuerySourceRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
QueryVolumeRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Balance Mutator Requests, Responses, and Commands

Status
AdjustBalanceRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
SetBalanceRequest :: Init(void)
{
    return (BalanceRegularExpressionBasis::Init(*this));
}

Status
BalanceResponse :: Init(const Model::ZoneModel::IdentifierType & aZoneIdentifier,
                        const Model::BalanceModel::BalanceType & aBalance)
{
    return (BalanceResponseBasis::Init(kZoneObject, aZoneIdentifier, aBalance));
}

// MARK: (Sound Mode) Equalizer Band Mutator Requests, Responses, and Commands

Status
DecreaseEqualizerBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
IncreaseEqualizerBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
SetEqualizerBandRequest :: Init(void)
{
    return (EqualizerBandLevelRegularExpressionBasis::Init(*this));
}

Status
EqualizerBandResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel)
{
    return (EqualizerBandResponseBasis::Init(kZoneObject, aZoneIdentifier, aEqualizerBandIdentifier, aBandLevel));
}

// MARK: (Sound Mode) Equalizer Preset Mutator Requests, Responses, and Commands

Status
SetEqualizerPresetRequest :: Init(void)
{
    return (EqualizerPresetRegularExpressionBasis::Init(*this));
}

Status
EqualizerPresetResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    return (EqualizerPresetBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aEqualizerPresetIdentifier));
}

// MARK: (Sound Mode) Highpass Crossover Mutator Requests, Responses, and Commands

Status
SetHighpassCrossoverRequest :: Init(void)
{
    return (HighpassCrossoverRegularExpressionBasis::Init(*this));
}

Status
HighpassCrossoverResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                  const Model::CrossoverModel::FrequencyType &aHighpassFrequency)
{
    return (EqualizerHighpassCrossoverBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aHighpassFrequency));
}

// MARK: (Sound Mode) Lowpass Crossover Mutator Requests, Responses, and Commands

Status
SetLowpassCrossoverRequest :: Init(void)
{
    return (LowpassCrossoverRegularExpressionBasis::Init(*this));
}

Status
LowpassCrossoverResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                 const Model::CrossoverModel::FrequencyType &aLowpassFrequency)
{
    return (EqualizerLowpassCrossoverBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aLowpassFrequency));
}

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

Status
MuteRequest :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

Status
ToggleMuteRequest :: Init(void)
{
    return (ToggleMuteRegularExpressionBasis::Init(*this));
}

Status
MuteResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                     const Model::VolumeModel::MuteType &aMute)
{
    return (VolumeMuteResponseBasis::Init(kZoneObject, aZoneIdentifier, aMute));
}

// MARK: Name Mutator Requests, Responses, and Commands

Status
SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

Status
NameResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char * aName)
{
    return (NameSetResponseBasis::Init(kZoneObject, aZoneIdentifier, aName));
}

Status
NameResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char * aName, const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kZoneObject, aZoneIdentifier, aName, aNameLength));
}

// MARK: Sound Mode Mutator Requests, Responses, and Commands

Status
SetSoundModeRequest :: Init(void)
{
    return (SoundModeRegularExpressionBasis::Init(*this));
}

Status
SoundModeResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const Model::SoundModel::SoundMode &aMode)
{
    return (EqualizerSoundModeBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aMode));
}

// MARK: Source Mutator Requests, Responses, and Commands

Status
SetSourceRequest :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

Status
SourceResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetResponseBasis::Init(kZoneObject, aZoneIdentifier, aSourceIdentifier));
}

Status
SetSourceAllRequest :: Init(void)
{
    return (SourceAllRegularExpressionBasis::Init(*this));
}

Status
SourceAllResponse :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    static const char * const kAllZonesObject = "X";

    return (SourceAllResponseBasis::Init(kAllZonesObject, aSourceIdentifier));
}

// MARK: (Sound Mode) Tone Mutator Requests, Responses, and Commands

Status
DecreaseBassRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
IncreaseBassRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
DecreaseTrebleRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
IncreaseTrebleRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
SetToneRequest :: Init(void)
{
    return (ToneRegularExpressionBasis::Init(*this));
}

Status
ToneResponse :: Init(const Model::ZoneModel::IdentifierType & aZoneIdentifier,
                     const Model::ToneModel::LevelType &      aBass,
                     const Model::ToneModel::LevelType &      aTreble)
{
    return (ToneBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aBass, aTreble));
}

// MARK: Volume Mutator Requests, Responses, and Commands

Status
DecreaseVolumeRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
IncreaseVolumeRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
SetVolumeRequest :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

Status
VolumeResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume)
{
    return (VolumeResponseBasis::Init(kZoneObject, aZoneIdentifier, aVolume));
}

Status
SetVolumeAllRequest :: Init(void)
{
    return (VolumeAllRegularExpressionBasis::Init(*this));
}

Status
VolumeAllResponse :: Init(const Model::VolumeModel::LevelType &aVolume)
{
    static const char * const kAllZonesObject = "X";

    return (VolumeAllResponseBasis::Init(kAllZonesObject, aVolume));
}

Status
SetVolumeFixedRequest :: Init(void)
{
    return (VolumeFixedRegularExpressionBasis::Init(*this));
}

Status
VolumeFixedResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aVolumeFixed)
{
    return (VolumeFixedResponseBasis::Init(kZoneObject, aZoneIdentifier, aVolumeFixed));
}

}; // namespace Zones

}; // namespace Command

}; // namespace Server

}; // namespace HLX
