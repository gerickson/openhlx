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
 *      This file implements an object for a HLX server peer-to-peer
 *      network connection.
 *
 */

#include "ConnectionBasis.hpp"

#include <errno.h>
#include <ifaddrs.h>
#include <unistd.h>

#if defined(__MACH__)
#include <net/if_dl.h>
#include <net/if_types.h>
#endif // defined(__MACH__)
#include <net/route.h>

#include <sys/types.h>

#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "ConnectionBasisDelegate.hpp"


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Detail
{

#if !defined(__linux__)
typedef decltype(sockaddr::sa_len) sa_len_t;

struct RouteMessage
{
    struct rt_msghdr    mHeader;
    union
    {
        uint8_t         mStorage[512];
        struct sockaddr mSocketAddress;
    } mUnion;
};
#endif // defined(__linux__)

static Status
SetAddress(IPAddress &aIPAddress, const SocketAddress &aSocketAddress)
{
    const sa_family_t lFamily = aSocketAddress.uSocketAddress.sa_family;
    Status            lRetval;

    switch (lFamily)
    {

    case AF_INET:
        lRetval = aIPAddress.SetAddress(IPAddress::Version::kIPv4,
                                        &aSocketAddress.uSocketAddressIPv4.sin_addr,
                                        sizeof (struct in_addr));
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case AF_INET6:
        lRetval = aIPAddress.SetAddress(IPAddress::Version::kIPv6,
                                        &aSocketAddress.uSocketAddressIPv6.sin6_addr,
                                        sizeof (struct in6_addr));
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    default:
        lRetval = -EAFNOSUPPORT;
        break;

    }

 done:
    return (lRetval);
}

static Status
SetAddress(const sa_family_t &aFamily,
           const struct sockaddr &aSocketAddress,
           void *aAddress)
{
    Status lRetval;

    switch (aFamily)
    {

    case AF_INET:
    case AF_INET6:
        {
            if (aFamily == AF_INET) {
                *((struct in_addr *)aAddress) = ((const struct sockaddr_in &)aSocketAddress).sin_addr;
            } else if (aFamily == AF_INET6) {
                *((struct in6_addr *)aAddress) = ((const struct sockaddr_in6 &)aSocketAddress).sin6_addr;
            }

            lRetval = kStatus_Success;
        }
        break;

    default:
        lRetval = -EAFNOSUPPORT;
        break;

    }

    return (lRetval);
}

#if defined(__MACH__)
static Status
GetDefaultRouterAddress(const sa_family_t &aSocketAddressFamily,
                        const size_t &aSocketAddressLength,
                        void *aAddress)
{
    const pid_t              lProcessId = getpid();
    RouteMessage             lRouteMessage;
    const struct rt_msghdr * lRouteMessageHeader;
    const uint8_t *          lRouteMessageData;
    const struct sockaddr *  lDefaultRouterAddress = nullptr;
    int                      lSocket = -1;
    ssize_t                  lStatus;
    int                      lSequenceId = 0;
    ssize_t                  lLength;
    Status                   lRetval = -EADDRNOTAVAIL;

    // Zero-out the route message

    memset(&lRouteMessage, 0, sizeof(lRouteMessage));

    // Initialize the route message

    lRouteMessage.mHeader.rtm_type                = RTM_GET;
    lRouteMessage.mHeader.rtm_version             = RTM_VERSION;
    lRouteMessage.mHeader.rtm_addrs               = RTA_DST;
    lRouteMessage.mHeader.rtm_flags               = (RTF_UP | RTF_GATEWAY);
    lRouteMessage.mHeader.rtm_pid                 = lProcessId;
    lRouteMessage.mHeader.rtm_seq                 = ++lSequenceId;

    lRouteMessage.mUnion.mSocketAddress.sa_family = aSocketAddressFamily;
#if defined(__MACH__)
    lRouteMessage.mUnion.mSocketAddress.sa_len    = static_cast<sa_len_t>(aSocketAddressLength);
#endif

    lRouteMessage.mHeader.rtm_msglen  = lLength = static_cast<decltype(rt_msghdr::rtm_msglen)>((sizeof (struct rt_msghdr) + aSocketAddressLength));

    // Establish the routing socket

    lSocket = socket(PF_ROUTE, SOCK_RAW, AF_UNSPEC);
    nlREQUIRE_ACTION(lSocket > 0, done, lRetval = -errno);

    // Send the routing socket get request

    lStatus = write(lSocket, reinterpret_cast<const uint8_t *>(&lRouteMessage), static_cast<size_t>(lLength));
    nlREQUIRE_ACTION(lStatus == lLength, done, lRetval = -EIO);

    // Receive the routing socket response

    do
    {
        lStatus = read(lSocket, reinterpret_cast<char *>(&lRouteMessage), sizeof (lRouteMessage));
        nlREQUIRE_ACTION(lStatus > 0, done, lRetval = -errno);
    } while ((lStatus > 0) && ((lRouteMessage.mHeader.rtm_seq != lSequenceId) || (lRouteMessage.mHeader.rtm_pid != lProcessId)));


    lRouteMessageHeader = &lRouteMessage.mHeader;
    lRouteMessageData   = (reinterpret_cast<const uint8_t *>(lRouteMessageHeader + 1));

    // Iterate through the returned routing socket response addresses
    // if any address flags were set in the response.

    if (lRouteMessageHeader->rtm_addrs)
    {
        for (int i = 1; i; i <<= 1)
        {
            if (i & lRouteMessageHeader->rtm_addrs)
            {
                const struct sockaddr *sa = reinterpret_cast<const struct sockaddr *>(lRouteMessageData);
                if (i == RTA_GATEWAY)
                {
                    lDefaultRouterAddress = sa;
                }

                lRouteMessageData += aSocketAddressLength;
            }
        }
    }

    if (lDefaultRouterAddress != nullptr)
    {
        lRetval = SetAddress(aSocketAddressFamily, *lDefaultRouterAddress, aAddress);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    if (lSocket >= 0)
    {
        close(lSocket);
    }

    return (lRetval);
}
#else
static Status
GetDefaultRouterAddress(const sa_family_t &aSocketAddressFamily,
                        const size_t &aSocketAddressLength,
                        void *aAddress)
{
    Status                   lRetval = -ENOSYS;

    return (lRetval);
}
#endif // defined(__MACH__)

static Status
GetDefaultRouterAddress(const IPAddress &aHostAddress, IPAddress &aDefaultRouterAddress)
{
    IPAddress::Version lIPVersion;
    SocketAddress      lSocketAddress;
    Status             lRetval;

    lRetval = aHostAddress.GetVersion(lIPVersion);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (lIPVersion == IPAddress::Version::kIPv4)
    {
        const sa_family_t lFamily  = AF_INET;
        const size_t      lLength  = sizeof (struct sockaddr_in);
        void *            lAddress = &lSocketAddress.uSocketAddressIPv4.sin_addr;

        lRetval = GetDefaultRouterAddress(lFamily, lLength, lAddress);
        nlREQUIRE_SUCCESS(lRetval, done);

        lSocketAddress.uSocketAddress.sa_family = lFamily;
    }
    else if (lIPVersion == IPAddress::Version::kIPv6)
    {
        const sa_family_t lFamily  = AF_INET6;

        const size_t      lLength  = sizeof (struct sockaddr_in6);
        void *            lAddress = &lSocketAddress.uSocketAddressIPv6.sin6_addr;

        lRetval = GetDefaultRouterAddress(lFamily, lLength, lAddress);
        nlREQUIRE_SUCCESS(lRetval, done);

        lSocketAddress.uSocketAddress.sa_family = lFamily;
    }

    lRetval = SetAddress(aDefaultRouterAddress, lSocketAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

static Status
GetHostAddress(const CFSocketNativeHandle &aSocket, IPAddress &aHostAddress)
{
#if defined(SOCK_MAXADDRLEN)
    static constexpr size_t  kSocketAddressLengthMax = SOCK_MAXADDRLEN;
#else
    static constexpr size_t  kSocketAddressLengthMax = sizeof (struct sockaddr_storage);
#endif // defined(SOCK_MAXADDRLEN)
    uint8_t                  lBuffer[kSocketAddressLengthMax];
    socklen_t                lBufferLength = ElementsOf(lBuffer);
    SocketAddress *          lOurSocketAddress;
    int                      lStatus;
    Status                   lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSocket > 0, done, lRetval = -ENOTCONN);

    lOurSocketAddress = reinterpret_cast<SocketAddress *>(&lBuffer[0]);

    lStatus = getsockname(aSocket, &lOurSocketAddress->uSocketAddress, &lBufferLength);
    nlREQUIRE_ACTION(lStatus == 0, done, lRetval = -errno);

    lRetval = SetAddress(aHostAddress, *lOurSocketAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

static Status
GetConfiguration(const char *                      aIfName,
                 const sa_family_t &               aFamily,
                 const struct ifaddrs *            aIfAddrs,
                 NetworkModel::EthernetEUI48Type * aEthernetEUI48,
                 IPAddress &                       aNetmask)
{
    bool                    lSetEthernetEUI48 = false;
    bool                    lSetNetmask       = false;
    const struct ifaddrs *  lIfAddr;
    Status                  lRetval = -EADDRNOTAVAIL;


    for (lIfAddr = aIfAddrs; lIfAddr != nullptr; lIfAddr = lIfAddr->ifa_next)
    {
        // Skip over any interface addresses that don't match the
        // interface name we are looking for.

        if (strcmp(aIfName, lIfAddr->ifa_name) != 0) {
            continue;
        }

        if (lIfAddr->ifa_addr != nullptr)
        {
            const sa_family_t lFamily = lIfAddr->ifa_addr->sa_family;

            // If requested, attempt to get the Ethernet EUI-48.

            if (aEthernetEUI48 != nullptr)
            {
                const uint8_t *lOctets     = nullptr;
                size_t         lOctetCount = 0;

#if defined(__linux__)
                if (lFamily == AF_PACKET)
                {
                    const struct sockaddr_ll * lLinkAddress = reinterpret_cast<struct sockaddr_ll *>(lIfAddr->ifa_addr);
                }
#else
                if (lFamily == AF_LINK)
                {
                    const struct sockaddr_dl * lLinkAddress = reinterpret_cast<struct sockaddr_dl *>(lIfAddr->ifa_addr);

                    if (lLinkAddress->sdl_alen > 0)
                    {
                        lOctets     = reinterpret_cast<const uint8_t *>(LLADDR(lLinkAddress));
                        lOctetCount = lLinkAddress->sdl_alen;
                    }
                    else
                    {
                        static constexpr uint8_t kDefaultOctets[sizeof (*aEthernetEUI48)] = { 0, 0, 0, 0, 0, 0 };
                        lOctets     = &kDefaultOctets[0];
                        lOctetCount = ElementsOf(kDefaultOctets);
                    }

                    if ((lOctets != nullptr) && (lOctetCount > 0))
                    {
                        memcpy(*aEthernetEUI48, lOctets, std::min(sizeof (*aEthernetEUI48), lOctetCount));

                        lSetEthernetEUI48 = true;
                    }
                }
#endif // defined(__linux__)
            }
        }

        if (lIfAddr->ifa_netmask != nullptr)
        {
            const sa_family_t lFamily = lIfAddr->ifa_netmask->sa_family;

            if (lFamily == aFamily)
            {
                lRetval = SetAddress(aNetmask, *reinterpret_cast<const SocketAddress *>(lIfAddr->ifa_netmask));
                nlREQUIRE_SUCCESS(lRetval, done);

                lSetNetmask = true;
            }
        }

        if (lSetNetmask && ((aEthernetEUI48 != nullptr) && lSetEthernetEUI48))
        {
            lRetval = kStatus_Success;
            break;
        }
    }

 done:
    return (lRetval);
}

static bool
operator ==(const IPAddress &aIPAddress, const SocketAddress &aSocketAddress)
{
    IPAddress  lIPAddress;
    Status     lStatus;
    bool       lRetval = false;

    lStatus = SetAddress(lIPAddress, aSocketAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    lRetval = (aIPAddress == lIPAddress);

 done:
    return (lRetval);
}

static Status
GetConfiguration(const struct ifaddrs *            aIfAddrs,
                 const IPAddress &                 aHostAddress,
                 NetworkModel::EthernetEUI48Type * aEthernetEUI48,
                 IPAddress &                       aNetmask)
{
    const struct ifaddrs * lIfAddr;
    sa_family_t            lHostAddressFamily;
    const char *           lIfName = nullptr;
    Status                 lRetval = -EADDRNOTAVAIL;

    // Perform the first search to establish the interface name
    // associated with the current host address.
    //
    // The host address is used to to filter and select one among many
    // network interfaces in a O(2n) interface list search: once
    // through the list to find the interface name associated with the
    // current host address and again through the list to find the
    // Ethernet EUI-48 and IP netmask for that interface name.

    for (lIfAddr = aIfAddrs; lIfAddr != nullptr; lIfAddr = lIfAddr->ifa_next)
    {
        // Skip interface "addresses" with no host address since such
        // addresses leave us nothing to compare against the specified
        // host address.

        if (lIfAddr->ifa_addr == nullptr)
        {
            continue;
        }

        lHostAddressFamily = lIfAddr->ifa_addr->sa_family;

        // Skip interface addresses with host addresses that are
        // neither IPv4 nor IPv6 addresses.

        if ((lHostAddressFamily != AF_INET) && (lHostAddressFamily != AF_INET6))
        {
            continue;
        }

        // If the specified host address matches the interface host
        // address, save the interface name for the second pass
        // search.

        if (aHostAddress == *reinterpret_cast<const SocketAddress *>(lIfAddr->ifa_addr))
        {
            lIfName = lIfAddr->ifa_name;

            break;
        }
    }

    // If we found an interface and name with a host address that
    // matched, start the second pass search.

    if (lIfName != nullptr)
    {
        lRetval = GetConfiguration(lIfName,
                                   lHostAddressFamily,
                                   aIfAddrs,
                                   aEthernetEUI48,
                                   aNetmask);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

static Status
GetConfiguration(const IPAddress &aHostAddress,
                 NetworkModel::EthernetEUI48Type *aEthernetEUI48,
                 IPAddress &aNetmask)
{
    int                    lStatus;
    struct ifaddrs *       lIfAddrs = nullptr;
    Status                 lRetval  = kStatus_Success;


    // Get the list of interface addresses.

    lStatus = getifaddrs(&lIfAddrs);
    nlREQUIRE_ACTION(lStatus == 0, done, lRetval = -errno);

    // Get the Ethernet EUI-48 (optional) and netmask.

    lRetval = GetConfiguration(lIfAddrs, aHostAddress, aEthernetEUI48, aNetmask);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lIfAddrs != nullptr)
    {
        freeifaddrs(lIfAddrs);
    }

    return (lRetval);
}

static Status
GetConfiguration(const CFSocketNativeHandle &aSocket,
                 NetworkModel::EthernetEUI48Type *aEthernetEUI48,
                 IPAddress &aHostAddress,
                 IPAddress &aNetmask,
                 IPAddress &aDefaultRouterAddress)
{
    Status  lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSocket > 0, done, lRetval = -ENOTCONN);

    // Get the host address.

    lRetval = GetHostAddress(aSocket, aHostAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Get the Ethernet EUI-48 and IP address netmask.

    lRetval = GetConfiguration(aHostAddress, aEthernetEUI48, aNetmask);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Get the default router address, based on the version of the
    // host address.

    lRetval = GetDefaultRouterAddress(aHostAddress, aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Detail

/**
 *  @brief
 *    This is a class default constructor.
 *
 *  This constructs an instance of the class with the specified URL
 *  scheme.
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme supported by the
 *                          connection.
 *
 */
ConnectionBasis :: ConnectionBasis(CFStringRef aSchemeRef) :
    Common::ConnectionBasis(aSchemeRef),
    mIdentifier(0),
    mConnectedSocket(-1),
    mState(kState_Unknown),
    mDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConnectionBasis :: ~ConnectionBasis(void)
{
    DeclareScopedFunctionTracer(lTracer);

    mDelegate = nullptr;

    SetState(kState_Unknown);

    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection basis on a run loop with the
 *  specified run loop parameters and connection scheme identifier.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection basis with.
 *  @param[in]  aIdentifier         An immutable reference to the
 *                                  identifier associated with this
 *                                  connection and its protocol scheme
 *                                  (for example, "telnet").
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Init(const RunLoopParameters &aRunLoopParameters, const IdentifierType &aIdentifier)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = Common::ConnectionBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mIdentifier        = aIdentifier;
    mState             = kState_Ready;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to the HLX client peer.
 *
 *  This establishes connection state for the HLX client peer at the
 *  specified socket and peer address.
 *
 *  @param[in]  aSocket        An immutable reference to the native
 *                             socket descriptor associated with the
 *                             accepted connection.
 *  @param[in]  aPeerAddress   An immutable reference to the socket
 *                             address associated with the peer client
 *                             at the remote end of the accepted
 *                             connection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the peer address was already
 *                                    set.
 *  @retval  -EINVAL                  The socket was invalid or the
 *                                    connection scheme is null or has
 *                                    zero (0) length.
 *  @retval  -ENOMEM                  If memory could not be allocated.
 *
 */
Status
ConnectionBasis :: Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress)
{
    constexpr bool  kIsHostOrder = true;
    HostURLAddress  lPeerAddress;
    Status          lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSocket > 0, done, lRetval = -EINVAL);

    // Initialize our peer address.

    lRetval = lPeerAddress.Init(GetScheme(), aPeerAddress, !kIsHostOrder);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SetPeerAddress(lPeerAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

    mConnectedSocket = aSocket;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the HLX client peer.
 *
 *  This attempts to asynchronously disconnect from the
 *  currently-connected HLX client peer, if any.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Disconnect(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to close the socket associated with the HLX client peer.
 *
 *  If the socket associated with the HLX client peer is open, this
 *  closes it.
 *
 */
void
ConnectionBasis :: Close(void)
{
    if (mConnectedSocket != -1)
    {
        close(mConnectedSocket);

        mConnectedSocket = -1;
    }
}

/**
 *  @brief
 *    Return the delegate for the connection basis.
 *
 *  @returns
 *    A pointer to the delegate for the connection basis.
 *
 */
ConnectionBasisDelegate *
ConnectionBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the connection basis.
 *
 *  This attempts to set a delegate for the connection basis.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionBasis :: SetDelegate(ConnectionBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

done:
    return (lRetval);
}

// MARK: Connection Delegation Actions

/**
 *  @brief
 *    Signals to connection delegates that a connection will accept.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is about to
 *  accept.
 *
 */
void
ConnectionBasis :: OnWillAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillAccept(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection is accepting.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is accepting.
 *
 */
void
ConnectionBasis :: OnIsAccepting(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionIsAccepting(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did accept.
 *
 *  This is invoked by a connection in response to a successful
 *  connection to signal delegates that the connection did accept.
 *
 */
void
ConnectionBasis :: OnDidAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidAccept(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did not
 *    accept.
 *
 *  This is invoked by a connection in response to a failed
 *  connection to signal delegates that the connection did not
 *  accept.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed connection.
 *
 */
void
ConnectionBasis :: OnDidNotAccept(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotAccept(*this, aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that the connection received
 *    application data.
 *
 *  This is invoked by a connection in response to the receipt of
 *  application data to signal delegates of that data.
 *
 *  @param[in]  aBuffer  A shared pointer to a mutable buffer
 *                       containing the application data.
 *
 */
void
ConnectionBasis :: OnApplicationDataReceived(ConnectionBuffer::MutableCountedPointer aBuffer)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidReceiveApplicationData(*this, aBuffer);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection will
 *    disconnect.
 *
 *  This is invoked by a connection in response to a pending
 *  disconnection to signal delegates that the connection will
 *  disconnect.
 *
 */
void
ConnectionBasis :: OnWillDisconnect(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillDisconnect(*this, GetPeerAddress().GetURL());
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did
 *    disconnect.
 *
 *  This is invoked by a connection in response to a disconnection
 *  to signal delegates that the connection did disconnect.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the disconnection.
 *
 */
void
ConnectionBasis :: OnDidDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidDisconnect(*this, GetPeerAddress().GetURL(), aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did
 *    not disconnect.
 *
 *  This is invoked by a connection in response to a failed
 *  disconnection to signal delegates that the connection did
 *  not disconnect.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed disconnection.
 *
 */
void
ConnectionBasis :: OnDidNotDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotDisconnect(*this, GetPeerAddress().GetURL(), aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection experienced an
 *    error.
 *
 *  This is invoked by a connection in response to a connection error
 *  to signal delegates that such a connection error occurred.
 *
 *  @note
 *    This action may occur along with other actions with respect to
 *    the same underlying event or cause.
 *
 *  @param[in]  aError  An immutable reference to the error associated
 *                      with the event.
 *
 */
void
ConnectionBasis :: OnError(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionError(*this, aError);
    }
}

/**
 *  @brief
 *    Returns the current connection scheme identifier.
 *
 *  @returns
 *    The current connection scheme identifier.
 *
 */
ConnectionBasis::IdentifierType
ConnectionBasis :: GetIdentifier(void) const
{
    return (mIdentifier);
}

/**
 *  @brief
 *    Get the network configuration associated with the
 *    currently-connected server socket.
 *
 *  This attempts to get the network configuration associated with the
 *  currently-connected server socket including the Ethernet EUI-48,
 *  host IP address, IP netmask, and default router IP address.
 *
 *  @param[out]  aEthernetEUI48         An optional pointer to storage for the
 *                                      Ethernet EUI-48 associated
 *                                      with the host server network
 *                                      interface for the
 *                                      currently-connected socket. If
 *                                      the pointer is null, the
 *                                      Ethernet EUI-48 is not
 *                                      retrieved and returned.
 *  @param[out]  aHostAddress           A reference to storage for the
 *                                      host IP address associated
 *                                      with the host server network
 *                                      interface for the
 *                                      currently-connected socket.
 *  @param[out]  aNetmask               A reference to storage for the
 *                                      IP netmask associated with the
 *                                      host server network interface
 *                                      for the currently-connected
 *                                      socket.
 *  @param[out]  aDefaultRouterAddress  A reference to storage for the
 *                                      default router IP address
 *                                      associated with the host
 *                                      server network interface for
 *                                      the currently-connected
 *                                      socket.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EADDRNOTAVAIL           One or more of the requested
 *                                    addresses was/were could not be
 *                                    retrieved.
 *  @retval  -EAFNOSUPPORT            An unsupported address family was
 *                                    encountered.
 *  @retval  -EIO                     An error occurred interacting with
 *                                    the host route infrastructure.
 *  @retval  -ENOTCONN                There is no currently-connected
 *                                    server socket.
 *  @retval  -ENOTSOCK                The currently-connected socket is
 *                                    not actually a socket but some
 *                                    other type of descriptor.
 *
 */
Status
ConnectionBasis :: GetConfiguration(NetworkModel::EthernetEUI48Type *aEthernetEUI48,
                                    IPAddress &aHostAddress,
                                    IPAddress &aNetmask,
                                    IPAddress &aDefaultRouterAddress) const
{
    Status  lRetval;


    lRetval = Detail::GetConfiguration(mConnectedSocket,
                                       aEthernetEUI48,
                                       aHostAddress,
                                       aNetmask,
                                       aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the network configuration associated with the
 *    currently-connected server socket.
 *
 *  This attempts to get the network configuration associated with the
 *  currently-connected server socket including the Ethernet EUI-48,
 *  host IP address, IP netmask, and default router IP address.
 *
 *  @param[out]  aEthernetEUI48         A reference to storage for the
 *                                      Ethernet EUI-48 associated
 *                                      with the host server network
 *                                      interface for the
 *                                      currently-connected socket.
 *  @param[out]  aHostAddress           A reference to storage for the
 *                                      host IP address associated
 *                                      with the host server network
 *                                      interface for the
 *                                      currently-connected socket.
 *  @param[out]  aNetmask               A reference to storage for the
 *                                      IP netmask associated with the
 *                                      host server network interface
 *                                      for the currently-connected
 *                                      socket.
 *  @param[out]  aDefaultRouterAddress  A reference to storage for the
 *                                      default router IP address
 *                                      associated with the host
 *                                      server network interface for
 *                                      the currently-connected
 *                                      socket.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EADDRNOTAVAIL           One or more of the requested
 *                                    addresses was/were could not be
 *                                    retrieved.
 *  @retval  -EAFNOSUPPORT            An unsupported address family was
 *                                    encountered.
 *  @retval  -EIO                     An error occurred interacting with
 *                                    the host route infrastructure.
 *  @retval  -ENOTCONN                There is no currently-connected
 *                                    server socket.
 *  @retval  -ENOTSOCK                The currently-connected socket is
 *                                    not actually a socket but some
 *                                    other type of descriptor.
 *
 */
Status
ConnectionBasis :: GetConfiguration(NetworkModel::EthernetEUI48Type &aEthernetEUI48,
                                    Common::IPAddress &aHostAddress,
                                    Common::IPAddress &aNetmask,
                                    Common::IPAddress &aDefaultRouterAddress) const
{
    NetworkModel::EthernetEUI48Type *  lEthernetEUI48 = &aEthernetEUI48;
    Status                             lRetval;


    lRetval = GetConfiguration(lEthernetEUI48,
                               aHostAddress,
                               aNetmask,
                               aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the network configuration associated with the
 *    currently-connected server socket.
 *
 *  This attempts to get the network configuration associated with the
 *  currently-connected server socket including the host IP address,
 *  IP netmask, and default router IP address.
 *
 *  @param[out]  aHostAddress           A reference to storage for the
 *                                      host IP address associated
 *                                      with the host server network
 *                                      interface for the
 *                                      currently-connected socket.
 *  @param[out]  aNetmask               A reference to storage for the
 *                                      IP netmask associated with the
 *                                      host server network interface
 *                                      for the currently-connected
 *                                      socket.
 *  @param[out]  aDefaultRouterAddress  A reference to storage for the
 *                                      default router IP address
 *                                      associated with the host
 *                                      server network interface for
 *                                      the currently-connected
 *                                      socket.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EADDRNOTAVAIL           One or more of the requested
 *                                    addresses was/were could not be
 *                                    retrieved.
 *  @retval  -EAFNOSUPPORT            An unsupported address family was
 *                                    encountered.
 *  @retval  -EIO                     An error occurred interacting with
 *                                    the host route infrastructure.
 *  @retval  -ENOTCONN                There is no currently-connected
 *                                    server socket.
 *  @retval  -ENOTSOCK                The currently-connected socket is
 *                                    not actually a socket but some
 *                                    other type of descriptor.
 *
 */
Status
ConnectionBasis :: GetConfiguration(Common::IPAddress &aHostAddress,
                                    Common::IPAddress &aNetmask,
                                    Common::IPAddress &aDefaultRouterAddress) const
{
    NetworkModel::EthernetEUI48Type *  lEthernetEUI48 = nullptr;
    Status                             lRetval;


    lRetval = GetConfiguration(lEthernetEUI48,
                               aHostAddress,
                               aNetmask,
                               aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the connection is in the specified state.
 *
 *  @param[in]  aState  The state to confirm.
 *
 *  @returns
 *    True if the connection is in the specified state; otherwise,
 *    false.
 *
 */
bool
ConnectionBasis :: IsState(State aState) const
{
    const bool lRetval = (mState == aState);


    return (lRetval);
}

/**
 *  @brief
 *    Returns the current connection state.
 *
 *  @returns
 *    The current connection state.
 *
 */
ConnectionBasis::State
ConnectionBasis :: GetState(void) const
{
    return (mState);
}

/**
 *  @brief
 *    Sets the current connection state.
 *
 *  @param[in]  aState  The state to set the connection to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the state was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionBasis :: SetState(State aState)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(mState != aState, done, lRetval = kStatus_ValueAlreadySet);

    mState = aState;

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
