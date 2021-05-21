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
 *      This file defines a type for managing IPv4 or IPv6 socket
 *      addresses.
 *
 */

#ifndef HLXCOMMONSOCKETADDRESS_HPP
#define HLXCOMMONSOCKETADDRESS_HPP

#include <netinet/in.h>
#include <sys/socket.h>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    Type for managing IPv4 or IPv6 socket addresses.
 *
 *  @ingroup common
 *
 */
union SocketAddress
{
    sockaddr      uSocketAddress;      //!< Abstract socket address.
    sockaddr_in   uSocketAddressIPv4;  //!< IPv4 socket address.
    sockaddr_in6  uSocketAddressIPv6;  //!< IPv6 socket address.
};

extern bool operator ==(const SocketAddress &aFirst, const SocketAddress &aSecond);

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONSOCKETADDRESS_HPP
