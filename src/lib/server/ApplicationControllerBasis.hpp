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
 *      This file defines a derivable object for creating HLX server
 *      application controllers.
 *
 */

#ifndef OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP
#define OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP


#include <map>

#include <OpenHLX/Common/ApplicationObjectControllerContainerTemplate.hpp>
#include <OpenHLX/Common/ConnectionManagerBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Server
{

class Controller;

namespace Application
{

class Controller;

/**
 *  @brief
 *    An object for effecting an HLX server application controller for
 *    any HLX server application.
 *
 *  @ingroup server
 *
 */
class ControllerBasis :
    public Common::Application::ObjectControllerContainerTemplate<Server::ObjectControllerBasis>
{
public:
    virtual ~ControllerBasis(void);

    // Initializer(s)

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    // Accessors

    const Server::CommandManager &     GetCommandManager(void) const;
    Server::CommandManager &           GetCommandManager(void);
    const Server::ConnectionManager &  GetConnectionManager(void) const;
    Server::ConnectionManager &        GetConnectionManager(void);

    // Listen

    Common::Status Listen(void);
    Common::Status Listen(const Common::ConnectionManagerBasis::Versions &aVersions);
    Common::Status Listen(const char *aMaybeURL);
    Common::Status Listen(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions);

protected:
    /**
     *  A convenience type for an object that manages a container of
     *  server object controllers.
     *
     */
    typedef Common::Application::ObjectControllerContainerTemplate<Server::ObjectControllerBasis> ServerObjectControllerContainer;

protected:
    ControllerBasis(void);

private:
    Server::ConnectionManager  mConnectionManager;
    Server::CommandManager     mCommandManager;
};

}; // namespace Application

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP
