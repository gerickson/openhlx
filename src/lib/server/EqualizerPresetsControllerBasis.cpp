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
 *      equalizer presets controller, in a server.
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

// Class-scoped Command Request Regular Expression Data

/**
 *  Class-scoped server equalizer preset decrease equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::EqualizerPresets::DecreaseBandRequest  EqualizerPresetsControllerBasis::kDecreaseBandRequest;

/**
 *  Class-scoped server equalizer preset increase equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::EqualizerPresets::IncreaseBandRequest  EqualizerPresetsControllerBasis::kIncreaseBandRequest;

/**
 *  Class-scoped server equalizer preset query command request regular
 *  expression.
 *
 */
Server::Command::EqualizerPresets::QueryRequest         EqualizerPresetsControllerBasis::kQueryRequest;

/**
 *  Class-scoped server equalizer preset set equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::EqualizerPresets::SetBandRequest       EqualizerPresetsControllerBasis::kSetBandRequest;

/**
 *  Class-scoped server zone set name command request regular
 *  expression.
 *
 */
Server::Command::EqualizerPresets::SetNameRequest       EqualizerPresetsControllerBasis::kSetNameRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the equalizer presets controller with the
 *  specified equalizer presets collection model and the maximum
 *  number of allowed equalizer presets.
 *
 *  @param[in]  aEqualizerPresetsModel  A mutable reference to the
 *                                      equalizer presets collection
 *                                      model to construct the
 *                                      controller with. This is
 *                                      retained by a weak pointer
 *                                      reference and, consequently,
 *                                      must remain in scope for the
 *                                      lifetime of the controller.
 *  @param[in]  aEqualizerPresetsMax    An immutable reference to the
 *                                      maximum number of allowed
 *                                      equalizer presets managed by
 *                                      the controller.
 *
 */
EqualizerPresetsControllerBasis :: EqualizerPresetsControllerBasis(Model::EqualizerPresetsModel &aEqualizerPresetsModel,
                                                                   const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetsMax) :
    Server::ObjectControllerBasis(),
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
EqualizerPresetsControllerBasis :: Init(CommandManager &aCommandManager)
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
EqualizerPresetsControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

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

/**
 *  @brief
 *    Handle and generate the server command response for an equalizer
 *    preset query request of all equalizer presets.
 *
 *  This handles and generates the server command response for an
 *  equalizer preset query request of all equalizer presets.
 *
 *  @param[in,out]  aBuffer  A mutable reference to the shared
 *                           pointer into which the response is to be
 *                           generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer presets model has
 *                                  not been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If an equalizer preset identifier is
 *                                  smaller or larger than supported.
 *
 */
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

/**
 *  @brief
 *    Handle and generate the server command response for an equalizer
 *    preset query request of a specific equalizer preset.
 *
 *  This handles and generates the server command response for an
 *  equalizer preset query request of a specific equalizer preset.
 *
 *  @param[in]      aEqualizerPresetIdentifier  An immutable reference
 *                                              to the identifier of
 *                                              the equalizer preset
 *                                              queried.
 *  @param[in,out]  aBuffer                     A mutable reference to
 *                                              the shared pointer into
 *                                              which the response is
 *                                              to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer presets model has
 *                                  not been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If an equalizer preset identifier is
 *                                  smaller or larger than supported.
 *
 */
Status
EqualizerPresetsControllerBasis :: HandleQueryReceived(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                                       Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
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

    // Name Response

    lRetval = lEqualizerPresetModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aEqualizerPresetIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Band Response

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

/**
 *  @brief
 *    Handle and generate the server command response into the
 *    specified buffer with the specified equalizer band identifier at
 *    the provided level within the specified equalizer preset
 *    identifier.
 *
 *  @param[in]      aEqualizerPresetIdentifier  A reference to the
 *                                              specific equalizer
 *                                              preset identifier
 *                                              for which the response
 *                                              buffer is to be
 *                                              formed.
 *  @param[in]      aEqualizerBandIdentifier    A reference to the
 *                                              specific equalizer
 *                                              band identifier
 *                                              for which the response
 *                                              buffer is to be formed.
 *  @param[in]      aBandLevel                  An immutable reference
 *                                              to the equalizer band
 *                                              level for which the
 *                                              response buffer is to
 *                                              be formed.
 *  @param[in,out]  aBuffer                     A mutable reference to
 *                                              the shared pointer
 *                                              into which the
 *                                              response is to be
 *                                              generated.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
/* static */ Status
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
