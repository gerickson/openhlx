/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file implements objects for HLX server network data model
 *      commands and their constituent requests and responses.
 *
 */

#include "NetworkControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Network
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the network interface query command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the network interface query command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    static const char * const kNetworkObject = "E";

    return (QueryResponseBasis::Init(kNetworkObject));
}

// MARK: Network Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set Ethernet network interface DHCPv4 enabled
 *  state command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetDHCPv4EnabledRequest :: Init(void)
{
    return (DHCPv4EnabledRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface DHCPv4 enabled
 *  state command response buffer.
 *
 *  @param[in]  aEnabled   An immutable reference to the enabled state
 *                         for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DHCPv4EnabledResponse :: Init(const Model::NetworkModel::EnabledType &aEnabled)
{
    return (DHCPv4EnabledBufferBasis::Init(*this, aEnabled));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface EUI-48 address
 *  command response buffer.
 *
 *  @param[in]  aEthernetEUI48   An immutable reference to the
 *                               EUI-48 address state for which to
 *                               form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EthernetEUI48Response :: Init(const Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48)
{
    return (EthernetEUI48BufferBasis::Init(*this, aEthernetEUI48));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface default router IP
 *  address command response buffer.
 *
 *  @param[in]  aDefaultRouterAddress  An immutable reference to the
 *                                     default router IP address for
 *                                     which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IPDefaultRouterAddressResponse :: Init(const Common::IPAddress &aDefaultRouterAddress)
{
    return (IPDefaultRouterAddressBufferBasis::Init(*this, aDefaultRouterAddress));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface host IP address
 *  command response buffer.
 *
 *  @param[in]  aHostAddress  An immutable reference to the host IP
 *                            address for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IPHostAddressResponse :: Init(const Common::IPAddress &aHostAddress)
{
    return (IPHostAddressBufferBasis::Init(*this, aHostAddress));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface IP netmask command
 *  response buffer.
 *
 *  @param[in]  aNetmask  An immutable reference to the IP netmask for
 *                        which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IPNetmaskResponse :: Init(const Common::IPAddress &aNetmask)
{
    return (IPNetmaskBufferBasis::Init(*this, aNetmask));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set Ethernet network interface Control4 SDDP
 *  enabled state command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetSDDPEnabledRequest :: Init(void)
{
    return (SDDPEnabledRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface Control4 SDDP
 *  enabled state command response buffer.
 *
 *  @param[in]  aEnabled   An immutable reference to the enabled state
 *                         for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SDDPEnabledResponse :: Init(const Model::NetworkModel::EnabledType &aEnabled)
{
    return (SDDPEnabledBufferBasis::Init(*this, aEnabled));
}

}; // namespace Network

}; // namespace Command

}; // namespace Server

}; // namespace HLX
