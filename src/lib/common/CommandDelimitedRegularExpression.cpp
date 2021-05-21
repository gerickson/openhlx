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
 *      characters) HLX command regular expressions.
 *
 */

#include "CommandDelimitedRegularExpression.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>


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
 *  delimiters and with the specified regular expression pattern and
 *  expected number of substring matches for the pattern.
 *
 *  @param[in]  aDelimiters          An immutable reference to the
 *                                   command delimiters to compose
 *                                   the command buffer with.
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
DelimitedRegularExpression :: Init(const Delimiters &aDelimiters,
                                   const char *aRegexp,
                                   const size_t &aExpectedMatchCount)
{
    /*
     * Ideally, the underlying buffer would have an initialization
     * interface that would allow initialization through an array of
     * start and end iterators; however, for now, we deal with the
     * slight inefficiency of composiing a temporary string.
     */
    std::string  lTempRegexp;
    Status       lRetval = kStatus_Success;


    lTempRegexp += aDelimiters.mStart;

    lTempRegexp += aRegexp;

    lTempRegexp += aDelimiters.mEnd;

    lRetval = RegularExpression::Init(lTempRegexp.c_str(), aExpectedMatchCount);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
