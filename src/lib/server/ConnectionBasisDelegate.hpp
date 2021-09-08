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
 *      This file defines a delegate interface for the HLX server
 *      peer-to-peer network connection basis object.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONBASISDELEGATE_HPP
#define OPENHLXSERVERCONNECTIONBASISDELEGATE_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Server
{

class ConnectionBasis;

/**
 *  @brief
 *    A delegate interface for the HLX server peer-to-peer network
 *    connection basis object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the pending and stable state of a server
 *  connection as it moves through its lifetime.
 *
 *  @ingroup server
 *
 */
class ConnectionBasisDelegate
{
public:
    // Con/destructor

    ConnectionBasisDelegate(void) = default;
    ~ConnectionBasisDelegate(void) = default;

    // Accept Methods

    /**
     *  @brief
     *    Delegation from a connection that it will accept a
     *    connection from a peer client.
     *
     *  @param[in]  aConnection  A reference to the connection
     *                           that issued the delegation.
     *
     */
    virtual void ConnectionWillAccept(ConnectionBasis &aConnection) = 0;

    /**
     *  @brief
     *    Delegation from a connection that it is in the process of
     *    accepting a connection from a peer client.
     *
     *  @param[in]  aConnection  A reference to the connection
     *                           that issued the delegation.
     *
     */
    virtual void ConnectionIsAccepting(ConnectionBasis &aConnection) = 0;

    /**
     *  @brief
     *    Delegation from a connection that it did accept a
     *    connection from a peer client.
     *
     *  @param[in]  aConnection  A reference to the connection
     *                           that issued the delegation.
     *
     */
    virtual void ConnectionDidAccept(ConnectionBasis &aConnection) = 0;

    /**
     *  @brief
     *    Delegation from a connection that it did not accept a
     *    connection from a peer client.
     *
     *  @param[in]  aConnection  A reference to the connection
     *                           that issued the delegation.
     *  @param[in]  aError       An immutable reference to the
     *                           error associated with the
     *                           failed accept.
     *
     */
    virtual void ConnectionDidNotAccept(ConnectionBasis &aConnection, const Common::Error &aError) = 0;

    // MARK: Application Data Method

    /**
     *  @brief
     *    Delegation from a connection that the connection has received
     *    application data.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aBuffer      The buffer containing the received
     *                           application data.
     *
     */
    virtual void ConnectionDidReceiveApplicationData(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer aBuffer) = 0;

    // MARK: Disconnect Methods

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer
     *    will disconnect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *
     */
    virtual void ConnectionWillDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer
     *    did disconnect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the disconnection.
     *
     */
    virtual void ConnectionDidDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) = 0;

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer
     *    did not disconnect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failed
     *                           disconnection.
     *
     */
    virtual void ConnectionDidNotDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // MARK: Error Method

    /**
     *  @brief
     *    Delegation from a connection that the connection to a
     *    peer experienced an error.
     *
     *  @note
     *    This delegation may occur along with other delegations with
     *    respect to the same underlying event or cause.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the event.
     *
     */
    virtual void ConnectionError(ConnectionBasis &aConnection, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONBASISDELEGATE_HPP
