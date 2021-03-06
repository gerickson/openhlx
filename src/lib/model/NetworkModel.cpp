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
 *      This file implements an object for managing an HLX network
 *      connectivity data model.
 *
 */

#include "NetworkModel.hpp"

#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkModel :: NetworkModel(void) :
    mDHCPv4EnabledIsNull(true),
    mDHCPv4Enabled(false),
    mEthernetEUI48IsNull(true),
    mEthernetEUI48(),
    mDefaultRouterAddressIsNull(true),
    mDefaultRouterAddress(),
    mHostAddressIsNull(true),
    mHostAddress(),
    mNetmaskIsNull(true),
    mNetmask(),
    mSDDPEnabledIsNull(true),
    mSDDPEnabled(false)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null IP address, network prefix
 *  length, gateway address, Ethernet hardware address, DHCPv4 enabled
 *  state, and Control4 SDDP enabled state.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
NetworkModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mDHCPv4EnabledIsNull        = true;
    mDHCPv4Enabled              = false;

    mEthernetEUI48IsNull        = true;
    memset(mEthernetEUI48, 0, sizeof (mEthernetEUI48));

    mDefaultRouterAddressIsNull = true;
    mHostAddressIsNull          = true;
    mNetmaskIsNull              = true;

    mSDDPEnabledIsNull          = true;
    mSDDPEnabled                = false;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified network model.
 *
 *  @param[in]  aNetworkModel  An immutable reference to the
 *                             network model to initialize with.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
NetworkModel :: Init(const NetworkModel &aNetworkModel)
{
    Status lRetval = kStatus_Success;

    *this = aNetworkModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified network model to this one.
 *
 *  @param[in]  aNetworkModel  An immutable reference to the
 *                             network model to assign (copy)
 *                             to this one.
 *
 *  @returns
 *    A reference to this network model after the assignment (copy)
 *    is complete.
 *
 */
NetworkModel &
NetworkModel :: operator =(const NetworkModel &aNetworkModel)
{
    mDHCPv4EnabledIsNull         = aNetworkModel.mDHCPv4EnabledIsNull;
    mDHCPv4Enabled               = aNetworkModel.mDHCPv4Enabled;

    mEthernetEUI48IsNull         = aNetworkModel.mEthernetEUI48IsNull;
    memcpy(mEthernetEUI48, aNetworkModel.mEthernetEUI48, sizeof (mEthernetEUI48));

    mDefaultRouterAddressIsNull  = aNetworkModel.mDefaultRouterAddressIsNull;
    mDefaultRouterAddress        = aNetworkModel.mDefaultRouterAddress;

    mHostAddressIsNull           = aNetworkModel.mHostAddressIsNull;
    mHostAddress                 = aNetworkModel.mHostAddress;

    mNetmaskIsNull               = aNetworkModel.mNetmaskIsNull;
    mNetmask                     = aNetworkModel.mNetmask;

    mSDDPEnabledIsNull           = aNetworkModel.mSDDPEnabledIsNull;
    mSDDPEnabled                 = aNetworkModel.mSDDPEnabled;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model HLX IP address.
 *
 *  This attempts to get the model HLX IP address, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aHostAddress  A mutable reference to storage for the
 *                            HLX IP address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the HLX IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa SetHostAddress
 *
 */
Status
NetworkModel :: GetHostAddress(IPAddressType &aHostAddress) const
{
    Status lRetval = ((mHostAddressIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aHostAddress = mHostAddress;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model HLX network IP address prefix length.
 *
 *  This attempts to get the model HLX network IP address prefix
 *  length, if it has been previously initialized or set.
 *
 *  @param[out]  aNetmask  A mutable reference to storage for the
 *                              HLX network IP address prefix length, if
 *                              successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the HLX network IP address prefix
 *                                  length has not been initialized
 *                                  with a known value.
 *
 *  @sa SetNetmask
 *
 */
Status
NetworkModel :: GetNetmask(IPAddressType &aNetmask) const
{
    Status lRetval = ((mNetmaskIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aNetmask = mNetmask;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model gateway IP address.
 *
 *  This attempts to get the model gateway IP address, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aDefaultRouterAddress  A mutable reference to storage for
 *                                the gateway IP address, if
 *                                successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the gateway IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa SetDefaultRouterAddress
 *
 */
Status
NetworkModel :: GetDefaultRouterAddress(IPAddressType &aDefaultRouterAddress) const
{
    Status lRetval = ((mDefaultRouterAddressIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aDefaultRouterAddress = mDefaultRouterAddress;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model Ethernet network interface hardware
 *    address.
 *
 *  This attempts to get the model Ethernet network interface hardware
 *  address, if it has been previously initialized or set.
 *
 *  @param[out]  aEthernetEUI48    A mutable reference to storage for
 *                                 the Ethernet network interface EUI-48
 *                                 address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the Ethernet network interface
 *                                  hardware address has not been
 *                                  initialized with a known value.
 *
 *  @sa SetEthernetEUI48
 *
 */
Status
NetworkModel :: GetEthernetEUI48(EthernetEUI48Type &aEthernetEUI48) const
{
    Status lRetval = ((mEthernetEUI48IsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        memcpy(aEthernetEUI48, mEthernetEUI48, sizeof (mEthernetEUI48));
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model IPv4 Dynamic Host Control Protocol
 *    (DHCP) enabled state.
 *
 *  This attempts to get the model IPv4 Dynamic Host Control Protocol
 *  (DHCP) enabled state, if it has been previously initialized or
 *  set.
 *
 *  @param[out]  aDHCPv4Enabled  A mutable reference to storage for
 *                               the IPv4 Dynamic Host Control Protocol
 *                               (DHCP) enabled state, if
 *                               successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IPv4 Dynamic Host Control
 *                                  Protocol (DHCP) enabled state has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa SetDHCPv4Enabled
 *
 */
Status
NetworkModel :: GetDHCPv4Enabled(EnabledType &aDHCPv4Enabled) const
{
    Status lRetval = ((mDHCPv4EnabledIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aDHCPv4Enabled = mDHCPv4Enabled;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model Control4 Simple Device Discovery
 *    Protocol (SDDP) enabled state.
 *
 *  This attempts to get the model Control4 Simple Device Discovery
 *  Protocol (SDDP) enabled state, if it has been previously
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
 *  @sa SetSDDPEnabled
 *
 */
Status
NetworkModel :: GetSDDPEnabled(EnabledType &aSDDPEnabled) const
{
    Status lRetval = ((mSDDPEnabledIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aSDDPEnabled = mSDDPEnabled;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model HLX IP address.
 *
 *  This attempts to set the model with the specified HLX IP address.
 *
 *  @param[in]  aHostAddress  An immutable reference to the HLX IP
 *                           address to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aHostAddress
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetHostAddress(const IPAddressType &aHostAddress)
{
    Status lRetval = kStatus_Success;

    if (mHostAddress == aHostAddress)
    {
        lRetval = ((mHostAddressIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mHostAddress = aHostAddress;
    }

    mHostAddressIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model HLX network IP netmask.
 *
 *  This attempts to set the model with the specified HLX network IP
 *  netmask.
 *
 *  @param[in]  aNetmask  An immutable reference to the HLX
 *                        network IP netmask to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aNetmask
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aNetmask
 *                                    value is out of range.
 *
 */
Status
NetworkModel :: SetNetmask(const IPAddressType &aNetmask)
{
    Status lRetval = kStatus_Success;

    if (mNetmask == aNetmask)
    {
        lRetval = ((mNetmaskIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mNetmask = aNetmask;
    }

    mNetmaskIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model gateway IP address.
 *
 *  This attempts to set the model with the specified gateway IP address.
 *
 *  @param[in]  aDefaultRouterAddress  An immutable reference to the gateway IP
 *                               address to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aDefaultRouterAddress
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetDefaultRouterAddress(const IPAddressType &aDefaultRouterAddress)
{
    Status lRetval = kStatus_Success;

    if (mDefaultRouterAddress == aDefaultRouterAddress)
    {
        lRetval = ((mDefaultRouterAddressIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mDefaultRouterAddress = aDefaultRouterAddress;
    }

    mDefaultRouterAddressIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model Ethernet network interface hardware address.
 *
 *  This attempts to set the model with the specified Ethernet network
 *  interface hardware address.
 *
 *  @param[in]  aEthernetEUI48  An immutable reference to the Ethernet
 *                              network interface EUI-48 address to
 *                              set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aEthernetEUI48
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetEthernetEUI48(const EthernetEUI48Type &aEthernetEUI48)
{
    Status lRetval = kStatus_Success;

    if (memcmp(mEthernetEUI48, aEthernetEUI48, sizeof (mEthernetEUI48)) == 0)
    {
        lRetval = ((mEthernetEUI48IsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        memcpy(mEthernetEUI48, aEthernetEUI48, sizeof (mEthernetEUI48));
    }

    mEthernetEUI48IsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model IPv4 Dynamic Host Control Protocol (DHCP)
 *    enabled state.
 *
 *  This attempts to set the model with the specified IPv4 Dynamic
 *  Host Control Protocol (DHCP) enabled state.
 *
 *  @param[in]  aDHCPv4Enabled  An immutable reference to the IPv4
 *                              Dynamic Host Control Protocol (DHCP)
 *                              enabled state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aDHCPv4Enabled
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetDHCPv4Enabled(const EnabledType &aDHCPv4Enabled)
{
    Status lRetval = kStatus_Success;

    if (mDHCPv4Enabled == aDHCPv4Enabled)
    {
        lRetval = ((mDHCPv4EnabledIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mDHCPv4Enabled = aDHCPv4Enabled;
    }

    mDHCPv4EnabledIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model Control4 Simple Device Discovery Protocol
 *    (SDDP) enabled state.
 *
 *  This attempts to set the model with the specified Control4 Simple
 *  Device Discovery Protocol (SDDP) enabled state.
 *
 *  @param[in]  aSDDPEnabled  An immutable reference to the Control4
 *                            Simple Device Discovery Protocol (SDDP)
 *                            enabled state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aSDDPEnabled
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetSDDPEnabled(const EnabledType &aSDDPEnabled)
{
    Status lRetval = kStatus_Success;

    if (mSDDPEnabled == aSDDPEnabled)
    {
        lRetval = ((mSDDPEnabledIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mSDDPEnabled = aSDDPEnabled;
    }

    mSDDPEnabledIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided network model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aNetworkModel  An immutable reference to the
 *                             network model to compare for equality.
 *
 *  @returns
 *    True if this network model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
NetworkModel :: operator ==(const NetworkModel &aNetworkModel) const
{
    return ((mDHCPv4EnabledIsNull        == aNetworkModel.mDHCPv4EnabledIsNull       ) &&
            (mDHCPv4Enabled              == aNetworkModel.mDHCPv4Enabled             ) &&
            (mEthernetEUI48IsNull        == aNetworkModel.mEthernetEUI48IsNull       ) &&
            (memcmp(mEthernetEUI48, aNetworkModel.mEthernetEUI48, sizeof (mEthernetEUI48)) == 0) &&
            (mDefaultRouterAddressIsNull == aNetworkModel.mDefaultRouterAddressIsNull) &&
            (mDefaultRouterAddress       == aNetworkModel.mDefaultRouterAddress      ) &&
            (mHostAddressIsNull          == aNetworkModel.mHostAddressIsNull         ) &&
            (mHostAddress                == aNetworkModel.mHostAddress               ) &&
            (mNetmaskIsNull              == aNetworkModel.mNetmaskIsNull             ) &&
            (mNetmask                    == aNetworkModel.mNetmask                   ) &&
            (mSDDPEnabledIsNull          == aNetworkModel.mSDDPEnabledIsNull         ) &&
            (mSDDPEnabled                == aNetworkModel.mSDDPEnabled               ));
}

}; // namespace Model

}; // namespace HLX
