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
 *      This file defines a delegate interface for the top-level HLX
 *      client controller object.
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLERDELEGATE_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLERDELEGATE_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Client/ApplicationControllerRefreshDelegate.hpp>
#include <OpenHLX/Client/ApplicationControllerStateChangeDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/ApplicationControllerErrorDelegate.hpp>
#include <OpenHLX/Common/IPAddress.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Client
{

namespace Application
{

class Controller;

/**
 *  @brief
 *    A delegate interface for the top-level HLX client controller
 *    object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of the client as it
 *  moves through its lifetime, including:
 *
 *    - The state of its connection.
 *    - The state of its data model and properties.
 *
 *  @ingroup client
 *
 */
class ControllerDelegate :
    public ControllerRefreshDelegate,
    public ControllerStateChangeDelegate,
    public Common::Application::ControllerErrorDelegate
{
public:
    ControllerDelegate(void) = default;
    virtual ~ControllerDelegate(void) = default;

    // Resolve Delegation Methods

    /**
     *  @brief
     *    Delegation from the client controller that a host name will
     *    resolve.
     *
     *  @param[in]  aController         A reference to the client controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that will resolve.
     *
     */
    virtual void ControllerWillResolve(Controller &aController, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a host name is
     *    resolving.
     *
     *  @param[in]  aController         A reference to the client controller
     *                                  that issued the delegation.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that is resolving.
     *
     */
    virtual void ControllerIsResolving(Controller &aController, const char *aHost) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a host name has
     *    resolved to an IP address.
     *
     *  @note
     *    This delegation may be called more than once for a
     *    resolution, once for each IP address the host name resolves
     *    to.
     *
     *  @param[in]  aController         A reference to the client controller
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
     *    Delegation from the client controller that a host name did
     *    not resolve.
     *
     *  @param[in]  aController         A reference to the client controller
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

    // Connect Delegation Methods

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server will connect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aTimeout     The timeout for the connection.
     *
     */
    virtual void ControllerWillConnect(Controller &aController, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server is connecting.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aTimeout     The timeout for the connection.
     *
     */
    virtual void ControllerIsConnecting(Controller &aController, CFURLRef aURLRef, const Common::Timeout &aTimeout) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server did connect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *
     */
    virtual void ControllerDidConnect(Controller &aController, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server did not connect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failed connection.
     *
     */
    virtual void ControllerDidNotConnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Disconnect Delegation Methods

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server will disconnect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *
     */
    virtual void ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server did disconnect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the disconnection.
     *
     */
    virtual void ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a connection to a
     *    peer server did not disconnect.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aURLRef      The URL associated with the peer server.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failed
     *                           disconnection.
     *
     */
    virtual void ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLERDELEGATE_HPP
