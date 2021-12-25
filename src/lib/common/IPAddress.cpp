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
 *      This file implements an object for working with Internet
 *      Protocol version 4 (that is, IPv4) and version 6 (that is,
 *      IPv6) addresses.
 *
 */

#include "IPAddress.hpp"

#include <errno.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
IPAddress :: IPAddress(void) :
    mIsNull(true)
{
    return;
}

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This initializes the IP address with the specified version and
 *  address bytes.
 *
 *  @param[in]  aVersion  An immutable reference to the IP address
 *                        version to construct the address with.
 *  @param[in]  aBytes    A pointer to an immutable array of address
 *                        bytes to construct the address with.
 *  @param[in]  aSize     The size, in bytes, of @a aBytes. This must
 *                        be at least as large as required for @a aVersion.
 *
 */
IPAddress :: IPAddress(const Version &aVersion, const void *aBytes, const size_t &aSize)
{
    Status lStatus;

    lStatus = SetAddress(aVersion, aBytes, aSize);

    (void)lStatus;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aIPAddress  An immutable reference to the IP
 *                          address to copy from.
 *
 */
IPAddress :: IPAddress(const IPAddress &aIPAddress)
{
    *this = aIPAddress;
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified IP address to this one.
 *
 *  @param[in]  aIPAddress  An immutable reference to the IP
 *                          address to assign (copy).
 *
 *  @returns
 *    A reference to this IP address after the assignment (copy) is
 *    complete.
 *
 */
IPAddress &
IPAddress :: operator =(const IPAddress &aIPAddress)
{
    mIsNull    = aIPAddress.mIsNull;
    mVersion   = aIPAddress.mVersion;
    mAddresses = aIPAddress.mAddresses;

    return (*this);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided IP address against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aIPAddress  An immutable reference to the IP
 *                          address to compare for equality.
 *
 *  @returns
 *    True if this IP address is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
IPAddress :: operator ==(const IPAddress &aIPAddress) const
{
    bool lRetval = false;

    if (this == &aIPAddress)
    {
        lRetval = true;
    }
    else if (mIsNull == aIPAddress.mIsNull)
    {
        if (mIsNull)
        {
            lRetval = true;
        }
        else
        {
            if (mVersion == aIPAddress.mVersion)
            {
                switch (mVersion)
                {

                case Version::kIPv4:
                    lRetval = (memcmp(&mAddresses.mIPv4Address,
                                      &aIPAddress.mAddresses.mIPv4Address,
                                      sizeof (mAddresses.mIPv4Address)) == 0);
                    break;

                case Version::kIPv6:
                    lRetval = (memcmp(&mAddresses.mIPv6Address,
                                      &aIPAddress.mAddresses.mIPv6Address,
                                      sizeof (mAddresses.mIPv6Address)) == 0);
                    break;

                default:
                    break;

                }
            }
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the IP address version.
 *
 *  This attempts to get the IP address version, if it has been
 *  previously constructed or set.
 *
 *  @param[out]  aOutVersion  A mutable reference to storage for the
 *                            IP address version, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IP address version has not
 *                                  been constructed or set with a
 *                                  known value.
 *
 *  @sa SetAddress
 *
 */
Status
IPAddress :: GetVersion(Version &aOutVersion) const
{
    Status lRetval = ((mIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aOutVersion = mVersion;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Return whether the IP address is of the specified version.
 *
 *  @param[in]  aVersion  An immutable reference to the IP address
 *                        version to introspect against.
 *
 *  @returns
 *    True if the IP address is of the specified version; otherwise,
 *    false.
 *
 */
bool
IPAddress :: IsVersion(const Version &aVersion) const
{
    return (mVersion == aVersion);
}

/**
 *  @brief
 *    Return whether the IP address is of the kIPv4 version.
 *
 *  @returns
 *    True if the IP addresss is of the kIPv4 version; otherwise,
 *    false.
 *
 */
bool
IPAddress :: IsIPv4(void) const
{
    return (IsVersion(Version::kIPv4));
}

/**
 *  @brief
 *    Return whether the IP address is of the kIPv6 version.
 *
 *  @returns
 *    True if the IP addresss is of the kIPv6 version; otherwise,
 *    false.
 *
 */
bool
IPAddress :: IsIPv6(void) const
{
    return (IsVersion(Version::kIPv6));
}

/**
 *  @brief
 *    Attempt to get the IP address bytes.
 *
 *  This attempts to get the IP address bytes.
 *
 *  @param[out]  aOutBytes  A mutable pointer to an array in which
 *                          to copy the IP address bytes.
 *  @param[in]   aSize      The size, in bytes, of @a aOutBytes.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IP address has not been
 *                                  constructed or set with known
 *                                  values.
 *  @retval  -EINVAL                If @a aOutBytes was null or if @a
 *                                  aSize was insufficiently large
 *                                  relative to what is required for
 *                                  @a aVersion.
 */
Status
IPAddress :: GetAddress(void *aOutBytes, const size_t &aSize) const
{
    Status lRetval = ((mIsNull) ? kError_NotInitialized : kStatus_Success);

    nlREQUIRE_ACTION(aOutBytes != nullptr, done, lRetval = -EINVAL);

    if (lRetval == kStatus_Success)
    {
        const size_t lAddressSize = (IsIPv6() ? sizeof (mAddresses.mIPv6Address) :
                                        sizeof (mAddresses.mIPv4Address));
        const void * lAddress     = (IsIPv6() ? static_cast<const void *>(&mAddresses.mIPv6Address) :
                                     static_cast<const void *>(&mAddresses.mIPv4Address));

        nlREQUIRE_ACTION(aSize >= lAddressSize, done, lRetval = -EINVAL);

        memcpy(aOutBytes, lAddress, lAddressSize);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Format the IP address in a conventional text presentation format.
 *
 *  This may be used to format the
 *  conventional text presentation form of the IP address to the memory
 *  located at @a aBuffer and extending as much as @a aLength bytes, including
 *  its null termination character.
 *
 *  @param[out]  aBuffer  A pointer to storage for the formatted text
 *                        presentation format.
 *  @param[in]   aLength  The size of @a aBuffer, in bytes.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IP address has not been
 *                                  constructed or set with known
 *                                  values.
 *  @retval  -EINVAL                If @a aBuffer was null.
 *
 */
Status
IPAddress :: ToString(char *aBuffer, const size_t &aLength) const
{
    Status lRetval = ((mIsNull) ? kError_NotInitialized : kStatus_Success);

    nlREQUIRE_ACTION(aBuffer != nullptr, done, lRetval = -EINVAL);

    if (lRetval == kStatus_Success)
    {
        const int    lFamily  = (IsIPv6() ? AF_INET6 : AF_INET);
        const void * lAddress = (IsIPv6() ? static_cast<const void *>(&mAddresses.mIPv6Address) :
                                 static_cast<const void *>(&mAddresses.mIPv4Address));
        const char * lResult;

        lResult = inet_ntop(lFamily, lAddress, aBuffer, static_cast<socklen_t>(aLength));
        nlREQUIRE_ACTION(lResult != nullptr, done, lRetval = -errno);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the IP address version and address.
 *
 *  This attempts to set the IP address version and address with the
 *  specified version and address bytes.
 *
 *  @param[in]  aVersion  An immutable reference to the IP address
 *                        version to construct the address with.
 *  @param[in]  aBytes    A pointer to an immutable array of address
 *                        bytes to construct the address with.
 *  @param[in]  aSize     The size, in bytes, of @a aBytes. This must
 *                        be at least as large as required for @a
 *                        aVersion.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aBytes was null or if @a
 *                            aSize was insufficiently large
 *                            relative to what is required for
 *                            @a aVersion.
 */
Status
IPAddress :: SetAddress(const Version &aVersion, const void *aBytes, const size_t &aSize)
{
    const size_t lAddressSize = ((aVersion == Version::kIPv6) ? sizeof (mAddresses.mIPv6Address) : sizeof (mAddresses.mIPv4Address));
    void *       lAddress     = ((aVersion == Version::kIPv6) ? static_cast<void *>(&mAddresses.mIPv6Address) :
                                 static_cast<void *>(&mAddresses.mIPv4Address));
    Status       lRetval      = kStatus_Success;

    nlREQUIRE_ACTION(aBytes != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aSize >= lAddressSize, done, lRetval = -EINVAL);

    mVersion = aVersion;

    memcpy(lAddress, aBytes, lAddressSize);

    mIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to convert an IPv4 or IPv6 address from a conventional
 *    text presentation format to this object.
 *
 *  This attempts to convert an IPv4 or IPv6 address from a
 *  conventional text presentation format to this object. For IPv4
 *  addresses, that is the dotted-decimal ("ddd.ddd.ddd.ddd")
 *  format. For IPv6 addresses, that is the colon-separated format
 *  ("x:x:x:x:x:x:x:x"), potentially with zero-suppression ("::x") and
 *  IPv4-mapping ("x:x:x:x:x:x:d.d.d.d").
 *
 *  @param[in]  aString  A pointer to an immutable string buffer
 *                       containing the IPv4 or IPv6 address to
 *                       parse and convert to an IP address.
 *  @param[in]  aLength  The length of the string buffer pointed to by
 *                       @a aString.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -EINVAL                If @a aString was null, zero length,
 *                                  or contained an invalid or
 *                                  malformed IPv4 or IPv6 address.
 */
Status
IPAddress :: FromString(const char *aString, const size_t &aLength)
{
    bool      lIsIPv6;
    int       lFamily;
    Version   lVersion;
    Addresses lParsedAddresses;
    void *    lParsedAddress;
    size_t    lParsedAddressSize;
    int       lStatus;
    Status    lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aString != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLength > 0, done, lRetval = -EINVAL);

    lIsIPv6            = (memchr(aString, ':', aLength) != nullptr);

    lFamily            = (lIsIPv6 ? AF_INET6 : AF_INET);
    lVersion           = (lIsIPv6 ? Version::kIPv6 : Version::kIPv4);
    lParsedAddress     = (lIsIPv6 ? static_cast<void *>(&lParsedAddresses.mIPv6Address) :
                          static_cast<void *>(&lParsedAddresses.mIPv4Address));

    lStatus = inet_pton(lFamily, aString, lParsedAddress);
    nlREQUIRE_ACTION(lStatus == 1, done, lRetval = ((lStatus < 0) ? -errno : -EINVAL));

    lParsedAddressSize = (lIsIPv6 ? sizeof (lParsedAddresses.mIPv6Address) :
                          sizeof (lParsedAddresses.mIPv4Address));

    lRetval = SetAddress(lVersion, lParsedAddress, lParsedAddressSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to convert an IPv4 or IPv6 address from a conventional
 *    text presentation format to this object.
 *
 *  This attempts to convert an IPv4 or IPv6 address from a
 *  conventional text presentation format to this object. For IPv4
 *  addresses, that is the dotted-decimal ("ddd.ddd.ddd.ddd")
 *  format. For IPv6 addresses, that is the colon-separated format
 *  ("x:x:x:x:x:x:x:x"), potentially with zero-suppression ("::x") and
 *  IPv4-mapping ("x:x:x:x:x:x:d.d.d.d").
 *
 *  @param[in]  aString  A pointer to an immutable null-terminated C
 *                       string containing the IPv4 or IPv6 address to
 *                       parse and convert to an IP address.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -EINVAL                If @a aString was null or contained
 *                                  an invalid or malformed IPv4 or IPv6
 *                                  address.
 */
Status
IPAddress :: FromString(const char *aString)
{
    Status lRetval;

    nlREQUIRE_ACTION(aString != nullptr, done, lRetval = -EINVAL);

    lRetval = FromString(aString, strlen(aString));

 done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
