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
 *      This file implements an object for working with and managing an
 *      IETF RFC 1738-style Internet host URL and a resolved IPv4 or
 *      IPv6 socket address.
 *
 */

#include "HostURLAddress.hpp"

#include <errno.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <CoreFoundation/CFSocket.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL address with the specified scheme
 *  and socket address. The socket address may be in host or network
 *  order, as specified.
 *
 *  @param[in]  aScheme       A pointer to a null-terminated C string
 *                            containing the IETF RFC 1738-style
 *                            Internet host URL scheme.
 *  @param[in]  aAddress      An immutable reference to the socket
 *                            address.
 *  @param[in]  aIsHostOrder  Indicates whether @a aAddress has address
 *                            and port members in host or network
 *                            order.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aScheme is null or if the length
 *                            of @a aScheme is zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURLAddress :: Init(const char *aScheme,
                       const SocketAddress &aAddress,
                       const bool &aIsHostOrder)
{
    const sa_family_t   lFamily = aAddress.uSocketAddress.sa_family;
    size_t              lSchemeLength;
    in_port_t           lPort;
    char                lBuffer[INET6_ADDRSTRLEN];
    const char *        lResult = nullptr;
    OutputStringStream  lURLStream;
    Status              lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aScheme != nullptr, done, lRetval = -EINVAL);

    lSchemeLength = strlen(aScheme);
    nlREQUIRE_ACTION(lSchemeLength > 0, done, lRetval = -EINVAL);

    switch (lFamily)
    {
    case AF_INET:
        lPort = aIsHostOrder ? aAddress.uSocketAddressIPv4.sin_port : ntohs(aAddress.uSocketAddressIPv4.sin_port);
        lResult = inet_ntop(lFamily,
                            &aAddress.uSocketAddressIPv4.sin_addr,
                            lBuffer,
                            sizeof (lBuffer));
        if (lResult != nullptr)
        {
            lURLStream << aScheme << "://" << lResult << ":" << lPort << "/";
        }
        break;

    case AF_INET6:
        lPort = aIsHostOrder ? aAddress.uSocketAddressIPv6.sin6_port : ntohs(aAddress.uSocketAddressIPv6.sin6_port);
        lResult = inet_ntop(lFamily,
                            &aAddress.uSocketAddressIPv6.sin6_addr,
                            lBuffer,
                            sizeof (lBuffer));
        if (lResult != nullptr)
        {
            lURLStream << aScheme << "://" "[" << lResult << "]" ":" << lPort << "/";
        }
        break;

    default:
        break;
    }

    lRetval = HostURL::Init(lURLStream.str().c_str());
    nlREQUIRE_SUCCESS(lRetval, done);

    mAddress = aAddress;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL address with the specified scheme
 *  and socket address. The socket address may be in host or network
 *  order, as specified.
 *
 *  @param[in]  aScheme       A pointer to a CoreFoundation string
 *                            containing the IETF RFC 1738-style
 *                            Internet host URL scheme.
 *  @param[in]  aAddress      An immutable reference to the socket
 *                            address.
 *  @param[in]  aIsHostOrder  Indicates whether @a aAddress has address
 *                            and port members in host or network
 *                            order.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aScheme is null or if the length
 *                            of @a aScheme is zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURLAddress :: Init(CFStringRef aScheme,
                       const SocketAddress &aAddress,
                       const bool &aIsHostOrder)
{
    const CFString lScheme(aScheme);

    return (Init(lScheme.GetCString(), aAddress, aIsHostOrder));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL address with the specified scheme
 *  and socket address in host order.
 *
 *  @param[in]  aScheme       A pointer to a null-terminated C string
 *                            containing the IETF RFC 1738-style
 *                            Internet host URL scheme.
 *  @param[in]  aAddress      An immutable reference to the socket
 *                            address in host order.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aScheme is null or if the length
 *                            of @a aScheme is zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURLAddress :: Init(const char *aScheme,
                       const SocketAddress &aAddress)
{
    static constexpr bool  kIsHostOrder = true;

    return (Init(aScheme, aAddress, kIsHostOrder));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL address with the specified scheme
 *  and socket address in host order.
 *
 *  @param[in]  aScheme       A pointer to a CoreFoundation string
 *                            containing the IETF RFC 1738-style
 *                            Internet host URL scheme.
 *  @param[in]  aAddress      An immutable reference to the socket
 *                            address in host order.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aScheme is null or if the length
 *                            of @a aScheme is zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURLAddress :: Init(CFStringRef aScheme,
                       const SocketAddress &aAddress)
{
    static constexpr bool  kIsHostOrder = true;

    return (Init(aScheme, aAddress, kIsHostOrder));
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified host URL address to this one.
 *
 *  @param[in]  aHostURLAddress  An immutable reference to the host
 *                               URL address to assign (copy).
 *
 *  @returns
 *    A reference to this host URL address after the assignment (copy)
 *    is complete.
 *
 */
HostURLAddress &
HostURLAddress :: operator =(const HostURLAddress &aHostURLAddress)
{
    // Avoid a self-copy

    nlEXPECT(&aHostURLAddress != this, done);

    // Invoke the base class copy operator

    HostURL::operator =(aHostURLAddress);

    mAddress = aHostURLAddress.mAddress;

done:
    return (*this);
}

/**
 *  @brief
 *    This is a class move (swap) operator.
 *
 *  This moves (swaps) the specified host URL address with this one.
 *
 *  @param[in,out]  aHostURLAddress  A mutable reference to the host
 *                                   URL address to move (swap) with
 *                                   this one.
 *
 *  @returns
 *    A reference to this host URL address after the move (swap) is
 *    complete.
 *
 */
HostURLAddress &
HostURLAddress :: operator =(HostURLAddress &&aHostURLAddress)
{
    // Avoid a self-move

    nlEXPECT(&aHostURLAddress != this, done);

    // Invoke the base class move operator

    HostURL::operator =(aHostURLAddress);

    mAddress = std::move(aHostURLAddress.mAddress);

 done:
    return (*this);
}

/**
 *  @brief
 *    Return the socket address for the host URL address.
 *
 *  @returns
 *    The socket address for the host URL address.
 *
 */
const SocketAddress &
HostURLAddress :: GetAddress(void) const
{
    return (mAddress);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided host URL address against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aHostURLAddress  An immutable reference to the host URL
 *                               address to compare for equality.
 *
 *  @returns
 *    True if this host URL address is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
HostURLAddress :: operator ==(const HostURLAddress &aHostURLAddress) const
{
    const bool lRetval = ((static_cast<const HostURL &>(*this) ==
                           static_cast<const HostURL &>(aHostURLAddress)) &&
                          (mAddress == aHostURLAddress.mAddress));

    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
