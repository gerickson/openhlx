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
 *      This file implements a derivable object for all server-side
 *      HLX controllers.
 *
 */

#include "ObjectControllerBasis.hpp"

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
ObjectControllerBasis :: ObjectControllerBasis(void) :
    mCommandManager(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ObjectControllerBasis :: ~ObjectControllerBasis(void)
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
ObjectControllerBasis :: Init(CommandManager &aCommandManager)
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
 *  @param[in]  aContext              The context to register
 *                                    which will be passed
 *                                    back via the handler.
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
ObjectControllerBasis :: DoRequestHandlers(const RequestHandlerBasis *aFirstRequestHandler,
                                     const RequestHandlerBasis *aLastRequestHandler,
                                     void *aContext,
                                     const bool &aRegister)
{
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

/**
 *  @brief
 *    Send a server command response to all connected clients.
 *
 *  This attempts to send a server command response for a
 *  previously-received command request to all connected (and,
 *  consequently, subscribed) clients.
 *
 *  @param[in]  aBuffer  An immutable shared pointer to the
 *                       buffer containing the command response to
 *                       send.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the controller has not been
 *                                  successfully initialized.
 *
 */
Status
ObjectControllerBasis :: SendResponse(ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status  lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendResponse(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command response to the specified connected
 *    client.
 *
 *  This attempts to send a server command response for a
 *  previously-received command request from the specified connected
 *  client.
 *
 *  @param[in]  aConnection  A mutable reference to the connection
 *                           over which to to send the response.
 *  @param[in]  aBuffer      An immutable shared pointer to the
 *                           buffer containing the command response to
 *                           send.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the controller has not been
 *                                  successfully initialized.
 *
 */
Status
ObjectControllerBasis :: SendResponse(ConnectionBasis &aConnection, ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status  lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command error response to the specified connected
 *    client.
 *
 *  This attempts to send a server command error response for a
 *  previously-received command request from the specified connected
 *  client.
 *
 *  @param[in]  aConnection  A mutable reference to the connection
 *                           over which to to send the error response.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the controller has not been
 *                                  successfully initialized.
 *  @retval  -ENOMEM                If resources for the error response
 *                                  could not be allocated.
 *
 */
Status
ObjectControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendErrorResponse(aConnection);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command error response to the specified connected
 *    client using the provided buffer.
 *
 *  This attempts to send a server command error response for a
 *  previously-received command request from the specified connected
 *  client using the provided buffer.
 *
 *  @param[in]      aConnection  A mutable reference to the connection
 *                               over which to to send the error response.
 *  @param[in,out]  aBuffer      A reference to a shared pointer to the
 *                               buffer in which to put the error
 *                               response.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the controller has not been
 *                                  successfully initialized.
 *
 */
Status
ObjectControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection,
                                           Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
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
