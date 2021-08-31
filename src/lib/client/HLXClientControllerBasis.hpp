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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP


#include <map>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ObjectControllerBasis.hpp>
#include <OpenHLX/Client/ObjectControllerBasisRefreshDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HLXCommonControllerContainerTemplate.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>


namespace HLX
{

namespace Client
{

class Controller;

namespace Application
{

class Controller;
class ControllerRefreshDelegate;

/**
 *  @brief
 *    An object for effecting an HLX client controller for any HLX
 *    client application, whether a command line utility or a mobile
 *    app.
 *
 *  @ingroup client
 *
 */
class ControllerBasis :
    public Common::Application::ControllerContainerTemplate<Client::ObjectControllerBasis>,
    public ObjectControllerBasisRefreshDelegate
{
public:
    virtual ~ControllerBasis(void);

    // Initializer(s)

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    // Accessors

    const Client::CommandManager &     GetCommandManager(void) const;
    Client::CommandManager &           GetCommandManager(void);
    const Client::ConnectionManager &  GetConnectionManager(void) const;
    Client::ConnectionManager &        GetConnectionManager(void);

    Client::Application::ControllerRefreshDelegate *GetRefreshDelegate(void) const;

    Common::Status SetRefreshDelegate(Client::Application::ControllerRefreshDelegate *aRefreshDelegate);

    // Connection Management

    Common::Status Connect(const char *aMaybeURL);
    Common::Status Connect(const char *aMaybeURL, const Common::Timeout &aTimeout);
    Common::Status Connect(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions, const Common::Timeout &aTimeout);

    bool           IsConnected(void) const;

    Common::Status Disconnect(void);

    // Refresh

    Common::Status Refresh(void);

    // Controller Delegate Methods

    void ControllerIsRefreshing(Client::ObjectControllerBasis &aController, const uint8_t &aPercentComplete) final;
    void ControllerDidRefresh(Client::ObjectControllerBasis &aController) final;

protected:
    typedef Common::Application::ControllerContainerTemplate<Client::ObjectControllerBasis> ClientControllerContainer;

protected:
    ControllerBasis(void);

    bool IsRefreshing(void) const;

private:
    virtual void DeriveGroupState(void) = 0;

private:
    Client::ConnectionManager                         mConnectionManager;
    Client::CommandManager                            mCommandManager;
    size_t                                            mControllersDidRefreshCount;
    Client::Application::ControllerRefreshDelegate *  mRefreshDelegate;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP
