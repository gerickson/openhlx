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

Controller :: Controller(void) :
    Client::ConnectionManagerDelegate(),
    Server::ConnectionManagerDelegate(),
    Client::CommandManagerDelegate(),
    Server::CommandManagerDelegate(),
    Client::ControllerBasisDelegate(),
    mClientConnectionManager(),
    mClientCommandManager(),
    mServerConnectionManager(),
    mServerCommandManager(),
    mConfigurationController(),
    mZonesController(),
    mControllers(),
    mControllersDidRefreshCount(0),
    mDelegate(nullptr)
{
    DeclareScopedFunctionTracer(lTracer);
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
Controller :: ~Controller(void)
{
    DeclareScopedFunctionTracer(lTracer);
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

    lRetval = InitClient(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitServer(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

Status
Controller :: InitClient(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = InitClientConnectionManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitClientCommandManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitClientConnectionManager(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = mClientConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientConnectionManager.AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitClientCommandManager(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = mClientCommandManager.Init(mClientConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientCommandManager.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitServer(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = InitServerConnectionManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = InitServerCommandManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitServerConnectionManager(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = mServerConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mServerConnectionManager.AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitServerCommandManager(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    lRetval = mServerCommandManager.Init(mServerConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mServerCommandManager.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitControllers(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Controllers::iterator  lCurrent, lEnd;
    Status                 lRetval;


    (void)aRunLoopParameters;

    // Place the various controllers into the controller
    // container. Order is important since:
    //
    // 1) this is the order that most closely matches the order in
    //    which the actual HLX hardware responds to for the 'query
    //    current configuration' command.
    //
    // 2) this is the priority we want to run operations like refresh.

    AddController(mConfigurationController);
    AddController(mZonesController);

    // Intialize the controllers (skipping the configuration
    // controller as we just handled that).

    lCurrent = mControllers.begin();
    lEnd     = mControllers.end();

    while (lCurrent != lEnd)
    {
        lRetval = lCurrent->second.mController->Init(mClientCommandManager,
                                                     mServerCommandManager);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = static_cast<Client::ControllerBasis *>(lCurrent->second.mController)->SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

void
Controller :: AddController(ControllerBasis &aController)
{
    DeclareScopedFunctionTracer(lTracer);
    const ControllerState lControllerState = { &aController };


    mControllers[&aController] = lControllerState;

    return;
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the default timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
Controller :: Connect(const char *aMaybeURL)
{
    Status lRetval = kStatus_Success;

    lRetval = Connect(aMaybeURL, kTimeoutDefault);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
Controller :: Connect(const char *aMaybeURL, const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    lRetval = mClientConnectionManager.Connect(aMaybeURL, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL, host name, or host name and port with the
 *  provided timeout using IPv4 or IPv6 resolved addresses as
 *  specified.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the host name.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
Controller :: Connect(const char *aMaybeURL,
                      const Common::ConnectionManagerBasis::Versions &aVersions,
                      const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    lRetval = mClientConnectionManager.Connect(aMaybeURL, aVersions, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: Listen(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mServerConnectionManager.Listen();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
Controller :: Listen(const Common::ConnectionManagerBasis::Versions &aVersions)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mServerConnectionManager.Listen(aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
Controller :: Listen(const char *aMaybeURL)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mServerConnectionManager.Listen(aMaybeURL);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: Listen(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mServerConnectionManager.Listen(aMaybeURL, aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Refresh the state of the client controller.
 *
 *  This should be called on first-time client start-up or whenever
 *  the client controller state needs to be forcibly refreshed.
 *
 *  This iterates through each of the sub-controllers, tasking each
 *  with taking care of the refresh activity appropriate for its scope
 *  of concern.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        by a controller to perform the
 *                                        refresh.
 *
 */
Status
Controller :: Refresh(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status                 lRetval = kStatus_Success;
    Controllers::iterator  lCurrent, lEnd;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillRefresh(*this);
    }

    // Reset the overall refresh count.

    mControllersDidRefreshCount = 0;

    // Begin refreshing each controller.

    lCurrent = mControllers.begin();
    lEnd     = mControllers.end();

    while (lCurrent != lEnd)
    {
        lRetval = static_cast<Client::ControllerBasis *>(lCurrent->second.mController)->Refresh();
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the controller is in the middle of a refresh.
 *
 *  This returns a Boolean indicating whether (true) or not (false)
 *  the controller is in the middle of a refresh operation with the
 *  peer server controller for up-to-date state.
 *
 *  @returns
 *    True if the controller is refreshing; otherwise, false.
 *
 */
bool
Controller :: IsRefreshing(void) const
{
    return (mControllersDidRefreshCount != mControllers.size());
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
Controller :: SetDelegate(ControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    if (aDelegate == mDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

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
Controller :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost)
{
    (void)aConnectionManager;

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
Controller :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost)
{
    (void)aConnectionManager;

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
Controller :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;

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
Controller :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;

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
Controller :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillDisconnect(*this, aURLRef);
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
Controller :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidDisconnect(*this, aURLRef, aError);
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
Controller :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotDisconnect(*this, aURLRef, aError);
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
Controller :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerError(*this, aError);
    }
}

// MARK: Server-facing Client Controller Basis Delegate Methods

void
Controller :: ControllerIsRefreshing(Client::ControllerBasis &aController, const uint8_t &aPercentComplete)
{
    Proxy::ControllerBasis *    lController = static_cast<Proxy::ControllerBasis *>(&aController);
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = mControllers.find(lController);

    if (lControllerIterator != mControllers.end())
    {
        static const Percentage kPercentCompletePerController = CalculatePercentage(1,
                                                                                    static_cast<uint8_t>(mControllers.size()));
        const Percentage        lControllersPercentComplete   = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                                    static_cast<uint8_t>(mControllers.size()));
        const Percentage        lPercentComplete              = (lControllersPercentComplete + ((kPercentCompletePerController * aPercentComplete) / 100));

        if (mDelegate != nullptr)
        {
            mDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }
    }
}

/**
 *  @brief
 *    Delegation from a controller that the specified controller is
 *    done refreshing.
 *
 *  On the refresh completion of any one controller, this refreshes
 *  the overall refresh state of the parent client controller.
 *
 *  @param[in]  aController       A reference to the controller
 *                                that issued the delegation.
 *
 */
void
Controller :: ControllerDidRefresh(Client::ControllerBasis &aController)
{
    Proxy::ControllerBasis *    lController = static_cast<Proxy::ControllerBasis *>(&aController);
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = mControllers.find(lController);

    if (lControllerIterator != mControllers.end())
    {
        mControllersDidRefreshCount++;

        if (mDelegate != nullptr)
        {
            const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                    static_cast<uint8_t>(mControllers.size()));

            mDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }

        if (mControllersDidRefreshCount == mControllers.size())
        {
            if (mDelegate != nullptr)
            {
                mDelegate->ControllerDidRefresh(*this);
            }
        }
    }

    return;
}

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
    return;
}

void
Controller :: ControllerError(Client::ControllerBasis &aController, const Common::Error &aError)
{
    return;
}

}; // namespace Proxy

}; // namespace HLX
