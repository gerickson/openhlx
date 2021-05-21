/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file implements base, common (that is, independent of
 *      requestor or responder) command regular expression objects for
 *      HLX zone commands.
 *
 */

#include "CommandZonesRegularExpressionBases.hpp"

#include <CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Zones
{

/**
 *  The zone stereophonic channel balance command regular expression
 *  pattern string.
 *
 */
const char * const BalanceRegularExpressionBasis::kRegexp               = "BO([[:digit:]]+)([RL])([[:digit:]]+)";

/**
 *  The zone equalizer band level command regular expression pattern
 *  string.
 *
 */
const char * const EqualizerBandLevelRegularExpressionBasis::kRegexp    = "EO([[:digit:]]+)B([[:digit:]]+)L(-?[[:digit:]]+)";

/**
 *  The zone equalizer preset command regular expression pattern
 *  string.
 *
 */
const char * const EqualizerPresetRegularExpressionBasis::kRegexp       = "EO([[:digit:]]+)P([[:digit:]]+)";

/**
 *  The zone highpass crossover frequency command regular expression
 *  pattern string.
 *
 */
const char * const HighpassCrossoverRegularExpressionBasis::kRegexp     = "EO([[:digit:]]+)HP([[:digit:]]+)";

/**
 *  The zone lowpass crossover frequency command regular expression
 *  pattern string.
 *
 */
const char * const LowpassCrossoverRegularExpressionBasis::kRegexp      = "EO([[:digit:]]+)LP([[:digit:]]+)";

/**
 *  The zone volume mute command regular expression pattern string.
 *
 */
const char * const MuteRegularExpressionBasis::kRegexp                  = "V([U]?M)O([[:digit:]]+)";

/**
 *  The zone name command regular expression pattern string.
 *
 */
const char * const NameRegularExpressionBasis::kRegexp                  = "NO([[:digit:]]+)\"([[:print:]]+)\"";

/**
 *  The zone query command regular expression pattern string.
 *
 */
const char * const QueryRegularExpressionBasis::kRegexp                 = "QO([[:digit:]]+)";

/**
 *  The zone equalizer sound mode command regular expression pattern
 *  string.
 *
 */
const char * const SoundModeRegularExpressionBasis::kRegexp             = "EO([[:digit:]]+)M([[:digit:]]+)";

/**
 *  The single zone source (input) command regular expression pattern
 *  string.
 *
 */
const char * const SourceRegularExpressionBasis::kRegexp                = "CO([[:digit:]]+)I([[:digit:]]+)";

/**
 *  The all zones source (input) command regular expression pattern
 *  string.
 *
 */
const char * const SourceAllRegularExpressionBasis::kRegexp             = "CXI([[:digit:]]+)";

/**
 *  The zone volume mute toggle (flip) command regular expression
 *  pattern string.
 *
 */
const char * const ToggleMuteRegularExpressionBasis::kRegexp            = "VMTO([[:digit:]]+)";

/**
 *  The zone tone equalizer command regular expression pattern string.
 *
 */
const char * const ToneRegularExpressionBasis::kRegexp                  = "TO([[:digit:]]+)B(-?[[:digit:]]+)T(-?[[:digit:]]+)";

/**
 *  The single zone volume level command regular expression pattern
 *  string.
 *
 */
const char * const VolumeRegularExpressionBasis::kRegexp                = "VO([[:digit:]]+)R(-?[[:digit:]]+)";

/**
 *  The all zones volume level command regular expression pattern string.
 *
 */
const char * const VolumeAllRegularExpressionBasis::kRegexp             = "VXR(-?[[:digit:]]+)";

/**
 *  The zone volume fixed/locked state command regular expression
 *  pattern string.
 *
 */
const char * const VolumeFixedRegularExpressionBasis::kRegexp           = "VO([[:digit:]]+)F([01])";

/**
 *  The zone stereophonic channel balance command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t BalanceRegularExpressionBasis::kExpectedMatches            = 4;

/**
 *  The zone equalizer band level command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t EqualizerBandLevelRegularExpressionBasis::kExpectedMatches = 4;

/**
 *  The zone equalizer preset command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t EqualizerPresetRegularExpressionBasis::kExpectedMatches    = 3;

/**
 *  The zone highpass crossover frequency command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t HighpassCrossoverRegularExpressionBasis::kExpectedMatches  = 3;

/**
 *  The zone lowpass crossover frequency command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t LowpassCrossoverRegularExpressionBasis::kExpectedMatches   = 3;

/**
 *  The zone volume mute command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t MuteRegularExpressionBasis::kExpectedMatches               = 3;

/**
 *  The zone name command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t NameRegularExpressionBasis::kExpectedMatches               = 3;

/**
 *  The zone query command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t QueryRegularExpressionBasis::kExpectedMatches              = 2;

/**
 *  The zone equalizer sound mode command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t SoundModeRegularExpressionBasis::kExpectedMatches          = 3;

/**
 *  The single zone source (input) command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t SourceRegularExpressionBasis::kExpectedMatches             = 3;

/**
 *  The all zones source (input) command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t SourceAllRegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The zone volume mute toggle (flip) command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t ToggleMuteRegularExpressionBasis::kExpectedMatches         = 2;

/**
 *  The zone tone equalizer command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t ToneRegularExpressionBasis::kExpectedMatches               = 4;

/**
 *  The single zone volume level command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t VolumeRegularExpressionBasis::kExpectedMatches             = 3;

/**
 *  The all zones volume level command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t VolumeAllRegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The zone volume fixed/locked state command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t VolumeFixedRegularExpressionBasis::kExpectedMatches        = 3;

/**
 *  @brief
 *    This initializes the zone stereophonic channel balance command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone stereophonic channel
 *                                      balance command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone equalizer band level command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone equalizer band level
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandLevelRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone equalizer preset command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone equalizer preset command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone highpass crossover frequency command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone highpass crossover
 *                                      frequency command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
HighpassCrossoverRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone lowpass crossover frequency command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone lowpass crossover
 *                                      frequency command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LowpassCrossoverRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone volume mute command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone volume mute command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
MuteRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone name command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone name command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone query command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone query command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone equalizer sound mode command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone equalizer sound mode
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SoundModeRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the single zone source (input) command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone source (input) command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the all zones source (input) command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      all zones source (input)
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceAllRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone tone equalizer command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone tone equalizer command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToneRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone volume mute toggle (flip) command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone volume mute toggle
 *                                      (flip) command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToggleMuteRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the single zone volume level command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone volume level command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the all zones volume level command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      all zones volume level command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeAllRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the zone volume fixed/locked state command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      zone volume fixed/locked state
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeFixedRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

}; // namespace Zones

}; // namespace Command

}; // namespace Common

}; // namespace HLX
