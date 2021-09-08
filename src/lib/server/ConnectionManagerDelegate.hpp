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
 *      This file defines a delegate interface for the server
 *      connection manager object.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP
#define OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP

#include <OpenHLX/Common/ConnectionManagerDelegateBasis.hpp>


namespace HLX
{

namespace Server
{

class ConnectionManager;

/**
 *  @brief
 *    A delegate interface for the server connection manager object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the pending and stable state of a server
 *  connection as it moves through its lifetime.
 *
 *  @ingroup server
 *
 */
class ConnectionManagerDelegate :
    public Common::ConnectionManagerDelegateBasis
{
public:
    // Con/destructor

    ConnectionManagerDelegate(void) = default;
    ~ConnectionManagerDelegate(void) = default;

    // Listen Methods

    /**
     *  @brief
     *    Delegation from the connection manager that it will listen
     *    for connections from peer clients at the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL at which the server is
     *                                  to listen at.
     *
     */
    virtual void ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it is in the
     *    processing of listening for connections from peer clients at
     *    the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL at which the server is
     *                                  listening at.
     *
     */
    virtual void ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it is listening
     *    for connections from peer clients at the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL at which the server is
     *                                  listening at.
     *
     */
    virtual void ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it did not
     *    listen for connections from peer clients at the specified
     *    URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL at which the server
     *                                  was to listen at.
     *  @param[in]  aError              An immutable reference to the
     *                                  error associated with the
     *                                  failed server listen.
     *
     */
    virtual void ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Accept Methods

    /**
     *  @brief
     *    Delegation from the connection manager that it will accept
     *    a connection from the peer client at the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL for the client from
     *                                  which the server will accept a
     *                                  connection.
     *
     */
    virtual void ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it is in the
     *    process of accepting a connection from the peer client at
     *    the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL for the client from
     *                                  which the server is accepting
     *                                  a connection.
     *
     */
    virtual void ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it did accept
     *    a connection from the peer client at the specified URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL for the client from
     *                                  which the server did accept a
     *                                  connection.
     *
     */
    virtual void ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the connection manager that it did not
     *    accept a connection from the peer client at the specified
     *    URL.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the
     *                                  delegation.
     *  @param[in]  aURLRef             The URL for the client from
     *                                  which the server did not
     *                                  accept a connection.
     *  @param[in]  aError              An immutable reference to the
     *                                  error associated with the
     *                                  failed server accept.
     *
     */
    virtual void ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP
