/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file implements objects for HLX client zone data model
 *      commands and their constituent requests and responses.
 *
 */

#include "ZonesControllerCommands.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Zones
{

static const char * const  kZoneObject     = "O";

static const char * const  kMuteProperty   = "VM";
static const char          kSourceProperty = 'C';
static const char          kVolumeProperty = 'V';

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone query command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference to the identifier
 *                                of the zone to query.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (QueryRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone query command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone query command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference to the identifier
 *                                of the zone to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
Query :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
QueryMuteRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (QueryPropertyRequestBasis::Init(kZoneObject, kMuteProperty, aZoneIdentifier));
}

Status
QueryMute :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
QuerySourceRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (QueryPropertyRequestBasis::Init(kZoneObject, kSourceProperty, aZoneIdentifier));
}

Status
QuerySource :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
QueryVolumeRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (QueryPropertyRequestBasis::Init(kZoneObject, kVolumeProperty, aZoneIdentifier));
}

Status
QueryVolume :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Balance Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the
 *                                stereophonic channel balance state.
 *  @param[in]  aBalance          An immutable reference to the
 *                                balance state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetBalanceRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const Model::BalanceModel::BalanceType &aBalance)
{
    return (BalanceSetRequestBasis::Init(kZoneObject, aZoneIdentifier, aBalance));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone stereophonic channel balance command
 *  response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
BalanceResponse :: Init(void)
{
    return (BalanceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance set command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the
 *                                stereophonic channel balance state.
 *  @param[in]  aBalance          An immutable reference to the
 *                                balance state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetBalance :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                   const BalanceModel::BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;


    lRetval = mRequest.Init(aZoneIdentifier, aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance increase
 *  towards the left channel by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBalanceLeftRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (BalanceAdjustRequestBasis::Init(kZoneObject, aZoneIdentifier, BalanceModel::kChannelLeft));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance increase
 *  towards the left channel by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBalanceLeft :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance increase
 *  towards the right channel by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBalanceRightRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (BalanceAdjustRequestBasis::Init(kZoneObject, aZoneIdentifier, BalanceModel::kChannelRight));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone stereophonic channel balance increase
 *  towards the right channel by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to adjust the
 *                                balance.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBalanceRight :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Equalizer Band Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetEqualizerBandRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    return (EqualizerBandSetRequestBasis::Init(kZoneObject,
                                               aZoneIdentifier,
                                               aEqualizerBandIdentifier,
                                               aEqualizerBandLevel));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone equalizer band level command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerBandResponse :: Init(void)
{
    return (EqualizerBandLevelRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level set command.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetEqualizerBand :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                         const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                         const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier,
                            aEqualizerBandIdentifier,
                            aEqualizerBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level increase command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseEqualizerBandRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    return (EqualizerBandIncreaseRequestBasis::Init(kZoneObject,
                                                    aZoneIdentifier,
                                                    aEqualizerBandIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level increase command.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseEqualizerBand :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier,
                            aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level decrease command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseEqualizerBandRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    return (EqualizerBandDecreaseRequestBasis::Init(kZoneObject,
                                                    aZoneIdentifier,
                                                    aEqualizerBandIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer band level decrease command.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseEqualizerBand :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: (Sound Mode) Equalizer Preset Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer preset set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          for the zone for which
 *                                          to set the equalizer
 *                                          preset.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetEqualizerPresetRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                  const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    return (EqualizerPresetBufferBasis::Init(*this, kZoneObject, aZoneIdentifier, aEqualizerPresetIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone equalier preset command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetResponse :: Init(void)
{
    return (EqualizerPresetRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer preset set command.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          for the zone for which
 *                                          to set the equalizer
 *                                          preset.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetEqualizerPreset :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                           const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: (Sound Mode) Highpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone highpass crossover frequency set command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the highpass
 *                                        crossover frequency.
 *  @param[in]  aHighpassFrequency        An immutable reference to the
 *                                        highpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetHighpassCrossoverRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                    const Model::CrossoverModel::FrequencyType &aHighpassFrequency)
{
    return (EqualizerHighpassCrossoverBufferBasis::Init(*this,
                                                        kZoneObject,
                                                        aZoneIdentifier,
                                                        aHighpassFrequency));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone highpass filter crossover frequency
 *  command response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
HighpassCrossoverResponse :: Init(void)
{
    return (HighpassCrossoverRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone highpass crossover frequency set command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the highpass
 *                                        crossover frequency.
 *  @param[in]  aHighpassFrequency        An immutable reference to the
 *                                        highpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetHighpassCrossover :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                             const Model::CrossoverModel::FrequencyType &aHighpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: (Sound Mode) Lowpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone lowpass crossover frequency set command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the lowpass
 *                                        crossover frequency.
 *  @param[in]  aLowpassFrequency         An immutable reference to the
 *                                        lowpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetLowpassCrossoverRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                   const Model::CrossoverModel::FrequencyType &aLowpassFrequency)
{
    return (EqualizerLowpassCrossoverBufferBasis::Init(*this,
                                                       kZoneObject,
                                                       aZoneIdentifier,
                                                       aLowpassFrequency));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone lowpass filter crossover frequency
 *  command response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LowpassCrossoverResponse :: Init(void)
{
    return (LowpassCrossoverRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone lowpass crossover frequency set command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the lowpass
 *                                        crossover frequency.
 *  @param[in]  aLowpassFrequency         An immutable reference to the
 *                                        lowpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetLowpassCrossover :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                            const Model::CrossoverModel::FrequencyType &aLowpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute set (assert/enable) command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set
 *                                (assert/enable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMuteRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (VolumeMuteSetRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone volume mute command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
MuteResponse :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute set (assert/enable) command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set
 *                                (assert/enable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMute :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute clear (deassert/disable)
 *  command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to clear
 *                                (deassert/disable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ClearMuteRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (VolumeMuteClearRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute clear (deassert/disable)
 *  command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to clear
 *                                (deassert/disable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ClearMute :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute toggle (flip) command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to toggle (flip)
 *                                volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMuteRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (VolumeMuteToggleRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume mute toggle (flip) command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to toggle (flip)
 *                                volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMute :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone set name command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the name.
 *  @param[in]  aName             A pointer to the null-terminated
 *                                C string to set the zone name to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetNameRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const char *aName)
{
    return (NameSetRequestBasis::Init(kZoneObject,
                                      aZoneIdentifier,
                                      aName));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone name command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone set name command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the name.
 *  @param[in]  aName             A pointer to the null-terminated
 *                                C string to set the zone name to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetName :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Sound Mode Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer sound mode set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the equalizer
 *                                        sound mode.
 *  @param[in]  aSoundMode                An immutable reference to the
 *                                        equalizer sound mode to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSoundModeRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                            const Model::SoundModel::SoundMode &aSoundMode)
{
    return (EqualizerSoundModeBufferBasis::Init(*this,
                                                kZoneObject,
                                                aZoneIdentifier,
                                                aSoundMode));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone equalizer sound mode command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SoundModeResponse :: Init(void)
{
    return (SoundModeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer sound mode set command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the equalizer
 *                                        sound mode.
 *  @param[in]  aSoundMode                An immutable reference to the
 *                                        equalizer sound mode to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSoundMode :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                     const Model::SoundModel::SoundMode &aSoundMode)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone source (input) set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSourceRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                         const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetRequestBasis::Init(kZoneObject,
                                        aZoneIdentifier,
                                        aSourceIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the single zone source (input) command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceResponse :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone source (input) set command.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSource :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                  const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones source (input) set command request
 *  buffer.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSourceAllRequest :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    static const char * const kAllZonesObject = "X";

    return (SourceAllBufferBasis::Init(*this, kAllZonesObject, aSourceIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the all zones source (input) command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceAllResponse :: Init(void)
{
    return (SourceAllRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones source (input) set command.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSourceAll :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: (Sound Mode) Tone Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass band level decrease
 *  by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseBassRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char * const  kDecreaseBassOperation = "BD";

    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  kDecreaseBassOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer treble band level decrease
 *  by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseTrebleRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char * const  kDecreaseTrebleOperation = "TD";

    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  kDecreaseTrebleOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass band level increase
 *  by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBassRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char * const  kIncreaseBassOperation = "BU";

    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  kIncreaseBassOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer treble band level increase
 *  by one (1) unit command request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseTrebleRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char * const  kIncreaseTrebleOperation = "TU";

    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  kIncreaseTrebleOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass and treble band
 *  levels set command request buffer.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the bass and
 *                                        treble levels.
 *  @param[in]  aBass                     An immutable reference to the
 *                                        bass level to set.
 *  @param[in]  aTreble                   An immutable reference to the
 *                                        treble level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetToneRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const Model::ToneModel::LevelType &aBass,
                       const Model::ToneModel::LevelType &aTreble)
{
    return (ToneBufferBasis::Init(*this,
                                  kZoneObject,
                                  aZoneIdentifier,
                                  aBass,
                                  aTreble));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone tone equalizer command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToneResponse :: Init(void)
{
    return (ToneRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass band level decrease
 *  by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseBass :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer treble band level decrease
 *  by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseTreble :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass band level increase
 *  by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseBass :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer treble band level increase
 *  by one (1) unit command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseTreble :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass and treble band
 *  levels set command.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the bass and
 *                                        treble levels.
 *  @param[in]  aBass                     An immutable reference to the
 *                                        bass level to set.
 *  @param[in]  aTreble                   An immutable reference to the
 *                                        treble level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetTone :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                const ToneModel::LevelType &aBass,
                const ToneModel::LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aBass, aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level set command request
 *  buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                         const Model::VolumeModel::LevelType &aLevel)
{
    return (VolumeSetRequestBasis::Init(kZoneObject, aZoneIdentifier, aLevel));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the single zone volume level command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeResponse :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level set command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolume :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                  const Model::VolumeModel::LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones volume level set command request
 *  buffer.
 *
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeAllRequest :: Init(const Model::VolumeModel::LevelType &aLevel)
{
    static const char * const kAllZonesObject = "X";

    return (VolumeAllBufferBasis::Init(*this, kAllZonesObject, aLevel));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the all zones volume level command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeAllResponse :: Init(void)
{
    return (VolumeAllRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the all zones volume level set command.
 *
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeAll :: Init(const Model::VolumeModel::LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level increase by one (1) unit
 *  command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseVolumeRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (VolumeIncreaseRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level increase by one (1) unit
 *  command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseVolume :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level decrease by one (1) unit
 *  command request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseVolumeRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (VolumeDecreaseRequestBasis::Init(kZoneObject, aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume level decrease by one (1) unit
 *  command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseVolume :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume fixed/locked state set command
 *  request buffer.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                locked state.
 *  @param[in]  aLocked           An immutable reference to the volume
 *                                locked state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeFixedRequest :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              const Model::VolumeModel::FixedType &aLocked)
{
    return (VolumeFixedRequestBasis::Init(kZoneObject, aZoneIdentifier, aLocked));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the zone volume fixed/locked state command
 *  response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
VolumeFixedResponse :: Init(void)
{
    return (VolumeFixedRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone volume fixed/locked state set command.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                locked state.
 *  @param[in]  aLocked           An immutable reference to the volume
 *                                locked state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeFixed :: Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                       const Model::VolumeModel::FixedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aZoneIdentifier, aLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Zones

}; // namespace Command

}; // namespace Client

}; // namespace HLX
