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
 *      HLX configuration commands.
 *
 */

#include "CommandConfigurationRegularExpressionBases.hpp"

#include <CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Configuration
{

/**
 *  The configuration load from backup command regular expression
 *  pattern string.
 *
 */
const char * const LoadFromBackupRegularExpressionBasis::kRegexp     = "LOAD";

/**
 *  The configuration query current command regular expression pattern
 *  string.
 *
 */
const char * const QueryCurrentRegularExpressionBasis::kRegexp       = "QX";

/**
 *  The configuration reset to defaults command regular expression
 *  pattern string.
 *
 */
const char * const ResetToDefaultsRegularExpressionBasis::kRegexp    = "RESET";

/**
 *  The configuration save to backup command regular expression
 *  pattern string.
 *
 */
const char * const SaveToBackupRegularExpressionBasis::kRegexp       = "SAVE";

/**
 *  The configuration load from backup command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t LoadFromBackupRegularExpressionBasis::kExpectedMatches  = 1;

/**
 *  The configuration query current command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t QueryCurrentRegularExpressionBasis::kExpectedMatches    = 1;

/**
 *  The configuration reset to defaults command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t ResetToDefaultsRegularExpressionBasis::kExpectedMatches = 1;

/**
 *  The configuration save to backup command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t SaveToBackupRegularExpressionBasis::kExpectedMatches    = 1;

/**
 *  @brief
 *    This initializes the configuration load from backup command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      configuration load from backup
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LoadFromBackupRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the configuration query current command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      configuration query current
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryCurrentRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the configuration reset to defaults command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      configuration reset to defaults
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ResetToDefaultsRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the configuration save to backup command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      configuration save to backup
 *                                      command regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SaveToBackupRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

}; // namespace Configuration

}; // namespace Command

}; // namespace Common

}; // namespace HLX
