/*
 *    Copyright (c) 2018-2021 Grant Erickson
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

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTNETWORKCONTROLLER_HPP
