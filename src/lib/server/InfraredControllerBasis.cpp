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
 *      infrared remote control interface controller, in a server.
 *
 */

#include "InfraredControllerBasis.hpp"

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

//!< Class-scoped server infrared query command request regular expression.
Server::Command::Infrared::QueryRequest        InfraredControllerBasis::kQueryRequest;

//!< Class-scoped server infrared set disabled state command request
//!< regular expression.
Server::Command::Infrared::SetDisabledRequest  InfraredControllerBasis::kSetDisabledRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the infrared controller with the specified
 *  infrared model.
 *
 *  @param[in]  aInfraredModel  A mutable reference to the infrared
 *                              model to construct the controller
 *                              with. This is retained by a weak
 *                              pointer reference and, consequently,
 *                              must remain in scope for the lifetime
 *                              of the controller.
 *
 */
InfraredControllerBasis :: InfraredControllerBasis(Model::InfraredModel &aInfraredModel) :
    Server::ObjectControllerBasis(),
    mInfraredModel(aInfraredModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
InfraredControllerBasis :: ~InfraredControllerBasis(void)
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
InfraredControllerBasis :: Init(CommandManager &aCommandManager)
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

// MARK: Implementation

Status
InfraredControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetDisabledRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for an infrared
 *    query request.
 *
 *  This handles and generates the server command response for an
 *  infrared query request.
 *
 *  @param[in,out]  aBuffer  A mutable reference to the shared
 *                           pointer into which the response is to be
 *                           generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the infrared model has
 *                                  not been completely and successfully
 *                                  initialized.
 *
 */
Status
InfraredControllerBasis :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    InfraredModel::DisabledType  lDisabled;
    Status                       lRetval;


    lRetval = mInfraredModel.GetDisabled(lDisabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleDisabledResponse(lDisabled, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

Status
InfraredControllerBasis :: HandleDisabledResponse(const InfraredModel::DisabledType &aDisabled, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Infrared::DisabledResponse  lDisabledResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    lStatus = lDisabledResponse.Init(aDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lDisabledResponse.GetBuffer();
    lSize = lDisabledResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

}; // namespace Server

}; // namespace HLX
