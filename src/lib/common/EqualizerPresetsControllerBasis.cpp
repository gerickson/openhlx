/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file implements a base object for realizing a HLX equalizer
 *      presets controller, either in a client or server context.
 *
 */

#include "EqualizerPresetsControllerBasis.hpp"

#include <errno.h>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

/**
 *  The maximum number of equalizer presets supported by the HLX
 *  server controller.
 *
 */
const EqualizerPresetsControllerBasis::IdentifierType  EqualizerPresetsControllerBasis::kEqualizerPresetsMax = 10;

// MARK: Observer Methods

/**
 *  @brief
 *    Get the maximum number of supported HLX equalizer presets.
 *
 *  @param[out]  aEqualizerPresets  The maximum number of HLX
 *                                  equalizer presets on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
EqualizerPresetsControllerBasis :: GetEqualizerPresetsMax(IdentifierType &aEqualizerPresets)
{
    Status  lRetval = kStatus_Success;

    aEqualizerPresets = GetEqualizerPresetsMax();

    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX equalizer presets.
 *
 *  @returns
 *    The maximum number of supported HLX equalizer presets.
 *
 */
EqualizerPresetsControllerBasis::IdentifierType
EqualizerPresetsControllerBasis :: GetEqualizerPresetsMax(void)
{
    return (kEqualizerPresetsMax);
}

/**
 *  @brief
 *    Determine whether or not an equalizer preset identifier is valid.
 *
 *  This determines whether or not the specified equalizer preset
 *  identifier is valid.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to the
 *                                          equalizer preset identifer
 *                                          to validate.
 *
 *  @returns
 *    True if the identifier is valid; otherwise, false.
 *
 */
bool
EqualizerPresetsControllerBasis :: IsValidIdentifier(const IdentifierType &aEqualizerPresetIdentifier)
{
    const bool lIsValid = (ValidateIdentifier(aEqualizerPresetIdentifier) == kStatus_Success);


    return (lIsValid);
}

/**
 *  @brief
 *    Validate an equalizer preset identifier.
 *
 *  This validates the specified equalizer preset identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to the
 *                                          equalizer preset identifer
 *                                          to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the equalizer preset identifier is
 *                            smaller or larger than supported.
 *
 */
Status
EqualizerPresetsControllerBasis :: ValidateIdentifier(const IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aEqualizerPresetIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aEqualizerPresetIdentifier <= kEqualizerPresetsMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the equalizer presets controller basis.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsControllerBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresets.Init(kEqualizerPresetsMax);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
