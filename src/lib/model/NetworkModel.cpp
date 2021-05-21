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
    mHLXAddressIsNull(true),
    mHLXAddress(),
    mPrefixLengthIsNull(true),
    mPrefixLength(),
    mGatewayAddressIsNull(true),
    mGatewayAddress(),
    mEthernetAddressIsNull(true),
    mEthernetAddress(),
    mDHCPv4EnabledIsNull(true),
    mDHCPv4Enabled(false),
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

    mHLXAddressIsNull      = true;
    memset(mHLXAddress, 0, sizeof (mHLXAddress));

    mPrefixLengthIsNull    = true;
    mPrefixLength          = 0;

    mGatewayAddressIsNull  = true;
    memset(mGatewayAddress, 0, sizeof (mGatewayAddress));

    mEthernetAddressIsNull = true;
    memset(mEthernetAddress, 0, sizeof (mEthernetAddress));

    mDHCPv4EnabledIsNull   = true;
    mDHCPv4Enabled         = false;
    mSDDPEnabledIsNull     = true;
    mSDDPEnabled           = false;

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
    mHLXAddressIsNull      = aNetworkModel.mHLXAddressIsNull;
    memcpy(mHLXAddress, aNetworkModel.mHLXAddress, sizeof (mHLXAddress));

    mPrefixLengthIsNull    = aNetworkModel.mPrefixLengthIsNull;
    mPrefixLength          = aNetworkModel.mPrefixLength;

    mGatewayAddressIsNull  = aNetworkModel.mGatewayAddressIsNull;
    memcpy(mGatewayAddress, aNetworkModel.mGatewayAddress, sizeof (mGatewayAddress));

    mEthernetAddressIsNull = aNetworkModel.mEthernetAddressIsNull;
    memcpy(mEthernetAddress, aNetworkModel.mEthernetAddress, sizeof (mEthernetAddress));

    mDHCPv4EnabledIsNull   = aNetworkModel.mDHCPv4EnabledIsNull;
    mDHCPv4Enabled         = aNetworkModel.mDHCPv4Enabled;
    mSDDPEnabledIsNull     = aNetworkModel.mSDDPEnabledIsNull;
    mSDDPEnabled           = aNetworkModel.mSDDPEnabled;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model HLX IP address.
 *
 *  This attempts to get the model HLX IP address, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aHLXAddress  A mutable reference to storage for the
 *                            HLX IP address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the HLX IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa SetHLXAddress
 *
 */
Status
NetworkModel :: GetHLXAddress(IPAddressType &aHLXAddress) const
{
    Status lRetval = ((mHLXAddressIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        memcpy(aHLXAddress, mHLXAddress, sizeof (mHLXAddress));
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
 *  @param[out]  aPrefixLength  A mutable reference to storage for the
 *                              HLX network IP address prefix length, if
 *                              successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the HLX network IP address prefix
 *                                  length has not been initialized
 *                                  with a known value.
 *
 *  @sa SetPrefixLength
 *
 */
Status
NetworkModel :: GetPrefixLength(PrefixLengthType &aPrefixLength) const
{
    Status lRetval = ((mPrefixLengthIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aPrefixLength = mPrefixLength;
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
 *  @param[out]  aGatewayAddress  A mutable reference to storage for
 *                                the gateway IP address, if
 *                                successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the gateway IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa SetGatewayAddress
 *
 */
Status
NetworkModel :: GetGatewayAddress(IPAddressType &aGatewayAddress) const
{
    Status lRetval = ((mGatewayAddressIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        memcpy(aGatewayAddress, mGatewayAddress, sizeof (mGatewayAddress));
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
 *  @param[out]  aEthernetAddress  A mutable reference to storage for
 *                                 the Ethernet network interface hardware
 *                                 address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the Ethernet network interface
 *                                  hardware address has not been
 *                                  initialized with a known value.
 *
 *  @sa SetEthernetAddress
 *
 */
Status
NetworkModel :: GetEthernetAddress(EthernetAddressType &aEthernetAddress) const
{
    Status lRetval = ((mEthernetAddressIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        memcpy(aEthernetAddress, mEthernetAddress, sizeof (mEthernetAddress));
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model IPv4 Dynamic Host Control Protocl
 *    (DHCP) enabled state.
 *
 *  This attempts to get the model IPv4 Dynamic Host Control Protocl
 *  (DHCP) enabled state, if it has been previously initialized or
 *  set.
 *
 *  @param[out]  aDHCPv4Enabled  A mutable reference to storage for
 *                               the IPv4 Dynamic Host Control Protocl
 *                               (DHCP) enabled state, if
 *                               successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IPv4 Dynamic Host Control Protocl
 *                                  (DHCP) enabled state has not been
 *                                  initialized with a known value.
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
 *  @param[in]  aHLXAddress  An immutable reference to the HLX IP
 *                           address to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aHLXAddress
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetHLXAddress(const IPAddressType &aHLXAddress)
{
    Status lRetval = kStatus_Success;

    if (memcmp(mHLXAddress, aHLXAddress, sizeof (mHLXAddress)) == 0)
    {
        lRetval = ((mHLXAddressIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        memcpy(mHLXAddress, aHLXAddress, sizeof (mHLXAddress));
    }

    mHLXAddressIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model HLX network IP address prefix length.
 *
 *  This attempts to set the model with the specified HLX network IP
 *  address prefix length.
 *
 *  @param[in]  aPrefixLength  An immutable reference to the HLX
 *                             network IP address prefix length to
 *                             set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aPrefixLength
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aPrefixLength
 *                                    value is out of range.
 *
 */
Status
NetworkModel :: SetPrefixLength(const PrefixLengthType &aPrefixLength)
{
    Status lRetval = kStatus_Success;

    if (mPrefixLength == aPrefixLength)
    {
        lRetval = ((mPrefixLengthIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mPrefixLength = aPrefixLength;
    }

    mPrefixLengthIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model gateway IP address.
 *
 *  This attempts to set the model with the specified gateway IP address.
 *
 *  @param[in]  aGatewayAddress  An immutable reference to the gateway IP
 *                               address to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aGatewayAddress
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetGatewayAddress(const IPAddressType &aGatewayAddress)
{
    Status lRetval = kStatus_Success;

    if (memcmp(mGatewayAddress, aGatewayAddress, sizeof (mGatewayAddress)) == 0)
    {
        lRetval = ((mGatewayAddressIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        memcpy(mGatewayAddress, aGatewayAddress, sizeof (mGatewayAddress));
    }

    mGatewayAddressIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model Ethernet network interface hardware address.
 *
 *  This attempts to set the model with the specified Ethernet network
 *  interface hardware address.
 *
 *  @param[in]  aEthernetAddress  An immutable reference to the Ethernet
 *                                network interface hardware address
 *                                to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aEthernetAddress
 *                                    value has already been set.
 *
 */
Status
NetworkModel :: SetEthernetAddress(const EthernetAddressType &aEthernetAddress)
{
    Status lRetval = kStatus_Success;

    if (memcmp(mEthernetAddress, aEthernetAddress, sizeof (mEthernetAddress)) == 0)
    {
        lRetval = ((mEthernetAddressIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        memcpy(mEthernetAddress, aEthernetAddress, sizeof (mEthernetAddress));
    }

    mEthernetAddressIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model IPv4 Dynamic Host Control Protocl (DHCP)
 *    enabled state.
 *
 *  This attempts to set the model with the specified IPv4 Dynamic
 *  Host Control Protocl (DHCP) enabled state.
 *
 *  @param[in]  aDHCPv4Enabled  An immutable reference to the IPv4
 *                              Dynamic Host Control Protocl (DHCP)
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
    return ((mHLXAddressIsNull      == aNetworkModel.mHLXAddressIsNull     ) &&
            (mHLXAddress            == aNetworkModel.mHLXAddress           ) &&
            (mPrefixLengthIsNull    == aNetworkModel.mPrefixLengthIsNull   ) &&
            (mPrefixLength          == aNetworkModel.mPrefixLength         ) &&
            (mGatewayAddressIsNull  == aNetworkModel.mGatewayAddressIsNull ) &&
            (mGatewayAddress        == aNetworkModel.mGatewayAddress       ) &&
            (mEthernetAddressIsNull == aNetworkModel.mEthernetAddressIsNull) &&
            (mEthernetAddress       == aNetworkModel.mEthernetAddress      ) &&
            (mDHCPv4EnabledIsNull   == aNetworkModel.mDHCPv4EnabledIsNull  ) &&
            (mDHCPv4Enabled         == aNetworkModel.mDHCPv4Enabled        ) &&
            (mSDDPEnabledIsNull     == aNetworkModel.mSDDPEnabledIsNull    ) &&
            (mSDDPEnabled           == aNetworkModel.mSDDPEnabled          ));
}

}; // namespace Model

}; // namespace HLX
