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
 *      This file implements a derivable object for a HLX server name
 *      data model set property mutation command response buffer.
 *
 */

#include "CommandNameSetResponseBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Command
{

Status NameSetResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const char *aName)
{
    return (NameSetBufferBasis::Init(*this, aObject, aIdentifier, aName));
}

Status NameSetResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const char *aName, const size_t &aNameLength)
{
    return (NameSetBufferBasis::Init(*this, aObject, aIdentifier, aName, aNameLength));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
