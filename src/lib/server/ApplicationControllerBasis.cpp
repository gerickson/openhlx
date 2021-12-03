/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements a derivable object for creating HLX
 *      server application controllers.
 *
 */

#include "ApplicationControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Application
{

// MARK: Server Controller Basis

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ControllerBasis :: ControllerBasis(void) :
    ServerObjectControllerContainer(),
    mConnectionManager(),
    mCommandManager()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the server controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  server controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the server controller was already
 *                                    added as a delegate to the
 *                                    connection manager, command
 *                                    manager, or child controllers.
 *
 */
Status
ControllerBasis :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;


    lRetval = ServerObjectControllerContainer::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mCommandManager.Init(mConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Determine whether the controller supports connections with
 *    the specified protocol scheme.
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
ControllerBasis :: SupportsScheme(CFStringRef aScheme) const
{
    bool lRetval;

    lRetval = mConnectionManager.SupportsScheme(aScheme);

    return (lRetval);
}

// MARK: Accessors

/**
 *  @brief
 *    Return the command manager for the controller.
 *
 *  @returns
 *    An immutable reference to the command manager for the
 *    controller.
 *
 */
const Server::CommandManager &
ControllerBasis :: GetCommandManager(void) const
{
    return (mCommandManager);
}

/**
 *  @brief
 *    Return the command manager for the controller.
 *
 *  @returns
 *    A mutable reference to the command manager for the
 *    controller.
 *
 */
Server::CommandManager &
ControllerBasis :: GetCommandManager(void)
{
    return (mCommandManager);
}

/**
 *  @brief
 *    Return the connection manager for the controller.
 *
 *  @returns
 *    An immutable reference to the connection manager for the
 *    controller.
 *
 */
const Server::ConnectionManager &
ControllerBasis :: GetConnectionManager(void) const
{
    return (mConnectionManager);
}

/**
 *  @brief
 *    Return the connection manager for the controller.
 *
 *  @returns
 *    A mutable reference to the connection manager for the
 *    controller.
 *
 */
Server::ConnectionManager &
ControllerBasis :: GetConnectionManager(void)
{
    return (mConnectionManager);
}

// MARK: Listen

/**
 *  @brief
 *    Listen for unsolicited, asynchronous connections from HLX client
 *    peers.
 *
 *  This attempts to asynchronously listen for unsolicited connections
 *  from HLX client peers at the IPv4 and IPv6 "any" addresses on the
 *  default HLX port.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the controller was already
 *                                    added as an accept delegate.
 *  @retval  -EBUSY                   If the controller is already
 *                                    listening.
 *  @retval  -ENOMEM                  Resources could not be allocated
 *                                    to listen.
 *
 */
Status
ControllerBasis :: Listen(void)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Listen();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Listen for unsolicited, asynchronous connections from HLX client
 *    peers.
 *
 *  This attempts to asynchronously listen for unsolicited connections
 *  from HLX client peers at the IPv4 and/or IPv6 "any" addresses on the
 *  default HLX port.
 *
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the listen host name(s).
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the controller was already
 *                                    added as an accept delegate.
 *  @retval  -EBUSY                   If the controller is already
 *                                    listening.
 *  @retval  -ENOMEM                  Resources could not be allocated
 *                                    to listen.
 *
 */
Status
ControllerBasis :: Listen(const Common::ConnectionManagerBasis::Versions &aVersions)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Listen(aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Listen for unsolicited, asynchronous connections from HLX client
 *    peers at the specified listen URL, host name, or address.
 *
 *  This attempts to asynchronously listen for unsolicited connections
 *  from HLX client peers at the IPv4 and IPv6 addresses associated
 *  with the specified URL, host name, or host name and port.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port to listen on. The URL or host name
 *                         may be a name to be resolved or a literal
 *                         IP address.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the controller was already
 *                                    added as an accept delegate.
 *  @retval  -EBUSY                   If the controller is already
 *                                    listening.
 *  @retval  -ENOMEM                  Resources could not be allocated
 *                                    to listen.
 *
 */
Status
ControllerBasis :: Listen(const char *aMaybeURL)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Listen(aMaybeURL);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Listen for unsolicited, asynchronous connections from HLX client
 *    peers at the specified listen URL, host name, or address.
 *
 *  This attempts to asynchronously listen for unsolicited connections
 *  from HLX client peers at the IPv4 and/or IPv6 addresses associated
 *  with the specified URL, host name, or host name and port.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port to listen on. The URL or host name
 *                         may be a name to be resolved or a literal
 *                         IP address.
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the listen host name(s).
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the controller was already
 *                                    added as an accept delegate.
 *  @retval  -EBUSY                   If the controller is already
 *                                    listening.
 *  @retval  -ENOMEM                  Resources could not be allocated
 *                                    to listen.
 *
 */
Status
ControllerBasis :: Listen(const char *aMaybeURL,
                          const Common::ConnectionManagerBasis::Versions &aVersions)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Listen(aMaybeURL, aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Application

}; // namespace Server

}; // namespace HLX
