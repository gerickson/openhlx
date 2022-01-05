/*
 *    Copyright (c) 2018-2022 Grant Erickson
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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a HLX Ethernet network interface.
 *
 */

#ifndef OPENHLXCLIENTNETWORKCONTROLLER_HPP
#define OPENHLXCLIENTNETWORKCONTROLLER_HPP

#include <OpenHLX/Common/NetworkControllerBasis.hpp>
#include <OpenHLX/Client/NetworkControllerBasis.hpp>
#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a HLX Ethernet network interface.
 *
 *  @ingroup client
 *  @ingroup network
 *
 */
class NetworkController :
    public Common::NetworkControllerBasis,
    public Client::NetworkControllerBasis
{
public:
    NetworkController(void);
    virtual ~NetworkController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status GetDHCPv4Enabled(Model::NetworkModel::EnabledType &aDHCPv4Enabled) const;
    Common::Status GetEthernetEUI48(Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48) const;
    Common::Status GetDefaultRouterIPAddress(Model::NetworkModel::IPAddressType &aDefaultRouterAddress) const;
    Common::Status GetHostIPAddress(Model::NetworkModel::IPAddressType &aHostAddress) const;
    Common::Status GetIPNetmask(Model::NetworkModel::IPAddressType &aNetmask) const;
    Common::Status GetSDDPEnabled(Model::NetworkModel::EnabledType &aSDDPEnabled) const;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTNETWORKCONTROLLER_HPP
