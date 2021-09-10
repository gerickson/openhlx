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
 *      This file implements an object for creating HLX server
 *      network connections.
 *
 */

#include "ConnectionFactory.hpp"

#include <CFUtilities/CFString.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ConnectionTelnet.hpp>
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
ConnectionFactory :: ConnectionFactory(void) :
    mRunLoopParameters(),
    mConnections()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConnectionFactory :: ~ConnectionFactory(void)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection factory with the specified
 *  run loop parameters.
 *
 *  @note
 *    The connection factory itself is not a run loop source;
 *    however, the connections it allocates and instantiates
 *    likely will be.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection factory with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionFactory :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

/**
 *  @brief
 *    Determine whether the factory supports creating a connection
 *    with the specified protocol scheme.
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
ConnectionFactory :: SupportsScheme(CFStringRef aSchemeRef) const
{
    const CFString lRequestedScheme(aSchemeRef);
    bool lRetval = false;

    if (lRequestedScheme == ConnectionTelnet::kScheme)
    {
        lRetval = true;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Create a connection with the specified protocol scheme.
 *
 *  This attempts to create a connection with the specified protocol
 *  scheme (for example, "telnet").
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme for which to create
 *                          a connection.
 *
 *  @returns
 *    A valid unique pointer to the newly-created connection on success;
 *    otherwise, an empty, invalid unique pointer.
 *
 */
std::unique_ptr<ConnectionBasis>
ConnectionFactory :: CreateConnection(CFStringRef aSchemeRef) const
{
    const CFString                    lRequestedScheme(aSchemeRef);
    std::unique_ptr<ConnectionBasis>  lRetval;

    if (lRequestedScheme == ConnectionTelnet::kScheme)
    {
        lRetval.reset(new ConnectionTelnet());
        nlREQUIRE(lRetval != nullptr, done);
    }

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
