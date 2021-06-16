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
 *      command buffer that performs a name data model set property
 *      mutation.
 *
 */

#include "CommandNameSetBufferBasis.hpp"

#include <algorithm>
#include <string>

#include <errno.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Model;
using namespace Nuovations;


namespace HLX
{

namespace Common
{

namespace Command
{

static const char   kNameProperty = 'N';
static const char   kNameStartDelimiter = '"';
static const char   kNameEndDelimiter = '"';

static const size_t kNameLengthMax = 16;

/**
 *  @brief
 *    This is a class initializer for a name set operation.
 *
 *  This initializes the name property set operation of a specified
 *  name against a specific object and identifier into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
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
NameSetBufferBasis :: Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aName)
{
    return (Init(aBuffer, aObject, aIdentifier, aName, strlen(aName)));
}

/**
 *  @brief
 *    This is a class initializer for a name set operation.
 *
 *  This initializes the name property set operation of a specified
 *  name extent against a specific object and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the name set operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the name set
 *                               operation is to be made against.
 *  @param[in]      aName        A pointer to the start of the null-
 *                               terminated C string name to set.
 *  @param[in]      aNameLength  An immutable reference to the length,
 *                               in bytes, of @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameSetBufferBasis :: Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aName, const size_t &aNameLength)
{
    const size_t  lNameLength = ((aName == nullptr) ? 0 : aNameLength);
    std::string   lName;
    Status        lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aObject != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lName += kNameStartDelimiter;

    lName.append(aName, std::min(lNameLength, kNameLengthMax));

    lName += kNameEndDelimiter;

    lRetval = PropertyBufferBasis::Init(aBuffer, kNameProperty, aObject, aIdentifier, lName.c_str());

 done:
    return (lRetval);
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
