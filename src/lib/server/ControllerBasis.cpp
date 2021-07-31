/*
 *    Copyright (c) 2020-2021 Grant Erickson
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

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

ControllerBasis :: ControllerBasis(void) :
    mCommandManager(nullptr)
{
    return;
}

ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

Status
ControllerBasis :: Init(CommandManager &aCommandManager)
{
    Status lRetval = kStatus_Success;


    lRetval = Init(aCommandManager, kTimeoutDefault);

    return (lRetval);
}

Status
ControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mCommandManager = &aCommandManager;

    return (lRetval);
}

Status
ControllerBasis :: DoRequestHandlers(const RequestHandlerBasis *aFirstRequestHandler, const RequestHandlerBasis *aLastRequestHandler, const bool &aRegister)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr,      done, lRetval = kError_NotInitialized);
    nlREQUIRE_ACTION(aFirstRequestHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLastRequestHandler  != nullptr, done, lRetval = -EINVAL);

    while (aFirstRequestHandler != aLastRequestHandler)
    {
        if (aRegister)
        {
            lRetval = mCommandManager->RegisterRequestHandler(aFirstRequestHandler->mRequest, this, aFirstRequestHandler->mOnRequestReceivedHandler);
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        else
        {
            lRetval = mCommandManager->UnregisterRequestHandler(aFirstRequestHandler->mRequest, this);
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        ++aFirstRequestHandler;
    }

 done:
    return (lRetval);
}

Status
ControllerBasis :: SendResponse(ConnectionBasis &aConnection, ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendErrorResponse(aConnection);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendErrorResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
