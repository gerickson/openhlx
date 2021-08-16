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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP
#define OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP

#include <OpenHLX/Model/NetworkModel.hpp>
#include <OpenHLX/Server/NetworkControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup network
 *
 */
class NetworkControllerBasis
{
public:
    virtual ~NetworkControllerBasis(void);

protected:
    NetworkControllerBasis(Model::NetworkModel &aNetworkModel);

    // Initializer(s)

    Common::Status Init(void);

private:
    Common::Status RequestInit(void);

private:
    Model::NetworkModel & mNetworkModel;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

protected:
    // Observation (Query) Command Request Class (Static) Handlers

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

protected:
    static Server::Command::Network::QueryRequest  kQueryRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERNETWORKCONTROLLERBASIS_HPP
