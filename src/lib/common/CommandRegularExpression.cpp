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
 *      This file implements an object for managing regular expression
 *      text pattern search and matching used in the context of an HLX
 *      command.
 *
 */

#include <string>

#include <errno.h>

#include "CommandRegularExpression.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

namespace Command
{

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
 *                                   matches for @a aRegexp when the
 *                                   pattern successfully matches.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If @a aRegexp was null.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RegularExpression :: Init(const char *aRegexp, const size_t &aExpectedMatchCount)
{
    const int    lFlags  = 0;
    Status       lRetval = kStatus_Success;


    lRetval = mRegexp.Init(aRegexp, aExpectedMatchCount, lFlags);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aExpectedMatchCount > 0)
    {
        const regmatch_t lMatch = { 0, 0 };

        mMatches.reserve(aExpectedMatchCount);
        mMatches.assign(aExpectedMatchCount, lMatch);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the regular expression associated with the command.
 *
 *  @returns
 *    An immutable reference to the command regular expression.
 *
 */
const Common::RegularExpression &
RegularExpression :: GetRegularExpression(void) const
{
    return (mRegexp);
}

/**
 *  @brief
 *    Return the regular expression matches associated with the
 *    command.
 *
 *  @returns
 *    A mutable reference to the command regular expression matches.
 *
 */
Common::RegularExpression::Matches &
RegularExpression :: GetMatches(void)
{
    return (mMatches);
}

/**
 *  @brief
 *    Return the connection buffer associated with the command.
 *
 *  @returns
 *    A mutable pointer to the command connection buffer.
 *
 */
const ConnectionBuffer *
RegularExpression :: GetBuffer(void) const
{
    return (mBuffer.get());
}

/**
 *  @brief
 *    Associate a connection buffer with the command.
 *
 *  This associates the specified connection buffer with the command
 *  and retains a reference to the connection buffer.
 *
 *  @param[in]  aBuffer  An immutable shared reference to the connection
 *                       buffer to associate with the command.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RegularExpression :: SetBuffer(ConnectionBuffer::ImmutableCountedPointer &aBuffer)
{
    Status    lRetval = kStatus_Success;

    mBuffer = aBuffer;

    return (lRetval);
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
