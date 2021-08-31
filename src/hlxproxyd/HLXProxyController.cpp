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
 *      This file implements an object for effecting a HLX proxy
 *      controller.
 *
 */

#include "HLXProxyController.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

namespace Application
{

Controller :: Controller(void) :
    Common::Application::ControllerBasis(),
    Client::Application::ControllerBasis(),
    Server::Application::ControllerBasis(),
    Common::Application::ControllerContainerTemplate<Proxy::ObjectControllerBasis>(),
    Client::ConnectionManagerDelegate(),
    Server::ConnectionManagerDelegate(),
    Client::CommandManagerDelegate(),
    Server::CommandManagerDelegate(),
    Client::ControllerBasisErrorDelegate(),
    Client::ControllerBasisStateChangeDelegate(),
    ConfigurationControllerDelegate(),
    mConfigurationController(),
    mNetworkController(),
    mFavoritesController(),
    mGroupsController(),
    mFrontPanelController(),
    mInfraredController(),
    mEqualizerPresetsController(),
    mSourcesController(),
    mZonesController(),
    mDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
Controller :: ~Controller(void)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the proxy controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  proxy controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the proxy controller was already
 *                                    added as a delegate to the
 *                                    connection manager, command
 *                                    manager, or child controllers.
 *
 */
Status
Controller :: Init(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = Common::Application::ControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::Application::ControllerBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::Application::ControllerBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ProxyControllerContainer::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitClient();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitServer();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

Status
Controller :: InitClient(void)
{
    Status  lRetval;


    lRetval = Client::Application::ControllerBasis::GetConnectionManager().AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::Application::ControllerBasis::GetCommandManager().SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}


Status
Controller :: InitServer(void)
{
    Status  lRetval;


    lRetval = Server::Application::ControllerBasis::GetConnectionManager().AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::Application::ControllerBasis::GetCommandManager().SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitControllers(const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval;

    lRetval = InitClientControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitServerControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitProxyControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitClientControllers(const RunLoopParameters &aRunLoopParameters)
{
    Status  lRetval = kStatus_Success;


    (void)aRunLoopParameters;

    // Place the various controllers into the client controller
    // container. Order is important since this is the priority we
    // want to run client operations like refresh.

    Client::Application::ControllerBasis::AddController(mConfigurationController);
    Client::Application::ControllerBasis::AddController(mSourcesController);
    Client::Application::ControllerBasis::AddController(mZonesController);
    Client::Application::ControllerBasis::AddController(mGroupsController);
    Client::Application::ControllerBasis::AddController(mFavoritesController);
    Client::Application::ControllerBasis::AddController(mEqualizerPresetsController);
    Client::Application::ControllerBasis::AddController(mInfraredController);
    Client::Application::ControllerBasis::AddController(mFrontPanelController);
    Client::Application::ControllerBasis::AddController(mNetworkController);

    return (lRetval);
}

Status
Controller :: InitServerControllers(const RunLoopParameters &aRunLoopParameters)
{
    Status  lRetval = kStatus_Success;


    (void)aRunLoopParameters;

    // Place the various controllers into the server controller
    // container. Order is important since this is the order that most
    // closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command.

    Server::Application::ControllerBasis::AddController(mConfigurationController);
    Server::Application::ControllerBasis::AddController(mNetworkController);
    Server::Application::ControllerBasis::AddController(mFavoritesController);
    Server::Application::ControllerBasis::AddController(mGroupsController);
    Server::Application::ControllerBasis::AddController(mFrontPanelController);
    Server::Application::ControllerBasis::AddController(mInfraredController);
    Server::Application::ControllerBasis::AddController(mEqualizerPresetsController);
    Server::Application::ControllerBasis::AddController(mSourcesController);
    Server::Application::ControllerBasis::AddController(mZonesController);

    return (lRetval);
}

Status
Controller :: InitProxyControllers(const RunLoopParameters &aRunLoopParameters)
{
    Status                 lRetval;


    (void)aRunLoopParameters;

    // Place the various controllers into the server controller
    // container. Order is important since this is the order that most
    // closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command.

    ProxyControllerContainer::AddController(mConfigurationController);
    ProxyControllerContainer::AddController(mNetworkController);
    ProxyControllerContainer::AddController(mFavoritesController);
    ProxyControllerContainer::AddController(mGroupsController);
    ProxyControllerContainer::AddController(mFrontPanelController);
    ProxyControllerContainer::AddController(mInfraredController);
    ProxyControllerContainer::AddController(mEqualizerPresetsController);
    ProxyControllerContainer::AddController(mSourcesController);
    ProxyControllerContainer::AddController(mZonesController);

    {
        ProxyControllerContainer::Controllers::iterator  lCurrent = ProxyControllerContainer::GetControllers().begin();
        ProxyControllerContainer::Controllers::iterator  lEnd = ProxyControllerContainer::GetControllers().end();

        // Intialize the controllers, using the top-down proxy initializer.

        while (lCurrent != lEnd)
        {
            lRetval = lCurrent->second.mController->Init(Client::Application::ControllerBasis::GetCommandManager(),
                                                         Server::Application::ControllerBasis::GetCommandManager(),
                                                         kTimeoutDefault);
            nlREQUIRE_SUCCESS(lRetval, done);

            lCurrent++;
        }

        // Explicitly set this parent controller to be the delegate
        // for fanout of any proxy configuration controller
        // delegations.

        mConfigurationController.SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    {
        ClientControllerContainer::Controllers::iterator  lCurrent = ClientControllerContainer::GetControllers().begin();
        ClientControllerContainer::Controllers::iterator  lEnd = ClientControllerContainer::GetControllers().end();

        while (lCurrent != lEnd)
        {
            lRetval = lCurrent->second.mController->SetErrorDelegate(this);
            nlREQUIRE_SUCCESS(lRetval, done);

            lRetval = lCurrent->second.mController->SetRefreshDelegate(this);
            nlREQUIRE_SUCCESS(lRetval, done);

            lRetval = lCurrent->second.mController->SetStateChangeDelegate(this);
            nlREQUIRE_SUCCESS(lRetval, done);

            lCurrent++;
        }
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the proxy controller.
 *
 *  @returns
 *    A pointer to the delegate for the proxy controller.
 *
 */
ControllerDelegate *
Controller :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the proxy controller.
 *
 *  This attempts to set a delegate for the proxy controller.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
Controller :: SetDelegate(Proxy::Application::ControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    if (aDelegate == mDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    lRetval = Client::Application::ControllerBasis::SetRefreshDelegate(aDelegate);
    nlREQUIRE_SUCCESS(lRetval, done);

    mDelegate        = aDelegate;

done:
    return (lRetval);
}

// MARK: Server-facing Client Command Manager Delegate Methods

// MARK: Server-facing Client Connection Manager Delegate Methods

// MARK: Server-facing Client Connection Manager Connect Delegate Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
Controller :: ConnectionManagerWillConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillConnect(*this, aURLRef, aTimeout);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server is connecting.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
Controller :: ConnectionManagerIsConnecting(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsConnecting(*this, aURLRef, aTimeout);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *
 */
void
Controller :: ConnectionManagerDidConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidConnect(*this, aURLRef);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed connection.
 *
 */
void
Controller :: ConnectionManagerDidNotConnect(Client::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotConnect(*this, aURLRef, aError);
    }
}

// MARK: Client-facing Server Connection Manager Delegate Methods

// MARK: Client-facing Server Connection Manager Listen Delegate Methods

void Controller :: ConnectionManagerWillListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillListen(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerIsListening(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsListening(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidListen(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidNotListen(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotListen(*this, aURLRef, aError);
    }
}

// MARK: Client-facing Server Connection Manager Accept Delegate Methods

void Controller :: ConnectionManagerWillAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillAccept(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerIsAccepting(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsAccepting(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidAccept(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidNotAccept(Server::ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotAccept(*this, aURLRef, aError);
    }
}

// MARK: Common Connection Manager Delegate Methods

// MARK: Common Connection Manager Resolve Delegate Methods

/**
 *  @brief
 *    Delegation from the connection manager that a host name will
 *    resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that will resolve.
 *
 */
void
Controller :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillResolve(*this, aHost);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name is
 *    resolving.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that is resolving.
 *
 */
void
Controller :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsResolving(*this, aHost);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name has
 *    resolved to an IP address.
 *
 *  @note
 *    This delegation may be called more than once for a
 *    resolution, once for each IP address the host name resolves
 *    to.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did resolve.
 *  @param[in]  aIPAddress          An immutable reference to an IP
 *                                  address that the host name
 *                                  resolved to.
 *
 */
void
Controller :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidResolve(*this, aHost, aIPAddress);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name did
 *    not resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did not resolve.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  resolution.
 *
 */
void
Controller :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotResolve(*this, aHost, aError);
    }
}

// MARK: Common Connection Manager Disconnect Delegate Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *
 */
void
Controller :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillDisconnect(*this, aRoles, aURLRef);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the disconnection.
 *
 */
void
Controller :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidDisconnect(*this, aRoles, aURLRef, aError);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  disconnection.
 *
 */
void
Controller :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotDisconnect(*this, aRoles, aURLRef, aError);
    }
}

// MARK: Common Connection Manager Error Delegate Method

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server experienced an error.
 *
 *  @note
 *    This delegation may occur along with other delegations with
 *    respect to the same underlying event or cause.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the event.
 *
 */
void
Controller :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerError(*this, aRoles, aError);
    }
}

// MARK: Server-facing Client Object Controller Basis Delegate Methods

// MARK: Server-facing Client Object Controller Basis Error Delegate Methods

void
Controller :: ControllerError(Client::ControllerBasis &aController, const Common::Error &aError)
{
    (void)aController;
    (void)aError;

    return;
}

// MARK: Server-facing Client Object Controller Basis State Change Delegate Methods

/**
 *  Delegation callback for individual sub-controller state change
 *  notifications.
 *
 *  This is not simply a pass-through of sub-controller state change
 *  delegate to the end client due to the fact that some group
 *  sub-controller state changes need to be fanned out to the zone
 *  sub-controller for the zones that belong to a particular group.
 *
 *  In theory and ideally, Audio Authority would have implemented a
 *  group mute, source, or volume change as follows:
 *
 *    \<Group j Mute or Volume or Source Command Request>
 *    \<Zone i Mute or Volume or Source State Change>
 *    ...
 *    \<Zone n Mute or Volume or Source State Change>
 *    \<Group Mute or Volume or Source Command Response>
 *
 *  However, instead, all that we get in practice is:
 *
 *    \<Group j Mute or Volume or Source Command Request>
 *    \<Group j Mute or Volume or Source Command Response>
 *
 *  Leaving us to extract zone membership for the relevant group from
 *  the group sub-controller and to then intuit and apply the mute,
 *  volume, or source changes to the zone members based on the group
 *  command response.
 *
 *  @param[in]  aController               A reference to the controller
 *                                        that initiated the state change
 *                                        notification.
 *
 *  @param[in]  aStateChangeNotification  A read-only reference to the
 *                                        state change notification.
 *
 */
void
Controller :: ControllerStateDidChange(Client::ControllerBasis &aController,
                                       const Client::StateChange::NotificationBasis &aStateChangeNotification)
{
    (void)aController;
    (void)aStateChangeNotification;

    return;
}

// MARK: Client-facing Server Configuration Controller Delegate Methods

Status
Controller :: QueryCurrentConfiguration(ConfigurationController &aController, Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ProxyControllerContainer::Controllers::iterator  lCurrent, lEnd;
    Status                 lRetval;


    (void)aController;

    lCurrent = ProxyControllerContainer::GetControllers().begin();
    lEnd     = ProxyControllerContainer::GetControllers().end();

    while (lCurrent != lEnd)
    {
        lRetval = lCurrent->second.mController->QueryCurrentConfiguration(aConnection, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

done:
    return (lRetval);
}

}; // namespace Application

}; // namespace Proxy

}; // namespace HLX
