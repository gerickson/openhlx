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
 *      text pattern search and matching.
 *
 */

#include "RegularExpression.hpp"

#include <memory>

#include <errno.h>
#include <regex.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
RegularExpression :: RegularExpression(void) :
    mRegexp(),
    mPattern(),
    mExpectedMatchCount(0)
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aRegularExpression  An immutable reference to the
 *                                  regular expression to copy from.
 *
 */
RegularExpression :: RegularExpression(const RegularExpression &aRegularExpression) :
    mRegexp(aRegularExpression.mRegexp),
    mPattern(aRegularExpression.mPattern),
    mExpectedMatchCount(aRegularExpression.mExpectedMatchCount)
{
    return;
}

/**
 *  @brief
 *    This is a class move constructor.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      regular expression to move from.
 *
 */
RegularExpression :: RegularExpression(RegularExpression &&aRegularExpression) :
    mRegexp(std::move(aRegularExpression.mRegexp)),
    mPattern(std::move(aRegularExpression.mPattern)),
    mExpectedMatchCount(std::move(aRegularExpression.mExpectedMatchCount))
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
RegularExpression :: ~RegularExpression(void)
{
    if (mPattern)
    {
        regfree(mPattern.get());
    }
}

/**
 *  @brief
 *    This is a class copy assignment operator.
 *
 *  @param[in]  aRegularExpression  An immutable reference to the
 *                                  regular expression to assign by
 *                                  copy from.
 *
 *  @returns A mutable reference to the assigned object.
 */
RegularExpression &
RegularExpression :: operator =(const RegularExpression &aRegularExpression)
{
    mRegexp             = aRegularExpression.mRegexp;
    mPattern            = aRegularExpression.mPattern;
    mExpectedMatchCount = aRegularExpression.mExpectedMatchCount;

    return (*this);
}

/**
 *  @brief
 *    This is a class move assignment operator.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      regular expression to assign by
 *                                      move from.
 *
 *  @returns A mutable reference to the assigned object.
 */
RegularExpression &
RegularExpression :: operator =(RegularExpression &&aRegularExpression)
{
    mRegexp             = std::move(aRegularExpression.mRegexp);
    mPattern            = std::move(aRegularExpression.mPattern);
    mExpectedMatchCount = std::move(aRegularExpression.mExpectedMatchCount);

    return (*this);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the regular expression object with the specified
 *  regular expression and a default match count (zero (0)) and flags
 *  (none).
 *
 *  @param[in]  aRegexp  A pointer to a null-terminated C string
 *                       containing the regular expression pattern to
 *                       initialize with that will be used to match
 *                       against.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If @a aRegexp was null.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RegularExpression :: Init(const char *aRegexp)
{
    const size_t lExpectedMatchCount = 0;
    const int    lCompileFlags = 0;

    return (Init(aRegexp, lExpectedMatchCount, lCompileFlags));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the regular expression object with the specified
 *  regular expression and a default match count (zero (0)) and flags
 *  (none).
 *
 *  @param[in]  aRegexp              A pointer to a null-terminated C
 *                                   string containing the regular
 *                                   expression pattern to initialize
 *                                   with that will be used to match
 *                                   against.
 *  @param[in]  aExpectedMatchCount  The number of expected substring
 *                                   matches for @a aRegexp when @a
 *                                   Match successfully matches.
 *  @param[in]  aFlags               The regular expression compilation
 *                                   flags used to compile the @a aRegexp.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If @a aRegexp was null.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 *  @sa Match
 *
 */
Status
RegularExpression :: Init(const char *   aRegexp,
                          const size_t & aExpectedMatchCount,
                          int            aFlags)
{
    const int  lCompileFlags = (REG_EXTENDED | aFlags);
    int        lStatus;
    Status     lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aRegexp != nullptr, done, lRetval = -EINVAL);

    mPattern.reset(new regex_t);
    nlREQUIRE_ACTION(mPattern, done, lRetval = -ENOMEM);

    // Pre-compile the regular expression pattern for matching the
    // pattern.

    lStatus = regcomp(mPattern.get(), aRegexp, lCompileFlags);
    nlREQUIRE_ACTION(lStatus == 0, done, lRetval = kError_InitializationFailed);

    mRegexp             = aRegexp;
    mExpectedMatchCount = aExpectedMatchCount;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the regular expression pattern.
 *
 *  @returns
 *    The pointer to the null-terminated C string containing the
 *    regular expression pattern for the object.
 *
 */
const char *
RegularExpression :: GetRegexp(void) const
{
    return (mRegexp.c_str());
}

/**
 *  @brief
 *    Return the expected regular expression pattern substring match count.
 *
 *  @returns
 *    The expected regular expression pattern substring match count.
 *
 */
size_t
RegularExpression :: GetExpectedMatchCount(void) const
{
    return (mExpectedMatchCount);
}

/**
 *  @brief
 *    Attempt to match a null-terminated C string against the
 *    regular expression pattern associated with the object.
 *
 *  This attempts to match the specified null-terminated C string
 *  against the compiled regular expression pattern associated with
 *  the object.
 *
 *  @param[in]  aString  A pointer to a null-terminated C string to
 *                       match against the regular expression pattern.
 *
 *  @retval  kStatus_Success  If successful; otherwise, see regex(3)
 *                            diagnostic codes for other errors.
 *
 */
Status
RegularExpression :: Match(const char *aString) const
{
    return (Match(aString, strlen(aString)));
}

/**
 *  @brief
 *    Attempt to match a string extent against the regular expression
 *    pattern associated with the object.
 *
 *  This attempts to match the specified string extent against the
 *  compiled regular expression pattern associated with the object.
 *
 *  @param[in]  aString  A pointer to the start of the string extent to
 *                       match against the regular expression pattern.
 *  @param[in]  aLength  The length, in bytes, of the string extent.
 *
 *  @retval  kStatus_Success  If successful; otherwise, see regex(3)
 *                            diagnostic codes for other errors.
 *
 */
Status
RegularExpression :: Match(const char *aString, const size_t &aLength) const
{
    return (RegularExpression::Match(mPattern.get(), aString, aLength, 0, nullptr));
}

/**
 *  @brief
 *    Attempt to match a null-terminated C string against the
 *    regular expression pattern associated with the object and
 *    return the resulting substring matches.
 *
 *  This attempts to match the specified null-terminated C string
 *  against the compiled regular expression pattern associated with
 *  the object and return the resulting substring matches.
 *
 *  @param[in]      aString   A pointer to a null-terminated C string
 *                            to match against the regular expression
 *                            pattern.
 *  @param[in,out]  aMatches  A mutable reference to the substring match
 *                            collection to populate with the substring
 *                            matches on success.
 *
 *  @retval  kStatus_Success  If successful; otherwise, see regex(3)
 *                            diagnostic codes for other errors.
 *
 */
Status
RegularExpression :: Match(const char *aString, Matches &aMatches) const
{
    return (Match(aString, strlen(aString), aMatches));
}

/**
 *  @brief
 *    Attempt to match a string extent against the regular expression
 *    pattern associated with the object and return the resulting
 *    substring matches.
 *
 *  This attempts to match the specified string extent against the
 *  compiled regular expression pattern associated with the object and
 *  return the resulting substring matches.
 *
 *  @param[in]      aString   A pointer to the start of the string extent
 *                            to match against the regular expression
 *                            pattern.
 *  @param[in]      aLength   The length, in bytes, of the string extent.
 *  @param[in,out]  aMatches  A mutable reference to the substring match
 *                            collection to populate with the substring
 *                            matches on success.
 *
 *  @retval  kStatus_Success  If successful; otherwise, see regex(3)
 *                            diagnostic codes for other errors.
 *
 */
Status
RegularExpression :: Match(const char *aString, const size_t &aLength, Matches &aMatches) const
{
    Status lRetval;

    if ((mExpectedMatchCount > 0) && (aMatches.capacity() < mExpectedMatchCount))
    {
        regmatch_t lMatch;

        aMatches.reserve(mExpectedMatchCount);
        aMatches.assign(mExpectedMatchCount, lMatch);
    }

    lRetval = RegularExpression::Match(mPattern.get(), aString, aLength, mExpectedMatchCount, &aMatches.at(0));

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to match a string extent against the provided compiled
 *    regular expression pattern associated and return the resulting
 *    substring matches.
 *
 *  This attempts to match the specified string extent against the
 *  provided compiled regular expression pattern and return the
 *  resulting substring matches.
 *
 *  @param[in]      aPattern             An immutable pointer to the
 *                                       compiled regular expression
 *                                       to match against.
 *  @param[in]      aString              A pointer to the start of
 *                                       the string extent to match
 *                                       against the regular
 *                                       expression pattern.
 *  @param[in]      aLength              The length, in bytes, of the
 *                                       string extent.
 *  @param[in]      aExpectedMatchCount  The number of expected substring
 *                                       matches and the size of the
 *                                       array pointed to by @a
 *                                       aMatches.
 *  @param[in,out]  aMatches             A pointer to the substring
 *                                       match array to populate with
 *                                       the substring matches on
 *                                       success.
 *
 *  @retval  kStatus_Success  If successful; otherwise, see regex(3)
 *                            diagnostic codes for other errors.
 *
 */
Status
RegularExpression :: Match(const regex_t * aPattern,
                           const char *    aString,
                           const size_t &  aLength,
                           const size_t &  aExpectedMatchCount,
                           regmatch_t *    aMatches)
{
    const int lExecFlags = 0;
    Status    lRetval;

    lRetval = regnexec(aPattern, aString, aLength, aExpectedMatchCount, aMatches, lExecFlags);

    return (lRetval);
}

/**
 *  @brief
 *    This is a class less than comparison operator.
 *
 *  This compares the provided regular expression and its pattern
 *  against this one to determine if, lexicographically, this one is
 *  less than the other.
 *
 *  This is useful as a sorting predicate for regular expressions when
 *  an input string is compared against a collection of regular
 *  expressions for a match against any of them.
 *
 *  @param[in]  aRegularExpression  An immutable reference to the regular
 *                                  expression to compare.
 *
 *  @returns
 *    True if this regular expression pattern is lexicographically
 *    less than the specified one; otherwise, false.
 *
 */
bool
RegularExpression :: operator <(const RegularExpression &aRegularExpression) const
{
    return (mRegexp < aRegularExpression.mRegexp);
}

namespace Utilities
{

/**
 *  @brief
 *    Return the distance, in bytes, between end and start offset of a
 *    substring match.
 *
 *  @param[in]  aMatch  An immutable reference to the substring match
 *                      structure to return the distance of.
 *
 *  @returns
 *    The distance, in bytes, between the end and start offset of the
 *    specified substring match.
 *
 */
size_t
Distance(const regmatch_t &aMatch)
{
    const size_t lDistance = static_cast<const size_t>(aMatch.rm_eo - aMatch.rm_so);

    return (lDistance);
}

};

}; // namespace Common

}; // namespace HLX
