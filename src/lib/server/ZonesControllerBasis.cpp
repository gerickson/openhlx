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
 *      zones controller, in a server.
 *
 */

#include "ZonesControllerBasis.hpp"

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
 *  Class-scoped server zone adjust balance command request regular
 *  expression.
 *
 */
Server::Command::Zones::AdjustBalanceRequest          ZonesControllerBasis::kAdjustBalanceRequest;

/**
 *  Class-scoped server zone tone equalizer decrease bass level
 *  command request regular expression.
 *
 */
Server::Command::Zones::DecreaseBassRequest           ZonesControllerBasis::kDecreaseBassRequest;

/**
 *  Class-scoped server zone tone equalizer increase bass level
 *  command request regular expression.
 *
 */
Server::Command::Zones::IncreaseBassRequest           ZonesControllerBasis::kIncreaseBassRequest;

/**
 *  Class-scoped server zone tone equalizer decrease treble level
 *  command request regular expression.
 *
 */
Server::Command::Zones::DecreaseTrebleRequest         ZonesControllerBasis::kDecreaseTrebleRequest;

/**
 *  Class-scoped server zone tone equalizer increase treble level
 *  command request regular expression.
 *
 */
Server::Command::Zones::IncreaseTrebleRequest         ZonesControllerBasis::kIncreaseTrebleRequest;

/**
 *  Class-scoped server zone equalizer decrease equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::Zones::DecreaseEqualizerBandRequest  ZonesControllerBasis::kDecreaseEqualizerBandRequest;

/**
 *  Class-scoped server zone equalizer increase equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::Zones::IncreaseEqualizerBandRequest  ZonesControllerBasis::kIncreaseEqualizerBandRequest;

/**
 *  Class-scoped server zone decrease volume level command request regular
 *  expression.
 *
 */
Server::Command::Zones::DecreaseVolumeRequest         ZonesControllerBasis::kDecreaseVolumeRequest;

/**
 *  Class-scoped server zone increase volume level command request regular
 *  expression.
 *
 */
Server::Command::Zones::IncreaseVolumeRequest         ZonesControllerBasis::kIncreaseVolumeRequest;

/**
 *  Class-scoped server zone volume mute state command request regular
 *  expression.
 *
 */
Server::Command::Zones::MuteRequest                   ZonesControllerBasis::kMuteRequest;

/**
 *  Class-scoped server zone query command request regular expression.
 *
 */
Server::Command::Zones::QueryRequest                  ZonesControllerBasis::kQueryRequest;

/**
 *  Class-scoped server zone query volume mute state command request regular
 *  expression.
 *
 */
Server::Command::Zones::QueryMuteRequest              ZonesControllerBasis::kQueryMuteRequest;

/**
 *  Class-scoped server zone query source (input) command request regular
 *  expression.
 *
 */
Server::Command::Zones::QuerySourceRequest            ZonesControllerBasis::kQuerySourceRequest;

/**
 *  Class-scoped server zone query volume level command request regular
 *  expression.
 *
 */
Server::Command::Zones::QueryVolumeRequest            ZonesControllerBasis::kQueryVolumeRequest;

/**
 *  Class-scoped server zone set stereophonic channel balance command
 *  request regular expression.
 *
 */
Server::Command::Zones::SetBalanceRequest             ZonesControllerBasis::kSetBalanceRequest;

/**
 *  Class-scoped server zone equalizer set equalizer band level
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetEqualizerBandRequest       ZonesControllerBasis::kSetEqualizerBandRequest;

/**
 *  Class-scoped server zone preset equalizer set equalizer preset
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetEqualizerPresetRequest     ZonesControllerBasis::kSetEqualizerPresetRequest;

/**
 *  Class-scoped server zone highpass filter set crossover frequency
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetHighpassCrossoverRequest   ZonesControllerBasis::kSetHighpassCrossoverRequest;

/**
 *  Class-scoped server zone lowpass filter set crossover frequency
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetLowpassCrossoverRequest    ZonesControllerBasis::kSetLowpassCrossoverRequest;

/**
 *  Class-scoped server zone set name command request regular
 *  expression.
 *
 */
Server::Command::Zones::SetNameRequest                ZonesControllerBasis::kSetNameRequest;

/**
 *  Class-scoped server zone set sound (that is, equalizer) mode
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetSoundModeRequest           ZonesControllerBasis::kSetSoundModeRequest;

/**
 *  Class-scoped server zone set source (input) command request
 *  regular expression.
 *
 */
Server::Command::Zones::SetSourceRequest              ZonesControllerBasis::kSetSourceRequest;

/**
 *  Class-scoped server all zones set source (input) command request
 *  regular expression.
 *
 */
Server::Command::Zones::SetSourceAllRequest           ZonesControllerBasis::kSetSourceAllRequest;

/**
 *  Class-scoped server zone tone equalizer set tone (bass/treble)
 *  command request regular expression.
 *
 */
Server::Command::Zones::SetToneRequest                ZonesControllerBasis::kSetToneRequest;

/**
 *  Class-scoped server zone set volume level command request regular
 *  expression.
 *
 */
Server::Command::Zones::SetVolumeRequest              ZonesControllerBasis::kSetVolumeRequest;

/**
 *  Class-scoped server all zones set volume level command request regular
 *  expression.
 *
 */
Server::Command::Zones::SetVolumeAllRequest           ZonesControllerBasis::kSetVolumeAllRequest;

/**
 *  Class-scoped server zone query volume fixed/locked state command
 *  request regular expression.
 *
 */
Server::Command::Zones::SetVolumeFixedRequest         ZonesControllerBasis::kSetVolumeFixedRequest;

/**
 *  Class-scoped server zone toggle volume mute state command request regular
 *  expression.
 *
 */
Server::Command::Zones::ToggleMuteRequest             ZonesControllerBasis::kToggleMuteRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the zones controller with the specified zones
 *  collection model and the maximum number of allowed zones.
 *
 *  @param[in]  aZonesModel  A mutable reference to the zones
 *                           collection model to construct the
 *                           controller with. This is retained by a
 *                           weak pointer reference and, consequently,
 *                           must remain in scope for the lifetime of
 *                           the controller.
 *  @param[in]  aZonesMax    An immutable reference to the maximum
 *                           number of allowed zones managed by the
 *                           controller.
 *
 */
ZonesControllerBasis :: ZonesControllerBasis(Model::ZonesModel &aZonesModel,
                                             const Model::ZoneModel::IdentifierType &aZonesMax) :
    Server::ObjectControllerBasis(),
    mZonesModel(aZonesModel),
    mZonesMax(aZonesMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesControllerBasis :: ~ZonesControllerBasis(void)
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
ZonesControllerBasis :: Init(CommandManager &aCommandManager)
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
ZonesControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kAdjustBalanceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseBassRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseBassRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseTrebleRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseTrebleRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQuerySourceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetBalanceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetEqualizerBandRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetEqualizerPresetRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetHighpassCrossoverRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetLowpassCrossoverRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSoundModeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSourceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSourceAllRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetToneRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeAllRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeFixedRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kToggleMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    request of all zones.
 *
 *  This handles and generates the server command response for a zone
 *  query request of all zones.
 *
 *  @param[in]      aIsConfiguration  An immutable reference to a Boolean
 *                                    indicating whether the query
 *                                    request is coming from a query
 *                                    current configuration (true) or
 *                                    a zone query (false) request.
 *  @param[in,out]  aBuffer           A mutable reference to the shared
 *                                    pointer into which the response is
 *                                    to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has
 *                                  not been completely and successfully
 *                                  initialized.
 *
 */
Status
ZonesControllerBasis :: HandleQueryReceived(const bool &aIsConfiguration,
                                            Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;


    for (auto lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= mZonesMax; lZoneIdentifier++)
    {
        lRetval = HandleQueryReceived(aIsConfiguration, lZoneIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query request of a specific zone.
 *
 *  @param[in]      aIsConfiguration  An immutable reference to a Boolean
 *                                    indicating whether the query
 *                                    request is coming from a query
 *                                    current configuration (true) or
 *                                    a zone query (false) request.
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not been
 *                                  completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a zone identifier is smaller
 *                                  or larger than supported.
 *
 */
Status
ZonesControllerBasis :: HandleQueryReceived(const bool &aIsConfiguration,
                                            const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                            ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const ZoneModel *                        lZoneModel;
    const char *                             lName;
    Server::Command::Zones::NameResponse     lNameResponse;
    BalanceModel::BalanceType                lBalance;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    lRetval = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Name Response

    lRetval = lZoneModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aZoneIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Source Response

    lRetval = HandleQuerySourceReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Volume Response

    lRetval = HandleQueryVolumeReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Volume Fixed Response (include if for configuration)

    if (aIsConfiguration)
    {
        lRetval = HandleQueryVolumeFixed(aZoneIdentifier, *lZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    // Mute Response

    lRetval = HandleQueryMuteReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Sound Mode Response

    lRetval = HandleQuerySoundMode(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Balance Response

    lRetval = lZoneModel->GetBalance(lBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lBalanceResponse.Init(aZoneIdentifier, lBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBalanceResponse.GetBuffer();
    lSize = lBalanceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    volume mute state request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query volume mute state request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not been
 *                                  completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a zone identifier is smaller
 *                                  or larger than supported.
 *
 */
Status
ZonesControllerBasis :: HandleQueryMuteReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const ZoneModel *  lZoneModel;
    Status             lRetval;


    lRetval = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleQueryMuteReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    source (input) request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query source (input) request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not been
 *                                  completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a zone identifier is smaller
 *                                  or larger than supported.
 *
 */
Status
ZonesControllerBasis :: HandleQuerySourceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const ZoneModel *  lZoneModel;
    Status             lRetval;


    lRetval = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleQuerySourceReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    volume level request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query volume level request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not been
 *                                  completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a zone identifier is smaller
 *                                  or larger than supported.
 *
 */
Status
ZonesControllerBasis :: HandleQueryVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const ZoneModel *  lZoneModel;
    Status             lRetval;


    lRetval = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleQueryVolumeReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    equalizer preset identifier request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query equalizer preset identifier request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryEqualizerPreset(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                   const ZoneModel &aZoneModel,
                                                   ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lRetval;

    lRetval = aZoneModel.GetEqualizerPreset(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleEqualizerPresetResponse(aZoneIdentifier, lEqualizerPresetIdentifier, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    highpass filter crossover frequency request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query highpass filter crossover frequency request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryHighpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                     const ZoneModel &aZoneModel,
                                                     ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    CrossoverModel::FrequencyType      lHighpassFrequency;
    Status                             lRetval;

    lRetval = aZoneModel.GetHighpassFrequency(lHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleHighpassCrossoverResponse(aZoneIdentifier, lHighpassFrequency, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    lowpass filter crossover frequency request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query lowpass filter crossover frequency request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryLowpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    const ZoneModel &aZoneModel,
                                                    ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    CrossoverModel::FrequencyType      lLowpassFrequency;
    Status                             lRetval;

    lRetval = aZoneModel.GetLowpassFrequency(lLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleLowpassCrossoverResponse(aZoneIdentifier, lLowpassFrequency, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    volume mute state request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query volume mute state request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryMuteReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                const Model::ZoneModel &aZoneModel,
                                                Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    VolumeModel::MuteType              lMute;
    Status                             lRetval;


    lRetval = aZoneModel.GetMute(lMute);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleMuteResponse(aZoneIdentifier, lMute, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    sound (equalizer) mode request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query sound (equalizer) mode request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQuerySoundMode(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                             const ZoneModel &aZoneModel,
                                             ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    SoundModel::SoundMode              lSoundMode;
    Status                             lRetval;

    lRetval = aZoneModel.GetSoundMode(lSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleSoundModeResponse(aZoneIdentifier, lSoundMode, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    switch (lSoundMode)
    {

    case SoundModel::kSoundModeZoneEqualizer:
        lRetval = HandleQueryZoneEqualizer(aZoneIdentifier, aZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModePresetEqualizer:
        lRetval = HandleQueryEqualizerPreset(aZoneIdentifier, aZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeTone:
        lRetval = HandleQueryTone(aZoneIdentifier, aZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeLowpass:
        lRetval = HandleQueryLowpassCrossover(aZoneIdentifier, aZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeHighpass:
        lRetval = HandleQueryHighpassCrossover(aZoneIdentifier, aZoneModel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    source (input) request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query source (input) request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQuerySourceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  const Model::ZoneModel &aZoneModel,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    SourceModel::IdentifierType             lSourceIdentifier;
    Server::Command::Zones::SourceResponse  lSourceResponse;
    const uint8_t *                         lBuffer;
    size_t                                  lSize;
    Status                                  lRetval;


    lRetval = aZoneModel.GetSource(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lSourceResponse.Init(aZoneIdentifier, lSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lSourceResponse.GetBuffer();
    lSize = lSourceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    tone equalizer bass and treble levels request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query tone equalizer bass and treble levels request of a specific
 *  zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryTone(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                        const ZoneModel &aZoneModel,
                                        ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ToneModel::LevelType  lBass;
    ToneModel::LevelType  lTreble;
    Status                lRetval;

    lRetval = aZoneModel.GetTone(lBass, lTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleToneResponse(aZoneIdentifier, lBass, lTreble, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    volume level request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query volume level request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  const Model::ZoneModel &aZoneModel,
                                                  Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    VolumeModel::LevelType  lVolume;
    Status                  lRetval;


    lRetval = aZoneModel.GetVolume(lVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, lVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    volume fixed/locked state request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  query volume fixed/locked state request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryVolumeFixed(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                               const ZoneModel &aZoneModel,
                                               ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    VolumeModel::FixedType             lVolumeFixed;
    Status                             lRetval;


    lRetval = aZoneModel.GetVolumeFixed(lVolumeFixed);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeFixedResponse(aZoneIdentifier, lVolumeFixed, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone query
 *    zone equalizer band levels request of a specific zone.
 *
 *  This handles and generates the server command response for a zone
 *  zone equalizer band levels request of a specific zone.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference to the
 *                                    identifier of the zone queried.
 *  @param[in]      aZoneModel        An immutable reference to the
 *                                    zone model of the zone queried.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleQueryZoneEqualizer(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                 const ZoneModel &aZoneModel,
                                                 ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    const EqualizerBandModel *     lEqualizerBandModel;
    EqualizerBandModel::LevelType  lEqualizerBandLevel;
    Status                         lRetval;


    for (auto lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        lRetval = aZoneModel.GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lEqualizerBandModel->GetLevel(lEqualizerBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = HandleEqualizerBandResponse(aZoneIdentifier, lEqualizerBandIdentifier, lEqualizerBandLevel, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    equalizer band level request.
 *
 *  This handles and generates the server command response for a
 *  zone equalizer band level request.
 *
 *  @param[in]      aZoneIdentifier           An immutable reference
 *                                            for the zone for which
 *                                            the zone equalizer band
 *                                            level state was set.
 *  @param[in]      aEqualizerBandIdentifier  An immutable reference
 *                                            to the equalizer band on
 *                                            the zone equalizer that
 *                                            was set.
 *  @param[in]      aEqualizerBandLevel       An immutable reference to
 *                                            band level that was set.
 *  @param[in,out]  aBuffer                   A mutable reference to
 *                                            the shared pointer into
 *                                            which the response is to
 *                                            be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleEqualizerBandResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                    const EqualizerBandModel::LevelType &aEqualizerBandLevel,
                                                    ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::EqualizerBandResponse  lEqualizerBandResponse;
    const uint8_t *                                lBuffer;
    size_t                                         lSize;
    Status                                         lRetval;

    lRetval = lEqualizerBandResponse.Init(aZoneIdentifier, aEqualizerBandIdentifier, aEqualizerBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lEqualizerBandResponse.GetBuffer();
    lSize = lEqualizerBandResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    equalizer preset identifier request.
 *
 *  This handles and generates the server command response for a
 *  zone equalizer preset identifier request.
 *
 *  @param[in]      aZoneIdentifier             An immutable reference
 *                                              for the zone for which
 *                                              the zone equalizer
 *                                              preset identifier was
 *                                              set.
 *  @param[in]      aEqualizerPresetIdentifier  An immutable reference
 *                                              to the equalizer preset
 *                                              identifier that was set.
 *  @param[in,out]  aBuffer                     A mutable reference to
 *                                              the shared pointer
 *                                              into which the
 *                                              response is to be
 *                                              generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleEqualizerPresetResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                      const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                                      ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::EqualizerPresetResponse  lEqualizerPresetResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lRetval;

    lRetval = lEqualizerPresetResponse.Init(aZoneIdentifier, aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lEqualizerPresetResponse.GetBuffer();
    lSize = lEqualizerPresetResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    highpass filter crossover frequency request.
 *
 *  This handles and generates the server command response for a
 *  zone highpass filter crossover frequency request.
 *
 *  @param[in]      aZoneIdentifier     An immutable reference for the
 *                                      zone for which the highpass
 *                                      filter crossover frequency was
 *                                      set.
 *  @param[in]      aHighpassFrequency  An immutable reference to the
 *                                      highpass filter crossover
 *                                      frequency that was set.
 *  @param[in,out]  aBuffer             A mutable reference to the
 *                                      shared pointer into which the
 *                                      response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleHighpassCrossoverResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                        const CrossoverModel::FrequencyType &aHighpassFrequency,
                                                        ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::HighpassCrossoverResponse  lHighpassCrossoverResponse;
    const uint8_t *                                    lBuffer;
    size_t                                             lSize;
    Status                                             lRetval;

    lRetval = lHighpassCrossoverResponse.Init(aZoneIdentifier, aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lHighpassCrossoverResponse.GetBuffer();
    lSize = lHighpassCrossoverResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    lowpass filter crossover frequency request.
 *
 *  This handles and generates the server command response for a
 *  zone lowpass filter crossover frequency request.
 *
 *  @param[in]      aZoneIdentifier    An immutable reference for the
 *                                     zone for which the lowpass
 *                                     filter crossover frequency was
 *                                     set.
 *  @param[in]      aLowpassFrequency  An immutable reference to the
 *                                     lowpass filter crossover
 *                                     frequency that was set.
 *  @param[in,out]  aBuffer            A mutable reference to the
 *                                     shared pointer into which the
 *                                     response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleLowpassCrossoverResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                       const CrossoverModel::FrequencyType &aLowpassFrequency,
                                                       ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::LowpassCrossoverResponse  lLowpassCrossoverResponse;
    const uint8_t *                                   lBuffer;
    size_t                                            lSize;
    Status                                            lRetval;

    lRetval = lLowpassCrossoverResponse.Init(aZoneIdentifier, aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lLowpassCrossoverResponse.GetBuffer();
    lSize = lLowpassCrossoverResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    volume mute state request.
 *
 *  This handles and generates the server command response for a
 *  zone volume mute state request.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference for the
 *                                    zone for which the volume mute
 *                                    state was set.
 *  @param[in]      aMute             An immutable reference to the
 *                                    volume mute state that was set.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleMuteResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                           const VolumeModel::MuteType &aMute,
                                           ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::MuteResponse  lMuteResponse;
    const uint8_t *                       lBuffer;
    size_t                                lSize;
    Status                                lRetval;

    lRetval = lMuteResponse.Init(aZoneIdentifier, aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lMuteResponse.GetBuffer();
    lSize = lMuteResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    sound (equalizer) mode request.
 *
 *  This handles and generates the server command response for a
 *  zone sound (equalizer) request.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference for the
 *                                    zone for which the sound
 *                                    (equalizer) mode was set.
 *  @param[in]      aSoundMode        An immutable reference to the
 *                                    sound (equalizer) mode that was
 *                                    set.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleSoundModeResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                const SoundModel::SoundMode &aSoundMode,
                                                ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::SoundModeResponse  lSoundModeResponse;
    const uint8_t *                            lBuffer;
    size_t                                     lSize;
    Status                                     lRetval;


    lRetval = lSoundModeResponse.Init(aZoneIdentifier, aSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lSoundModeResponse.GetBuffer();
    lSize = lSoundModeResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    tone equalizer bass and treble levels request.
 *
 *  This handles and generates the server command response for a
 *  zone tone equalizer bass and treble levels request.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference for the
 *                                    zone for which the tone equalizer
 *                                    levels were set.
 *  @param[in]      aBass             An immutable reference to the
 *                                    tone equalizer bass level that
 *                                    was set.
 *  @param[in]      aTreble           An immutable reference to the tone
 *                                    equalizer treble level that was
 *                                    set.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleToneResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                           const Model::ToneModel::LevelType &aBass,
                                           Model::ToneModel::LevelType &aTreble,
                                           ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::ToneResponse  lToneResponse;
    const uint8_t *                       lBuffer;
    size_t                                lSize;
    Status                                lRetval;


    lRetval = lToneResponse.Init(aZoneIdentifier, aBass, aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lToneResponse.GetBuffer();
    lSize = lToneResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    volume level request.
 *
 *  This handles and generates the server command response for a
 *  zone volume level request.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference for the
 *                                    zone for which the volume level
 *                                    was set.
 *  @param[in]      aVolume           An immutable reference to the
 *                                    volume level that was set.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleVolumeResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                             const VolumeModel::LevelType &aVolume,
                                             ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::VolumeResponse  lVolumeResponse;
    const uint8_t *                         lBuffer;
    size_t                                  lSize;
    Status                                  lRetval;


    lRetval = lVolumeResponse.Init(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lVolumeResponse.GetBuffer();
    lSize = lVolumeResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a zone
 *    volume fixed/locked state request.
 *
 *  This handles and generates the server command response for a
 *  zone volume fixed/locked state request.
 *
 *  @param[in]      aZoneIdentifier   An immutable reference for the
 *                                    zone for which the volume fixed/
 *                                    locked state was set.
 *  @param[in]      aVolumeFixed      An immutable reference to the
 *                                    volume fixed/locked state that
 *                                    was set.
 *  @param[in,out]  aBuffer           A mutable reference to the
 *                                    shared pointer into which the
 *                                    response is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
ZonesControllerBasis :: HandleVolumeFixedResponse(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                  const VolumeModel::FixedType &aVolumeFixed,
                                                  ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::VolumeFixedResponse  lVolumeFixedResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lRetval;


    lRetval = lVolumeFixedResponse.Init(aZoneIdentifier, aVolumeFixed);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lVolumeFixedResponse.GetBuffer();
    lSize = lVolumeFixedResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
