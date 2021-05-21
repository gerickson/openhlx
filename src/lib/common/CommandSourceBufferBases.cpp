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
 *      This file implements base and derived objects for composing HLX
 *      commands that observe or set source (input) data model
 *      properties.
 *
 */

#include "CommandSourceBufferBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Common
{

namespace Command
{

static const char kSourceProperty     = 'C';
static const char kSourceSetOperation = 'I';

/**
 *  @brief
 *    This is the class initializer for a source (input) mutation
 *     operation.
 *
 *  This initializes a source (input) property mutation
 *  operation against a specific object identifier into
 *  the specified command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the source
 *                                     (input) mutation is to be made
 *                                     against. For example, "O" for a
 *                                     zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object identifier which the
 *                                     source (input) mutation is to
 *                                     be made against.
 *  @param[in]      aOperation         A pointer to a null-terminated
 *                                     C string representing the
 *                                     mutation operation to perform.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceBufferBasis :: Init(BufferBasis &aBuffer,
                          const char *aObject,
                          const IdentifierType &aIdentifier,
                          const char *aOperation)
{
    return (PropertyBufferBasis::Init(aBuffer,
                                      kSourceProperty,
                                      aObject,
                                      aIdentifier,
                                      aOperation));
}

/**
 *  @brief
 *    This is the class initializer for a source (input) set
 *    operation for all objects.
 *
 *  This initializes the source (input) property set operation of a
 *  specified source against all objects into the specified command
 *  buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     objects for which the source
 *                                     (input) set operation
 *                                     are to be made against. For
 *                                     example, "O" for all zone objects.
 *  @param[in]      aSourceIdentifier  An immutable reference to the
 *                                     source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceAllBufferBasis :: Init(BufferBasis &aBuffer,
                             const char *aObject,
                             const IdentifierType &aSourceIdentifier)
{
    OutputStringStream  lSourceIdentifierStream;
    std::string         lBuffer;


    lSourceIdentifierStream << aSourceIdentifier;

    // Compose the buffer.

    lBuffer = kSourceProperty;
    lBuffer += aObject;
    lBuffer += kSourceSetOperation;
    lBuffer += lSourceIdentifierStream.str();

    return (aBuffer.Init(lBuffer.c_str(),
                         lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer for a source (input) set
 *    operation for a single object.
 *
 *  This initializes the source (input) property set operation of a
 *  specified source against a specific object and identifier into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the source
 *                                     (input) set operation
 *                                     is to be made against. For
 *                                     example, "O" for a zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object dentifier which the
 *                                     source (input) set
 *                                     operation is to be made
 *                                     against.
 *  @param[in]      aSourceIdentifier  An immutable reference to the
 *                                     source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceSetBufferBasis :: Init(BufferBasis &aBuffer,
                             const char *aObject,
                             const IdentifierType &aIdentifier,
                             const IdentifierType &aSourceIdentifier)
{
    return (PropertySetBufferBasis::Init(aBuffer,
                                         kSourceProperty,
                                         aObject,
                                         aIdentifier,
                                         kSourceSetOperation,
                                         aSourceIdentifier));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
