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
 *      This file defines an object for....
 *
 */

#ifndef OPENHLXPROXYINFRAREDCONTROLLER_HPP
#define OPENHLXPROXYINFRAREDCONTROLLER_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/InfraredControllerBasis.hpp>
#include <OpenHLX/Client/InfraredControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/InfraredControllerBasis.hpp>
#include <OpenHLX/Server/InfraredControllerBasis.hpp>
#include <OpenHLX/Server/InfraredControllerCommands.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Server
{

class ConnectionBasis;

}; // namespace Server

namespace Proxy
{

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup proxy
 *  @ingroup infrared
 *
 */
class InfraredController :
    public Proxy::ControllerBasis,
    public Common::InfraredControllerBasis,
    public Client::InfraredControllerBasis,
    public Server::InfraredControllerBasis
{
public:
    InfraredController(void);
    virtual ~InfraredController(void);

    Common::Status Init(Client::CommandManager &aCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    // Server-facing Client Command Completion Handler Trampolines

    // Server-facing Client Notification Handler Trampolines

    // Client-facing Server Command Request Handler Trampolines

private:
    // Proxy Handlers

public:
    // Proxy Handler Trampolines

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status DoRequestHandlers(const bool &aRegister);

    // Server-facing Client Command Completion Handlers

    // Server-facing Client Notification Handlers

    // Client-facing Server Command Completion Handlers
};

}; // namespace Proxy

}; // namespace HLX

#endif // HLXPROXYINFRAREDCONTROLLER_HPP
