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
 *      This file implements a base object for all server-side HLX
 *      controllers.
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

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ControllerBasis :: ControllerBasis(void) :
    mCommandManager(nullptr)
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

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the controller basis with a default timeout.
 *
 *  @param[in]  aCommandManager  A mutable reference to the command
 *                               manager to initialize this controller
 *                               basis with as the controller
 *                               basis is to be a delegate of the
 *                               command manager.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    mCommandManager = &aCommandManager;

    return (lRetval);
}

/**
 *  @brief
 *    Register or unregister notification handlers.
 *
 *  This registers or unregisters the unsolicited server command
 *  request handlers that this controller is interested in and will
 *  handle on behalf of the server.
 *
 *  @param[in]  aFirstRequestHandler  An iterator to the first
 *                                    command request handler to
 *                                    register or unregister.
 *  @param[in]  aLastRequestHandler   An iterator to the last
 *                                    comamnd request handler to
 *                                    register or unregister.
 *  @param[in]  aRegister             Indicates whether to
 *                                    register (true) or unregister
 *                                    (false) the handlers.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If either of the handler iterators
 *                                        was null.
 *  @retval  -EEXIST                      If a registration already exists.
 *  @retval  -ENOENT                      If there was no such handler
 *                                        registration to unregister.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ControllerBasis :: DoRequestHandlers(const RequestHandlerBasis *aFirstRequestHandler,
                                     const RequestHandlerBasis *aLastRequestHandler,
                                     void *aContext,
                                     const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr,      done, lRetval = kError_NotInitialized);
    nlREQUIRE_ACTION(aFirstRequestHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLastRequestHandler  != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aContext != nullptr, done, lRetval = -EINVAL);

    while (aFirstRequestHandler != aLastRequestHandler)
    {
        if (aRegister)
        {
            lRetval = mCommandManager->RegisterRequestHandler(aFirstRequestHandler->mRequest, aContext, aFirstRequestHandler->mOnRequestReceivedHandler);
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        else
        {
            lRetval = mCommandManager->UnregisterRequestHandler(aFirstRequestHandler->mRequest, aContext);
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        ++aFirstRequestHandler;
    }

done:
    return (lRetval);
}

Status
ControllerBasis :: SendResponse(ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendResponse(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

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
