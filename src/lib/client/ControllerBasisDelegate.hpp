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

#ifndef OPENHLXCLIENTCONTROLLERBASISDELEGATE_HPP
#define OPENHLXCLIENTCONTROLLERBASISDELEGATE_HPP

#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>


namespace HLX
{

namespace Client
{

class ControllerBasis;

/**
 *  @brief
 *    A delegate interface for the client controller basis object.
 *
 *  @ingroup client
 *
 */
class ControllerBasisDelegate
{
public:
    ControllerBasisDelegate(void) = default;
    ~ControllerBasisDelegate(void) = default;

    /**
     *  @brief
     *    Delegation from a controller that the controller is
     *    refreshing.
     *
     *  @param[in]  aController       A reference to the controller
     *                                that issued the delegation.
     *  @param[in]  aPercentComplete  A reference to the percentage
     *                                (0-100) of the refresh operation
     *                                that has completed.
     *
     */
    virtual void ControllerIsRefreshing(ControllerBasis &aController, const uint8_t &aPercentComplete) = 0;

    /**
     *  @brief
     *    Delegation from a controller that the controller is
     *    done refreshing.
     *
     *  @param[in]  aController       A reference to the controller
     *                                that issued the delegation.
     *
     */
    virtual void ControllerDidRefresh(ControllerBasis &aController) = 0;

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
    virtual void ControllerStateDidChange(ControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification) = 0;

    /**
     *  @brief
     *    Delegation from a controller that the controller experienced
     *    an error.
     *
     *  @param[in]  aController  A reference to the controller that
     *                           issued the delegation.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the event.
     *
     */
    virtual void ControllerError(ControllerBasis &aController, const Common::Error &aError) = 0;

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONTROLLERBASISDELEGATE_HPP
