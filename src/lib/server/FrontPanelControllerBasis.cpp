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

// Request data

Server::Command::FrontPanel::QueryRequest          FrontPanelControllerBasis::kQueryRequest;
Server::Command::FrontPanel::SetBrightnessRequest  FrontPanelControllerBasis::kSetBrightnessRequest;
Server::Command::FrontPanel::SetLockedRequest      FrontPanelControllerBasis::kSetLockedRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FrontPanelControllerBasis :: FrontPanelControllerBasis(Model::FrontPanelModel &aFrontPanelModel) :
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

Status
FrontPanelControllerBasis :: Init(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
FrontPanelControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

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
