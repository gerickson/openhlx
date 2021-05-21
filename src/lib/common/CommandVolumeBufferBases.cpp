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
 *      commands that observe or set volume data model properties.
 *
 */

#include "CommandVolumeBufferBases.hpp"

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

static const char kVolumeProperty = 'V';
static const char kVolumeSetOperation = 'R';

/**
 *  @brief
 *    This is the class initializer for a volume level set operation
 *    for a single object.
 *
 *  This initializes the volume level property set operation of a
 *  specified volume level against a specific object and identifier
 *  into the specified command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the volume
 *                                     level set operation
 *                                     is to be made against. For
 *                                     example, "O" for a zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object dentifier which the
 *                                     volume level set
 *                                     operation is to be made
 *                                     against.
 *  @param[in]      aLevel             An immutable reference to the
 *                                     volume level to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeBufferBasis :: Init(BufferBasis &aBuffer,
                          const char *aObject,
                          const IdentifierModel::IdentifierType &aIdentifier,
                          const VolumeModel::LevelType &aLevel)
{
    return (PropertySetBufferBasis::Init(aBuffer,
                                         kVolumeProperty,
                                         aObject,
                                         aIdentifier,
                                         kVolumeSetOperation,
                                         aLevel));
}

/**
 *  @brief
 *    This is the class initializer for a volume level set
 *    operation for all objects.
 *
 *  This initializes the volume level property set operation of a
 *  specified volume level against all objects into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     objects for which the volume
 *                                     level set operation
 *                                     are to be made against. For
 *                                     example, "O" for all zone objects.
 *  @param[in]      aLevel             An immutable reference to the
 *                                     volume level to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeAllBufferBasis :: Init(BufferBasis &aBuffer,
                             const char *aObject,
                             const LevelType &aLevel)
{
    OutputStringStream  lVolumeStream;
    std::string         lBuffer;


    // Upcast the volume to a signed integer to ensure it is
    // interpretted as something to be converted rather than a
    // character literal.

    lVolumeStream << static_cast<int>(aLevel);

    lBuffer = kVolumeProperty;
    lBuffer += aObject;
    lBuffer += kVolumeSetOperation;
    lBuffer += lVolumeStream.str();

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer for a volume fixed/locked state
 *    set operation.
 *
 *  This initializes the volume fixed/locked state property set
 *  operation of a specified volume fixed/locked state against a
 *  specific object and identifier into the specified command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the volume
 *                                     fixed/locked state set operation
 *                                     is to be made against. For
 *                                     example, "O" for a zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object dentifier which the
 *                                     volume fixed/locked state set
 *                                     operation is to be made
 *                                     against.
 *  @param[in]      aFixed             An immutable reference to the
 *                                     volume fixed/locked state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeFixedBufferBasis :: Init(BufferBasis &aBuffer,
                               const char *aObject,
                               const IdentifierModel::IdentifierType &aIdentifier,
                               const VolumeModel::FixedType &aFixed)
{
    static const char kVolumeFixedOperation = 'F';

    return (PropertySetBufferBasis::Init(aBuffer,
                                         kVolumeProperty,
                                         aObject,
                                         aIdentifier,
                                         kVolumeFixedOperation,
                                         aFixed));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute set operation.
 *
 *  This initializes the volume mute property set operation of a
 *  specified volume mute against a specific object and identifier
 *  into the specified command buffer.
 *
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     command buffer to compose the
 *                                     operation into.
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the volume
 *                                     mute set operation
 *                                     is to be made against. For
 *                                     example, "O" for a zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object dentifier which the
 *                                     volume mute set operation is to
 *                                     be made against.
 *  @param[in]      aMute              An immutable reference to the
 *                                     volume mute to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteBufferBasis :: Init(BufferBasis &aBuffer,
                              const char *aObject,
                              const IdentifierModel::IdentifierType &aIdentifier,
                              const VolumeModel::MuteType &aMute)
{
    static const char * const  sMuteOperations[] = { "UM", "M" };
    OutputStringStream         lIdentifierStream;
    std::string                lBuffer;


    lIdentifierStream << aIdentifier;

    // Compose the buffer.

    lBuffer = kVolumeProperty;
    lBuffer += sMuteOperations[aMute];
    lBuffer += aObject;
    lBuffer += lIdentifierStream.str();

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
