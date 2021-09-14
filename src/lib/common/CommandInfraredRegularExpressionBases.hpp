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
 *      HLX infrared remote control commands.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDINFRAREDREGULAREXPRESSIONBASES_HPP
#define OPENHLXCOMMONCOMMANDINFRAREDREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace Infrared
{

/**
 *  @brief
 *    Base regular expression object for HLX infrared remote control
 *    disabled property command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX infrared remote
 *  control disabled property command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class DisabledRegularExpressionBasis
{
protected:
    DisabledRegularExpressionBasis(void) = default;
    virtual ~DisabledRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

}; // namespace Infrared

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDINFRAREDREGULAREXPRESSIONBASES_HPP
