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

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/CommandManagerDelegate.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include "HLXProxyControllerDelegate.hpp"


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
    public Client::CommandManagerDelegate
{
public:
    Controller(void);
    ~Controller(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    ControllerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(ControllerDelegate *aDelegate, void *aContext);

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

    // Common Connection Manager Delegate Methods

    // Resolve

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError) final;

    // Disconnect

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::Error &aError) final;


    // Client-facing Server Connection Manager Delegate Methods

private:
    Common::RunLoopParameters       mRunLoopParameters;
    Client::ConnectionManager       mClientConnectionManager;
    Client::CommandManager          mClientCommandManager;
    ControllerDelegate *            mDelegate;
    void *                          mDelegateContext;
};
}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLER_HPP
