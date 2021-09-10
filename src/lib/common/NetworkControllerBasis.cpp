/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements...
 *
 */

#include "NetworkControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the network interface controller basis.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NetworkControllerBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mNetworkModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observers

/**
 *  @brief
 *    Return the data model for the controller.
 *
 *  @returns
 *    An immutable reference to the data model for the controller.
 *
 */
Model::NetworkModel &
NetworkControllerBasis :: GetModel(void)
{
    return (mNetworkModel);
}

/**
 *  @brief
 *    Return the data model for the controller.
 *
 *  @returns
 *    A mutable reference to the data model for the controller.
 *
 */
const Model::NetworkModel &
NetworkControllerBasis :: GetModel(void) const
{
    return (mNetworkModel);
}

}; // namespace Common

}; // namespace HLX
