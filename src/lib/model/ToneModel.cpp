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
 *      This file implements an object for managing a HLX zone DSP sound
 *      model tone data model.
 *
 */

#include "ToneModel.hpp"

#include <utility>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Model
{

/**
 *  The level type value for indicating a maximally-boosted
 *  tone equalizer bass or treble level.
 *
 */
const ToneModel::LevelType ToneModel::kLevelMax  =  12;

/**
 *  The level type value for indicating a neutral (that is,
 *  non-boosted or non-attenuated) tone equalizer bass or treble level.
 *
 */
const ToneModel::LevelType ToneModel::kLevelFlat =   0;

/**
 *  The level type value for indicating a maximally-attenuated
 *  tone equalizer bass or treble level.
 *
 */
const ToneModel::LevelType ToneModel::kLevelMin  = -12;

static const ToneModel::LevelType kLevelDefault = ToneModel::kLevelFlat;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ToneModel :: ToneModel(void) :
    mBassIsNull(true),
    mBass(kLevelDefault),
    mTrebleIsNull(true),
    mTreble(kLevelDefault)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null bass and treble level.
 *
 *  The bass and treble levels must be explicitly set with #SetBass
 *  and #SetTreble before #GetBass, #GetTone, or #GetTreble may be
 *  used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetBass
 *  @sa SetTone
 *  @sa SetTreble
 *
 */
Status
ToneModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mBass         = kLevelDefault;
    mBassIsNull   = true;
    mTreble       = kLevelDefault;
    mTrebleIsNull = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified tone equalizer bass
 *  and treble levels.
 *
 *  @param[in]  aBass    An immutable reference to the tone
 *                       equalizer bass level to initialize the model
 *                       with.
 *  @param[in]  aTreble  An immutable reference to the tone
 *                       equalizer treble level to initialize the
 *                       model with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The specified @a aBass or @a aTreble
 *                            value is out of range.
 *
 */
Status
ToneModel :: Init(const LevelType &aBass, const LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = SetBass(aBass);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = SetTreble(aTreble);
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
 *  This initializes the class with the specified tone equalizer
 *  model.
 *
 *  @param[in]  aToneModel  An immutable reference to the tone
 *                          equalizer model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToneModel :: Init(const ToneModel &aToneModel)
{
    Status lRetval = kStatus_Success;

    lRetval = Init(aToneModel.mBass, aToneModel.mTreble);

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified tone equalizer model to this
 *  one.
 *
 *  @param[in]  aToneModel  An immutable reference to the tone
 *                          equalizer model to assign (copy) to
 *                          this one.
 *
 *  @returns
 *    A reference to this tone equalizer model after the assignment
 *    (copy) is complete.
 *
 */
ToneModel &
ToneModel :: operator =(const ToneModel &aToneModel)
{
    mBassIsNull   = aToneModel.mBassIsNull;
    mBass         = aToneModel.mBass;
    mTrebleIsNull = aToneModel.mTrebleIsNull;
    mTreble       = aToneModel.mTreble;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer bass level.
 *
 *  This attempts to get the model tone equalizer bass level, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aBass  A mutable reference to storage for the
 *                      model tone equalizer bass level, if
 *                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer bass
 *                                  level value has not been
 *                                  initialized with a known value.
 *
 */
Status
ToneModel :: GetBass(LevelType &aBass) const
{
    Status lRetval = ((mBassIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aBass = mBass;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer bass and treble levels.
 *
 *  This attempts to get the model tone equalizer bass and treble
 *  levels, if they have been previously initialized or set.
 *
 *  @param[out]  aBass    A mutable reference to storage for the
 *                        model tone equalizer bass level, if
 *                        successful.
 *  @param[out]  aTreble  A mutable reference to storage for the
 *                        model tone equalizer treble level, if
 *                        successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer bass
 *                                  or treble level values have not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
ToneModel :: GetTone(LevelType &aBass, LevelType &aTreble) const
{
    Status lRetval;

    lRetval = GetBass(aBass);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = GetTreble(aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer treble level.
 *
 *  This attempts to get the model tone equalizer treble level, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aTreble  A mutable reference to storage for the
 *                        model tone equalizer treble level, if
 *                        successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer treble
 *                                  level value has not been
 *                                  initialized with a known value.
 *
 */
Status
ToneModel :: GetTreble(LevelType &aTreble) const
{
    Status lRetval = ((mTrebleIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aTreble = mTreble;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Decrease the model bass level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the model bass level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutBass  A mutable reference to storage for the
 *                         resulting tone equalizer bass level, if
 *                         successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum bass level.
 *
 */
Status
ToneModel :: DecreaseBass(LevelType &aOutBass)
{
    static const LevelType kAdjustment = -1;

    return (AdjustBass(kAdjustment, aOutBass));
}

/**
 *  @brief
 *    Decrease the model treble level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the model treble level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutTreble  A mutable reference to storage for the
 *                           resulting tone equalizer treble level, if
 *                           successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum treble level.
 *
 */
Status
ToneModel :: DecreaseTreble(LevelType &aOutTreble)
{
    static const LevelType kAdjustment = -1;

    return (AdjustTreble(kAdjustment, aOutTreble));
}

/**
 *  @brief
 *    Increase the model bass level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the model bass level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutBass  A mutable reference to storage for the
 *                         resulting tone equalizer bass level, if
 *                         successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum bass level.
 *
 */
Status
ToneModel :: IncreaseBass(LevelType &aOutBass)
{
    static const LevelType kAdjustment = 1;

    return (AdjustBass(kAdjustment, aOutBass));
}

/**
 *  @brief
 *    Increase the model treble level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the model treble level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutTreble  A mutable reference to storage for the
 *                           resulting tone equalizer treble level, if
 *                           successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum treble level.
 *
 */
Status
ToneModel :: IncreaseTreble(LevelType &aOutTreble)
{
    static const LevelType kAdjustment = 1;

    return (AdjustTreble(kAdjustment, aOutTreble));
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer bass level.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  bass level.
 *
 *  @param[in]  aBass  An immutable reference to the tone
 *                     equalizer bass level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBass value has
 *                                    already been set.
 *  @retval  -ERANGE                  The specified @a aBass value is
 *                                    out of range.
 *
 */
Status
ToneModel :: SetBass(const LevelType &aBass)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBass >= ToneModel::kLevelMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aBass <= ToneModel::kLevelMax, done, lRetval = -ERANGE);

    if (mBass == aBass)
    {
        lRetval = ((mBassIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mBass = aBass;
    }

    mBassIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer bass and treble levels.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  bass and treble levels.
 *
 *  @param[in]  aBass    An immutable reference to the tone
 *                       equalizer bass level to set.
 *  @param[in]  aTreble  An immutable reference to the tone
 *                       equalizer treble level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBass or @a
 *                                    aTreble value has already been
 *                                    set.
 *  @retval  -ERANGE                  The specified @a aBass or @a
 *                                    aTreble value is out of range.
 *
 */
Status
ToneModel :: SetTone(const LevelType &aBass, const LevelType &aTreble)
{
    const LevelType  kCurrentBass = mBass;
    const bool       kCurrentBassIsNull = mBassIsNull;
    Status           lBassStatus = kStatus_Success;
    Status           lTrebleStatus = kStatus_Success;
    Status           lRetval = kStatus_Success;


    lBassStatus = SetBass(aBass);
    nlREQUIRE_ACTION(lBassStatus >= kStatus_Success, done, lRetval = lBassStatus);

    // Attempt to set the treble. If the status is not successful
    // (greater than or equal to kStatus_Success), then restore the
    // bass state.

    lTrebleStatus = SetTreble(aTreble);
    nlREQUIRE_ACTION(lTrebleStatus >= kStatus_Success, done, mBass = kCurrentBass; mBassIsNull = kCurrentBassIsNull; lRetval = lTrebleStatus);

 done:
    // If either bass or treble successfully changed, then it must
    // appear to the caller as though both changed, even if one of
    // them may have been kStatus_ValueAlreadySet.

    if ((lBassStatus >= kStatus_Success) && (lTrebleStatus >= kStatus_Success))
    {
        if (lBassStatus > kStatus_Success)
        {
            lRetval = lTrebleStatus;
        }
        else if (lTrebleStatus > kStatus_Success)
        {
            lRetval = lBassStatus;
        }
        else
        {
            lRetval = kStatus_Success;
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer treble level.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  treble level.
 *
 *  @param[in]  aTreble  An immutable reference to the tone
 *                       equalizer treble level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aTreble value has
 *                                    already been set.
 *  @retval  -ERANGE                  The specified @a aTreble value is
 *                                    out of range.
 *
 */
Status
ToneModel :: SetTreble(const LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aTreble >= ToneModel::kLevelMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aTreble <= ToneModel::kLevelMax, done, lRetval = -ERANGE);

    if (mTreble == aTreble)
    {
        lRetval = ((mTrebleIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mTreble = aTreble;
    }

    mTrebleIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Adjust the tone equalizer bass level by the specified amount.
 *
 *  This attempts to adjust the tone equalizer bass level by the
 *  specified amount.
 *
 *  @param[in]   aAdjustment  The amount to adjust the tone equalizer
 *                            bass level by.
 *  @param[out]  aOutBass     A mutable reference to storage for the
 *                            resulting tone equalizer bass level, if
 *                            successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the tone equalizer bass level
 *                                  value has not been initialized
 *                                  with a known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a bass level that would exceed
 *                                  the maximum boost or attenuation
 *                                  levels.
 *
 *  @sa Init
 *  @sa SetBass
 *
 */
Status
ToneModel :: AdjustBass(const LevelType &aAdjustment, LevelType &aOutBass)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mBassIsNull, done, lRetval = kError_NotInitialized);

    lRetval = SetBass(mBass + aAdjustment);
    nlEXPECT_SUCCESS(lRetval, done);

    aOutBass = mBass;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Adjust the tone equalizer treble level by the specified amount.
 *
 *  This attempts to adjust the tone equalizer treble level by the
 *  specified amount.
 *
 *  @param[in]   aAdjustment  The amount to adjust the tone equalizer
 *                            treble level by.
 *  @param[out]  aOutTreble   A mutable reference to storage for the
 *                            resulting tone equalizer treble level, if
 *                            successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the tone equalizer treble level
 *                                  value has not been initialized
 *                                  with a known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a treble level that would exceed
 *                                  the maximum boost or attenuation
 *                                  levels.
 *
 *  @sa Init
 *  @sa SetTreble
 *
 */
Status
ToneModel :: AdjustTreble(const LevelType &aAdjustment, LevelType &aOutTreble)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mTrebleIsNull, done, lRetval = kError_NotInitialized);

    lRetval = SetTreble(mTreble + aAdjustment);
    nlEXPECT_SUCCESS(lRetval, done);

    aOutTreble = mTreble;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided tone model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aToneModel  An immutable reference to the tone
 *                          model to compare for equality.
 *
 *  @returns
 *    True if this tone model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool ToneModel :: operator ==(const ToneModel &aToneModel) const
{
    return ((mBassIsNull   == aToneModel.mBassIsNull  ) &&
            (mBass         == aToneModel.mBass        ) &&
            (mTrebleIsNull == aToneModel.mTrebleIsNull) &&
            (mTreble       == aToneModel.mTreble      ));
}

}; // namespace Model

}; // namespace HLX
