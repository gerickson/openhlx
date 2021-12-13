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
