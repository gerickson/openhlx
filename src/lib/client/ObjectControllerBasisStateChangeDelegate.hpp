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
 *      This file defines a delegate interface for the client
 *      controller basis object.
 *
 */

#ifndef OPENHLXCLIENTOBJECTCONTROLLERBASISSTATECHANGEDELEGATE_HPP
#define OPENHLXCLIENTOBJECTCONTROLLERBASISSTATECHANGEDELEGATE_HPP

#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>


namespace HLX
{

namespace Client
{

class ObjectControllerBasis;

/**
 *  @brief
 *    A state change delegate interface for the client controller
 *    basis object.
 *
 *  @ingroup client
 *
 */
class ObjectControllerBasisStateChangeDelegate
{
public:
    ObjectControllerBasisStateChangeDelegate(void) = default;
    virtual ~ObjectControllerBasisStateChangeDelegate(void) = default;

    /**
     *  @brief
     *    Delegation from a controller that the controller state
     *    has changed in response to a change from the peer server
     *    controller.
     *
     *  @param[in]  aController               A reference to the
     *                                        controller that issued
     *                                        the delegation.
     *  @param[in]  aStateChangeNotification  An immutable reference
     *                                        to a notification
     *                                        describing the state
     *                                        change.
     *
     */
    virtual void ControllerStateDidChange(ObjectControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification) = 0;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTOBJECTCONTROLLERBASISSTATECHANGEDELEGATE_HPP
