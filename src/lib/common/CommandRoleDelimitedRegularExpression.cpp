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
 *      This file implements a derived object for composing delimited
 *      (that is, initiated and terminated by a delimiting pair of
 *      characters) HLX command regular expressions, where the
 *      delimiters used are based on an enumerated role designation.
 *
 */

#include "CommandRoleDelimitedRegularExpression.hpp"

#include <OpenHLX/Common/CommandRoleDelimiters.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the command regular expression with the specified
 *  role and with the specified regular expression pattern and
 *  expected number of substring matches for the pattern.
 *
 *  @param[in]  aRole                An immutable reference to the
 *                                   command role to compose the
 *                                   command regular expression with.
 *  @param[in]  aRegexp              A pointer to a null-terminated C
 *                                   string containing the regular
 *                                   expression pattern to initialize
 *                                   with that will be used to match
 *                                   against.
 *  @param[in]  aExpectedMatchCount  The number of expected substring
 *                                   matches for @a aRegexp when @a
 *                                   the pattern successfully matches.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If @a aRegexp was null.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RoleDelimitedRegularExpression :: Init(const Role &aRole,
                                       const char *aRegexp,
                                       const size_t &aExpectedMatchCount)
{
    return (DelimitedRegularExpression::Init(GetRoleRegularExpressionDelimiters(aRole), aRegexp, aExpectedMatchCount));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
