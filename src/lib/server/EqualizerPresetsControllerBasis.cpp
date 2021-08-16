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

#include "EqualizerPresetsControllerBasis.hpp"

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

Server::Command::EqualizerPresets::DecreaseBandRequest  EqualizerPresetsControllerBasis::kDecreaseBandRequest;
Server::Command::EqualizerPresets::IncreaseBandRequest  EqualizerPresetsControllerBasis::kIncreaseBandRequest;
Server::Command::EqualizerPresets::QueryRequest         EqualizerPresetsControllerBasis::kQueryRequest;
Server::Command::EqualizerPresets::SetBandRequest       EqualizerPresetsControllerBasis::kSetBandRequest;
Server::Command::EqualizerPresets::SetNameRequest       EqualizerPresetsControllerBasis::kSetNameRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerPresetsControllerBasis :: EqualizerPresetsControllerBasis(Model::EqualizerPresetsModel &aEqualizerPresetsModel,
                                                                   const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetsMax) :
    mEqualizerPresetsModel(aEqualizerPresetsModel),
    mEqualizerPresetsMax(aEqualizerPresetsMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
EqualizerPresetsControllerBasis :: ~EqualizerPresetsControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

Status
EqualizerPresetsControllerBasis :: Init(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
EqualizerPresetsControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = kDecreaseBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
