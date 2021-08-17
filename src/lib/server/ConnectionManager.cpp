/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      connection between an HLX server and its clients.
 *
 */

#include "ConnectionManager.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Server/ConnectionBasis.hpp>
#include <OpenHLX/Server/ConnectionTelnet.hpp>
#include <OpenHLX/Server/ListenerTelnet.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionManager :: ConnectionManager(void) :
    ConnectionManagerBasis(),
    mListenerFactory(),
    mConnectionFactory(),
    mListeners(),
    mActiveConnections(),
    mInactiveConnections(),
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
    static const ConnectionManagerBasis::Roles kRoles = kRoleServer;
    Status lRetval = kStatus_Success;


    lRetval = ConnectionManagerBasis::Init(kRoles, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mListenerFactory.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionFactory.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

Status
ConnectionManager :: Listen(const SocketAddress *aFirst, const SocketAddress *aLast)
{
    DeclareScopedFunctionTracer(lTracer);
    const SocketAddress *  lCurrent = aFirst;
    bool                   lSchemeSupported;
    Status                 lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aFirst != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLast != nullptr, done, lRetval = -EINVAL);

    lSchemeSupported = mListenerFactory.SupportsScheme(ListenerTelnet::kScheme);
    nlREQUIRE_ACTION(lSchemeSupported, done, lRetval = -EPROTONOSUPPORT);

    while (lCurrent != aLast)
    {
        Listeners::value_type  lListener;

        lListener = mListenerFactory.CreateListener(ListenerTelnet::kScheme);
        nlREQUIRE_ACTION(lListener != nullptr, done, lRetval = -ENOMEM);

        lRetval = lListener->Init(mRunLoopParameters);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lListener->SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lListener->Listen(this, *lCurrent);
        nlREQUIRE_SUCCESS(lRetval, done);

        mListeners.push_back(std::move(lListener));

        lCurrent++;
    }

 done:
    if (lRetval != kStatus_Success)
    {
        // XXX - Do we need a OnListenerDidNotListen here?
    }

    return (lRetval);
}

Status ConnectionManager :: Listen(void)
{
    static constexpr Versions kVersions = (kIPv4 | kIPv6);

    return (Listen(kVersions));
}

Common::Status ConnectionManager :: Listen(const Versions &aVersions)
{
    DeclareScopedFunctionTracer(lTracer);
    std::array<SocketAddress, 2>  lSocketAddresses;
    size_t                        n = 0;
    Status                        lRetval = kStatus_Success;

    if ((aVersions & kIPv4) == kIPv4)
    {
        // IPv4 Listener

        memset(&lSocketAddresses[n], 0, sizeof (lSocketAddresses[n]));

        lSocketAddresses[n].uSocketAddress.sa_family           = AF_INET;
        lSocketAddresses[n].uSocketAddressIPv4.sin_port        = htons(23);
        lSocketAddresses[n].uSocketAddressIPv4.sin_addr.s_addr = INADDR_ANY;

        n++;
    }

    if ((aVersions & kIPv6) == kIPv6)
    {
        // IPv6 Listener

        memset(&lSocketAddresses[n], 0, sizeof (lSocketAddresses[n]));

        lSocketAddresses[n].uSocketAddress.sa_family           = AF_INET6;
        lSocketAddresses[n].uSocketAddressIPv6.sin6_port       = htons(23);
        lSocketAddresses[n].uSocketAddressIPv6.sin6_addr       = in6addr_any;

        n++;
    }

    lRetval = Listen(lSocketAddresses.begin(),
                     lSocketAddresses.begin() + n);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Common::Status
ConnectionManager :: Listen(const char *aMaybeURL)
{
    DeclareScopedFunctionTracer(lTracer);
    const Versions kVersions = (kIPv4 | kIPv6);
    Status         lRetval   = kStatus_Success;

    nlREQUIRE_ACTION(aMaybeURL != nullptr, done, lRetval = -EINVAL);

    lRetval = Listen(aMaybeURL, kVersions);

 done:
    return (lRetval);
}

Common::Status
ConnectionManager :: Listen(const char *aMaybeURL,
                            const Versions &aVersions)
{
    typedef std::vector<SocketAddress> SocketAddresses;

    CFString                    lHostName;
    int32_t                     lPossiblePort;
    CFURLRef                    lURLRef = nullptr;
    IPAddresses                 lIPAddresses;
    IPAddresses::const_iterator lCurrent;
    IPAddresses::const_iterator lLast;
    SocketAddresses             lSocketAddresses;
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

        // Otherwise, if the URL decoding was not successful, then try
        // parsing out a host or IP address or host or IP address and
        // port.

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

    while (lCurrent != lLast)
    {
        IPAddress::Version lVersion;
        SocketAddress      lSocketAddress;

        lRetval = lCurrent->GetVersion(lVersion);
        nlREQUIRE_SUCCESS(lRetval, done);

        memset(&lSocketAddress, 0, sizeof (lSocketAddress));

        if (lVersion == IPAddress::Version::kIPv4)
        {
            lSocketAddress.uSocketAddress.sa_family    = AF_INET;
            lSocketAddress.uSocketAddressIPv4.sin_port = ((lPossiblePort == -1) ? htons(23) : htons(lPossiblePort));

            lRetval = lCurrent->GetAddress(&lSocketAddress.uSocketAddressIPv4.sin_addr,
                                           sizeof (lSocketAddress.uSocketAddressIPv4.sin_addr));
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        else if (lVersion == IPAddress::Version::kIPv6)
        {
            lSocketAddress.uSocketAddress.sa_family     = AF_INET6;
            lSocketAddress.uSocketAddressIPv6.sin6_port = ((lPossiblePort == -1) ? htons(23) : htons(lPossiblePort));

            lRetval = lCurrent->GetAddress(&lSocketAddress.uSocketAddressIPv6.sin6_addr,
                                           sizeof (lSocketAddress.uSocketAddressIPv6.sin6_addr));
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        lSocketAddresses.push_back(lSocketAddress);

        lCurrent++;
    }

    lRetval = Listen(&lSocketAddresses[0],
                     &lSocketAddresses[0] + lSocketAddresses.size());
    nlREQUIRE_SUCCESS(lRetval, done);

done:
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
 *    Predicate class adapter for comparing a raw, bare pointer
 *    against a std::unique_ptr of a particular type.
 *
 *  This is a template predicate class to allow for heterogeneous
 *  searching of raw pointer types among a collection of
 *  std::unique_ptr type using std::find_if.
 *
 *  The find method on the collection itself only allows for searching
 *  of homogeneous types which, for a unique_ptr, doesn't work when
 *  trying to find the unique_ptr in the collection that belongs to the
 *  raw pointer outside it. Doing so would violate the contract of the
 *  unique_ptr by having two unique_ptrs pointing to the same
 *  underlying object.
 *
 *  @tparam  T  The underlying type to compare among a raw, bare pointer to
 *              @a T and a std::unique_ptr to @a T.
 *
 *  @private
 *
 */
template <typename T>
class HeterogeneousCompare
{
public:
    typedef T *                 raw_type;
    typedef std::unique_ptr<T>  unique_ptr_type;

    HeterogeneousCompare(const raw_type aRaw) :
        mRaw(aRaw)
    {
        return;
    }

    bool operator ()(const unique_ptr_type &aPointer) const
    {
        return (mCompare(aPointer.get(), mRaw));
    }

    bool operator ()(const raw_type aRaw) const
    {
        return (mCompare(aRaw, mRaw));
    }

private:
    const raw_type                 mRaw;
    std::equal_to<const raw_type>  mCompare;
};

Status
ConnectionManager :: Send(ConnectionBasis &aConnection, ConnectionBuffer::ImmutableCountedPointer aBuffer)
{
    HeterogeneousCompare<ConnectionBasis>  lComparator(&aConnection);
    Connections::iterator                  lCurrent = mActiveConnections.begin();
    Connections::iterator                  lLast    = mActiveConnections.end();
    Status                                 lRetval;


    // First, send over the specified connection.

    lRetval = aConnection.Send(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Next, send over all other active connections, skipping the
    // already-sent-upon specified connection, since a connection
    // serves as a proxy for an active subscription to server state
    // changes.

    while ((lCurrent != lLast))
    {
        if (!lComparator(*lCurrent))
        {
            lRetval = (*lCurrent)->Send(aBuffer);
            nlREQUIRE_SUCCESS(lRetval, next);
        }

    next:
        lCurrent++;
    }

 done:
    return (lRetval);
}

// NOTE: Until C++17, only std::vector allows for moving a unique_ptr
//       from one collection to another.
//
Status
ConnectionManager :: DisposeInactiveConnection(ConnectionBasis &aConnection)
{
    Connections::iterator  lCurrent = mActiveConnections.begin();
    Connections::iterator  lLast    = mActiveConnections.end();
    Connections::iterator  lResult;
    Status                 lRetval = kStatus_Success;


    lResult = std::find_if(lCurrent, lLast, HeterogeneousCompare<ConnectionBasis>(&aConnection));
    nlREQUIRE_ACTION(lResult != lLast, done, lRetval = -ENOENT);

    // Move the result to the inactive connections collection.

    mInactiveConnections.push_back(std::move(*lResult));

    // Purge the result from the active connections collection.

    mActiveConnections.erase(lResult);

 done:
    return (lRetval);
}

void ConnectionManager :: FlushInactiveConnections(void)
{
    mInactiveConnections.clear();
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

// MARK: Listener Basis Delegate Methods

// MARK: Listener Basis Listen Methods

void ConnectionManager :: ListenerWillListen(ListenerBasis &aListener)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerWillListen(*this, aListener.GetAddress().GetURL());

            ++begin;
        }
    }
}

void ConnectionManager :: ListenerIsListening(ListenerBasis &aListener)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerIsListening(*this, aListener.GetAddress().GetURL());

            ++begin;
        }
    }
}

void ConnectionManager :: ListenerDidListen(ListenerBasis &aListener)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidListen(*this, aListener.GetAddress().GetURL());

            ++begin;
        }
    }
}

void ConnectionManager :: ListenerDidNotListen(ListenerBasis &aListener, const Common::Error &aError)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidNotListen(*this, aListener.GetAddress().GetURL(), aError);

            ++begin;
        }
    }
}

// Error

void ConnectionManager :: ListenerError(ListenerBasis &aListener, const Common::Error &aError)
{
    (void)aListener;

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

// MARK: Listener Basis Accept Delegate Method

Status ConnectionManager :: ListenerDidAccept(ListenerBasis &aListener, const int &aSocket, const Common::SocketAddress &aPeerAddress)
{
    CFStringRef                       lScheme = aListener.GetScheme();
    bool                              lSchemeSupported;
    Connections::value_type           lConnection;
    Status                            lRetval = kStatus_Success;

    // We have a new connection, first flush any
    // previously-garbage-collected inactive connections.

    FlushInactiveConnections();

    // Attempt to allocate and connect a new connection.

    lSchemeSupported = mConnectionFactory.SupportsScheme(lScheme);
    nlREQUIRE_ACTION(lSchemeSupported, done, lRetval = -EPROTONOSUPPORT);

    lConnection = mConnectionFactory.CreateConnection(lScheme);
    nlREQUIRE_ACTION(lConnection != nullptr, done, lRetval = -ENOMEM);

    lRetval = lConnection->Init(mRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lConnection->SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lConnection->Connect(aSocket, aPeerAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the connection to the tracked list of active connections.

    mActiveConnections.push_back(std::move(lConnection));

 done:
    return (lRetval);
}

// MARK: Connection Basis Delegate Methods

// MARK: Connection Basis Accept Methods

void
ConnectionManager :: ConnectionWillAccept(ConnectionBasis &aConnection)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerWillAccept(*this, aConnection.GetPeerAddress().GetURL());

            ++begin;
        }
    }
}

void
ConnectionManager :: ConnectionIsAccepting(ConnectionBasis &aConnection)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerIsAccepting(*this, aConnection.GetPeerAddress().GetURL());

            ++begin;
        }
    }
}

void ConnectionManager :: ConnectionDidAccept(ConnectionBasis &aConnection)
{
    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidAccept(*this, aConnection.GetPeerAddress().GetURL());

            ++begin;
        }
    }
}

void
ConnectionManager :: ConnectionDidNotAccept(ConnectionBasis &aConnection, const Common::Error &aError)
{
    Status  lStatus;

    if (!mDelegates.empty())
    {
        ConnectionManagerDelegates::iterator begin = mDelegates.begin();
        ConnectionManagerDelegates::iterator end = mDelegates.end();

        while (begin != end)
        {
            (*begin)->ConnectionManagerDidNotAccept(*this, aConnection.GetPeerAddress().GetURL(), aError);

            ++begin;
        }
    }

    // We cannot actually delete the active connection here since we
    // are in the middle of a delegation from it. We need to mark it
    // for garbage collection and then purge it later.

    lStatus = DisposeInactiveConnection(aConnection);
    nlVERIFY_SUCCESS(lStatus);
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
    Status  lStatus;

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

    // We cannot actually delete the active connection here since we
    // are in the middle of a delegation from it. We need to mark it
    // for garbage collection and then purge it later.

    lStatus = DisposeInactiveConnection(aConnection);
    nlVERIFY_SUCCESS(lStatus);
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

}; // namespace Server

}; // namespace HLX
