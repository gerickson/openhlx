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
 *      This file defines a refresh delegate interface for the top-
 *      level HLX client controller object.
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLERREFRESHDELEGATE_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLERREFRESHDELEGATE_HPP


#include <OpenHLX/Client/HLXClientControllerBasis.hpp>
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
 *    A refresh delegate interface for the top-level HLX client
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
class ControllerRefreshDelegate
{
public:
    ControllerRefreshDelegate(void) = default;
    virtual ~ControllerRefreshDelegate(void) = default;

    // Refresh Delegation Methods

    /**
     *  @brief
     *    Delegation from the client controller that a state refresh
     *    with the peer server is about to begin.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *
     */
    virtual void ControllerWillRefresh(Client::Application::ControllerBasis &aController) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a state refresh
     *    with the peer server is in progress.
     *
     *  @param[in]  aController       A reference to the client controller
     *                                that issued the delegation.
     *  @param[in]  aPercentComplete  A reference to the percentage
     *                                (0-100) of the refresh operation
     *                                that has completed.
     *
     */
    virtual void ControllerIsRefreshing(Client::Application::ControllerBasis &aController, const uint8_t &aPercentComplete) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a state refresh
     *    with the peer server did complete successfully.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *
     */
    virtual void ControllerDidRefresh(Client::Application::ControllerBasis &aController) = 0;

    /**
     *  @brief
     *    Delegation from the client controller that a state refresh
     *    with the peer server did not complete successfully.
     *
     *  @param[in]  aController  A reference to the client controller that
     *                           issued the delegation.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the failure to refresh.
     *
     */
    virtual void ControllerDidNotRefresh(Client::Application::ControllerBasis &aController, const Common::Error &aError) = 0;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLERREFRESHDELEGATE_HPP
