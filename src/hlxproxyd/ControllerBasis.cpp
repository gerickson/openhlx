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
 *      This file implements a base object for....
 *
 */

#include "ControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

ControllerBasis :: ControllerBasis(void)
{
    return;
}

ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

Status
ControllerBasis :: Init(Client::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = Init(aCommandManager, kTimeoutDefault);

    return (lRetval);
}

Status
ControllerBasis :: Init(Client::CommandManager &aCommandManager, const Common::Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    (void)aCommandManager;
    (void)aTimeout;

    return (lRetval);
}

}; // namespace Proxy

}; // namespace HLX
