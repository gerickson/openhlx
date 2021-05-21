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
 *      This file implements functions for managing IPv4 or IPv6 socket
 *      addresses.
 *
 */

#include "SocketAddress.hpp"

#include <string.h>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    Return whether the specified socket addresses are equal.
 *
 *  @param[in]  aFirst   The first of two socket addresses to
 *                       compare.
 *  @param[in]  aSecond  The second of two socket addresses to
 *                       compare.
 *
 *  @returns
 *    True if the specified socket addresses are equal; otherwise,
 *    false.
 *
 */
bool
operator ==(const SocketAddress &aFirst, const SocketAddress &aSecond)
{
    bool lRetval = false;

    if (&aFirst == &aSecond)
    {
        lRetval = true;
    }
    else if (aFirst.uSocketAddress.sa_family ==
             aSecond.uSocketAddress.sa_family)
    {
        switch (aFirst.uSocketAddress.sa_family)
        {

        case AF_INET:
            lRetval = (memcmp(&aFirst.uSocketAddressIPv4,
                              &aSecond.uSocketAddressIPv4,
                              sizeof (sockaddr_in)) == 0);
            break;

        case AF_INET6:
            lRetval = (memcmp(&aFirst.uSocketAddressIPv6,
                              &aSecond.uSocketAddressIPv6,
                              sizeof (sockaddr_in6)) == 0);
            break;

        default:
            lRetval = (memcmp(&aFirst,
                              &aSecond,
                              sizeof (SocketAddress)) == 0);
            break;

        }
    }

    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
