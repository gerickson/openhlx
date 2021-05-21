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
 *      characters) HLX command regular expressions, where the
 *      delimiters used are based on an enumerated role designation.
 *
 */

#ifndef HLXCOMMONCOMMANDROLEDELIMITEDREGULAREXPRESSION_HPP
#define HLXCOMMONCOMMANDROLEDELIMITEDREGULAREXPRESSION_HPP

#include <CommandDelimitedRegularExpression.hpp>
#include <CommandRole.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing delimited HLX command buffers,
 *    based on the role composing the command.
 *
 *  A derived object for composing delimited (that is, initiated and
 *  terminated by a delimiting pair of characters) HLX command
 *  buffers, where the delimiters used are based on an enumerated role
 *  designation chosen by the command composer.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class RoleDelimitedRegularExpression :
    public DelimitedRegularExpression
{
protected:
    RoleDelimitedRegularExpression(void) = default;
    virtual ~RoleDelimitedRegularExpression(void) = default;

    Common::Status Init(const Role &aRole, const char *aRegexp, const size_t &aExpectedMatchCount);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDDELIMITEDREGULAREXPRESSION_HPP
