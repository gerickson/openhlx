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

// Request data

Server::Command::Infrared::QueryRequest        InfraredControllerBasis::kQueryRequest;
Server::Command::Infrared::SetDisabledRequest  InfraredControllerBasis::kSetDisabledRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
InfraredControllerBasis :: InfraredControllerBasis(Model::InfraredModel &aInfraredModel) :
    Server::ControllerBasis(),
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

Status
InfraredControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
InfraredControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
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
