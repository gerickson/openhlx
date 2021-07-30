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
 *      This file defines a delegate interface for the top-level HLX
 *      server controller object.
 *
 */

#ifndef HLXSERVERCONTROLLERDELEGATE_HPP
#define HLXSERVERCONTROLLERDELEGATE_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/IPAddress.hpp>


namespace HLX
{

namespace Server
{

class Controller;

/**
 *  @brief
 *    A delegate interface for the top-level HLX server controller
 *    object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of the server as it
 *  moves through its lifetime, including:
 *
 *    - The state of its connections.
 *
 *  @ingroup server
 *
 */
class ControllerDelegate
{
public:
    ControllerDelegate(void) = default;
    virtual ~ControllerDelegate(void) = default;

    // Resolve Delegation Methods

    /**
     *  @brief
     *    Delegation from the server controller that a host name will
     *    resolve.
     *
     *  @param[in]  aController         A reference to the server controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that will resolve.
     *
     */
    virtual void ControllerWillResolve(Controller &aController, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the server controller that a host name is
     *    resolving.
     *
     *  @param[in]  aController         A reference to the server controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that is resolving.
     *
     */
    virtual void ControllerIsResolving(Controller &aController, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the server controller that a host name has
     *    resolved to an IP address.
     *
     *  @note
     *    This delegation may be called more than once for a
     *    resolution, once for each IP address the host name resolves
     *    to.
     *
     *  @param[in]  aController         A reference to the server controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did resolve.
     *  @param[in]  aIPAddress          An immutable reference to an IP
     *                                  address that the host name
     *                                  resolved to.
     *
     */
    virtual void ControllerDidResolve(Controller &aController, const char *aHost, const Common::IPAddress &aIPAddress) = 0;

    /**
     *  @brief
     *    Delegation from the server controller that a host name did
     *    not resolve.
     *
     *  @param[in]  aController         A reference to the server controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did not resolve.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  resolution.
     *
     */
    virtual void ControllerDidNotResolve(Controller &aController, const char *aHost, const Common::Error &aError) = 0;

    // Listen Delegation Methods

    virtual void ControllerWillListen(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerIsListening(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidListen(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidNotListen(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Accept Delegation Methods

    virtual void ControllerWillAccept(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerIsAccepting(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidAccept(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidNotAccept(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Disconnect Delegation Methods

    /**
     *  @brief
     *    Delegation from the server controller that a connection to a
     *    peer server will disconnect.
     *
     *  @param[in]  aController  A reference to the server controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *
     */
    virtual void ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the server controller that a connection to a
     *    peer server did disconnect.
     *
     *  @param[in]  aController  A reference to the server controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the disconnection.
     *
     */
    virtual void ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    /**
     *  @brief
     *    Delegation from the server controller that a connection to a
     *    peer server did not disconnect.
     *
     *  @param[in]  aController  A reference to the server controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failed
     *                           disconnection.
     *
     */
    virtual void ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Error Delegation Method

    /**
     *  @brief
     *    Delegation from the server controller that the experienced
     *    an error.
     *
     *  @note
     *    This delegation may occur along with other delegations with
     *    respect to the same underlying event or cause.
     *
     *  @param[in]  aController  A reference to the server controller that
     *                           issued the delegation.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the event.
     *
     */
    virtual void ControllerError(Controller &aController, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONTROLLERDELEGATE_HPP
