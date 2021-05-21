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
 *      This file implements derivable objects for HLX client data model
 *      property mutation command request buffers.
 *
 */

#include "CommandPropertyRequestBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBuffer.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an object property mutation request operation
 *  against a specific object property and identifier.
 *
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property request operation
 *                               is to be made against. For example,
 *                               "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               request operation is to be made
 *                               against.
 *  @param[in]      aOperation   A pointer to a null-terminated C
 *                               string representing the request
 *                               operation to perform. This might be
 *                               an increment left, increment right,
 *                               center, or set to a specific level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertyRequestBasis :: Init(char aProperty,
                             const char *aObject,
                             const IdentifierType &aIdentifier,
                             const char *aOperation)
{
    return (PropertyBufferBasis::Init(*this,
                                      aProperty,
                                      aObject,
                                      aIdentifier,
                                      aOperation));
}

/**
 *  @brief
 *    This is the class initializer for a single character request
 *    operation of a Boolean value.
 *
 *  This initializes an object property set request operation of a
 *  specified value against a specific object property and identifier.
 *
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property request operation
 *                               is to be made against. For example,
 *                               "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               request operation is to be made
 *                               against.
 *  @param[in]      aOperation   The set request operation to perform.
 *  @param[in]      aValue       An immutable reference to the Boolean
 *                               value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetRequestBasis :: Init(char aProperty,
                                const char *aObject,
                                const IdentifierType &aIdentifier,
                                char aOperation,
                                const bool &aValue)
{
    return (PropertySetBufferBasis::Init(*this,
                                         aProperty,
                                         aObject,
                                         aIdentifier,
                                         aOperation,
                                         aValue));
}

/**
 *  @brief
 *    This is the class initializer for a single character request
 *    operation of a unsigned 8-bit value.
 *
 *  This initializes an object property set request operation of a
 *  specified value against a specific object property and identifier.
 *
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property request operation
 *                               is to be made against. For example,
 *                               "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               request operation is to be made
 *                               against.
 *  @param[in]      aOperation   The set request operation to perform.
 *  @param[in]      aValue       An immutable reference to the unsigned
 *                               8-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetRequestBasis :: Init(char aProperty,
                                const char *aObject,
                                const IdentifierType &aIdentifier,
                                char aOperation,
                                const uint8_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this,
                                         aProperty,
                                         aObject,
                                         aIdentifier,
                                         aOperation,
                                         aValue));
}

/**
 *  @brief
 *    This is the class initializer for a single character request
 *    operation of a signed 8-bit value.
 *
 *  This initializes an object property set request operation of a
 *  specified value against a specific object property and identifier.
 *
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property request operation
 *                               is to be made against. For example,
 *                               "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               request operation is to be made
 *                               against.
 *  @param[in]      aOperation   The set request operation to perform.
 *  @param[in]      aValue       An immutable reference to the signed
 *                               8-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetRequestBasis :: Init(char aProperty,
                                const char *aObject,
                                const IdentifierType &aIdentifier,
                                char aOperation,
                                const int8_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this,
                                         aProperty,
                                         aObject,
                                         aIdentifier,
                                         aOperation,
                                         aValue));
}

/**
 *  @brief
 *    This is the class initializer for a multi-character request
 *    operation of an unsigned 16-bit value.
 *
 *  This initializes an object property set request operation of a
 *  specified value against a specific object property and identifier
 *  into the specified command buffer.
 *
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property request operation
 *                               is to be made against. For example,
 *                               "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               request operation is to be made
 *                               against.
 *  @param[in]      aOperation   A pointer to a null-terminated C
 *                               string representing the set request
 *                               operation to perform.
 *  @param[in]      aValue       An immutable reference to the unsigned
 *                               16-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetRequestBasis :: Init(char aProperty,
                                const char *aObject,
                                const IdentifierType &aIdentifier,
                                const char *aOperation,
                                const uint16_t &aValue)
{
    return (PropertySetBufferBasis::Init(*this,
                                         aProperty,
                                         aObject,
                                         aIdentifier,
                                         aOperation,
                                         aValue));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
