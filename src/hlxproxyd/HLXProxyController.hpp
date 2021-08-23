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
 *      This file defines an object for effecting a HLX proxy
 *      controller.
 *
 */

#ifndef OPENHLXPROXYAPPLICATIONCONTROLLER_HPP
#define OPENHLXPROXYAPPLICATIONCONTROLLER_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/CommandManagerDelegate.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Client/ControllerBasisDelegate.hpp>
#include <OpenHLX/Client/HLXClientControllerBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HLXCommonControllerBasis.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/CommandManagerDelegate.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Server/ConnectionManagerDelegate.hpp>

#include "ConfigurationController.hpp"
#include "ConfigurationControllerDelegate.hpp"
#include "EqualizerPresetsController.hpp"
#include "FavoritesController.hpp"
#include "FrontPanelController.hpp"
#include "GroupsController.hpp"
#include "HLXProxyControllerDelegate.hpp"
#include "InfraredController.hpp"
#include "NetworkController.hpp"
#include "SourcesController.hpp"
#include "ZonesController.hpp"


namespace HLX
{

namespace Proxy
{

namespace Application
{

/**
 *  @brief
 *    An object for effecting a HLX proxy controller.
 *
 *  @ingroup proxy
 *
 */
class Controller :
    public Common::Application::Foo<Server::ControllerBasis>,
    public Client::Application::ControllerBasis,
    public Client::ConnectionManagerDelegate,
    public Server::ConnectionManagerDelegate,
    public Client::CommandManagerDelegate,
    public Server::CommandManagerDelegate,
    public Client::ControllerBasisErrorDelegate,
    public Client::ControllerBasisStateChangeDelegate,
    public Common::Application::Foo<Proxy::ControllerBasis>,
    public ConfigurationControllerDelegate
{
public:
    Controller(void);
    virtual ~Controller(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    ControllerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(ControllerDelegate *aDelegate);

    Common::Status Connect(const char *aMaybeURL);
    Common::Status Connect(const char *aMaybeURL, const Common::Timeout &aTimeout);
    Common::Status Connect(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions, const Common::Timeout &aTimeout);

    Common::Status Listen(void);
    Common::Status Listen(const Common::ConnectionManagerBasis::Versions &aVersions);
    Common::Status Listen(const char *aMaybeURL);
    Common::Status Listen(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions);


    // Server-facing Client Command Manager Delegate Methods

    // Server-facing Client Connection Manager Delegate Methods

    // Server-facing Client Connection Manager Connect Delegate Methods

    void ConnectionManagerWillConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerIsConnecting(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerDidConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Client-facing Server Connection Manager Delegate Methods

    // Client-facing Server Connection Manager Listen Delegate Methods

    void ConnectionManagerWillListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsListening(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Client-facing Server Connection Manager Accept Delegate Methods

    void ConnectionManagerWillAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsAccepting(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Common Connection Manager Delegate Methods

    // Common Connection Manager Resolve Delegate Methods

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError) final;

    // Common Connection Manager Disconnect Delegate Methods

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;

    // Common Connection Manager Error Delegate Method

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError) final;

    // Server-facing Client Object Controller Basis Delegate Methods

    void ControllerError(Client::ControllerBasis &aController, const Common::Error &aError) final;
    void ControllerStateDidChange(Client::ControllerBasis &aController, const Client::StateChange::NotificationBasis &aStateChangeNotification) final;

    // Client-facing Server Controller Basis Delegate Methods

    // Client-facing Server Configuration Controller Delegate Methods

    Common::Status QueryCurrentConfiguration(ConfigurationController &aController, Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) final;

private:
    Common::Status InitClient(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitClientConnectionManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitClientCommandManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServer(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServerConnectionManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServerCommandManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitControllers(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitClientControllers(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServerControllers(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitProxyControllers(const Common::RunLoopParameters &aRunLoopParameters);

    void DeriveGroupState(void) final { return; }

private:
    // Sub-controller order is important since 1) this is the order that
    // most closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command and 2) ...

    Common::RunLoopParameters       mRunLoopParameters;
    Client::ConnectionManager       mClientConnectionManager;
    Client::CommandManager          mClientCommandManager;
    Server::ConnectionManager       mServerConnectionManager;
    Server::CommandManager          mServerCommandManager;
    ConfigurationController         mConfigurationController;
    NetworkController               mNetworkController;
    FavoritesController             mFavoritesController;
    GroupsController                mGroupsController;
    FrontPanelController            mFrontPanelController;
    InfraredController              mInfraredController;
    EqualizerPresetsController      mEqualizerPresetsController;
    SourcesController               mSourcesController;
    ZonesController                 mZonesController;
    ControllerDelegate *            mDelegate;
};

}; // namespace Application

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYAPPLICATIONCONTROLLER_HPP
