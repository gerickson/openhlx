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
 *      This file defines an object for working with Internet Protocol
 *      version 4 (that is, IPv4) and version 6 (that is, IPv6)
 *      addresses.
 *
 */

#ifndef OPENHLXCOMMONIPADDRESS_HPP
#define OPENHLXCOMMONIPADDRESS_HPP

#include <stdint.h>

#include <arpa/inet.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    Object used to represent an IPv4 or IPv6 Internet Protocol (IP)
 *    address.
 *
 */
class IPAddress
{
public:
    /**
     *  Indicates the IP address version.
     *
     */
    enum class Version : uint8_t
    {
        kIPv4 = 4, //!< The address is an IPv4 address.
        kIPv6 = 6  //!< The address is an IPv6 address.
    };

public:
    IPAddress(void);
    ~IPAddress(void) = default;

    IPAddress(const Version &aVersion, const void *aBytes, const size_t &aSize);
    IPAddress(const IPAddress &aIPAddress);

    IPAddress & operator =(const IPAddress &aIPAddress);

    bool operator ==(const IPAddress &aIPAddress) const;

    Status GetVersion(Version &aOutVersion) const;

    bool IsVersion(const Version &aVersion) const;
    bool IsIPv4(void) const;
    bool IsIPv6(void) const;

    Status GetAddress(void *aOutBytes, const size_t &aSize) const;
    Status ToString(char *aBuffer, const size_t &aLength) const;

    Status SetAddress(const Version &aVersion, const void *aBytes, const size_t &aSize);
    Status FromString(const char *aString, const size_t &aLength);
    Status FromString(const char *aString);

private:
    bool    mIsNull;
    Version mVersion;
    union Addresses
    {
        struct in_addr  mIPv4Address;
        struct in6_addr mIPv6Address;
    } mAddresses;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONIPADDRESS_HPP
