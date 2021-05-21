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
 *      This file implements a derived object for composing a HLX
 *      client command request buffer that performs a name data model
 *      set property mutation.
 *
 */

#include "CommandNameSetRequestBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer for a name set request operation.
 *
 *  This initializes the name property set request operation of a
 *  specified name against a specific object and identifier.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the name set operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the name set
 *                               operation is to be made against.
 *  @param[in]      aName        A pointer to the null-terminated C
 *                               string name to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameSetRequestBasis :: Init(const char *aObject,
                            const IdentifierModel::IdentifierType &aIdentifier,
                            const char *aName)
{
    return (NameSetBufferBasis::Init(*this, aObject, aIdentifier, aName));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
