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
 *      This file implements an object for managing a HLX output volume
 *      data model.
 *
 */

#include "VolumeModel.hpp"

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
 *  The volume level type value for the minimum volume level.
 *
 */
const VolumeModel::LevelType VolumeModel::kLevelMin = -80;

/**
 *  The volume level type value for the maximum volume level.
 *
 */
const VolumeModel::LevelType VolumeModel::kLevelMax =   0;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
VolumeModel :: VolumeModel(void) :
    mLevelIsNull(true),
    mLevel(kLevelMin),
    mFixedIsNull(true),
    mFixed(false),
    mMuteIsNull(true),
    mMute(true)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null level, mute state, and
 *  fixed/locked state.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetMute
 *  @sa SetFixed
 *  @sa SetVolume
 *
 */
Status
VolumeModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mLevel       = kLevelMin;
    mLevelIsNull = true;
    mFixedIsNull = true;
    mFixed       = false;
    mMute        = true;
    mMuteIsNull  = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified level and mute state.
 *
 *  @param[in]  aLevel  An immutable reference to the tone
 *                      equalizer bass level to initialize the model
 *                      with.
 *  @param[in]  aMute   An immutable reference to the tone
 *                      equalizer treble level to initialize the
 *                      model with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The specified @a aVolume is out of range.
 *
 */
Status
VolumeModel :: Init(const LevelType &aLevel, const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = SetVolume(aLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = SetMute(aMute);
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
 *  This initializes the class with the specified volume model.
 *
 *  @param[in]  aVolumeModel  An immutable reference to the
 *                            volume model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeModel :: Init(const VolumeModel &aVolumeModel)
{
    Status lRetval = kStatus_Success;

    *this = aVolumeModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified volume model to this
 *  one.
 *
 *  @param[in]  aVolumeModel  An immutable reference to the
 *                            volume model to assign (copy) to
 *                            this one.
 *
 *  @returns
 *    A reference to this volume model after the assignment
 *    (copy) is complete.
 *
 */
VolumeModel &
VolumeModel :: operator =(const VolumeModel &aVolumeModel)
{
    mLevelIsNull = aVolumeModel.mLevelIsNull;
    mLevel       = aVolumeModel.mLevel;
    mFixedIsNull = aVolumeModel.mFixedIsNull;
    mFixed       = aVolumeModel.mFixed;
    mMuteIsNull  = aVolumeModel.mMuteIsNull;
    mMute        = aVolumeModel.mMute;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model volume level.
 *
 *  This attempts to get the model volume level, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aLevel  A mutable reference to storage for the
 *                       model volume level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *
 *  @sa SetVolume
 *
 */
Status
VolumeModel :: GetVolume(LevelType &aLevel) const
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
 *    Attempt to get the model volume fixed/locked state.
 *
 *  This attempts to get the model volume fixed/locked state, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aFixed  A mutable reference to storage for the
 *                       model volume fixed/locked state, if
 *                       successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume fixed state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa SetFixed
 *
 */
Status
VolumeModel :: GetFixed(FixedType &aFixed) const
{
    Status lRetval = ((mFixedIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aFixed = mFixed;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model volume mute state.
 *
 *  This attempts to get the model volume mute state, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aMute  A mutable reference to storage for the
 *                      model volume mute state, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa SetMute
 *  @sa ToggleMute
 *
 */
Status
VolumeModel :: GetMute(MuteType &aMute) const
{
    Status lRetval = ((mMuteIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aMute = mMute;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Decrease the model volume level by one (1) unit.
 *
 *  This attempts to decrease the model volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting model volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  minimum model volume level.
 *
 *  @sa SetVolume
 *
 */
Status
VolumeModel :: DecreaseVolume(LevelType &aOutLevel)
{
    static const LevelType kAdjustment = -1;

    return (AdjustVolume(kAdjustment, aOutLevel));
}

/**
 *  @brief
 *    Increase the model volume level by one (1) unit.
 *
 *  This attempts to increase the model volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting model volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  maximum model volume level.
 *
 *  @sa SetVolume
 *
 */
Status
VolumeModel :: IncreaseVolume(LevelType &aOutLevel)
{
    static const LevelType kAdjustment = 1;

    return (AdjustVolume(kAdjustment, aOutLevel));
}

/**
 *  @brief
 *    This sets the model volume fixed/locked state.
 *
 *  This attempts to set the model with the specified volume
 *  fixed/locked state.
 *
 *  @param[in]  aFixed  An immutable reference to the model
 *                      volume fixed/locked state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aFixed value
 *                                    has already been set.
 *
 */
Status
VolumeModel :: SetFixed(const FixedType &aFixed)
{
    Status lRetval = kStatus_Success;

    if (mFixed == aFixed)
    {
        lRetval = ((mFixedIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mFixed = aFixed;
    }

    mFixedIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model volume mute state.
 *
 *  This attempts to set the model with the specified volume
 *  mute state.
 *
 *  @param[in]  aMute  An immutable reference to the model
 *                     volume mute state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aMute value
 *                                    has already been set.
 *
 */
Status
VolumeModel :: SetMute(const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    if (mMute == aMute)
    {
        lRetval = ((mMuteIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mMute = aMute;
    }

    mMuteIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model volume level.
 *
 *  This attempts to set the model with the specified volume
 *  level.
 *
 *  @param[in]  aLevel  An immutable reference to the model volume
 *                      level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLevel value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aLevel value
 *                                    is out of range.
 *
 */
Status
VolumeModel :: SetVolume(const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aLevel >= VolumeModel::kLevelMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aLevel <= VolumeModel::kLevelMax, done, lRetval = -ERANGE);

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
 *    Attempt to toggle (flip) the model volume mute state.
 *
 *  This attempts to toggle (flip) the model volume mute state.
 *
 *  @param[out]  aOutMute  A mutable reference to storage for the
 *                         resulting model volume mute state, if
 *                         successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 */
Status
VolumeModel :: ToggleMute(MuteType &aOutMute)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mMuteIsNull, done, lRetval = kError_NotInitialized);

    mMute = !mMute;

    aOutMute = mMute;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Adjust the model volume level by the specified amount.
 *
 *  This attempts to adjust the model volume level by the specified
 *  amount.
 *
 *  @param[in]   aAdjustment  The amount to adjust the model volume
 *                            level by.
 *  @param[out]  aOutLevel    A mutable reference to storage for the
 *                            resulting model volume level, if
 *                            successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume level has
 *                                  not been initialized with a known
 *                                  value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  minimum or maximum model volume
 *                                  levels.
 *
 *  @sa SetVolume
 *
 */
Status
VolumeModel :: AdjustVolume(const LevelType &aAdjustment, LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mLevelIsNull, done, lRetval = kError_NotInitialized);

    lRetval = SetVolume(mLevel + aAdjustment);
    nlEXPECT_SUCCESS(lRetval, done);

    aOutLevel = mLevel;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided volume model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aVolumeModel  An immutable reference to the volume
 *                            model to compare for equality.
 *
 *  @returns
 *    True if this volume model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool VolumeModel :: operator ==(const VolumeModel &aVolumeModel) const
{
    return ((mLevelIsNull == aVolumeModel.mLevelIsNull) &&
            (mLevel       == aVolumeModel.mLevel      ) &&
            (mFixedIsNull == aVolumeModel.mFixedIsNull ) &&
            (mFixed       == aVolumeModel.mFixed       ) &&
            (mMuteIsNull  == aVolumeModel.mMuteIsNull  ) &&
            (mMute        == aVolumeModel.mMute        ));
}

}; // namespace Model

}; // namespace HLX
