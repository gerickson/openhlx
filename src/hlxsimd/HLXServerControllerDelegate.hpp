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

    // Resolve

    virtual void ControllerWillResolve(Controller &aController, const char *aHost) = 0;
    virtual void ControllerIsResolving(Controller &aController, const char *aHost) = 0;
    virtual void ControllerDidResolve(Controller &aController, const char *aHost, const Common::IPAddress &aIPAddress) = 0;
    virtual void ControllerDidNotResolve(Controller &aController, const char *aHost, const Common::Error &aError) = 0;

    // Listen

    virtual void ControllerWillListen(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerIsListening(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidListen(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidNotListen(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Accept

    virtual void ControllerWillAccept(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerIsAccepting(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidAccept(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidNotAccept(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Disconnect

    virtual void ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef) = 0;
    virtual void ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;
    virtual void ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Error

    virtual void ControllerError(Controller &aController, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONTROLLERDELEGATE_HPP
