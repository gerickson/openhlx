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
 *      This file implements base objects for composing HLX commands that
 *      observe or set a data model object property.
 *
 */

#include "CommandPropertyBufferBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an object property mutation operation against a
 *  specific object property and identifier into the specified command
 *  buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   A pointer to a null-terminated C
 *                               string representing the operation to
 *                               perform. This might be an increment
 *                               left, increment right, center, or set
 *                               to a specific level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertyBufferBasis :: Init(BufferBasis &          aBuffer,
                            char                   aProperty,
                            const char *           aObject,
                            const IdentifierType & aIdentifier,
                            const char *           aOperation)
{
    OutputStringStream  lIdentifierStream;
    std::string         lBuffer;

    // Upcast the identifier to an unsigned integer to ensure it is
    // interpretted as something to be converted rather than a
    // character literal.

    lIdentifierStream << aIdentifier;

    // Compose the buffer with the property, the object the property
    // belongs to, the object identifier, and the property operation
    // (for example, increment, set, etc.).

    lBuffer = aProperty;
    lBuffer += aObject;
    lBuffer += lIdentifierStream.str();
    lBuffer += aOperation;

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer for a single character set
 *    operation of a signed integer value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the signed
 *                               integer value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               char aOperation,
                               const int &aValue)
{
    const char lOperation[2] = { aOperation, '\0' };

    return (Init(aBuffer,
                 aProperty,
                 aObject,
                 aIdentifier,
                 &lOperation[0],
                 aValue));
}

/**
 *  @brief
 *    This is the class initializer for a single character set
 *    operation of a Boolean value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the Boolean
 *                               value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               char aOperation,
                               const bool &aValue)
{
    return (Init(aBuffer,
                 aProperty,
                 aObject,
                 aIdentifier,
                 aOperation,
                 static_cast<int>(aValue)));
}

/**
 *  @brief
 *    This is the class initializer for a single character set
 *    operation of a unsigned 8-bit value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the unsigned
 *                               8-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               char aOperation,
                               const uint8_t &aValue)
{
    return (Init(aBuffer,
                 aProperty,
                 aObject,
                 aIdentifier,
                 aOperation,
                 static_cast<int>(aValue)));
}

/**
 *  @brief
 *    This is the class initializer for a single character set
 *    operation of a signed 8-bit value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the signed
 *                               8-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               char aOperation,
                               const int8_t &aValue)
{
    return (Init(aBuffer,
                 aProperty,
                 aObject,
                 aIdentifier,
                 aOperation,
                 static_cast<int>(aValue)));
}

/**
 *  @brief
 *    This is the class initializer for a multi-character set
 *    operation of a signed integer value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the signed
 *                               integer value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               const char *aOperation,
                               const int &aValue)
{
    OutputStringStream  lValueStream;
    std::string         lOperationString;


    lValueStream << aValue;

    // Compose the buffer.

    lOperationString = aOperation + lValueStream.str();

    return (PropertyBufferBasis::Init(aBuffer,
                                      aProperty,
                                      aObject,
                                      aIdentifier,
                                      lOperationString.c_str()));
}

/**
 *  @brief
 *    This is the class initializer for a multi-character set
 *    operation of a unsigned 16-bit value.
 *
 *  This initializes an object property set operation of a specified
 *  value against a specific object property and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property on the object to mutate.
 *                               For example, 'B' for the balance
 *                               property.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the property operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the property
 *                               operation is to be made against.
 *  @param[in]      aOperation   The set operation to perform.
 *  @param[in]      aValue       An immutable reference to the unsigned
 *                               16-bit value to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
PropertySetBufferBasis :: Init(BufferBasis &aBuffer,
                               char aProperty,
                               const char *aObject,
                               const IdentifierType &aIdentifier,
                               const char *aOperation,
                               const uint16_t &aValue)
{
    return (Init(aBuffer,
                 aProperty,
                 aObject,
                 aIdentifier,
                 aOperation,
                 static_cast<int>(aValue)));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
