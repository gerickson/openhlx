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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a HLX Ethernet network interface.
 *
 */

#include "NetworkController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkController :: NetworkController(void) :
    Common::NetworkControllerBasis(),
    Client::NetworkControllerBasis(Common::NetworkControllerBasis::mNetworkModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
NetworkController :: ~NetworkController(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NetworkController :: Init(CommandManager &aCommandManager,
                          const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::NetworkControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::NetworkControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Attempt to get the HLX server IPv4 Dynamic Host Control Protocol
 *    (DHCP) enabled state.
 *
 *  This attempts to get the HLX server IPv4 Dynamic Host Control
 *  Protocol (DHCP) enabled state, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aDHCPv4Enabled  A mutable reference to storage for
 *                               the IPv4 Dynamic Host Control
 *                               Protocol (DHCP) enabled state, if
 *                               successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IPv4 Dynamic Host Control
 *                                  Protocol (DHCP) enabled state has
 *                                  not been initialized with a known
 *                                  value.
 *
 */
Status
NetworkController :: GetDHCPv4Enabled(Model::NetworkModel::EnabledType &aDHCPv4Enabled) const
{
    Status lRetval;

    lRetval = GetModel().GetDHCPv4Enabled(aDHCPv4Enabled);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server network interface hardware
 *    address.
 *
 *  This attempts to get the HLX server network interface hardware
 *  address, if it has been previously initialized or set.
 *
 *  @param[out]  aEthernetEUI48    A mutable reference to storage for
 *                                 the network interface EUI-48
 *                                 address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the network interface
 *                                  hardware address has not been
 *                                  initialized with a known value.
 *
 */
Status
NetworkController :: GetEthernetEUI48(Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48) const
{
    Status lRetval;

    lRetval = GetModel().GetEthernetEUI48(aEthernetEUI48);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server default router (that is, gateway)
 *    IP address.
 *
 *  This attempts to get the HLX server default router (that is,
 *  gateway) IP address, if it has been previously initialized or set.
 *
 *  @param[out]  aDefaultRouterAddress  A mutable reference to storage
 *                                      for the default router IP
 *                                      address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the default router IP address
 *                                  has not been initialized with a
 *                                  known value.
 *
 */
Status
NetworkController :: GetDefaultRouterIPAddress(Model::NetworkModel::IPAddressType &aDefaultRouterAddress) const
{
    Status lRetval;

    lRetval = GetModel().GetDefaultRouterAddress(aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server host IP address.
 *
 *  This attempts to get the HLX server host IP address, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aHostAddress           A mutable reference to storage
 *                                      for the host IP address, if
 *                                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the host IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
NetworkController :: GetHostIPAddress(Model::NetworkModel::IPAddressType &aHostAddress) const
{
    Status lRetval;

    lRetval = GetModel().GetHostAddress(aHostAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server IP netmask.
 *
 *  This attempts to get the HLX server IP netmask, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aNetmask               A mutable reference to storage
 *                                      for the IP netmask, if
 *                                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IP netmask has not been
 *                                  initialized with a known value.
 *
 */
Status
NetworkController :: GetIPNetmask(Model::NetworkModel::IPAddressType &aNetmask) const
{
    Status lRetval;

    lRetval = GetModel().GetNetmask(aNetmask);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server Control4 Simple Device Discovery
 *    Protocol (SDDP) enabled state.
 *
 *  This attempts to get the HLX server Control4 Simple Device
 *  Discovery Protocol (SDDP) enabled state, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aSDDPEnabled  A mutable reference to storage for
 *                             the Control4 Simple Device Discovery
 *                             Protocol (SDDP) enabled state, if
 *                             successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the Control4 Simple Device
 *                                  Discovery Protocol (SDDP) enabled
 *                                  state has not been initialized
 *                                  with a known value.
 *
 */
Status
NetworkController :: GetSDDPEnabled(Model::NetworkModel::EnabledType &aSDDPEnabled) const
{
    Status lRetval;

    lRetval = GetModel().GetSDDPEnabled(aSDDPEnabled);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
