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
 *      HLX physical front panel commands.
 *
 */

#include "CommandFrontPanelRegularExpressionBases.hpp"

#include <OpenHLX/Common/CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace FrontPanel
{

/**
 *  The front panel brightness command regular expression pattern
 *  string.
 *
 */
const char * const BrightnessRegularExpressionBasis::kRegexp    = "SD([0-3])";

/**
 *  The front panel locked command regular expression pattern
 *  string.
 *
 */
const char * const LockedRegularExpressionBasis::kRegexp        = "FPL([01])";

/**
 *  The front panel brightness command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t BrightnessRegularExpressionBasis::kExpectedMatches = 2;

/**
 *  The front panel locked command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t LockedRegularExpressionBasis::kExpectedMatches     = 2;

/**
 *  @brief
 *    This initializes the front panel brightness command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      front panel brightness command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BrightnessRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the front panel locked command regular
 *    expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      front panel locked command
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LockedRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Common

}; // namespace HLX
