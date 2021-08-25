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
 *      This file implements an object for...
 *
 */

#include "HLXServerControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Application
{

// MARK: Server Controller Basis

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ControllerBasis :: ControllerBasis(Server::Application::Controller &aController) :
    FooType(),
    mController(aController)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the server controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  server controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the server controller was already
 *                                    added as a delegate to the
 *                                    connection manager, command
 *                                    manager, or child controllers.
 *
 */
Status
ControllerBasis :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;
    Controllers::iterator begin, end;

    lRetval = FooType::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Application

}; // namespace Server

}; // namespace HLX
