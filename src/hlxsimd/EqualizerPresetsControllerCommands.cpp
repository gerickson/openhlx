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
 *      This file implements objects for HLX server equalizer preset data
 *      model commands and their constituent requests and responses.
 *
 */

#include "EqualizerPresetsControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;

namespace HLX
{

namespace Server
{

namespace Command
{

namespace EqualizerPresets
{

static const char * const kEqualizerPresetObject = "EP";

const char * const DecreaseBandRequest::kRequestRegexp = "EP([[:digit:]]+)B([[:digit:]]+)(D)";
const char * const IncreaseBandRequest::kRequestRegexp = "EP([[:digit:]]+)B([[:digit:]]+)(U)";

const size_t DecreaseBandRequest::kExpectedMatches     = 4;
const size_t IncreaseBandRequest::kExpectedMatches     = 4;

// MARK: Observer Requests, Responses, and Commands

Status QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

Status
QueryResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset)
{
    return (QueryResponseBasis::Init(kEqualizerPresetObject, aEqualizerPreset));
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Band Level Mutator Requests, Responses, and Commands

Status DecreaseBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status IncreaseBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status SetBandRequest :: Init(void)
{
    return (BandLevelRegularExpressionBasis::Init(*this));
}

Status BandResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aLevel)
{
    const char * const kPresetObject = kEqualizerPresetObject + 1;

    return (EqualizerBandResponseBasis::Init(kPresetObject, aEqualizerIdentifier, aEqualizerBandIdentifier, aLevel));
}

// MARK: Name Mutator Requests, Responses, and Commands

Status SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

Status NameResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset, const char * aName)
{
    return (NameSetResponseBasis::Init(kEqualizerPresetObject, aEqualizerPreset, aName));
}

Status
NameResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kEqualizerPresetObject, aEqualizerPreset, aName, aNameLength));
}

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Server

}; // namespace HLX
