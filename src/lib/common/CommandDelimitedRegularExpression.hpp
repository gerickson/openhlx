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
 *      This file defines a derived object for composing delimited
 *      (that is, initiated and terminated by a delimiting pair of
 *      characters) HLX command regular expressions.
 *
 */

#ifndef HLXCOMMONCOMMANDDELIMITEDREGULAREXPRESSION_HPP
#define HLXCOMMONCOMMANDDELIMITEDREGULAREXPRESSION_HPP

#include <stddef.h>

#include <OpenHLX/Common/CommandDelimiters.hpp>
#include <OpenHLX/Common/CommandRegularExpression.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing delimited HLX command regular
 *    expressions.
 *
 *  A derived object for composing delimited (that is, initiated and
 *  terminated by a delimiting pair of characters) HLX command regular
 *  expressions.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class DelimitedRegularExpression :
    public RegularExpression
{
protected:
    DelimitedRegularExpression(void) = default;
    virtual ~DelimitedRegularExpression(void) = default;

    Common::Status Init(const Delimiters &aDelimiters, const char *aRegexp, const size_t &aExpectedMatchCount);

private:
    // Explicitly hide base class initializers

    using RegularExpression::Init;
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDDELIMITEDREGULAREXPRESSION_HPP
