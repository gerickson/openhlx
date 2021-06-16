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
 *      This file defines an object for managing regular expression
 *      text pattern search and matching.
 *
 */

#ifndef HLXCOMMONREGULAREXPRESSION_HPP
#define HLXCOMMONREGULAREXPRESSION_HPP

#include <memory>
#include <string>
#include <vector>

// On Apple-defined and -supported platforms (that is, iOS/iPadOS/macOS/
// tvOS/watchOS), the platform-supplied regex.h defines regnexec as a
// Darwin-specific extension available since macOS 10.8 and iOS 6.0,
// that is needed by this regular expression library to perform
// regular expression matches on the non-null-terminated text-based
// HLX protocol.
//
// Otherwise, regnexec is supplied by the TRE regular expression
// library.

#if defined(__APPLE__) && __APPLE__
#include <regex.h>
#else
#include <tre/regex.h>
#endif /* defined(__APPLE__) && __APPLE__ */

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for managing regular expression text pattern search
 *    and matching.
 *
 *  @ingroup common
 *
 */
class RegularExpression
{
public:
    /**
     *  A collection of substring matches for the regular expression
     *  when matched against an input string.
     *
     */
    typedef std::vector<regmatch_t> Matches;

public:
    RegularExpression(void);
    RegularExpression(const RegularExpression &aRegularExpression);
    RegularExpression(RegularExpression &&aRegularExpression);
    ~RegularExpression(void);

    RegularExpression &operator =(const RegularExpression &aRegularExpression);
    RegularExpression &operator =(RegularExpression &&aRegularExpression);

    Status Init(const char *aRegexp);
    Status Init(const char *aRegexp, const size_t &aExpectedMatchCount, int aFlags);

    const char *GetRegexp(void) const;
    size_t GetExpectedMatchCount(void) const;

    Status Match(const char *aString) const;
    Status Match(const char *aString, const size_t &aLength) const;
    Status Match(const char *aString, Matches &aMatches) const;
    Status Match(const char *aString, const size_t &aLength, Matches &aMatches) const;

    bool operator <(const RegularExpression &aRegularExpression) const;

private:
    static Status Match(const regex_t * aPattern,
                        const char *    aString,
                        const size_t &  aLength,
                        const size_t &  aExpectedMatchCount,
                        regmatch_t *    aMatches);

private:
    std::string              mRegexp;
    std::shared_ptr<regex_t> mPattern;
    size_t                   mExpectedMatchCount;
};

namespace Utilities
{

extern size_t Distance(const regmatch_t &aMatch);

};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONREGULAREXPRESSION_HPP
