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
 *      This file implements derived objects for a HLX client Ethernet
 *      network interface object data model state change notifications
 *      (SCNs).
 *
 */

#include "NetworkStateChangeNotifications.hpp"

#include <string.h>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkDHCPv4EnabledNotification :: NetworkDHCPv4EnabledNotification(void) :
    NotificationBasis(),
    mEnabled(false)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface DHCPv4 enabled
 *  property state change notification with the specified enabled
 *  state.
 *
 *  @param[in]  aEnabled  An immutable reference to the enabled state
 *                        that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkDHCPv4EnabledNotification :: Init(const EnabledType &aEnabled)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_NetworkDHCPv4Enabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    mEnabled = aEnabled;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the Ethernet network interface DHCPv4 enabled property.
 *
 *  @returns
 *    The enabled state of the DHCPv4 state that changed.
 *
 */
NetworkDHCPv4EnabledNotification :: EnabledType
NetworkDHCPv4EnabledNotification :: GetEnabled(void) const
{
    return (mEnabled);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkEthernetEUI48Notification :: NetworkEthernetEUI48Notification(void) :
    NotificationBasis(),
    mEthernetEUI48()
{
    memset(mEthernetEUI48, 0, sizeof (mEthernetEUI48));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface EUI-48 address
 *  property state change notification with the specified Ethernet
 *  EUI-48 address.
 *
 *  @param[in]  aEthernetEUI48  An immutable reference to the
 *                              Ethernet EUI-48 address that
 *                              changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkEthernetEUI48Notification :: Init(const EthernetEUI48Type &aEthernetEUI48)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_NetworkEthernetEUI48);
    nlREQUIRE_SUCCESS(lRetval, done);

    memcpy(mEthernetEUI48, aEthernetEUI48, sizeof (mEthernetEUI48));

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the Ethernet network interface EUI-48 address property.
 *
 *  @returns
 *    The EUI-48 address of the Ethernet network interface that
 *    changed.
 *
 */
const NetworkEthernetEUI48Notification :: EthernetEUI48Type &
NetworkEthernetEUI48Notification :: GetEthernetEUI48(void) const
{
    return (mEthernetEUI48);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface IP address
 *  property state change notification with the specified IP address.
 *
 *  @param[in]  aType       An immutable reference to the state change
 *                          notification type to initialize with. This
 *                          indicates what object class and what
 *                          property within that object class changed.
 *  @param[in]  aIPAddress  An immutable reference to the IP address
 *                          that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkIPAddressNotificationBasis :: Init(const Type &aType, const IPAddress &aIPAddress)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(aType);
    nlREQUIRE_SUCCESS(lRetval, done);

    mIPAddress = aIPAddress;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the Ethernet network interface IP address property.
 *
 *  @returns
 *    The IP address of the Ethernet network interface that changed.
 *
 */
const IPAddress &
NetworkIPAddressNotificationBasis :: GetIPAddress(void) const
{
    return (mIPAddress);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface default router IP
 *  address property state change notification with the specified IP
 *  address.
 *
 *  @param[in]  aDefaultRouterIPAddress  An immutable reference to
 *                                       the IP address that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkIPDefaultRouterAddressNotification :: Init(const IPAddress &aDefaultRouterIPAddress)
{
    constexpr Type lType   = kStateChangeType_NetworkIPDefaultRouterAddress;
    Status         lRetval = kStatus_Success;

    lRetval = NetworkIPAddressNotificationBasis::Init(lType, aDefaultRouterIPAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface host IP address
 *  property state change notification with the specified IP address.
 *
 *  @param[in]  aHostIPAddress  An immutable reference to the IP
 *                              address that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkIPHostAddressNotification :: Init(const IPAddress &aHostIPAddress)
{
    constexpr Type lType   = kStateChangeType_NetworkIPHostAddress;
    Status         lRetval = kStatus_Success;

    lRetval = NetworkIPAddressNotificationBasis::Init(lType, aHostIPAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface IP netmask
 *  property state change notification with the specified IP netmask.
 *
 *  @param[in]  aIPNetmask  An immutable reference to the IP netmask
 *                          that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkIPNetmaskNotification :: Init(const IPAddress &aIPNetmask)
{
    constexpr Type lType   = kStateChangeType_NetworkIPNetmask;
    Status         lRetval = kStatus_Success;

    lRetval = NetworkIPAddressNotificationBasis::Init(lType, aIPNetmask);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkSDDPEnabledNotification :: NetworkSDDPEnabledNotification(void) :
    NotificationBasis(),
    mEnabled(false)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the Ethernet network interface Control4 SDDP
 *  enabled property state change notification with the specified
 *  enabled state.
 *
 *  @param[in]  aEnabled  An immutable reference to the enabled state
 *                        that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkSDDPEnabledNotification :: Init(const EnabledType &aEnabled)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_NetworkSDDPEnabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    mEnabled = aEnabled;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the Ethernet network interface Control4 SDDP enabled
 *    property.
 *
 *  @returns
 *    The enabled state of the Control4 SDDP state that changed.
 *
 */
NetworkSDDPEnabledNotification :: EnabledType
NetworkSDDPEnabledNotification :: GetEnabled(void) const
{
    return (mEnabled);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
