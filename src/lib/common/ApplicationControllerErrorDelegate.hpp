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

#ifndef OPENHLXCOMMONAPPLICATIONCONTROLLERDELEGATE_HPP
#define OPENHLXCOMMONAPPLICATIONCONTROLLERDELEGATE_HPP

#include <OpenHLX/Common/ApplicationControllerBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Application
{

/**
 *  @brief
 *    A delegate interface for a top-level HLX application controller
 *    object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of the application controller as
 *  it encounters errors, which may be preceded by other
 *  domain-specific error delegations.
 *
 *  @ingroup common
 *
 */
class ControllerErrorDelegate
{
public:
    ControllerErrorDelegate(void) = default;
    virtual ~ControllerErrorDelegate(void) = default;

    /**
     *  @brief
     *    Delegation from the controller that the experienced an
     *    error.
     *
     *  @note
     *    This delegation may occur along with other delegations with
     *    respect to the same underlying event or cause.
     *
     *  @param[in]  aController  A reference to the controller that
     *                           issued the delegation.
     *  @param[in]  aError       An immutable reference to the error
     *                           associated with the event.
     *
     */
    virtual void ControllerError(Common::Application::ControllerBasis &aController, const Common::Error &aError) = 0;
};

}; // namespace Application

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONAPPLICATIONCONTROLLERDELEGATE_HPP
