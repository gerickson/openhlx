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
 *      This file defines a delegate interface for the connection
 *      manager object.
 *
 */

#ifndef HLXCOMMONCONNECTIONMANAGERDELEGATEBASIS_HPP
#define HLXCOMMONCONNECTIONMANAGERDELEGATEBASIS_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Common
{

class ConnectionManagerBasis;

/**
 *  @brief
 *    A delegate interface for the client connection manager object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the pending and stable state of a client
 *  connection as it moves through its lifetime.
 *
 *  @ingroup client
 *
 */
class ConnectionManagerDelegateBasis
{
public:
    ConnectionManagerDelegateBasis(void) = default;
    virtual ~ConnectionManagerDelegateBasis(void) = default;

    // Resolve

    /**
     *  @brief
     *    Delegation from the connection manager that a host name will
     *    resolve.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that will resolve.
     *
     */
    virtual void ConnectionManagerWillResolve(ConnectionManagerBasis &aConnectionManager, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a host name is
     *    resolving.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that is resolving.
     *
     */
    virtual void ConnectionManagerIsResolving(ConnectionManagerBasis &aConnectionManager, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a host name has
     *    resolved to an IP address.
     *
     *  @note
     *    This delegation may be called more than once for a
     *    resolution, once for each IP address the host name resolves
     *    to.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did resolve.
     *  @param[in]  aIPAddress          An immutable reference to an IP
     *                                  address that the host name
     *                                  resolved to.
     *
     */
    virtual void ConnectionManagerDidResolve(ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a host name did
     *    not resolve.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did not resolve.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  resolution.
     *
     */
    virtual void ConnectionManagerDidNotResolve(ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError) = 0;

    // Disconnect

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server will disconnect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *
     */
    virtual void ConnectionManagerWillDisconnect(ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did disconnect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the disconnection.
     *
     */
    virtual void ConnectionManagerDidDisconnect(ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did not disconnect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  disconnection.
     *
     */
    virtual void ConnectionManagerDidNotDisconnect(ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Error

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server experienced an error.
     *
     *  @note
     *    This delegation may occur along with other delegations with
     *    respect to the same underlying event or cause.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the event.
     *
     */
    virtual void ConnectionManagerError(ConnectionManagerBasis &aConnectionManager, const Common::Error &aError) = 0;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCONNECTIONMANAGERDELEGATEBASIS_HPP
