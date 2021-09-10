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
 *      physical front panel controller, in a server.
 *
 */

#include "FrontPanelControllerBasis.hpp"

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
 *  Class-scoped server front panel query command request regular
 *  expression.
 *
 */
Server::Command::FrontPanel::QueryRequest          FrontPanelControllerBasis::kQueryRequest;

/**
 *  Class-scoped server front panel set brightness level command
 *  request regular expression.
 *
 */
Server::Command::FrontPanel::SetBrightnessRequest  FrontPanelControllerBasis::kSetBrightnessRequest;

/**
 *  Class-scoped server front panel set locked state command request
 *  regular expression.
 *
 */
Server::Command::FrontPanel::SetLockedRequest      FrontPanelControllerBasis::kSetLockedRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the front panels controller with the specified
 *  front panel model.
 *
 *  @param[in]  aFrontPanelModel  A mutable reference to the front
 *                                panel model to construct the
 *                                controller with. This is retained by
 *                                a weak pointer reference and,
 *                                consequently, must remain in scope
 *                                for the lifetime of the controller.
 *
 */
FrontPanelControllerBasis :: FrontPanelControllerBasis(Model::FrontPanelModel &aFrontPanelModel) :
    Server::ObjectControllerBasis(),
    mFrontPanelModel(aFrontPanelModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
FrontPanelControllerBasis :: ~FrontPanelControllerBasis(void)
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
FrontPanelControllerBasis :: Init(CommandManager &aCommandManager)
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
FrontPanelControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetBrightnessRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetLockedRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a front panel
 *    query request.
 *
 *  This handles and generates the server command response for an
 *  front panel query request.
 *
 *  @param[in,out]  aBuffer  A mutable reference to the shared
 *                           pointer into which the response is to be
 *                           generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the front panels model has
 *                                  not been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If an front panel identifier is
 *                                  smaller or larger than supported.
 *
 */
Status
FrontPanelControllerBasis :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    FrontPanelModel::BrightnessType          lBrightness;
    FrontPanelModel::LockedType              lLocked;
    Status                                   lRetval;


    lRetval = mFrontPanelModel.GetBrightness(lBrightness);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleBrightnessResponse(lBrightness, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mFrontPanelModel.GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleLockedResponse(lLocked, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

Status
FrontPanelControllerBasis :: HandleBrightnessResponse(const FrontPanelModel::BrightnessType &aBrightness, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::BrightnessResponse  lBrightnessResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lBrightnessResponse.Init(aBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lBrightnessResponse.GetBuffer();
    lSize = lBrightnessResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

Status
FrontPanelControllerBasis :: HandleLockedResponse(const FrontPanelModel::LockedType &aLocked, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::LockedResponse  lLockedResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    lStatus = lLockedResponse.Init(aLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lLockedResponse.GetBuffer();
    lSize = lLockedResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

}; // namespace Server

}; // namespace HLX
