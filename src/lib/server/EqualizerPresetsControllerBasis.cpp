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
using namespace HLX::Model;
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

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

Status
EqualizerPresetsControllerBasis :: HandleQueryReceived(ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;

    for (auto lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin; lEqualizerPresetIdentifier <= mEqualizerPresetsMax; lEqualizerPresetIdentifier++)
    {
        lRetval = HandleQueryReceived(lEqualizerPresetIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
EqualizerPresetsControllerBasis :: HandleQueryReceived(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                                       ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const EqualizerPresetModel *                     lEqualizerPresetModel;
    const char *                                     lName;
    Server::Command::EqualizerPresets::NameResponse  lNameResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    EqualizerBandModel::IdentifierType               lEqualizerBandIdentifier;
    const EqualizerBandModel *                       lEqualizerBandModel;
    EqualizerBandModel::LevelType                    lLevel;
    Server::Command::EqualizerPresets::BandResponse  lBandResponse;
    Status                                           lRetval;


    lRetval = mEqualizerPresetsModel.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lEqualizerPresetModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aEqualizerPresetIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        lRetval = lEqualizerPresetModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lEqualizerBandModel->GetLevel(lLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lBandResponse.Init(aEqualizerPresetIdentifier, lEqualizerBandIdentifier, lLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBuffer = lBandResponse.GetBuffer();
        lSize = lBandResponse.GetSize();

        lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

Status
EqualizerPresetsControllerBasis :: HandleBandResponse(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                                      const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                      const EqualizerBandModel::LevelType &aBandLevel,
                                                      ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::EqualizerPresets::BandResponse  lBandResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lRetval;

    lRetval = lBandResponse.Init(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBandResponse.GetBuffer();
    lSize = lBandResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
