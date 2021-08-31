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

#ifndef OPENHLXCLIENTOBJECTCONTROLLERBASISERRORDELEGATE_HPP
#define OPENHLXCLIENTOBJECTCONTROLLERBASISERRORDELEGATE_HPP

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Client
{

class ObjectControllerBasis;

/**
 *  @brief
 *    A delegate interface for the client controller basis object.
 *
 *  @ingroup client
 *
 */
class ObjectControllerBasisErrorDelegate
{
public:
    ObjectControllerBasisErrorDelegate(void) = default;
    virtual ~ObjectControllerBasisErrorDelegate(void) = default;

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
    virtual void ControllerError(ObjectControllerBasis &aController, const Common::Error &aError) = 0;

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTOBJECTCONTROLLERBASISERRORDELEGATE_HPP
