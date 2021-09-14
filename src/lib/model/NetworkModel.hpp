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
 *      This file defines an object for managing an HLX network
 *      connectivity data model.
 *
 */

#ifndef OPENHLXMMODELNETWORKMODEL_HPP
#define OPENHLXMMODELNETWORKMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing an HLX network connectivity data model.
 *
 *  @ingroup model
 *
 */
class NetworkModel
{
public:
    /**
     *  A type for the Ethernet network interface enabled state.
     *
     */
    typedef bool    EnabledType;

    /**
     *  A type for the Ethernet network interface hardware address.
     *
     */
    typedef uint8_t EthernetAddressType[6];

    /**
     *  A type for the Ethernet network interface IPv4 or IPv6 network
     *  address.
     *
     */
    typedef uint8_t IPAddressType[17];

    /**
     *  A type for the Ethernet network interface IPv4 or IPv6 network
     *  prefix length.
     *
     */
    typedef uint8_t PrefixLengthType;

public:
    NetworkModel(void);
    virtual ~NetworkModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const NetworkModel &aNetworkModel);

    NetworkModel &operator =(const NetworkModel &aNetworkModel);

    Common::Status GetHLXAddress(IPAddressType &aHLXAddress) const;
    Common::Status GetPrefixLength(PrefixLengthType &aPrefixLength) const;
    Common::Status GetGatewayAddress(IPAddressType &aGatewayAddress) const;
    Common::Status GetEthernetAddress(EthernetAddressType &aEthernetAddress) const;
    Common::Status GetDHCPv4Enabled(EnabledType &aDHCPv4Enabled) const;
    Common::Status GetSDDPEnabled(EnabledType &aSDDPEnabled) const;

    Common::Status SetHLXAddress(const IPAddressType &aHLXAddress);
    Common::Status SetPrefixLength(const PrefixLengthType &aPrefixLength);
    Common::Status SetGatewayAddress(const IPAddressType &aGatewayAddress);
    Common::Status SetEthernetAddress(const EthernetAddressType &aEthernetAddress);
    Common::Status SetDHCPv4Enabled(const EnabledType &aDHCPv4Enabled);
    Common::Status SetSDDPEnabled(const EnabledType &aSDDPEnabled);

    bool operator ==(const NetworkModel &aNetworkModel) const;

private:
    bool                 mHLXAddressIsNull;
    IPAddressType        mHLXAddress;
    bool                 mPrefixLengthIsNull;
    PrefixLengthType     mPrefixLength;
    bool                 mGatewayAddressIsNull;
    IPAddressType        mGatewayAddress;
    bool                 mEthernetAddressIsNull;
    EthernetAddressType  mEthernetAddress;
    bool                 mDHCPv4EnabledIsNull;
    EnabledType          mDHCPv4Enabled;
    bool                 mSDDPEnabledIsNull;
    EnabledType          mSDDPEnabled;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELNETWORKMODEL_HPP
