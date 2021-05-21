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
 *      This file implements an abstract base object for composing HLX
 *      server command request regular expressions.
 *
 */

#include "CommandRequestBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <CommandRoleDelimiters.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;
using namespace HLX::Common::Command;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Command
{

Status RequestBasis :: Init(const char *aRegexp, const size_t &aMatches)
{
    static constexpr Role kRole = Role::kRequestor;

    return (RoleDelimitedRegularExpression::Init(kRole, aRegexp, aMatches));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
