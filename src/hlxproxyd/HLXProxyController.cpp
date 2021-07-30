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
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

Controller :: Controller(void) :
    Client::ConnectionManagerDelegate(),
    Client::CommandManagerDelegate(),
    mClientConnectionManager(),
    mClientCommandManager(),
    mServerConnectionManager(),
    mDelegate(nullptr),
    mDelegateContext(nullptr)
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
 *  This initializes the client controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  client controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the client controller was already
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

    lRetval = mClientConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientConnectionManager.AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientCommandManager.Init(mClientConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientCommandManager.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mServerConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mServerConnectionManager.AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
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

ControllerDelegate *
Controller :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
Controller :: SetDelegate(ControllerDelegate *aDelegate, void *aContext)
{
    Status lRetval = kStatus_Success;

    if ((aDelegate == mDelegate) && (aContext == mDelegateContext))
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mDelegate        = aDelegate;
    mDelegateContext = aContext;

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

}; // namespace Proxy

}; // namespace HLX
