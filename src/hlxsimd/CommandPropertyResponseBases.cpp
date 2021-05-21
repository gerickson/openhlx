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
 *      This file implements base objects for composing HLX response
 *      buffers for commands that observe or set a data model object
 *      property.
 *
 */

#include "CommandPropertyResponseBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/CommandBuffer.hpp>

using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

Status PropertyResponseBasis :: Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation)
{
    return (PropertyBufferBasis::Init(*this, aProperty, aObject, aIdentifier, aOperation));
}

Status PropertySetResponseBasis :: Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const bool &aValue)
{
    return (PropertySetBufferBasis::Init(*this, aProperty, aObject, aIdentifier, aOperation, aValue));
}

Status PropertySetResponseBasis :: Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const uint8_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this, aProperty, aObject, aIdentifier, aOperation, aValue));
}

Status PropertySetResponseBasis :: Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const int8_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this, aProperty, aObject, aIdentifier, aOperation, aValue));
}

Status PropertySetResponseBasis :: Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation, const uint16_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this, aProperty, aObject, aIdentifier, aOperation, aValue));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
