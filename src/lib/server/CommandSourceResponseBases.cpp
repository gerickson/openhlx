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
 *      This file implements derivable objects for HLX server source
 *      (input) data model property mutation command response buffers.
 *
 */

#include "CommandSourceResponseBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

Status SourceResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const char *aOperation)
{
    return (SourceBufferBasis::Init(*this, aObject, aIdentifier, aOperation));
}

Status SourceAllResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aSourceIdentifier)
{
    return (SourceAllBufferBasis::Init(*this, aObject, aSourceIdentifier));
}

Status SourceSetResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const IdentifierModel::IdentifierType &aSource)
{
    return (SourceSetBufferBasis::Init(*this, aObject, aIdentifier, aSource));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX