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
 *      HLX group commands.
 *
 */

#include "CommandGroupsRegularExpressionBases.hpp"

#include <CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Groups
{

/**
 *  The group volume level decrease command regular expression pattern
 *  string.
 *
 */
const char * const DecreaseVolumeRegularExpressionBasis::kRegexp    = "VG([[:digit:]]+)D";

/**
 *  The group volume level increase command regular expression pattern
 *  string.
 *
 */
const char * const IncreaseVolumeRegularExpressionBasis::kRegexp    = "VG([[:digit:]]+)U";

/**
 *  The group volume mute command regular expression pattern string.
 *
 */
const char * const MuteRegularExpressionBasis::kRegexp              = "V([U]?M)G([[:digit:]]+)";

/**
 *  The group name command regular expression pattern string.
 *
 */
const char * const NameRegularExpressionBasis::kRegexp              = "NG([[:digit:]]+)\"([[:print:]]+)\"";

/**
 *  The group query command regular expression pattern string.
 *
 */
const char * const QueryRegularExpressionBasis::kRegexp             = "QG([[:digit:]]+)";

/**
 *  The group source (input) command regular expression pattern
 *  string.
 *
 */
const char * const SourceRegularExpressionBasis::kRegexp            = "CG(X|[[:digit:]]+)I([[:digit:]]+)";

/**
 *  The group volume mute toggle (flip) command regular expression
 *  pattern string.
 *
 */
const char * const ToggleMuteRegularExpressionBasis::kRegexp        = "VMTG([[:digit:]]+)";

/**
 *  The group volume level command regular expression pattern string.
 *
 */
const char * const VolumeRegularExpressionBasis::kRegexp            = "VG([[:digit:]]+)R(-?[[:digit:]]+)";

/**
 *  The group volume level decrease command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t DecreaseVolumeRegularExpressionBasis::kExpectedMatches = 2;

/**
 *  The group volume level increase command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t IncreaseVolumeRegularExpressionBasis::kExpectedMatches = 2;

/**
 *  The group volume mute command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t MuteRegularExpressionBasis::kExpectedMatches           = 3;

/**
 *  The group name command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t NameRegularExpressionBasis::kExpectedMatches           = 3;

/**
 *  The group query command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t QueryRegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The group source (input) command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t SourceRegularExpressionBasis::kExpectedMatches         = 3;

/**
 *  The group volume mute toggle (flip) command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t ToggleMuteRegularExpressionBasis::kExpectedMatches     = 2;

/**
 *  The group volume level command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t VolumeRegularExpressionBasis::kExpectedMatches         = 3;

/**
 *  @brief
 *    This initializes the group volume level decrease command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group volume level decrease
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseVolumeRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the group volume level increase command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group volume level increase
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseVolumeRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the group volume mute command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group volume mute command
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
 *    This initializes the group name command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group name command regular
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
 *    This initializes the group query command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group query command regular
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
 *    This initializes the group source (input) command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group source (input) command
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
 *    This initializes the group volume mute toggle (flip) command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group volume mute toggle
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
 *    This initializes the group volume level command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      group volume level command
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

}; // namespace Groups

}; // namespace Command

}; // namespace Common

}; // namespace HLX
