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
 *      This file implements objects for HLX client equalizer preset data
 *      model commands and their constituent requests and responses.
 *
 */

#include "EqualizerPresetsControllerCommands.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <CommandEqualizerBandRequestBases.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

namespace EqualizerPresets
{

static const char * const kEqualizerPresetObject = "EP";
static const char * const kPresetObject = kEqualizerPresetObject + 1;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset query command request buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of
 *                                          the equalizer preset
 *                                          to query.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    return (QueryRequestBasis::Init(kEqualizerPresetObject, aEqualizerPresetIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the equalizer preset query command response
 *  regular expression.
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
 *  This initializes the equalizer preset query command.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of
 *                                          the equalizer preset
 *                                          to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
Query :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Equalizer Band Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset band level set command
 *  request buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
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
SetEqualizerBandRequest :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    return (EqualizerBandSetRequestBasis::Init(kPresetObject,
                                               aEqualizerPresetIdentifier,
                                               aEqualizerBandIdentifier,
                                               aEqualizerBandLevel));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the equalizer preset band level command response
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
    return (BandLevelRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset band level set command.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
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
SetEqualizerBand :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                         const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                         const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aEqualizerPresetIdentifier,
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
 *  This initializes the equalizer preset band level increase command
 *  request buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseEqualizerBandRequest :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    return (EqualizerBandIncreaseRequestBasis::Init(kPresetObject,
                                                    aEqualizerPresetIdentifier,
                                                    aEqualizerBandIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset band level increase command.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseEqualizerBand :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                              const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aEqualizerPresetIdentifier,
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
 *  This initializes the equalizer preset band level decrease command
 *  request buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseEqualizerBandRequest :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    return (EqualizerBandDecreaseRequestBasis::Init(kPresetObject,
                                                    aEqualizerPresetIdentifier,
                                                    aEqualizerBandIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer preset band level decrease command.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseEqualizerBand :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                              const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aEqualizerPresetIdentifier,
                            aEqualizerBandIdentifier);
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
 *  This initializes the equalizer preset set name command request
 *  buffer.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          for which to set the name.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string to set
 *                                          the equalizer preset name
 *                                          to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetNameRequest :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                       const char *aName)
{
    return (NameSetRequestBasis::Init(kEqualizerPresetObject,
                                      aEqualizerPresetIdentifier,
                                      aName));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the equalizer preset name command response
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
 *  This initializes the equalizer preset set name command.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          for which to set the name.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string to set
 *                                          the equalizer preset name
 *                                          to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetName :: Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aEqualizerPresetIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Client

}; // namespace HLX
