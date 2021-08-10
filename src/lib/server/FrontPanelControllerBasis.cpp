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
FrontPanelControllerBasis :: FrontPanelControllerBasis(void)
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

}; // namespace Server

}; // namespace HLX