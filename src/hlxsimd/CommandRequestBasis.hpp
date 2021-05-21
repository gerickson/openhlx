/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      server command request regular expressions.
 *
 */

#ifndef HLXSERVERCOMMANDREQUESTBASIS_HPP
#define HLXSERVERCOMMANDREQUESTBASIS_HPP

#include <CommandRegularExpressionBasis.hpp>
#include <CommandRoleDelimitedRegularExpression.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    An abstract base object for composing HLX server command
 *    request regular expressions.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class RequestBasis :
    public Common::Command::RegularExpressionBasis,
    public Common::Command::RoleDelimitedRegularExpression
{
protected:
    RequestBasis(void) = default;
    virtual ~RequestBasis(void) = default;

    virtual Common::Status Init(const char *aRegexp, const size_t &aMatches) final;
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDREQUESTBASIS_HPP
