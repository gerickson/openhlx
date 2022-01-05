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
#include <OpenHLX/Common/IPAddress.hpp>
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
 *  @ingroup network
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
 *  @ingroup network
 *  @ingroup state-change
 *
 */
class NetworkEthernetEUI48Notification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a EnabledType from the Ethernet
     *  network interface model.
     *
     */
    typedef Model::NetworkModel::EthernetEUI48Type EthernetEUI48Type;

public:
    NetworkEthernetEUI48Notification(void);
    virtual ~NetworkEthernetEUI48Notification(void) = default;

    Common::Status Init(const EthernetEUI48Type &aEthernetEUI48);

    const EthernetEUI48Type &GetEthernetEUI48(void) const;

private:
    EthernetEUI48Type mEthernetEUI48;
};

/**
 *  @brief
 *    A base, derivable object for a HLX client physical Ethernet
 *    network interface object IP address property data model state
 *    change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup network
 *  @ingroup state-change
 *
 */
class NetworkIPAddressNotificationBasis :
    public NotificationBasis
{
public:
    typedef Common::IPAddress IPAddress;

public:
    virtual ~NetworkIPAddressNotificationBasis(void) = default;

    const IPAddress &GetIPAddress(void) const;

protected:
    NetworkIPAddressNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const IPAddress &aIPAddress);

private:
    IPAddress mIPAddress;
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object default router IP address property data model state
 *    change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup network
 *  @ingroup state-change
 *
 */
class NetworkIPDefaultRouterAddressNotification :
    public NetworkIPAddressNotificationBasis
{
public:
    NetworkIPDefaultRouterAddressNotification(void) = default;
    virtual ~NetworkIPDefaultRouterAddressNotification(void) = default;

    Common::Status Init(const IPAddress &aDefaultRouterIPAddress);
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object host IP address property data model state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup network
 *  @ingroup state-change
 *
 */
class NetworkIPHostAddressNotification :
    public NetworkIPAddressNotificationBasis
{
public:
    NetworkIPHostAddressNotification(void) = default;
    virtual ~NetworkIPHostAddressNotification(void) = default;

    Common::Status Init(const IPAddress &aHostIPAddress);
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object IP netmask property data model state change notification
 *    (SCN).
 *
 *  @ingroup client
 *  @ingroup network
 *  @ingroup state-change
 *
 */
class NetworkIPNetmaskNotification :
    public NetworkIPAddressNotificationBasis
{
public:
    NetworkIPNetmaskNotification(void) = default;
    virtual ~NetworkIPNetmaskNotification(void) = default;

    Common::Status Init(const IPAddress &aIPNetmask);

    /**
     *  @brief
     *    Return the Ethernet network interface IP netmask property.
     *
     *  @returns
     *    The IP netmask of the Ethernet network interface that changed.
     *
     */
    inline const IPAddress &GetIPNetmask(void) const { return GetIPAddress(); }
};

/**
 *  @brief
 *    An object for a HLX client physical Ethernet network interface
 *    object Control4 SDDP enabled property data model state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup network
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
