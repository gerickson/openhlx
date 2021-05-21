/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines an abstract base object for composing HLX
 *      command regular expressions.
 *
 */

#ifndef HLXCOMMONCOMMANDREGULAREXPRESSIONBASIS_HPP
#define HLXCOMMONCOMMANDREGULAREXPRESSIONBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    An abstract base object for composing HLX command regular
 *    expressions.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class RegularExpressionBasis
{
public:
    virtual ~RegularExpressionBasis(void) = default;

    /**
     *  @brief
     *    This is the class intializer.
     *
     *  This initializes the command regular expression with the specified
     *  regular expression pattern and expected number of substring
     *  matches for the pattern.
     *
     *  @param[in]  aRegexp              A pointer to a null-terminated C
     *                                   string containing the regular
     *                                   expression pattern to initialize
     *                                   with that will be used to match
     *                                   against.
     *  @param[in]  aExpectedMatchCount  The number of expected substring
     *                                   matches for @a aRegexp when
     *                                   the pattern successfully matches.
     *
     *  @retval  kStatus_Success              If successful.
     *  @retval  -EINVAL                      If @a aRegexp was null.
     *  @retval  -ENOMEM                      If memory could not be allocated.
     *  @retval  kError_InitializationFailed  If initialization otherwise
     *                                        failed.
     *
     */
    virtual Status Init(const char *aRegexp, const size_t &aExpectedMatchCount) = 0;

protected:
    RegularExpressionBasis(void) = default;
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDREGULAREXPRESSIONBASIS_HPP
