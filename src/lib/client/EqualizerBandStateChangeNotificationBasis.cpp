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
 *      This file implements a base object for a HLX client object data
 *      model equalizer band level property state change notification
 *      (SCN).
 *
 */

#include "EqualizerBandStateChangeNotificationBasis.hpp"


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerBandNotificationBasis :: EqualizerBandNotificationBasis(void) :
    mBand(IdentifierModel::kIdentifierInvalid),
    mLevel(EqualizerBandModel::kLevelFlat)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the equalizer preset band level property state
 *  change notification with the specified equalizer band identifer
 *  and level and equalizer preset identifier.
 *
 *  @param[in]  aBandIdentifier    An immutable reference to the
 *                                 specific equalizer band identifier
 *                                 whose band level property state
 *                                 changed.
 *  @param[in]  aBandLevel         An immutable reference to the
 *                                 equalizer band level that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandNotificationBasis :: Init(const IdentifierType &aBandIdentifier,
                                       const LevelType &aBandLevel)
{
    Status lRetval = kStatus_Success;

    mBand  = aBandIdentifier;
    mLevel = aBandLevel;

    return (lRetval);
}

/**
 *  @brief
 *    Return the equalizer band for which the level changed.
 *
 *  @returns
 *    The equalizer band identifier.
 *
 */
EqualizerBandNotificationBasis::IdentifierType
EqualizerBandNotificationBasis :: GetBand(void) const
{
    return (mBand);
}

/**
 *  @brief
 *    Return the equalizer band level the equalizer band changed to.
 *
 *  @returns
 *    The equalizer band level.
 *
 */
EqualizerBandNotificationBasis::LevelType
EqualizerBandNotificationBasis :: GetLevel(void) const
{
    return (mLevel);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
