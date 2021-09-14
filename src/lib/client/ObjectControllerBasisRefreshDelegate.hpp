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

#ifndef OPENHLXCLIENTOBJECTCONTROLLERBASISREFRESHDELEGATE_HPP
#define OPENHLXCLIENTOBJECTCONTROLLERBASISREFRESHDELEGATE_HPP


namespace HLX
{

namespace Client
{

class ObjectControllerBasis;

/**
 *  @brief
 *    A controller refresh delegate interface for the client
 *    controller basis object.
 *
 *  @ingroup client
 *
 */
class ObjectControllerBasisRefreshDelegate
{
public:
    ObjectControllerBasisRefreshDelegate(void) = default;
    virtual ~ObjectControllerBasisRefreshDelegate(void) = default;

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
    virtual void ControllerIsRefreshing(ObjectControllerBasis &aController, const uint8_t &aPercentComplete) = 0;

    /**
     *  @brief
     *    Delegation from a controller that the controller is
     *    done refreshing.
     *
     *  @param[in]  aController       A reference to the controller
     *                                that issued the delegation.
     *
     */
    virtual void ControllerDidRefresh(ObjectControllerBasis &aController) = 0;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTOBJECTCONTROLLERBASISREFRESHDELEGATE_HPP
