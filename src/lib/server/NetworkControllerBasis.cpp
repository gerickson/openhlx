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
 *      This file implements a derivable object for realizing a HLX
 *      Ethernet network interface controller, in a server.
 *
 */

#include "NetworkControllerBasis.hpp"

#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Class-scoped Command Request Regular Expression Data

/**
 *  Class-scoped server network interface query command request regular
 *  expression.
 *
 */
Server::Command::Network::QueryRequest  NetworkControllerBasis::kQueryRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the network interface controller with the
 *  specified network interface model.
 *
 *  @param[in]  aNetworkModel  A mutable reference to the network
 *                             interface model to construct the
 *                             controller with. This is retained by a
 *                             weak pointer reference and,
 *                             consequently, must remain in scope for
 *                             the lifetime of the controller.
 *
 */
NetworkControllerBasis :: NetworkControllerBasis(Model::NetworkModel &aNetworkModel) :
    Server::ObjectControllerBasis(),
    mNetworkModel(aNetworkModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
NetworkControllerBasis :: ~NetworkControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NetworkControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
NetworkControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

// MARK: Observation (Query) Command Request Class (Static) Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a network
 *    interface query request.
 *
 *  This handles and generates the server command response for an
 *  network interface query request.
 *
 *  @param[in]      aInputBuffer   A pointer to a null-terminated
 *                                 C string containing the content to
 *                                 place into the response buffer.
 *  @param[in,out]  aOutputBuffer  A mutable reference to the shared
 *                                 pointer into which the response is
 *                                 to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the network interface model has
 *                                  not been completely and successfully
 *                                  initialized.
 *
 */
Status
NetworkControllerBasis :: HandleQueryReceived(const char *aInputBuffer, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
{
    const uint8_t *  lBuffer;
    size_t           lSize;
    Status           lRetval;


    lBuffer = reinterpret_cast<const uint8_t *>(aInputBuffer);
    lSize = strlen(aInputBuffer);

    lRetval = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

}; // namespace Server

}; // namespace HLX
