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

/**
 *  @brief
 *    This is the class initializer for a name set command response.
 *
 *  This initializes the name property set command response of a
 *  specified name against a specific object and identifier.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the name set operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier for which the name response
 *                               is to be formed.
 *  @param[in]      aName        A pointer to the null-terminated C
 *                               string name that was set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameSetResponseBasis :: Init(const char *aObject,
                             const IdentifierModel::IdentifierType &aIdentifier,
                             const char *aName)
{
    return (NameSetBufferBasis::Init(*this, aObject, aIdentifier, aName));
}

/**
 *  @brief
 *    This is the class initializer for a name set command response.
 *
 *  This initializes the name property set command response of a
 *  specified name against a specific object and identifier.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the name set operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier for which the name response
 *                               is to be formed.
 *  @param[in]      aName        A pointer to the start of the string
 *                               name that was set.
 *  @param[in]      aNameLength  An immutable reference to the length,
 *                               in bytes, of @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameSetResponseBasis :: Init(const char *aObject,
                             const IdentifierModel::IdentifierType &aIdentifier,
                             const char *aName,
                             const size_t &aNameLength)
{
    return (NameSetBufferBasis::Init(*this, aObject, aIdentifier, aName, aNameLength));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
