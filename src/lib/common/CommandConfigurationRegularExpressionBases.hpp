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
 *      This file defines base, common (that is, independent of
 *      requestor or responder) command regular expression objects for
 *      HLX configuration commands.
 *
 */

#ifndef HLXCOMMONCOMMANDCONFIGURATIONREGULAREXPRESSIONBASES_HPP
#define HLXCOMMONCOMMANDCONFIGURATIONREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace Configuration
{

/**
 *  @brief
 *    Base regular expression object for HLX configuration load from
 *    backup command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX configuration load
 *  from backup command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackupRegularExpressionBasis
{
protected:
    LoadFromBackupRegularExpressionBasis(void) = default;
    virtual ~LoadFromBackupRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX configuration query
 *    current command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX configuration query
 *  current command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrentRegularExpressionBasis
{
protected:
    QueryCurrentRegularExpressionBasis(void) = default;
    virtual ~QueryCurrentRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX configuration reset to
 *    defaults command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX configuration reset
 *  to defaults command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaultsRegularExpressionBasis
{
protected:
    ResetToDefaultsRegularExpressionBasis(void) = default;
    virtual ~ResetToDefaultsRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX configuration save to
 *    backup command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX configuration save to
 *  backup command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackupRegularExpressionBasis
{
protected:
    SaveToBackupRegularExpressionBasis(void) = default;
    virtual ~SaveToBackupRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDCONFIGURATIONREGULAREXPRESSIONBASES_HPP
