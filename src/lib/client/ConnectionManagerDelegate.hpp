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
 *      This file defines a delegate interface for the client
 *      connection manager object.
 *
 */

#ifndef HLXCLIENTCONNECTIONMANAGERDELEGATE_HPP
#define HLXCLIENTCONNECTIONMANAGERDELEGATE_HPP

#include <OpenHLX/Common/ConnectionManagerDelegateBasis.hpp>


namespace HLX
{

namespace Client
{

class ConnectionManager;

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
class ConnectionManagerDelegate :
    public Common::ConnectionManagerDelegateBasis
{
public:
    ConnectionManagerDelegate(void) = default;
    ~ConnectionManagerDelegate(void) = default;

    // Connect

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server will connect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aTimeout            The timeout for the connection.
     *
     */
    virtual void ConnectionManagerWillConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server is connecting.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aTimeout            The timeout for the connection.
     *
     */
    virtual void ConnectionManagerIsConnecting(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did connect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *
     */
    virtual void ConnectionManagerDidConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did not connect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed connection.
     *
     */
    virtual void ConnectionManagerDidNotConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCONNECTIONMANAGERDELEGATE_HPP
