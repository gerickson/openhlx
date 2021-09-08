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
 *      connection listener basis object.
 *
 */

#ifndef OPENHLXSERVERLISTENERBASISACCEPTDELEGATE_HPP
#define OPENHLXSERVERLISTENERBASISACCEPTDELEGATE_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>


namespace HLX
{

namespace Server
{

class ConnectionManager;

/**
 *  @brief
 *    A delegate interface for the server connection listener basis
 *    object.
 *
 *  @ingroup server
 *
 */
class ListenerBasisAcceptDelegate
{
public:
    // Con/destructor

    ListenerBasisAcceptDelegate(void) = default;
    ~ListenerBasisAcceptDelegate(void) = default;

    // Accept Method

    /**
     *  @brief
     *    Delegation from a connection listener that it did accept a
     *    connection from a peer client on the specified socket with
     *    the specified address.
     *
     *  The delegate should create whatever management state is
     *  necessary for tracking the accepted connection and return
     *  successful status if it was successfully able to do so. If the
     *  delegate is unable to successfully handle the accepted
     *  connection, non-successful status should be returned and the
     *  connection will be closed and discarded.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *  @param[in]  aSocket    An immutable reference to the socket
     *                         associated with the accepted connection.
     *  @param[in]  aAddress   An immutable reference to the socket
     *                         address associated with the peer client
     *                         at the remote end of the accepted
     *                         connection.
     *
     *  @returns
     *    kStatus_Success if the delegate successfully handled the
     *    delegation; otherwise, non-successful status on error.
     *
     */
    virtual Common::Status ListenerDidAccept(ListenerBasis &aListener, const int &aSocket, const Common::SocketAddress &aAddress) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERLISTENERBASISACCEPTDELEGATE_HPP
