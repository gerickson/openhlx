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
 *      HLX favorite commands.
 *
 */

#include "CommandFavoritesRegularExpressionBases.hpp"

#include <CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Favorites
{

/**
 *  The favorite name command regular expression pattern string.
 *
 */
const char * const NameRegularExpressionBasis::kRegexp         = "NF([[:digit:]]+)\"([[:print:]]+)\"";

/**
 *  The favorite query command regular expression pattern string.
 *
 */
const char * const QueryRegularExpressionBasis::kRegexp        = "QF([[:digit:]]+)";

/**
 *  The favorite name command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t NameRegularExpressionBasis::kExpectedMatches      = 3;

/**
 *  The favorite query command regular expression pattern expected
 *  substring matches.
 *
 */
const size_t QueryRegularExpressionBasis::kExpectedMatches     = 2;

/**
 *  @brief
 *    This initializes the favorite name command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      favorite name command regular
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
 *    This initializes the favorite query command regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      favorite query command regular
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

}; // namespace Favorites

}; // namespace Command

}; // namespace Common

}; // namespace HLX
