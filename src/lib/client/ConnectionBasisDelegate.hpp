/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines a delegate interface for the HLX client
 *      peer-to-peer network connection basis object.
 *
 */

#ifndef HLXCLIENTCONNECTIONBASISDELEGATE_HPP
#define HLXCLIENTCONNECTIONBASISDELEGATE_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Client
{

class ConnectionBasis;

/**
 *  @brief
 *    A delegate interface for the HLX client peer-to-peer network
 *    connection basis object.
 *
 *  @ingroup client
 *
 */
class ConnectionBasisDelegate
{
public:
    ConnectionBasisDelegate(void) = default;
    ~ConnectionBasisDelegate(void) = default;

    // MARK: Connect

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer will
     *    connect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *  @param[in]  aTimeout     The timeout for the connection.
     *
     */
    virtual void ConnectionWillConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer is
     *    connecting.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *  @param[in]  aTimeout     The timeout for the connection.
     *
     */
    virtual void ConnectionIsConnecting(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer did
     *    connect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *
     */
    virtual void ConnectionDidConnect(ConnectionBasis &aConnection, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from a connection that the connection to a peer did
     *    not connect.
     *
     *  @param[in]  aConnection  A reference to the connection that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failed connection.
     *
     */
    virtual void ConnectionDidNotConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // MARK: Application Data

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

    // MARK: Disconnect

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

    // MARK: Error

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

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCONNECTIONBASISDELEGATE_HPP
