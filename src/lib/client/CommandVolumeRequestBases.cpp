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
 *      This file implements derivable objects for HLX client volume data
 *      model properties mutation command request buffers.
 *
 */

#include "CommandVolumeRequestBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/CommandBuffer.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

static const char kVolumeProperty = 'V';

/**
 *  @brief
 *    This is the class initializer for a volume level mutation
 *    request operation.
 *
 *  This initializes a volume level property mutation request
 *  operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume level mutation request is to be
 *                           made against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume level
 *                           mutation request is to be made against.
 *  @param[in]  aOperation   A pointer to a null-terminated C string
 *                           representing the mutation request
 *                           operation to perform.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeRequestBasis :: Init(const char *aObject,
                           const IdentifierModel::IdentifierType &aIdentifier,
                           const char *aOperation)
{
    return (PropertyRequestBasis::Init(kVolumeProperty,
                                       aObject,
                                       aIdentifier,
                                       aOperation));
}

/**
 *  @brief
 *    This is the class initializer for a volume level increase
 *    mutation request operation.
 *
 *  This initializes a volume level property increase mutation request
 *  operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume level increase mutation request is
 *                           to be made against. For example, "O" for
 *                           a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume level
 *                           increase mutation request is to be made
 *                           against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeIncreaseRequestBasis :: Init(const char *aObject,
                                   const IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kVolumeIncreaseOperation = "U";

    return (VolumeRequestBasis::Init(aObject,
                                     aIdentifier,
                                     kVolumeIncreaseOperation));
}

/**
 *  @brief
 *    This is the class initializer for a volume level decrease
 *    mutation request operation.
 *
 *  This initializes a volume level property decrease mutation request
 *  operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume level decrease mutation request is
 *                           to be made against. For example, "O" for
 *                           a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume level
 *                           decrease mutation request is to be made
 *                           against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeDecreaseRequestBasis :: Init(const char *aObject,
                                   const IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kVolumeDecreaseOperation = "D";

    return (VolumeRequestBasis::Init(aObject,
                                     aIdentifier,
                                     kVolumeDecreaseOperation));
}

/**
 *  @brief
 *    This is the class initializer for a volume level set mutation
 *    request operation.
 *
 *  This initializes a volume level property set mutation request
 *  operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume level set mutation request is
 *                           to be made against. For example, "O" for
 *                           a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume level
 *                           set mutation request is to be made
 *                           against.
 *  @param[in]  aLevel       An immutable reference to the
 *                           volume level to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeSetRequestBasis :: Init(const char *aObject,
                              const IdentifierType &aIdentifier,
                              const LevelType &aLevel)
{
    return (VolumeBufferBasis::Init(*this,
                                    aObject,
                                    aIdentifier,
                                    aLevel));
}

/**
 *  @brief
 *    This is the class initializer for a volume fixed/locked state
 *    set mutation request operation.
 *
 *  This initializes a volume fixed/locked state property set mutation
 *  request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume fixed/locked state set mutation
 *                           request is to be made against. For
 *                           example, "O" for a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume fixed/locked
 *                           state set mutation request is to be made
 *                           against.
 *  @param[in]  aFixed       An immutable reference to the volume
 *                           fixed/locked state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeFixedRequestBasis :: Init(const char *aObject,
                                const IdentifierType &aIdentifier,
                                const FixedType &aFixed)
{
    return (VolumeFixedBufferBasis::Init(*this,
                                         aObject,
                                         aIdentifier,
                                         aFixed));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute mutation request
 *    operation.
 *
 *  This initializes a volume mute property mutation request operation
 *  against a specific object identifier.
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume mute mutation request is to be
 *                           made against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume mute
 *                           mutation request is to be made against.
 *  @param[in]  aOperation   A pointer to a null-terminated C string
 *                           representing the mutation request
 *                           operation to perform.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteRequestBasis :: Init(const char *aObject,
                               const IdentifierModel::IdentifierType &aIdentifier,
                               const char *aOperation)
{
    OutputStringStream  lIdentifierStream;
    std::string         lBuffer;


    lIdentifierStream << aIdentifier;

    // Compose the buffer.

    lBuffer = kVolumeProperty;
    lBuffer += aOperation;
    lBuffer += aObject;
    lBuffer += lIdentifierStream.str();

    return (RequestBasis::Init(lBuffer.c_str(),
                               lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute set
 *    (assert/enable) mutation request operation.
 *
 *  This initializes a volume mute property set (assert/enable)
 *  mutation request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume mute set (assert/enable) mutation
 *                           request is to be made against. For
 *                           example, "O" for a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume mute
 *                           set (assert/enable) mutation request is
 *                           to be made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteSetRequestBasis :: Init(const char *aObject,
                                  const IdentifierModel::IdentifierType &aIdentifier)
{
    static const MuteType kMute = true;

    return (VolumeMuteBufferBasis::Init(*this,
                                        aObject,
                                        aIdentifier,
                                        kMute));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute set
 *    (deassert/disable) mutation request operation.
 *
 *  This initializes a volume mute property set (deassert/disable)
 *  mutation request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume mute set (deassert/disable)
 *                           mutation request is to be made
 *                           against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume mute set
 *                           (deassert/disable) mutation request is to
 *                           be made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteClearRequestBasis :: Init(const char *aObject,
                                    const IdentifierModel::IdentifierType &aIdentifier)
{
    static const MuteType kMute = true;

    return (VolumeMuteBufferBasis::Init(*this,
                                        aObject,
                                        aIdentifier,
                                        !kMute));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute toggle (flip)
 *    mutation request operation.
 *
 *  This initializes a volume mute property toggle (flip)
 *  mutation request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume mute toggle (flip) mutation
 *                           request is to be made against. For
 *                           example, "O" for a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the volume mute
 *                           toggle (flip) mutation request is to be
 *                           made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteToggleRequestBasis :: Init(const char *aObject,
                                     const IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kMuteToggleOperation = "MT";

    return (VolumeMuteRequestBasis::Init(aObject,
                                         aIdentifier,
                                         kMuteToggleOperation));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
