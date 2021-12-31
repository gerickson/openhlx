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
 *      This file defines a derivable object for realizing a HLX
 *      Ethernet network interface controller, in a server.
 *
 */

#ifndef OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP
#define OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP

#include <OpenHLX/Model/NetworkModel.hpp>
#include <OpenHLX/Server/NetworkControllerCommands.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A derivable object for realizing a HLX Ethernet network
 *    interface controller, in a server.
 *
 *  @ingroup server
 *  @ingroup network
 *
 */
class NetworkControllerBasis :
    public Server::ObjectControllerBasis
{
public:
    virtual ~NetworkControllerBasis(void);

protected:
    NetworkControllerBasis(Model::NetworkModel &aNetworkModel);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

private:
    Model::NetworkModel & mNetworkModel;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status HandleQueryReceived(const bool &aIsConfiguration,
                                       const char *aInputBuffer,
                                       Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer) const;

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

    static Common::Status HandleDHCPv4EnabledResponse(const Model::NetworkModel::EnabledType &aEnabled,
                                                      Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleDefaultRouterAddressResponse(const Common::IPAddress &aDefaultRouterAddress,
                                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleEthernetEUI48Response(const Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48,
                                                      Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleHostAddressResponse(const Common::IPAddress &aHostAddress,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleNetmaskResponse(const Common::IPAddress &aNetmask,
                                                Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleSDDPEnabledResponse(const Model::NetworkModel::EnabledType &aEnabled,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

protected:
    static Server::Command::Network::QueryRequest             kQueryRequest;
    static Server::Command::Network::SetDHCPv4EnabledRequest  kSetDHCPv4EnabledRequest;
    static Server::Command::Network::SetSDDPEnabledRequest    kSetSDDPEnabledRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP
