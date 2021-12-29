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

#include <iomanip>
#include <ios>
#include <sstream>
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
 *  This initializes an operation with a specified value into the
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
 *    Ethernet EUI-48 address set operation.
 *
 *  This initializes the Ethernet network interface EUI-48 address
 *  property set operation of a specified EUI-48 address into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer         A mutable reference to the
 *                                  command buffer to compose the
 *                                  operation into.
 *  @param[in]      aEthernetEUI48  An immutable reference to the
 *                                  EUI-48 address to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EthernetEUI48BufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                                 const EthernetEUI48Type &aEthernetEUI48)
{
    static const char * const kMACProperty = "MAC";
    const size_t              n            = sizeof(EthernetEUI48Type);
    std::string               lPropertyString(kMACProperty);

    for (size_t i = 0; i < n; i++)
    {
        std::ostringstream         lValueStream;

        // Set the width to two (2), the fill to zero ('0'), the case
        // to upper, and the base to hexadecimal while upcasting the
        // value to an unsigned integer to ensure it is interpretted
        // as something to be converted rather than a character
        // literal.

        lValueStream << std::setw(2)
                     << std::setfill('0')
                     << std::uppercase
                     << std::hex
                     << static_cast<unsigned int>(aEthernetEUI48[i]);

        // Compose the buffer with the property and value.

        lPropertyString += lValueStream.str();

        if (i != (n - 1))
        {
            lPropertyString += '-';
        }
    }

    return (aBuffer.Init(lPropertyString.c_str(), lPropertyString.size()));
}

/**
 *  @brief
 *    This is the class initializer for a multi-character property of
 *    an IP address.
 *
 *  This initializes an IP address command operation or notification
 *  with a specified property and IP address into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aProperty    The property to mutate.
 *  @param[in]      aIPAddress   An immutable reference to the IP
 *                               address to use for the property
 *                               mutation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                      const char *aProperty,
                      const IPAddress &aIPAddress)
{
    char        lValueString[INET6_ADDRSTRLEN];
    std::string lPropertyString;
    Status      lRetval;


    lRetval = aIPAddress.ToString(&lValueString[0], INET6_ADDRSTRLEN);
    nlREQUIRE_SUCCESS(lRetval, done);

    lPropertyString  = aProperty;
    lPropertyString += lValueString;

    lRetval = aBuffer.Init(lPropertyString.c_str(), lPropertyString.size());

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer for a default router IP address.
 *
 *  This initializes a default router IP address command operation or
 *  notification with a specified default router IP address into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer                A mutable reference to the
 *                                         command buffer to compose
 *                                         the property mutation
 *                                         operation or notification
 *                                         into.
 *  @param[in]      aDefaultRouterAddress  An immutable reference to
 *                                         the default router IP
 *                                         address to use for the
 *                                         buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPDefaultRouterAddressBufferBasis :: Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aDefaultRouterAddress)
{
    static const char * const kIPDefaultRouterAddressProperty = "GW";

    return (IPBufferBasis::Init(aBuffer, kIPDefaultRouterAddressProperty, aDefaultRouterAddress));
}

/**
 *  @brief
 *    This is the class initializer for a host IP address.
 *
 *  This initializes a host IP address command operation or
 *  notification with a specified host IP address into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer                A mutable reference to the
 *                                         command buffer to compose
 *                                         the property mutation
 *                                         operation or notification
 *                                         into.
 *  @param[in]      aHostAddress           An immutable reference to
 *                                         the host IP address to use
 *                                         for the buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPHostAddressBufferBasis :: Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aHostAddress)
{
    static const char * const kIPHostAddressProperty = "IP";

    return (IPBufferBasis::Init(aBuffer, kIPHostAddressProperty, aHostAddress));
}

/**
 *  @brief
 *    This is the class initializer for a IP netmask.
 *
 *  This initializes a IP netmask command operation or notification
 *  with a specified IP netmask into the specified command buffer.
 *
 *  @param[in,out]  aBuffer                A mutable reference to the
 *                                         command buffer to compose
 *                                         the property mutation
 *                                         operation or notification
 *                                         into.
 *  @param[in]      aHostAddress           An immutable reference to
 *                                         the IP netmask to use for
 *                                         the buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPNetmaskBufferBasis :: Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aNetmask)
{
    static const char * const kIPNetmaskProperty = "NM";

    return (IPBufferBasis::Init(aBuffer, kIPNetmaskProperty, aNetmask));
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
