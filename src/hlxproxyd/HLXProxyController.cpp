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
 *      This file implements an object for effecting a HLX proxy
 *      controller.
 *
 */

#include "HLXProxyController.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

Controller :: Controller(void) :
    mDelegate(nullptr),
    mDelegateContext(nullptr)
{
    return;
}

Controller :: ~Controller(void)
{
    DeclareScopedFunctionTracer(lTracer);
}

Status
Controller :: Init(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

Status
Controller :: Connect(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    return (lRetval);
}

Status
Controller :: Listen(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    return (lRetval);
}

ControllerDelegate *Controller :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
Controller :: SetDelegate(ControllerDelegate *aDelegate, void *aContext)
{
    Status lRetval = kStatus_Success;

    if ((aDelegate == mDelegate) && (aContext == mDelegateContext))
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mDelegate        = aDelegate;
    mDelegateContext = aContext;

 done:
    return (lRetval);
}

}; // namespace Proxy

}; // namespace HLX
