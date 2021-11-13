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
 *      This file defines a state change delegate interface for the
 *      top- level HLX client controller object.
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLERSTATECHANGEDELEGATE_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLERSTATECHANGEDELEGATE_HPP


#include <OpenHLX/Client/ApplicationControllerBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Client
{

namespace Application
{

class Controller;

/**
 *  @brief
 *    A state change delegate interface for the top-level HLX client
 *    controller object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of the client as it
 *  moves through its lifetime, including:
 *
 *    - The state of its data model.
 *
 *  @ingroup client
 *
 */
class ControllerStateChangeDelegate
{
public:
    ControllerStateChangeDelegate(void) = default;
    virtual ~ControllerStateChangeDelegate(void) = default;

    // State Change Delegation Method

    /**
     *  @brief
     *    Delegation from the client controller that the controller
     *    state has changed in response to a change from the peer
     *    server controller.
     *
     *  @param[in]  aController               A reference to the
     *                                        client controller that
     *                                        issued the delegation.
     *  @param[in]  aStateChangeNotification  An immutable reference
     *                                        to a notification
     *                                        describing the state
     *                                        change.
     *
     */
    virtual void ControllerStateDidChange(Client::Application::ControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification) = 0;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLERSTATECHANGEDELEGATE_HPP
