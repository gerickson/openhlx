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

#ifndef OPENHLXPROXYCONTROLLER_HPP
#define OPENHLXPROXYCONTROLLER_HPP

#include <map>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/CommandManagerDelegate.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Client/ControllerBasisDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/CommandManagerDelegate.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Server/ConnectionManagerDelegate.hpp>

#include "HLXProxyControllerDelegate.hpp"
#include "ZonesController.hpp"


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    An object for effecting a HLX proxy controller.
 *
 *  @ingroup proxy
 *
 */
class Controller :
    public Client::ConnectionManagerDelegate,
    public Server::ConnectionManagerDelegate,
    public Client::CommandManagerDelegate,
    public Server::CommandManagerDelegate,
    public Client::ControllerBasisDelegate
{
public:
    Controller(void);
    ~Controller(void);

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

    Common::Status Refresh(void);

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

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError) final;

    // Common Connection Manager Disconnect Delegate Methods

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Common Connection Manager Error Delegate Method

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::Error &aError) final;

    // Server-facing Client Controller Basis Delegate Methods

    void ControllerIsRefreshing(Client::ControllerBasis &aController, const uint8_t &aPercentComplete) final;
    void ControllerDidRefresh(Client::ControllerBasis &aController) final;
    void ControllerError(Client::ControllerBasis &aController, const Common::Error &aError) final;
    void ControllerStateDidChange(Client::ControllerBasis &aController, const Client::StateChange::NotificationBasis &aStateChangeNotification) final;

    // Client-facing Server Controller Basis Delegate Methods

private:
    Common::Status InitClient(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitClientConnectionManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitClientCommandManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServer(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServerConnectionManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitServerCommandManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitControllers(const Common::RunLoopParameters &aRunLoopParameters);

    void AddController(ControllerBasis &aController);

    bool IsRefreshing(void) const;

private:
    struct ControllerState
    {
        ControllerBasis * mController;
    };

    typedef std::map<ControllerBasis *, ControllerState> Controllers;

    // Sub-controller order is important since 1) this is the order that
    // most closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command and 2) ...

    Common::RunLoopParameters       mRunLoopParameters;
    Client::ConnectionManager       mClientConnectionManager;
    Client::CommandManager          mClientCommandManager;
    Server::ConnectionManager       mServerConnectionManager;
    Server::CommandManager          mServerCommandManager;
    ZonesController                 mZonesController;
    Controllers                     mControllers;
    size_t                          mControllersDidRefreshCount;
    ControllerDelegate *            mDelegate;
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLER_HPP
