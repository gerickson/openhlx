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

/**
 *  Equalizer preset decrease equalizer band level command request
 *  regular expression pattern.
 *
 */
const char * const DecreaseBandRequest::kRequestRegexp = "EP([[:digit:]]+)B([[:digit:]]+)(D)";

/**
 *  Equalizer preset increase equalizer band level command request
 *  regular expression pattern.
 *
 */
const char * const IncreaseBandRequest::kRequestRegexp = "EP([[:digit:]]+)B([[:digit:]]+)(U)";

/**
 *  Expected number of equalizer preset decrease equalizer band level
 *  command request regular expression pattern matches.
 *
 */
const size_t DecreaseBandRequest::kExpectedMatches     = 4;

/**
 *  Expected number of equalizer preset increase equalizer band level
 *  command request regular expression pattern matches.
 *
 */
const size_t IncreaseBandRequest::kExpectedMatches     = 4;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset query command response buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of
 *                                          the equalizer preset
 *                                          to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    return (QueryResponseBasis::Init(kEqualizerPresetObject,
                                     aEqualizerPresetIdentifier));
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Band Level Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the decrease band level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the increase band level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseBandRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set band level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetBandRequest :: Init(void)
{
    return (BandLevelRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset band level command
 *  response buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          form the band level response.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to form the band
 *                                          level response.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to
 *                                          band level for which to
 *                                          form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
BandResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                     const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    const char * const kPresetObject = kEqualizerPresetObject + 1;

    return (EqualizerBandResponseBasis::Init(kPresetObject,
                                             aEqualizerPresetIdentifier,
                                             aEqualizerBandIdentifier,
                                             aEqualizerBandLevel));
}

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set name command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset name command response
 *  buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          identifier for which to
 *                                          form the name response.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string of the
 *                                          equalizer preset name for
 *                                          which to form the
 *                                          response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                     const char * aName)
{
    return (NameSetResponseBasis::Init(kEqualizerPresetObject,
                                       aEqualizerPresetIdentifier,
                                       aName));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset name command response
 *  buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          identifier for which to
 *                                          form the name response.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string of the
 *                                          equalizer preset name for
 *                                          which to form the
 *                                          response.
 *  @param[in]  aNameLength                 An immutable reference to
 *                                          the length, in bytes, of
 *                                          @a aName.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kEqualizerPresetObject,
                                       aEqualizerPresetIdentifier,
                                       aName,
                                       aNameLength));
}

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Server

}; // namespace HLX
