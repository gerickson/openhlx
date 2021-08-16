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
 *      This file defines base, common (that is, independent of requestor
 *      or responder) command regular expression objects for HLX group
 *      commands.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDGROUPSREGULAREXPRESSIONBASES_HPP
#define OPENHLXCOMMONCOMMANDGROUPSREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace Groups
{

/**
 *  @brief
 *    Base regular expression object for HLX group decrease volume
 *    level command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group decrease volume
 *  level command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class DecreaseVolumeRegularExpressionBasis
{
protected:
    DecreaseVolumeRegularExpressionBasis(void) = default;
    virtual ~DecreaseVolumeRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group increase volume
 *    level command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group increase volume
 *  level command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class IncreaseVolumeRegularExpressionBasis
{
protected:
    IncreaseVolumeRegularExpressionBasis(void) = default;
    virtual ~IncreaseVolumeRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group volume mute
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group volume mute
 *  command.
 *
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class MuteRegularExpressionBasis
{
protected:
    MuteRegularExpressionBasis(void) = default;
    virtual ~MuteRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group name command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group name command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *
 */
class NameRegularExpressionBasis
{
protected:
    NameRegularExpressionBasis(void) = default;
    virtual ~NameRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group query command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group query command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *
 */
class QueryRegularExpressionBasis
{
protected:
    QueryRegularExpressionBasis(void) = default;
    virtual ~QueryRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group source (input)
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group source (input)
 *  command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *
 */
class SourceRegularExpressionBasis
{
protected:
    SourceRegularExpressionBasis(void) = default;
    virtual ~SourceRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group volume mute
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group volume mute
 *  command.
 *
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class ToggleMuteRegularExpressionBasis
{
protected:
    ToggleMuteRegularExpressionBasis(void) = default;
    virtual ~ToggleMuteRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX group volume level
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX group volume level
 *  command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class VolumeRegularExpressionBasis
{
protected:
    VolumeRegularExpressionBasis(void) = default;
    virtual ~VolumeRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

}; // namespace Groups

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDGROUPSREGULAREXPRESSIONBASES_HPP
