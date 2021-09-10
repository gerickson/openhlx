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

/**
 *  Zone adjust stereophonic channel balance command request regular
 *  expression pattern.
 *
 */
const char * const AdjustBalanceRequest::kRequestRegexp         = "BO([[:digit:]]+)([LR])U";

/**
 *  Zone decrease tone equalizer bass level command request regular
 *  expression pattern.
 *
 */
const char * const DecreaseBassRequest::kRequestRegexp          = "TO([[:digit:]]+)B(D)";

/**
 *  Zone increase tone equalizer bass level command request regular
 *  expression pattern.
 *
 */
const char * const IncreaseBassRequest::kRequestRegexp          = "TO([[:digit:]]+)B(U)";

/**
 *  Zone decrease tone equalizer treble level command request regular
 *  expression pattern.
 *
 */
const char * const DecreaseTrebleRequest::kRequestRegexp        = "TO([[:digit:]]+)T(D)";

/**
 *  Zone increase tone equalizer treble level command request regular
 *  expression pattern.
 *
 */
const char * const IncreaseTrebleRequest::kRequestRegexp        = "TO([[:digit:]]+)T(U)";

/**
 *  Zone decrease tone equalizer band level command request regular
 *  expression pattern.
 *
 */
const char * const DecreaseEqualizerBandRequest::kRequestRegexp = "EO([[:digit:]]+)B([[:digit:]]+)(D)";

/**
 *  Zone increase tone equalizer band level command request regular
 *  expression pattern.
 *
 */
const char * const IncreaseEqualizerBandRequest::kRequestRegexp = "EO([[:digit:]]+)B([[:digit:]]+)(U)";

/**
 *  Zone decrease volume level command request regular expression
 *  pattern.
 *
 */
const char * const DecreaseVolumeRequest::kRequestRegexp        = "VO([[:digit:]]+)(D)";

/**
 *  Zone increase volume level command request regular expression
 *  pattern.
 *
 */
const char * const IncreaseVolumeRequest::kRequestRegexp        = "VO([[:digit:]]+)(U)";

/**
 *  Zone query volume mute state command request regular expression
 *  pattern.
 *
 */
const char * const QueryMuteRequest::kRequestRegexp             = "QVMO([[:digit:]]+)";

/**
 *  Zone query source (input) command request regular expression
 *  pattern.
 *
 */
const char * const QuerySourceRequest::kRequestRegexp           = "QCO([[:digit:]]+)";

/**
 *  Zone query volume level command request regular expression
 *  pattern.
 *
 */
const char * const QueryVolumeRequest::kRequestRegexp           = "QVO([[:digit:]]+)";

/**
 *  Expected number of zone adjust stereophonic channel balance
 *  command request regular expression pattern substring matches.
 *
 */
const size_t AdjustBalanceRequest::kExpectedMatches             = 3;

/**
 *  Expected number of zone decrease tone equalizer bass level command
 *  request regular expression pattern substring matches.
 *
 */
const size_t DecreaseBassRequest::kExpectedMatches              = 3;

/**
 *  Expected number of zone increase tone equalizer bass level command
 *  request regular expression pattern substring matches.
 *
 */
const size_t IncreaseBassRequest::kExpectedMatches              = 3;

/**
 *  Expected number of zone decrease tone equalizer treble level
 *  command request regular expression pattern substring matches.
 *
 */
const size_t DecreaseTrebleRequest::kExpectedMatches            = 3;

/**
 *  Expected number of zone increase tone equalizer treble level
 *  command request regular expression pattern substring matches.
 *
 */
const size_t IncreaseTrebleRequest::kExpectedMatches            = 3;

/**
 *  Expected number of zone decrease equalizer band level command
 *  request regular expression pattern substring matches.
 *
 */
const size_t DecreaseEqualizerBandRequest::kExpectedMatches     = 4;

/**
 *  Expected number of zone increase equalizer band level command
 *  request regular expression pattern substring matches.
 *
 */
const size_t IncreaseEqualizerBandRequest::kExpectedMatches     = 4;

/**
 *  Expected number of zone decrease volume level command request
 *  regular expression pattern substring matches.
 *
 */
const size_t DecreaseVolumeRequest::kExpectedMatches            = 3;

/**
 *  Expected number of zone increase volume level command request
 *  regular expression pattern substring matches.
 *
 */
const size_t IncreaseVolumeRequest::kExpectedMatches            = 3;

/**
 *  Expected number of zone query volume mute state command request
 *  regular expression pattern substring matches.
 *
 */
const size_t QueryMuteRequest::kExpectedMatches                 = 2;

/**
 *  Expected number of zone query source (input) command request
 *  regular expression pattern substring matches.
 *
 */
const size_t QuerySourceRequest::kExpectedMatches               = 2;

/**
 *  Expected number of zone query volume level command request regular
 *  expression pattern substring matches.
 *
 */
const size_t QueryVolumeRequest::kExpectedMatches               = 2;

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
 *  This initializes the zone query command response buffer.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the
 *                               identifier of the zone queried.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (QueryResponseBasis::Init(kZoneObject,
                                     aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query volume mute state command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryMuteRequest :: Init(void)
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
 *  This initializes the query source (input) command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QuerySourceRequest :: Init(void)
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
 *  This initializes the query volume level command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
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

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the adjust stereophonic channel balance command
 *  request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
AdjustBalanceRequest :: Init(void)
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
 *  This initializes the set stereophonic channel balance command
 *  request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetBalanceRequest :: Init(void)
{
    return (BalanceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the stereophonic channel balance command
 *  response.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the stereophonic
 *                                channel balance was set.
 *  @param[in]  aBalance          An immutable reference to the
 *                                stereophonic channel balance that
 *                                was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
BalanceResponse :: Init(const Model::ZoneModel::IdentifierType & aZoneIdentifier,
                        const Model::BalanceModel::BalanceType & aBalance)
{
    return (BalanceResponseBasis::Init(kZoneObject, aZoneIdentifier, aBalance));
}

// MARK: (Sound Mode) Equalizer Band Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the decrease equalizer band level command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseEqualizerBandRequest :: Init(void)
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
 *  This initializes the increase equalizer band level command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseEqualizerBandRequest :: Init(void)
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
 *  This initializes the set equalizer band level command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetEqualizerBandRequest :: Init(void)
{
    return (EqualizerBandLevelRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level command response.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference for
 *                                        the zone for which the zone
 *                                        equalizer band level state
 *                                        was set.
 *  @param[in]  aEqualizerBandIdentifier  An immutable reference
 *                                        to the equalizer band on the
 *                                        zone equalizer that was set.
 *  @param[in]  aEqualizerBandLevel       An immutable reference to
 *                                        band level that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
EqualizerBandResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                              const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    return (EqualizerBandResponseBasis::Init(kZoneObject,
                                             aZoneIdentifier,
                                             aEqualizerBandIdentifier,
                                             aEqualizerBandLevel));
}

// MARK: (Sound Mode) Equalizer Preset Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set equalizer preset command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetEqualizerPresetRequest :: Init(void)
{
    return (EqualizerPresetRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer preset command response.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          for the zone for which the
 *                                          zone equalizer preset
 *                                          identifier was set.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
EqualizerPresetResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    return (EqualizerPresetBufferBasis::Init(*this,
                                             kZoneObject,
                                             aZoneIdentifier,
                                             aEqualizerPresetIdentifier));
}

// MARK: (Sound Mode) Highpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone set highpass filter crossover frequency
 *  command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetHighpassCrossoverRequest :: Init(void)
{
    return (HighpassCrossoverRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone highpass filter crossover frequency
 *  command response.
 *
 *  @param[in]  aZoneIdentifier     An immutable reference for the
 *                                  zone for which the highpass filter
 *                                  crossover frequency was set.
 *  @param[in]  aHighpassFrequency  An immutable reference to the
 *                                  highpass filter crossover
 *                                  frequency that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
HighpassCrossoverResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                  const Model::CrossoverModel::FrequencyType &aHighpassFrequency)
{
    return (EqualizerHighpassCrossoverBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aHighpassFrequency));
}

// MARK: (Sound Mode) Lowpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone set lowpass filter crossover frequency
 *  command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetLowpassCrossoverRequest :: Init(void)
{
    return (LowpassCrossoverRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone lowpass filter crossover frequency
 *  command response.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone for which the lowpass filter
 *                                 crossover frequency was set.
 *  @param[in]  aLowpassFrequency  An immutable reference to the
 *                                 lowpass filter crossover
 *                                 frequency that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
LowpassCrossoverResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                 const Model::CrossoverModel::FrequencyType &aLowpassFrequency)
{
    return (EqualizerLowpassCrossoverBufferBasis::Init(*this,
                                                       kZoneObject,
                                                       aZoneIdentifier, aLowpassFrequency));
}

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set volume mute state command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
MuteRequest :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the toggle volume mute state command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToggleMuteRequest :: Init(void)
{
    return (ToggleMuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute state command response
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the volume mute
 *                                state was set.
 *  @param[in]  aMute             An immutable reference to the volume
 *                                mute state that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
MuteResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                     const Model::VolumeModel::MuteType &aMute)
{
    return (VolumeMuteResponseBasis::Init(kZoneObject,
                                          aZoneIdentifier,
                                          aMute));
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
 *  This initializes the zone name command response buffer.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference
 *                               for the zone
 *                               identifier for which to
 *                               form the name response.
 *  @param[in]  aName            A pointer to the null-
 *                               terminated C string of the
 *                               zone name for
 *                               which to form the
 *                               response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                     const char * aName)
{
    return (NameSetResponseBasis::Init(kZoneObject,
                                       aZoneIdentifier,
                                       aName));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone name command response buffer.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference
 *                               for the zone
 *                               identifier for which to
 *                               form the name response.
 *  @param[in]  aName            A pointer to the null-
 *                               terminated C string of the
 *                               zone name for
 *                               which to form the
 *                               response.
 *  @param[in]  aNameLength      An immutable reference to
 *                               the length, in bytes, of
 *                               @a aName.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kZoneObject,
                                       aZoneIdentifier,
                                       aName,
                                       aNameLength));
}

// MARK: Sound Mode Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set sound (equalizer) mode command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetSoundModeRequest :: Init(void)
{
    return (SoundModeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone sound (equalizer) mode command response.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the sound (equalizer)
 *                                mode was set.
 *  @param[in]  aMode             An immutable reference to the sound
 *                                (equalizer) mode that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
SoundModeResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const Model::SoundModel::SoundMode &aMode)
{
    return (EqualizerSoundModeBufferBasis::Init(*this,
                                                kZoneObject,
                                                aZoneIdentifier,
                                                aMode));
}

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set source (input) command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetSourceRequest :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone source (input) command response buffer.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone identifier for which to form
 *                                 the response.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) identifier for which
 *                                 to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetResponseBasis::Init(kZoneObject,
                                         aZoneIdentifier,
                                         aSourceIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the all zones set source (input) command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetSourceAllRequest :: Init(void)
{
    return (SourceAllRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones source (input) command response
 *  buffer.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) identifier for which
 *                                 to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceAllResponse :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    static const char * const kAllZonesObject = "X";

    return (SourceAllResponseBasis::Init(kAllZonesObject, aSourceIdentifier));
}

// MARK: (Sound Mode) Tone Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the decrease bass level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseBassRequest :: Init(void)
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
 *  This initializes the increase bass level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseBassRequest :: Init(void)
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
 *  This initializes the decrease treble level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseTrebleRequest :: Init(void)
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
 *  This initializes the increase treble level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseTrebleRequest :: Init(void)
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
 *  This initializes the set tone (bass/treble) levels command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetToneRequest :: Init(void)
{
    return (ToneRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer levels command response
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the tone equalizer
 *                                levels were set.
 *  @param[in]  aBass             An immutable reference to the tone
 *                                equalizer bass level that was set.
 *  @param[in]  aTreble           An immutable reference to the tone
 *                                equalizer treble level that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */ 
Status
ToneResponse :: Init(const Model::ZoneModel::IdentifierType & aZoneIdentifier,
                     const Model::ToneModel::LevelType &      aBass,
                     const Model::ToneModel::LevelType &      aTreble)
{
    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  aBass,
                                  aTreble));
}

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the decrease volume level command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseVolumeRequest :: Init(void)
{
    Status  lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the increase volume level command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseVolumeRequest :: Init(void)
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
 *  This initializes the set volume level command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetVolumeRequest :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level command response buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the volume level
 *                                was set.
 *  @param[in]  aVolume           An immutable reference to the volume
 *                                level that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const Model::VolumeModel::LevelType &aVolume)
{
    return (VolumeResponseBasis::Init(kZoneObject,
                                      aZoneIdentifier,
                                      aVolume));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the all zones set volume level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetVolumeAllRequest :: Init(void)
{
    return (VolumeAllRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones volume level command response
 *  buffer.
 *
 *  @param[in]  aVolume           An immutable reference to the
 *                                volume level that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeAllResponse :: Init(const Model::VolumeModel::LevelType &aVolume)
{
    static const char * const kAllZonesObject = "X";

    return (VolumeAllResponseBasis::Init(kAllZonesObject,
                                         aVolume));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone volume fixed/locked state command
 *  request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetVolumeFixedRequest :: Init(void)
{
    return (VolumeFixedRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume fixed/locked state command
 *  response buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which the volume mute
 *                                state was set.
 *  @param[in]  aVolumeFixed      An immutable reference to the volume
 *                                fixed/locked state that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeFixedResponse :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                            const Model::VolumeModel::FixedType &aVolumeFixed)
{
    return (VolumeFixedResponseBasis::Init(kZoneObject,
                                           aZoneIdentifier,
                                           aVolumeFixed));
}

}; // namespace Zones

}; // namespace Command

}; // namespace Server

}; // namespace HLX
