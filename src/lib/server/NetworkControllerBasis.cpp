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

// Request data

Server::Command::Network::QueryRequest  NetworkControllerBasis::kQueryRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkControllerBasis :: NetworkControllerBasis(Model::NetworkModel &aNetworkModel) :
    Server::ControllerBasis(),
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

Status
NetworkControllerBasis :: Init(CommandManager &aCommandManager)
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
NetworkControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
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
