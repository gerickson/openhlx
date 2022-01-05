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
 *      This file implements a base, derivable object for managing
 *      peer-to-peer connections between a HLX client and server.
 *
 */

#include "ConnectionManagerBasis.hpp"

#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include <sys/types.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/ConnectionManagerApplicationDataDelegate.hpp>
#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Common
{

static void
VersionsToFamilyHint(const ConnectionManagerBasis::Versions &aVersions,
                     struct addrinfo &outHints)
{
    using Version  = ConnectionManagerBasis::Version;
    using Versions = ConnectionManagerBasis::Versions;

    static constexpr Versions kBothIPVersionsMask = (Version::kIPv4 |
                                                     Version::kIPv6);


    if ((aVersions & kBothIPVersionsMask) == Version::kIPv6)
        outHints.ai_family = AF_INET6;
    else if ((aVersions & kBothIPVersionsMask) == Version::kIPv4)
        outHints.ai_family = AF_INET;
    else
        outHints.ai_family = AF_UNSPEC;
}

static Common::Error
MapGaiStatusToError(const int &aGaiStatus)
{
    Status lRetval;

    switch (aGaiStatus)
    {

    case EAI_ADDRFAMILY:
    case EAI_FAMILY:
        lRetval = -EAFNOSUPPORT;
        break;

    case EAI_AGAIN:
        lRetval = -EAGAIN;
        break;

    case EAI_BADFLAGS:
    case EAI_SERVICE:
        lRetval = -EINVAL;
        break;

    case EAI_MEMORY:
        lRetval = -ENOMEM;
        break;

    case EAI_NODATA:
    case EAI_NONAME:
        lRetval = -ENOENT;
        break;

    case EAI_SOCKTYPE:
        lRetval = -EPROTONOSUPPORT;
        break;

    case EAI_SYSTEM:
        lRetval = -errno;
        break;

    case EAI_OVERFLOW:
        lRetval = -EOVERFLOW;
        break;

    case EAI_FAIL:
    default:
        lRetval = kError_HostNameResolution;
        break;

    }

    return (lRetval);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionManagerBasis :: ConnectionManagerBasis(void) :
    mRoles(kRoleNone),
    mApplicationDataDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection manager with the specified role(s)
 *  and run loop parameters.
 *
 *  @param[in]  aRoles              An immutable reference to the role(s)
 *                                  in which the connection manager
 *                                  will be acting.
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
ConnectionManagerBasis :: Init(const Roles &aRoles,
                               const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    (void)aRunLoopParameters;

    mRoles = aRoles;

    return (lRetval);
}

/**
 *  @brief
 *    Resolve the specified host name or IP address to one or more
 *    actual IP addresses.
 *
 *  This takes the specified host name or literal IP address text
 *  representation and attempts to resolve it, using domain name
 *  services (DNS), to one or more actual IP addresses, the versions
 *  of which are returned are determined by the specified IP address
 *  version argument, @a aVersions.
 *
 *  @param[in]  aMaybeHost        A pointer to a null-terminated C
 *                                string containing the host name or
 *                                literal IP address text
 *                                representation to resolve.
 *  @param[in]  aVersions         A reference to the IP address
 *                                versions to include in the
 *                                resolution.
 *  @param[out]  aOutIPAddresses  A reference to a collection in which
 *                                to copy resolved IP addresses for @a
 *                                aMaybeHost.
 *
 *  @retval  kStatus_Success            If successful.
 *  @retval  -EAFNOSUPPORT              If no addresses were available
 *                                      consistent with @a aVersions.
 *  @retval  -EAGAIN                    If the domain name server was
 *                                      temporarily busy and has
 *                                      suggested the resolve be
 *                                      retried again later.
 *  @retval  -EINVAL                    If @a aMaybeHost was null or
 *                                      empty.
 *  @retval  -ENOMEM                    If memory could not be allocated
 *                                      for the resolved IP addresses.
 *  @retval  -ENOENT                    There are no resolvable addresses
 *                                      for the specified host name.
 *  @retval  -EOVERFLOW                 There was a buffer overflow due
 *                                      to an excessively long @a
 *                                      aMaybeHost argument.
 *  @retval  kError_HostNameResolution  There was some other resolution
 *                                      error.
 *
 */
Status
ConnectionManagerBasis :: Resolve(const char *aMaybeHost,
                                  const Versions &aVersions,
                                  IPAddresses &aOutIPAddresses)
{
    struct addrinfo         lHints;
    struct addrinfo *       lAddresses = nullptr;
    int                     lGaiStatus;
    Status                  lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aMaybeHost != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aMaybeHost[0] != '\0', done, lRetval = -EINVAL);

    OnWillResolve(aMaybeHost);

    memset(&lHints, 0, sizeof (lHints));

    VersionsToFamilyHint(aVersions, lHints);

    lHints.ai_socktype = SOCK_STREAM;
    lHints.ai_protocol = IPPROTO_TCP;
    lHints.ai_flags    = AI_ADDRCONFIG;

    OnIsResolving(aMaybeHost);

    lGaiStatus = getaddrinfo(aMaybeHost, nullptr, &lHints, &lAddresses);
    if (lGaiStatus != 0)
    {
        lRetval = MapGaiStatusToError(lGaiStatus);

        Log::Error().Write("Failed to resolve %s: %s\n",
                           aMaybeHost,
                           gai_strerror(lGaiStatus));
    }
    else
    {
        const struct addrinfo * lAddress;

        for (lAddress = lAddresses; lAddress != nullptr; lAddress = lAddress->ai_next)
        {
            IPAddress    lIPAddress;
            Status       lStatus;

            switch (lAddress->ai_family)
            {

            case AF_INET:
                lStatus = lIPAddress.SetAddress(IPAddress::Version::kIPv4,
                                                &reinterpret_cast<sockaddr_in *>(lAddress->ai_addr)->sin_addr,
                                                sizeof (struct in_addr));
                nlREQUIRE_SUCCESS(lStatus, done);

                OnDidResolve(aMaybeHost, lIPAddress);

                aOutIPAddresses.push_back(lIPAddress);
                break;

            case AF_INET6:
                lStatus = lIPAddress.SetAddress(IPAddress::Version::kIPv6,
                                                &reinterpret_cast<sockaddr_in6 *>(lAddress->ai_addr)->sin6_addr,
                                                sizeof (struct in6_addr));
                nlREQUIRE_SUCCESS(lStatus, done);

                OnDidResolve(aMaybeHost, lIPAddress);

                aOutIPAddresses.push_back(lIPAddress);
                break;

            default:
                break;

            }
        }

        if (lAddresses != nullptr)
        {
            freeaddrinfo(lAddresses);
        }
    }

done:
    if (lRetval != kStatus_Success)
    {
        OnDidNotResolve(aMaybeHost, lRetval);
    }

    return (lRetval);
}

/**
 *  @brief
 *    Return the roles in which the connection manager is acting.
 *
 *  @returns
 *    The roles in which the connection manager is acting.
 *
 */
const ConnectionManagerBasis::Roles &
ConnectionManagerBasis :: GetRoles(void) const
{
    return (mRoles);
}

/**
 *  @brief
 *    Return the application data delegate for the connection manager.
 *
 *  @returns
 *    A pointer to the application data delegate for the connection
 *    manager.
 *
 */
ConnectionManagerApplicationDataDelegate *
ConnectionManagerBasis :: GetApplicationDataDelegate(void) const
{
    return (mApplicationDataDelegate);
}

/**
 *  @brief
 *    Set the application data delegate for the connection manager.
 *
 *  This attempts to set an application data delegate for the
 *  connection manager.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionManagerBasis :: SetApplicationDataDelegate(ConnectionManagerApplicationDataDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mApplicationDataDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mApplicationDataDelegate = aDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to convert the specified string into a host name (or IP
 *    address) and TCP or UDP port number.
 *
 *  This attempts to convert the specified string into either one or
 *  both of a host name (or IP address) and TCP or UDP port number.
 *
 *  The format of @a aMaybeHost may be one of several:
 *
 *    1. "[\<IPv6 Address>]"
 *    2. "[\<IPv6 Address>]:<Port>"
 *    4. "\<IPv4 Address>"
 *    5. "\<IPv4 Address>:<Port>"
 *    6. "\<Host Name>"
 *    7. "\<Host Name>:<Port>"
 *
 *  potentially with leading space, which will be elided. Any
 *  resulting \<IPv6 Address>, \<IPv4 Address>, or \<Host Name> will end
 *  up in @a aOutHost and the port, if present, in @a aOutPort. If no
 *  port is present, @a aOutPort will be assigned -1 to indicate the
 *  absence.
 *
 *  @param[in]   aMaybeHost  A pointer to the null-terminated C string
 *                           potentially containing a host name or IP
 *                           address and, optionally, a TCP or UDP
 *                           port number to parse.
 *  @param[out]  aOutHost    A reference to storage for the parsed host
 *                           name or IP address.
 *  @param[out]  aOutPort    A reference to storage for the parsed TCP
 *                           or UDP port, if present. Otherwise, -1, if
 *                           absent.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          @a aMaybeHost was null, empty, or did not
 *                            contain a valid host name, IP address,
 *                            and TCP or UDP port number.
 *
 */
Status
ConnectionManagerBasis :: ParseHostOrHostAndPort(const char *aMaybeHost,
                                                 std::string &aOutHost,
                                                 int32_t &aOutPort)
{
    static constexpr char kOpeningBracket = '[';
    static constexpr char kClosingBracket = ']';
    static constexpr char kPortDelimiter = ':';
    size_t                lTotalLength;
    size_t                lRemainingLength;
    const char *          p = aMaybeHost;
    const char *          lResult;
    Status                lRetval = kStatus_Success;


    // Do some initial sanity checking.

    nlREQUIRE_ACTION(aMaybeHost != nullptr, done, lRetval = -EINVAL);

    lTotalLength = strlen(aMaybeHost);
    nlREQUIRE_ACTION(lTotalLength > 0, done, lRetval = -EINVAL);

    // Skip any leading space, if any.

    while ((p < (aMaybeHost + lTotalLength)) && isspace(*p))
    {
        p++;
    }

    // Attempt to handle the host portion.
    //
    // Since it's the easiest to detect, first rule out an IPv6
    // address. The only reliably way to do so is to search for the
    // delimiting '[' and ']'. Searching for ':' may yield one of the
    // other forms above (for example, (2), (5), or (7).

    lRemainingLength = (lTotalLength - static_cast<size_t>(p - aMaybeHost));

    lResult = static_cast<const char *>(memchr(p,
                                               kOpeningBracket,
                                               lRemainingLength));

    if (lResult != nullptr)
    {
        const char *lOpeningBracket = lResult;
        const char *lClosingBracket;

        // We found an opening bracket; this might be an IPv6
        // address. Search for its peer closing bracket.

        lRemainingLength = (lTotalLength - static_cast<size_t>(lOpeningBracket - aMaybeHost));

        lClosingBracket = static_cast<const char *>(memchr(lOpeningBracket,
                                                           kClosingBracket,
                                                           lRemainingLength));
        nlEXPECT_ACTION(lClosingBracket != nullptr, done, lRetval = -EINVAL);

        // Assign the first character of the IPv6 address after the
        // opening bracket up to, but not including, the closing
        // bracket to the host name.

        aOutHost.assign(lOpeningBracket + 1,
                        static_cast<size_t>(lClosingBracket - lOpeningBracket) - 1);

        // Move the parsing cursor past the closing bracket.

        p = lClosingBracket + 1;
    }
    else
    {
        // At this point, we either have an IPv4 address or a host
        // name, maybe with a port.
        //
        // Whether we have a port or not, we definitively know where
        // the IPv4 address or host name ends. If we have a port, it
        // ends at the port delimiter, ':'. If we don't have a port,
        // then it ends at the end of the string.

        lRemainingLength = (lTotalLength - static_cast<size_t>(p - aMaybeHost));

        lResult = static_cast<const char *>(memchr(p,
                                                   kPortDelimiter,
                                                   lRemainingLength));

        if (lResult != nullptr)
        {
            const size_t lHostLength = static_cast<size_t>(lResult - p);

            // There is a port and an IPv4 address or a host name.
            // Assign the latter to the host and handle the port
            // later.

            aOutHost.assign(p, lHostLength);

            p += lHostLength;
        }
        else
        {
            // There is no port, just an IPv4 address or a host name.
            // Just assign the remainder of the string to the host.

            aOutHost.assign(p, lRemainingLength);

            p += lRemainingLength;
        }
    }

    // Attempt to handle the port portion.

    if (p < (aMaybeHost + lTotalLength))
    {
        // If there's a port, then the next character MUST be the port
        // delimiter.

        nlREQUIRE_ACTION(*p++ == kPortDelimiter, done, lRetval = -EINVAL);

        // At this point, we say the port delimiter and moved past
        // it. Attempt to parse a valid port number and, if
        // successful, assign it back to the caller.

        {
            const size_t lMaybePortLength = (lTotalLength - static_cast<size_t>(p - aMaybeHost));
            uint16_t     lMaybePort;


            lRetval = Parse(p, lMaybePortLength, lMaybePort);
            nlEXPECT_SUCCESS_ACTION(lRetval, done, lRetval = -EINVAL; aOutPort = -1);

            aOutPort = lMaybePort;
        }
    }
    else
    {
        aOutPort = -1;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to convert the specified string into an absolute URL.
 *
 *  This attempts to convert the specified string into an absolute
 *  URL. At minimum, the string must contain a scheme, a scheme
 *  separator (':'), a network location separator ('//'), and a
 *  network location.
 *
 *  @param[in]   aMaybeURL  A pointer to the null-terminated C string
 *                          potentially containing a URL to convert.
 *
 *  @param[out]  aOutURL    A reference to storage for the converted URL.
 *                          The caller owns the reference and must
 *                          release it when no longer needed.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          @a aMaybeURL was null, empty, or did not
 *                            contain a scheme separator or network
 *                            location separator.
 *  @retval  -ENOMEM          Memory could not be allocated for the
 *                            converted URL.
 *
 */
Status
ConnectionManagerBasis :: ParseURL(const char *aMaybeURL, CFURLRef &aOutURL)
{
    const char * lSchemeNetLoc;
    size_t       lURLLength;
    CFURLRef     lURLRef = nullptr;
    Status       lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aMaybeURL != nullptr, done, lRetval = -EINVAL);

    lURLLength = strlen(aMaybeURL);
    nlREQUIRE_ACTION(lURLLength > 0, done, lRetval = -EINVAL);

    lSchemeNetLoc = strstr(aMaybeURL, "://");
    nlEXPECT_ACTION(lSchemeNetLoc != nullptr, done, lRetval = -EINVAL);


    lURLRef = CFURLCreateWithBytes(nullptr,
                                   reinterpret_cast<const UInt8 *>
                                   (aMaybeURL),
                                   static_cast<CFIndex>(lURLLength),
                                   CFStringGetSystemEncoding(),
                                   nullptr);
    nlREQUIRE_ACTION(lURLRef != nullptr, done, lRetval = -ENOMEM);

    aOutURL = lURLRef;

 done:
    return (lRetval);
}

namespace Utilities
{

/**
 *  Return a version object indicating which IP versions are desired.
 *
 *  @param[in]  aUseIPv6  An immutable reference indicating whether
 *                        IPv6 should be used.
 *  @param[in]  aUseIPv4  An immutable reference indicating
 *                        IPv4 should be used.
 *
 *  @returns
 *    The IP versions appropriate for the specified parameters.
 *
 */
ConnectionManagerBasis::Versions
GetVersions(const bool &aUseIPv6, const bool &aUseIPv4)
{
    using Version  = ConnectionManagerBasis::Version;
    using Versions = ConnectionManagerBasis::Versions;

    const Versions kVersions =
        (((aUseIPv6) ? Version::kIPv6 : 0) |
         ((aUseIPv4) ? Version::kIPv4 : 0));

    return (kVersions);
}

}; // namespace Utilities

}; // namespace Common

}; // namespace HLX
