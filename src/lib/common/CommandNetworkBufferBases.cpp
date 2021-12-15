/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements base and derived objects for composing
 *      HLX commands that observe or set Ethernet network interface
 *      data model properties.
 *
 */

#include "CommandNetworkBufferBases.hpp"

#include <string>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Network
{

/**
 *  @brief
 *    This is the class initializer for a multi-character property of
 *    a Boolean value.
 *
 *  This initializes a operation with a specified value into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property to mutate.
 *  @param[in]      aValue       An immutable reference to the Boolean
 *                               value to use for the property
 *                               mutation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EnabledBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                           const char *aProperty,
                           const EnabledType &aValue)
{
    OutputStringStream         lValueStream;
    std::string                lPropertyString;

    // Set the width to one (1) while upcasting the value to an
    // unsigned integer to ensure it is interpretted as something to
    // be converted rather than a character literal.

    lValueStream << std::setw(1) << static_cast<unsigned int>(aValue);

    // Compose the buffer with the property and value.

    lPropertyString = aProperty;
    lPropertyString += lValueStream.str();

    return (aBuffer.Init(lPropertyString.c_str(), lPropertyString.size()));
}

/**
 *  @brief
 *    This is a class initializer for an Ethernet network interface
 *    DHCPv4 enabled state set operation.
 *
 *  This initializes the Ethernet network interface DHCPv4 enabled
 *  state property set operation of a specified enabled state into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aEnabled     An immutable reference to the enabled
 *                               state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DHCPv4EnabledBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                                 const EnabledType &aEnabled)
{
    static const char * const kDHCPv4Property = "DHCP";

    return (EnabledBufferBasis::Init(aBuffer, kDHCPv4Property, aEnabled));
}

/**
 *  @brief
 *    This is a class initializer for an Ethernet network interface
 *    Control4 SDDP enabled state set operation.
 *
 *  This initializes the Ethernet network interface Control4 SDDP
 *  enabled state property set operation of a specified enabled state
 *  into the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aEnabled     An immutable reference to the enabled
 *                               state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SDDPEnabledBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                               const EnabledType &aEnabled)
{
    static const char * const kSDDPProperty = "SDDP";

    return (EnabledBufferBasis::Init(aBuffer, kSDDPProperty, aEnabled));
}

}; // namespace Network

}; // namespace Command

}; // namespace Common

}; // namespace HLX
