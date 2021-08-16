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
 *      HLX equalizer presets commands.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDEQUALIZERPRESETSREGULAREXPRESSIONBASES_HPP
#define OPENHLXCOMMONCOMMANDEQUALIZERPRESETSREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace EqualizerPresets
{

/**
 *  @brief
 *    Base regular expression object for HLX equalizer preset band
 *    level property command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX equalizer preset band
 *  level property command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class BandLevelRegularExpressionBasis
{
protected:
    BandLevelRegularExpressionBasis(void) = default;
    virtual ~BandLevelRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX equalizer preset name
 *    property command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX equalizer preset name
 *  property command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup equalizer-preset
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
 *    Base regular expression object for HLX equalizer preset query
 *    command.
 *
 *  This defines a base, common (that is, independent of client or
 *  server) regular expression object for HLX equalizer preset query
 *  command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup equalizer-preset
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

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDEQUALIZERPRESETSREGULAREXPRESSIONBASES_HPP
