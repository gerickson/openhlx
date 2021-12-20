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
 *      This file defines derived objects for a HLX client Ethernet
 *      network interface object data model state change notifications
 *      (SCNs).
 *
 */

#ifndef OPENHLXCLIENTNETWORKSTATECHANGENOTIFICATIONS_HPP
#define OPENHLXCLIENTNETWORKSTATECHANGENOTIFICATIONS_HPP

#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object DHCPv4 enabled property data model state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup front-panel
 *  @ingroup state-change
 *
 */
class NetworkDHCPv4EnabledNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a EnabledType from the Ethernet
     *  network interface model.
     *
     */
    typedef Model::NetworkModel::EnabledType EnabledType;

public:
    NetworkDHCPv4EnabledNotification(void);
    virtual ~NetworkDHCPv4EnabledNotification(void) = default;

    Common::Status Init(const EnabledType &aEnabled);

    EnabledType GetEnabled(void) const;

private:
    EnabledType mEnabled;
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object EUI-48 address property data model state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup front-panel
 *  @ingroup state-change
 *
 */
class NetworkEthernetAddressNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a EnabledType from the Ethernet
     *  network interface model.
     *
     */
    typedef Model::NetworkModel::EthernetAddressType EthernetAddressType;

public:
    NetworkEthernetAddressNotification(void);
    virtual ~NetworkEthernetAddressNotification(void) = default;

    Common::Status Init(const EthernetAddressType &aEthernetAddress);

    const EthernetAddressType &GetEthernetAddress(void) const;

private:
    EthernetAddressType mEthernetAddress;
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object Control4 SDDP enabled property data model state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup front-panel
 *  @ingroup state-change
 *
 */
class NetworkSDDPEnabledNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a EnabledType from the Ethernet
     *  network interface model.
     *
     */
    typedef Model::NetworkModel::EnabledType EnabledType;

public:
    NetworkSDDPEnabledNotification(void);
    virtual ~NetworkSDDPEnabledNotification(void) = default;

    Common::Status Init(const EnabledType &aEnabled);

    EnabledType GetEnabled(void) const;

private:
    EnabledType mEnabled;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTNETWORKSTATECHANGENOTIFICATIONS_HPP
