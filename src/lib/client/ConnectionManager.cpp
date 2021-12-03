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
 *      This file implements an object for managing a peer-to-peer
 *      connection between an HLX client and server.
 *
 */

#include "ConnectionManager.hpp"

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/HostURL.hpp>
#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Client/ConnectionBasis.hpp>
#include <OpenHLX/Client/ConnectionFactory.hpp>
#include <OpenHLX/Client/ConnectionTelnet.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

static Common::Status
CreateURL(const CFString &aScheme, const IPAddress &aIPAddress, const int32_t &aPossiblePort, CFURLRef &outURL)
{
    static const char * const  kSchemeDelimiter      = ":";
    static const char * const  kNetLocationDelimiter = "//";
    static const char * const  kPathDelimiter        = "/";
    static const char * const  kPortDelimiter        = ":";
    static const char * const  kIPv4StartDelimiter   = "";
    static const char * const  kIPv4EndDelimiter     = "";
    static const char * const  kIPv6StartDelimiter   = "[";
    static const char * const  kIPv6EndDelimiter     = "]";
    const char *               lIPStartDelimiter;
    const char *               lIPEndDelimiter;
    CFIndex                    lSchemeLength;
    IPAddress::Version         lVersion;
    char                       lBuffer[INET6_ADDRSTRLEN];
    OutputStringStream         lURLStream;
    HostURL                    lURL;
    Status                     lRetval = kStatus_Success;


    lSchemeLength = aScheme.GetLength();
    nlREQUIRE_ACTION(lSchemeLength > 0, done, lRetval = -EINVAL);

    lRetval = aIPAddress.GetVersion(lVersion);
    nlREQUIRE_SUCCESS(lRetval, done);

    nlREQUIRE_ACTION(aPossiblePort <= UINT16_MAX, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aPossiblePort >= -1, done, lRetval = -EINVAL);

    lRetval = aIPAddress.ToString(&lBuffer[0], sizeof (lBuffer));
    nlREQUIRE_SUCCESS(lRetval, done);

    switch (lVersion)
    {

    case IPAddress::Version::kIPv4:
        lIPStartDelimiter = kIPv4StartDelimiter;
        lIPEndDelimiter   = kIPv4EndDelimiter;
        break;

    case IPAddress::Version::kIPv6:
        lIPStartDelimiter = kIPv6StartDelimiter;
        lIPEndDelimiter   = kIPv6EndDelimiter;
        break;

    default:
        lRetval = -EINVAL;
        break;

    }

    if (lRetval == kStatus_Success)
    {
        lURLStream << aScheme.GetCString()
                   << kSchemeDelimiter
                   << kNetLocationDelimiter
                   << lIPStartDelimiter
                   << lBuffer
                   << lIPEndDelimiter;

        if (aPossiblePort != -1)
        {
            lURLStream << kPortDelimiter
                       << aPossiblePort;
        }

        lURLStream << kPathDelimiter;

        lRetval = lURL.Init(lURLStream.str().c_str());
        nlREQUIRE_SUCCESS(lRetval, done);

        outURL = CFURetain(lURL.GetURL());
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionManager :: ConnectionManager(void) :
    ConnectionManagerBasis(),
    mRunLoopParameters(),
    mConnectionFactory(),
    mConnection(nullptr),
    mConnectionTimer(),
    mDelegates()
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection manager with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection manager with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          Resources for the connection factory could
 *                            not be allocated.
 *
 */
Status
ConnectionManager :: Init(const RunLoopParameters &aRunLoopParameters)
{
    static const ConnectionManagerBasis::Roles kRoles = kRoleClient;
    Status lRetval = kStatus_Success;


    lRetval = ConnectionManagerBasis::Init(kRoles, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionFactory.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Determine whether the manager supports connections with the
 *    specified protocol scheme.
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme for which to check
 *                          support.
 *
 *  @returns
 *     True if the scheme is supported; otherwise, false.
 *
 */
bool
ConnectionManager :: SupportsScheme(CFStringRef aScheme) const
{
    bool lRetval = false;

    lRetval = mConnectionFactory.SupportsScheme(aScheme);

    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL, host name, or host name and port with the
 *  provided timeout using either IPv4 or IPv6 resolved addresses.
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
ConnectionManager :: Connect(const char *aMaybeURL, const Timeout &aTimeout)
{
    static constexpr Versions kVersions = (kIPv4 | kIPv6);


    return (Connect(aMaybeURL, kVersions, aTimeout));
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
ConnectionManager :: Connect(const char *aMaybeURL,
                             const Versions &aVersions,
                             const Timeout &aTimeout)
{
    CFString                    lHostName;
    CFString                    lScheme;
    int32_t                     lPossiblePort;
    CFURLRef                    lURLRef = nullptr;
    IPAddresses                 lIPAddresses;
    IPAddresses::const_iterator lCurrent;
    IPAddresses::const_iterator lLast;
    Status                      lRetval = kStatus_Success;


    // First, determine whether we were given a fully-formed URL from
    // which we need to extract a host name from the network location
    // or if we were simply given a host name or IP address (v4 or
    // v6).

    lRetval = ParseURL(aMaybeURL, lURLRef);

    if (lRetval == kStatus_Success)
    {
        // If the URL decoding/conversion was successful, then we have
        // at least a host name (which may be just an IP address) and
        // a scheme. We may or may not have a port.
        //
        // Resolve the host name and then start forming URLs from the
        // resolved IP addresses and attempt to connect to a URL
        // formed from them and the scheme until one succeeds.

        lHostName = CFURLCopyHostName(lURLRef);
        nlREQUIRE_ACTION(lHostName.GetString() != nullptr, done, lRetval = -ENOMEM);

        lScheme = CFURLCopyScheme(lURLRef);
        nlREQUIRE_ACTION(lScheme.GetString() != nullptr, done, lRetval = -ENOMEM);

        lPossiblePort = CFURLGetPortNumber(lURLRef);

        lRetval = Resolve(lHostName.GetCString(),
                          aVersions,
                          lIPAddresses);
        nlREQUIRE_SUCCESS(lRetval, done);

        CFURelease(lURLRef);
    }
    else if (lRetval == -EINVAL)
    {
        std::string lHost;

        // Otherwise, if the URL decoding was not successful, default
        // to the only connection scheme currently supported, telnet,
        // and try parsing out a host or IP address or ohst or IP
        // address and port.

        lScheme = ConnectionTelnet::kScheme;

        lRetval = ParseHostOrHostAndPort(aMaybeURL,
                                         lHost,
                                         lPossiblePort);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = Resolve(lHost.c_str(),
                          aVersions,
                          lIPAddresses);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    // If there's an IP address to connect to, create a URL from it
    // and attempt to connect to it.

    lCurrent = lIPAddresses.begin();
    lLast = lIPAddresses.end();

    if (lCurrent != lLast)
    {
        lRetval = CreateURL(lScheme, *lCurrent, lPossiblePort, lURLRef);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = Connect(lURLRef, aTimeout);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    CFURelease(lURLRef);

    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aURLRef   A reference to a CoreFoundation URL for the
 *                        HLX server peer to connect to.
 *  @param[in]  aTimeout  An immutable reference to the timeout by
 *                        which the connection should complete.
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
ConnectionManager :: Connect(CFURLRef aURLRef,
                             const Common::Timeout &aTimeout)
{
    ConnectionBasis * lConnection = nullptr;
    Status            lRetval = kStatus_Success;


    // If there is already a non-null connection, then there is an
    // active connection or one in flight. If that's the case, return
    // the appropriate error.

    if (mConnection != nullptr)
    {
       nlREQUIRE_ACTION(!mConnection->IsConnected(), done, lRetval = -EALREADY);
       nlREQUIRE_ACTION(!mConnection->IsConnecting(), done, lRetval = -EINPROGRESS);
    }
    else
    {
        lConnection = mConnectionFactory.GetConnection(aURLRef);
        nlREQUIRE_ACTION(lConnection != nullptr, done, lRetval = -EPROTONOSUPPORT);

        mConnection = lConnection;

        lRetval = mConnection->SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (aTimeout.IsMilliseconds())
    {
        lRetval = mConnectionTimer.Init(mRunLoopParameters, aTimeout);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = mConnectionTimer.SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    lRetval = mConnection->Connect(aURLRef, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aTimeout.IsMilliseconds())
    {
        lRetval = mConnectionTimer.Start();
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the HLX server peer.
 *
 *  This attempts to disconnect from the connected HLX server peer, if
 *  any.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENXIO                   If there is no allocated connection.
 *  @retval  -EALREADY                If the connection is already
 *                                    disconnected.
 *  @retval  -EINPROGRESS             If the connection is already in
 *                                    the process of disconnecting.
 *
 */
Status
ConnectionManager :: Disconnect(void)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mConnection != nullptr, done, lRetval = -ENXIO);
    nlEXPECT_ACTION(!mConnection->IsDisconnected(), done, lRetval = -EALREADY);
    nlEXPECT_ACTION(!mConnection->IsDisconnecting(), done, lRetval = -EINPROGRESS);

    lRetval = mConnection->Disconnect();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    if (lRetval == kStatus_Success)
    {
        mConnection->SetDelegate(nullptr);

        mConnection = nullptr;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Adds a delegate to the connection manager.
 *
 *  This attempts to add a delegate to the connection manager.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to add.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already
 *                                    added.
 *
 */
Status
ConnectionManager :: AddDelegate(ConnectionManagerDelegate *aDelegate)
{
    std::pair<ConnectionManagerDelegates::iterator, bool> lResult;
    Status lRetval;

    lResult = mDelegates.insert(aDelegate);

    lRetval = (lResult.second) ? kStatus_Success : kStatus_ValueAlreadySet;

    return (lRetval);
}

/**
 *  @brief
 *    Removes a delegate from the connection manager.
 *
 *  This attempts to remove a delegate from the connection manager.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to remove.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOENT          If the delegate had not been previously
 *                            added.
 *
 */
Status
ConnectionManager :: RemoveDelegate(ConnectionManagerDelegate *aDelegate)
{
    size_t lResult;
    Status lRetval;

    lResult = mDelegates.erase(aDelegate);

    lRetval = (lResult == 1) ? kStatus_Success : -ENOENT;

    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the connection manager is connected to a
 *    peer.
 *
 *  @returns
 *    True if the connection manager is connected to a peer;
 *    otherwise, false.
 *
 */
bool
ConnectionManager :: IsConnected(void) const
{
    bool lRetval;

    nlEXPECT_ACTION(mConnection != nullptr, done, lRetval = false);

    lRetval = mConnection->IsConnected();

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send the specified data to the connection peer.
 *
 *  @param[in]  aBuffer  An immutable shared pointer to the data to send to
 *                       the connection peer.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENXIO           If there is no allocated connection.
 *  @retval  -ENOTCONN        If the connection is not connected.
 *
 */
Status
ConnectionManager :: Send(ConnectionBuffer::ImmutableCountedPointer aBuffer)
{
    Status lRetval;

    nlREQUIRE_ACTION(mConnection != nullptr, done, lRetval = -ENXIO);
    nlREQUIRE_ACTION(mConnection->IsConnected(), done, lRetval = -ENOTCONN);

    lRetval = mConnection->Send(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

void
ConnectionManager :: OnWillResolve(const char *aHost)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerWillResolve(*this, GetRoles(), aHost);

            ++begin;
        }
    }
}

void
ConnectionManager :: OnIsResolving(const char *aHost)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerIsResolving(*this, GetRoles(), aHost);

            ++begin;
        }
    }
}

void
ConnectionManager :: OnDidResolve(const char *aHost, const IPAddress &aIPAddress)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidResolve(*this, GetRoles(), aHost, aIPAddress);

            ++begin;
        }
    }
}

void
ConnectionManager :: OnDidNotResolve(const char *aHost, const Common::Error &aError)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidNotResolve(*this, GetRoles(), aHost, aError);

            ++begin;
        }
    }
}

// MARK: Connection Basis Delegate Methods

// MARK: Connection Basis Connect Methods

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer will
 *    connect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *  @param[in]  aTimeout     The timeout for the connection.
 *
 */
void
ConnectionManager :: ConnectionWillConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerWillConnect(*this, aURLRef, aTimeout);

            ++begin;
        }
    }
}

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer is
 *    connecting.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *  @param[in]  aTimeout     The timeout for the connection.
 *
 */
void
ConnectionManager :: ConnectionIsConnecting(ConnectionBasis &aConnection, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerIsConnecting(*this, aURLRef, aTimeout);

            ++begin;
        }
    }
}

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer did
 *    connect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *
 */
void
ConnectionManager :: ConnectionDidConnect(ConnectionBasis &aConnection, CFURLRef aURLRef)
{
    (void)aConnection;

    mConnectionTimer.Destroy();

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidConnect(*this, aURLRef);

            ++begin;
        }
    }
}

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer did
 *    not connect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *  @param[in]  aError       An immutable reference to the error
 *                           associated with the failed connection.
 *
 */
void
ConnectionManager :: ConnectionDidNotConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnection;

    mConnectionTimer.Destroy();

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidNotConnect(*this, aURLRef, aError);

            ++begin;
        }
    }
}

// MARK: Connection Basis Application Data Methods

/**
 *  @brief
 *    Delegation from a connection that the connection has received
 *    application data.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aBuffer      The buffer containing the received
 *                           application data.
 *
 */
void
ConnectionManager :: ConnectionDidReceiveApplicationData(ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer aBuffer)
{
    (void)aConnection;

    if (GetApplicationDataDelegate() != nullptr)
    {
        GetApplicationDataDelegate()->ConnectionManagerDidReceiveApplicationData(*this, aConnection, aBuffer);
    }
}

// MARK: Connection Basis Disconnect Methods

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer
 *    will disconnect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *
 */
void
ConnectionManager :: ConnectionWillDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerWillDisconnect(*this, GetRoles(), aURLRef);
            ++begin;
        }
    }
}

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer
 *    did disconnect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *  @param[in]  aError       An immutable reference to the error
 *                           associated with the disconnection.
 *
 */
void
ConnectionManager :: ConnectionDidDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidDisconnect(*this, GetRoles(), aURLRef, aError);

            ++begin;
        }
    }
}

/**
 *  @brief
 *    Delegation from a connection that the connection to a peer
 *    did not disconnect.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aURLRef      The URL associated with the peer.
 *  @param[in]  aError       An immutable reference to the error
 *                           associated with the failed
 *                           disconnection.
 *
 */
void
ConnectionManager :: ConnectionDidNotDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidNotDisconnect(*this, GetRoles(), aURLRef, aError);

            ++begin;
        }
    }
}

// MARK: Connection Basis Error Method

/**
 *  @brief
 *    Delegation from a connection that the connection to a
 *    peer experienced an error.
 *
 *  @note
 *    This delegation may occur along with other delegations with
 *    respect to the same underlying event or cause.
 *
 *  @param[in]  aConnection  A reference to the connection that
 *                           issued the delegation.
 *  @param[in]  aError       An immutable reference to the error
 *                           associated with the event.
 *
 */
void
ConnectionManager :: ConnectionError(ConnectionBasis &aConnection, const Common::Error &aError)
{
    (void)aConnection;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerError(*this, GetRoles(), aError);

            ++begin;
        }
    }
}

// MARK: Timer Delegate Method

void
ConnectionManager :: TimerDidFire(Common::Timer &aTimer)
{
    if (aTimer == mConnectionTimer)
    {
        if (mConnection != nullptr)
        {
            const Status lStatus = mConnection->Disconnect(-ETIMEDOUT);
            nlREQUIRE_SUCCESS(lStatus, done);
        }
    }

 done:
    return;
}

}; // namespace Client

}; // namespace HLX
