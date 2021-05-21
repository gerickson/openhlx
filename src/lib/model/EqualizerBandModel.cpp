/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file implements an object for managing an HLX DSP equalizer
 *      band, whether used as part of an equalizer preset or zone
 *      equalizer.
 *
 */

#include "EqualizerBandModel.hpp"

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;



namespace HLX
{

namespace Model
{

/**
 *  The level type value for indicating a maximally-boosted
 *  equalizer band.
 *
 */
const EqualizerBandModel::LevelType EqualizerBandModel::kLevelMax  =  10;

/**
 *  The level type value for indicating a neutral (that is,
 *  non-boosted or non-attenuated) equalizer band.
 *
 */
const EqualizerBandModel::LevelType EqualizerBandModel::kLevelFlat =   0;

/**
 *  The level type value for indicating a maximally-attenuated
 *  equalizer band.
 *
 */
const EqualizerBandModel::LevelType EqualizerBandModel::kLevelMin  = -10;

static const EqualizerBandModel::FrequencyType kFrequencyDefault = 0;
static const EqualizerBandModel::LevelType     kLevelDefault     = EqualizerBandModel::kLevelFlat;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerBandModel :: EqualizerBandModel(void) :
    mFrequencyIsNull(true),
    mFrequency(kFrequencyDefault),
    mLevelIsNull(true),
    mLevel(kLevelDefault)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null frequency and level.
 *
 *  The frequency and level must be explicitly set with SetFrequency
 *  and #SetLevel before #GetFrequency, #GetLevel, #DecreaseLevel, or
 *  #IncreaseLevel may be used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetFrequency
 *  @sa SetLevel
 *
 */
Status
EqualizerBandModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mFrequencyIsNull = true;
    mFrequency       = kFrequencyDefault;
    mLevelIsNull     = true;
    mLevel           = kLevelDefault;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified equalizer band
 *  center frequency and boost/attenuation level.
 *
 *  @param[in]  aFrequency  An immutable reference to the equalizer
 *                          band center frequency to initialize the
 *                          model with.
 *  @param[in]  aLevel      An immutable reference to the equalizer
 *                          band boost/attenuation to initialize the
 *                          model with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ERANGE                  The specified @a aLevel value
 *                                    is out of range.
 *
 */
Status
EqualizerBandModel :: Init(const FrequencyType &aFrequency, const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = SetFrequency(aFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = SetLevel(aLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified equalizer band
 *  boost/attenuation level.
 *
 *  The level must be explicitly set with #SetLevel before #GetLevel,
 *  #DecreaseLevel, or #IncreaseLevel may be used successfully.
 *
 *  @param[in]  aFrequency  An immutable reference to the equalizer
 *                          band center frequency to initialize the
 *                          model with.
 *
 *  @retval  kStatus_Success          If successful.
 *
 */
Status
EqualizerBandModel :: Init(const FrequencyType &aFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = SetFrequency(aFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified equalizer band
 *  model.
 *
 *  @param[in]  aEqualizerBandModel  An immutable reference to the
 *                                   equalizer band model to
 *                                   initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandModel :: Init(const EqualizerBandModel &aEqualizerBandModel)
{
    Status lRetval = kStatus_Success;

    *this = aEqualizerBandModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified equalizer band model to this
 *  one.
 *
 *  @param[in]  aEqualizerBandModel  An immutable reference to the
 *                                   equalizer band model to assign
 *                                   (copy) to this one.
 *
 *  @returns
 *    A reference to this equalizer band model after the assignment
 *    (copy) is complete.
 *
 */
EqualizerBandModel &
EqualizerBandModel :: operator =(const EqualizerBandModel &aEqualizerBandModel)
{
    mFrequencyIsNull = aEqualizerBandModel.mFrequencyIsNull;
    mFrequency       = aEqualizerBandModel.mFrequency;
    mLevelIsNull     = aEqualizerBandModel.mLevelIsNull;
    mLevel           = aEqualizerBandModel.mLevel;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the equalizer band center frequency.
 *
 *  This attempts to get the equalizer band center frequency, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aFrequency  A mutable reference to storage for the
 *                           equalizer band center frequency, if
 *                           successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer band center
 *                                  frequency value has not been
 *                                  initialized with a known value.
 *
 *  @sa Init
 *  @sa SetFrequency
 *
 */
Status
EqualizerBandModel :: GetFrequency(FrequencyType &aFrequency) const
{
    Status lRetval = ((mFrequencyIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aFrequency = mFrequency;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer band boost/attenuation level.
 *
 *  This attempts to get the equalizer band boost/attenuation level,
 *  if it has been previously initialized or set.
 *
 *  @param[out]  aLevel  A mutable reference to storage for the
 *                       equalizer band boost/attenuation level, if
 *                       successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer band boost/
 *                                  attenuation level value has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetLevel
 *
 */
Status
EqualizerBandModel :: GetLevel(LevelType &aLevel) const
{
    Status lRetval = ((mLevelIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aLevel = mLevel;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer band center frequency.
 *
 *  This initializes the model with the specified equalizer band
 *  center frequency.
 *
 *  @param[in]  aFrequency  An immutable reference to the equalizer
 *                          band center frequency to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aFrequency value
 *                                    has already been set.
 *
 */
Status
EqualizerBandModel :: SetFrequency(const FrequencyType &aFrequency)
{
    Status lRetval = kStatus_Success;

    if (mFrequency == aFrequency)
    {
        lRetval = ((mFrequencyIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mFrequency = aFrequency;
    }

    mFrequencyIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    Decrease the equalizer band boost/attenuation level by one (1)
 *    unit.
 *
 *  This attempts to decrease the equalizer band boost/attenuation
 *  level by one (1) unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting equalizer band boost/attenuation
 *                          level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer band model boost/
 *                                  attenuation level value has not
 *                                  been initialized with a known
 *                                  value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  minimum attenuation level.
 *
 *  @sa Init
 *  @sa SetLevel
 *
 */
Status
EqualizerBandModel :: DecreaseLevel(LevelType &aOutLevel)
{
    static const LevelType kAdjustment = -1;

    return (AdjustLevel(kAdjustment, aOutLevel));
}

/**
 *  @brief
 *    Increase the equalizer band boost/attenuation level by one (1)
 *    unit.
 *
 *  This attempts to increase the equalizer band boost/attenuation
 *  level by one (1) unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting equalizer band boost/attenuation
 *                          level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer band model boost/
 *                                  attenuation level value has not
 *                                  been initialized with a known
 *                                  value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  maximum boost level.
 *
 *  @sa Init
 *  @sa SetLevel
 *
 */
Status
EqualizerBandModel :: IncreaseLevel(LevelType &aOutLevel)
{
    static const LevelType kAdjustment = 1;

    return (AdjustLevel(kAdjustment, aOutLevel));
}

/**
 *  @brief
 *    Adjust the equalizer band boost/attenuation level by the
 *    specified amount.
 *
 *  This attempts to adjust the equalizer band boost/attenuation level
 *  by the specified amount.
 *
 *  @param[in]   aAdjustment  The amount to adjust the equalizer band
 *                            boost/attenuation level by.
 *  @param[out]  aOutLevel    A mutable reference to storage for the
 *                            resulting equalizer band
 *                            boost/attenuation level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer band model boost/
 *                                  attenuation level value has not
 *                                  been initialized with a known
 *                                  value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  maximum boost or attenuation
 *                                  levels.
 *
 *  @sa Init
 *  @sa SetLevel
 *
 */
Status
EqualizerBandModel :: AdjustLevel(const LevelType &aAdjustment, LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mLevelIsNull, done, lRetval = kError_NotInitialized);

    lRetval = SetLevel(mLevel + aAdjustment);
    nlEXPECT_SUCCESS(lRetval, done);

    aOutLevel = mLevel;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer band boost/attenuation level.
 *
 *  This initializes the model with the specified equalizer band
 *  boost/attenuation level.
 *
 *  @param[in]  aLevel      An immutable reference to the equalizer
 *                          band boost/attenuation to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLevel value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aLevel value
 *                                    is out of range.
 *
 */
Status
EqualizerBandModel :: SetLevel(const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aLevel >= EqualizerBandModel::kLevelMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aLevel <= EqualizerBandModel::kLevelMax, done, lRetval = -ERANGE);

    if (mLevel == aLevel)
    {
        lRetval = ((mLevelIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mLevel = aLevel;
    }

    mLevelIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided equalizer band model against this one
 *  to determine if they are equal to one another.
 *
 *  @param[in]  aEqualizerBandModel  An immutable reference to the
 *                                   equalizer band model to compare
 *                                   for equality.
 *
 *  @returns
 *    True if this equalizer band model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool EqualizerBandModel :: operator ==(const EqualizerBandModel &aEqualizerBandModel) const
{
    return ((mFrequencyIsNull == aEqualizerBandModel.mFrequencyIsNull) &&
            (mFrequency       == aEqualizerBandModel.mFrequency      ) &&
            (mLevelIsNull     == aEqualizerBandModel.mLevelIsNull    ) &&
            (mLevel           == aEqualizerBandModel.mLevel          ));
}

}; // namespace Model

}; // namespace HLX
