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
 *      This file implements an object for managing the HLX physical
 *      front panel data model.
 *
 */

#include "FrontPanelModel.hpp"

#include <utility>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Model
{

/**
 *  The brightness type value for indicating the least bright front
 *  panel display level.
 *
 */
const FrontPanelModel::BrightnessType FrontPanelModel::kBrightnessMin = 0;

/**
 *  The brightness type value for indicating the most bright front
 *  panel display level.
 *
 */
const FrontPanelModel::BrightnessType FrontPanelModel::kBrightnessMax = 3;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FrontPanelModel :: FrontPanelModel(void) :
    mBrightnessIsNull(true),
    mBrightness(kBrightnessMin),
    mLockedIsNull(true),
    mLocked(true)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null brightness and locked
 *  state.
 *
 *  The brightness setting and locked state must be explicitly set
 *  with #SetBrightness and #SetLocked before #GetBrightness or
 *  #GetLocked may be used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetBrightness
 *  @sa SetLocked
 *
 */
Status
FrontPanelModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mBrightness        = kBrightnessMin;
    mBrightnessIsNull  = true;
    mLocked            = true;
    mLockedIsNull      = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified brightness setting
 *  and locked state.
 *
 *  @param[in]  aBrightness  An immutable reference to the brightness
 *                           setting to set.
 *  @param[in]  aLocked      An immutable reference to the locked state
 *                           to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ERANGE                  The specified @a aBrightness value
 *                                    is out of range.
 *
 */
Status
FrontPanelModel :: Init(const BrightnessType &aBrightness, const LockedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = SetBrightness(aBrightness);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = SetLocked(aLocked);
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
 *  This initializes the class with the specified front panel model.
 *
 *  @param[in]  aFrontPanelModel  An immutable reference to the
 *                                front panel model to initialize
 *                                with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
FrontPanelModel :: Init(const FrontPanelModel &aFrontPanelModel)
{
    Status lRetval = kStatus_Success;

    lRetval = Init(aFrontPanelModel.mBrightness, aFrontPanelModel.mLocked);

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified front panel model to this one.
 *
 *  @param[in]  aFrontPanelModel  An immutable reference to the
 *                                front panel model to assign (copy)
 *                                to this one.
 *
 *
 *  @returns
 *    A reference to this front panel model after the assignment
 *    (copy) is complete.
 *
 */
FrontPanelModel &
FrontPanelModel :: operator =(const FrontPanelModel &aFrontPanelModel)
{
    mBrightnessIsNull  = aFrontPanelModel.mBrightnessIsNull;
    mBrightness        = aFrontPanelModel.mBrightness;
    mLockedIsNull      = aFrontPanelModel.mLockedIsNull;
    mLocked            = aFrontPanelModel.mLocked;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the front panel brightness setting.
 *
 *  This attempts to get the front panel brightness setting, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aBrightness  A mutable reference to storage for the
 *                            front panel brightness setting, if
 *                            successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the front panel model brightness
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetBrightness
 *
 */
Status
FrontPanelModel :: GetBrightness(BrightnessType &aBrightness) const
{
    Status lRetval = ((mBrightnessIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aBrightness = mBrightness;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the front panel locked state.
 *
 *  This attempts to get the front panel locked state, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aLocked  A mutable reference to storage for the
 *                        front panel locked state, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the front panel model locked
 *                                  state value has not been
 *                                  initialized with a known value.
 *
 *  @sa Init
 *  @sa SetLocked
 *
 */
Status
FrontPanelModel :: GetLocked(LockedType &aLocked) const
{
    Status lRetval = ((mLockedIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aLocked = mLocked;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model brightness setting.
 *
 *  This attempts to set the model with the specified brightness
 *  setting.
 *
 *  @param[in]  aBrightness  An immutable reference to the brightness
 *                           setting to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBrightness
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aBrightness
 *                                    value is out of range.
 *
 */
Status
FrontPanelModel :: SetBrightness(const BrightnessType &aBrightness)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBrightness >= FrontPanelModel::kBrightnessMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aBrightness <= FrontPanelModel::kBrightnessMax, done, lRetval = -ERANGE);

    if (mBrightness == aBrightness)
    {
        lRetval = ((mBrightnessIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mBrightness = aBrightness;
    }

    mBrightnessIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model locked state.
 *
 *  This attempts to set the model with the specified locked state.
 *
 *  @param[in]  aLocked  An immutable reference to the locked state
 *                       to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLocked
 *                                    value has already been set.
 *
 */
Status
FrontPanelModel :: SetLocked(const LockedType &aLocked)
{
    Status lRetval = kStatus_Success;

    if (mLocked == aLocked)
    {
        lRetval = ((mLockedIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mLocked = aLocked;
    }

    mLockedIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided front panel model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aFrontPanelModel  An immutable reference to the
 *                                front panel model to compare
 *                                for equality.
 *
 *  @returns
 *    True if this front panel model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool FrontPanelModel :: operator ==(const FrontPanelModel &aFrontPanelModel) const
{
    return ((mBrightnessIsNull == aFrontPanelModel.mBrightnessIsNull) &&
            (mBrightness       == aFrontPanelModel.mBrightness      ) &&
            (mLockedIsNull     == aFrontPanelModel.mLockedIsNull    ) &&
            (mLocked           == aFrontPanelModel.mLocked          ));
}

}; // namespace Model

}; // namespace HLX
