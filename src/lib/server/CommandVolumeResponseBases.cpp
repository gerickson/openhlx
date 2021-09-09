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
 *      This file implements derivable objects for HLX server volume
 *      data model properties mutation command response buffers.
 *
 */

#include "CommandVolumeResponseBases.hpp"

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

/**
 *  @brief
 *    This is the class initializer for a volume level response.
 *
 *  This initializes a volume level property response against a
 *  specific object identifier.
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume level response is to be
 *                           formed. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier for which the volume level
 *                           response is to be formed.
 *  @param[in]  aLevel       An immutable reference to the
 *                           volume level in the response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const LevelType &aVolume)
{
    return (VolumeBufferBasis::Init(*this, aObject, aIdentifier, aVolume));
}

/**
 *  @brief
 *    This is the class initializer for an all-object volume level
 *    response.
 *
 *  This initializes an all-object volume level property response.
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the objecst for which the
 *                           volume level response is to be
 *                           formed. For example, "O" for zone
 *                           objects.
 *  @param[in]  aLevel       An immutable reference to the
 *                           volume level in the response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeAllResponseBasis :: Init(const char *aObject, const LevelType &aVolume)
{
    return (VolumeAllBufferBasis::Init(*this, aObject, aVolume));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute state response.
 *
 *  This initializes a volume fixed/locked state response against a
 *  specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume fixed/locked state response is to
 *                           be formed. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier for which the volume
 *                           fixed/locked state response is to be
 *                           formed.
 *  @param[in]  aMute        An immutable reference to the volume
 *                           fixed/locked state in the response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeFixedResponseBasis :: Init(const char *aObject, const IdentifierType &aIdentifier, const FixedType &aVolumeFixed)
{
    return (VolumeFixedBufferBasis::Init(*this, aObject, aIdentifier, aVolumeFixed));
}

/**
 *  @brief
 *    This is the class initializer for a volume mute state response.
 *
 *  This initializes a volume mute state response against a specific
 *  object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           volume mute state response is to be
 *                           formed. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier for which the volume mute state
 *                           response is to be formed.
 *  @param[in]  aMute        An immutable reference to the volume
 *                           mute state in the response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeMuteResponseBasis :: Init(const char *aObject, const IdentifierType &aIdentifier, const MuteType &aMute)
{
    return (VolumeMuteBufferBasis::Init(*this, aObject, aIdentifier, aMute));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
