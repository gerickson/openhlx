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
 *      This file defines an object for working with and managing an
 *      IETF RFC 1738-style Internet host URL and a resolved IPv4 or
 *      IPv6 socket address.
 *
 */

#ifndef HLXCOMMONHOSTURLADDRESS_HPP
#define HLXCOMMONHOSTURLADDRESS_HPP

#include <CoreFoundation/CFString.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HostURL.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for working with and managing an IETF RFC 1738-style
 *    Internet host URL and a resolved IPv4 or IPv6 socket address.
 *
 *  @ingroup common
 *
 */
class HostURLAddress :
    public HostURL
{
public:
    HostURLAddress(void) = default;
    ~HostURLAddress(void) = default;

    Status Init(CFStringRef aScheme, const SocketAddress &aAddress);
    Status Init(CFStringRef aScheme, const SocketAddress &aAddress, const bool &aIsHostOrder);
    Status Init(const char *aScheme, const SocketAddress &aAddress);
    Status Init(const char *aScheme, const SocketAddress &aAddress, const bool &aIsHostOrder);

    bool operator ==(const HostURLAddress &aHostURLAddress) const;

    HostURLAddress & operator =(const HostURLAddress &aHostURLAddress);
    HostURLAddress & operator =(HostURLAddress &&aHostURLAddress);

    const SocketAddress &GetAddress(void) const;

private:
    HostURLAddress(const HostURL &aHostURLAddress) = delete;
    HostURLAddress(HostURL &&aHostURLAddress) = delete;

private:
    SocketAddress  mAddress;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONHOSTURLADDRESS_HPP
