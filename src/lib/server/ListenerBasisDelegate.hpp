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
 *      peer-to-peer network connection listener basis object.
 *
 */

#ifndef OPENHLXSERVERLISTENERBASISDELEGATE_HPP
#define OPENHLXSERVERLISTENERBASISDELEGATE_HPP

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include <CoreFoundation/CFURL.h>


namespace HLX
{

namespace Server
{

class ListenerBasis;

/**
 *  @brief
 *    A delegate interface for the HLX server peer-to-peer network
 *    connection listener basis object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the pending and stable state of a server
 *  connection listener as it moves through its lifetime.
 *
 *  @ingroup server
 *
 */
class ListenerBasisDelegate
{
public:
    // Con/destructor

    ListenerBasisDelegate(void) = default;
    ~ListenerBasisDelegate(void) = default;

    // Listen Methods

    /**
     *  @brief
     *    Delegation from a connection listener that it will listen
     *    for connections from peer clients.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *
     */
    virtual void ListenerWillListen(ListenerBasis &aListener) = 0;

    /**
     *  @brief
     *    Delegation from a connection listener that it is in the
     *    process of listening for connections from peer clients.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *
     */
    virtual void ListenerIsListening(ListenerBasis &aListener) = 0;

    /**
     *  @brief
     *    Delegation from a connection listener that it is now
     *    listening for connections from peer clients.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *
     */
    virtual void ListenerDidListen(ListenerBasis &aListener) = 0;

    /**
     *  @brief
     *    Delegation from a connection listener that it did not listen
     *    for connections from peer clients.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *  @param[in]  aError     An immutable reference to the error
     *                         associated with the failed listen.
     *
     */
    virtual void ListenerDidNotListen(ListenerBasis &aListener, const Common::Error &aError) = 0;

    // Error Method

    /**
     *  @brief
     *    Delegation from a connection listener that the listener
     *    experienced an error.
     *
     *  @note
     *    This delegation may occur along with other delegations with
     *    respect to the same underlying event or cause.
     *
     *  @param[in]  aListener  A reference to the connection listener
     *                         that issued the delegation.
     *  @param[in]  aError     An immutable reference to the error
     *                         associated with the event.
     *
     */
    virtual void ListenerError(ListenerBasis &aListener, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERLISTENERBASISDELEGATE_HPP
