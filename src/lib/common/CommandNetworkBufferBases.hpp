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
 *      This file defines base and derived objects for composing HLX
 *      commands that observe or set Ethernet network interface data
 *      model properties.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDNETWORKBUFFERBASES_HPP
#define OPENHLXCOMMONCOMMANDNETWORKBUFFERBASES_HPP

#include <OpenHLX/Common/CommandQueryBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>


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
 *    A base object for composing a HLX command for mutating a
 *    property with an EnabledType value.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EnabledBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a EnabledType from the
     *  network model.
     *
     */
    typedef Model::NetworkModel::EnabledType  EnabledType;

protected:
    EnabledBufferBasis(void) = default;
    virtual ~EnabledBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const char *aProperty, const EnabledType &aValue);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model DHCPv4 enabled property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class DHCPv4EnabledBufferBasis :
    public EnabledBufferBasis
{
protected:
    DHCPv4EnabledBufferBasis(void) = default;
    virtual ~DHCPv4EnabledBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const EnabledType &aEnabled);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model EUI-48 address property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EthernetEUI48BufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a EnabledType from the
     *  network model.
     *
     */
    typedef Model::NetworkModel::EthernetEUI48Type  EthernetEUI48Type;

protected:
    EthernetEUI48BufferBasis(void) = default;
    virtual ~EthernetEUI48BufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const EthernetEUI48Type &aEthernetEUI48);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model IP address property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class IPBufferBasis
{
protected:
    IPBufferBasis(void) = default;
    virtual ~IPBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const char *aProperty, const IPAddress &aIPAddress);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model default router IP address
 *    property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class IPDefaultRouterAddressBufferBasis :
    public IPBufferBasis
{
protected:
    IPDefaultRouterAddressBufferBasis(void) = default;
    virtual ~IPDefaultRouterAddressBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aIPAddress);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model host IP address property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class IPHostAddressBufferBasis :
    public IPBufferBasis
{
protected:
    IPHostAddressBufferBasis(void) = default;
    virtual ~IPHostAddressBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aIPAddress);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model IP netmask property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class IPNetmaskBufferBasis :
    public IPBufferBasis
{
protected:
    IPNetmaskBufferBasis(void) = default;
    virtual ~IPNetmaskBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const IPAddress &aIPAddress);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    Ethernet network interface data model Control4 SDDP enabled
 *    property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class SDDPEnabledBufferBasis :
    public EnabledBufferBasis
{
protected:
    SDDPEnabledBufferBasis(void) = default;
    virtual ~SDDPEnabledBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const EnabledType &aEnabled);
};

}; // namespace Network

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDNETWORKBUFFERBASES_HPP
