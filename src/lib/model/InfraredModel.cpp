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
 *      This file implements an object for managing the HLX infrared
 *      remote control data model.
 *
 */

#include "InfraredModel.hpp"

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
 *  @brief
 *    This is the class default constructor.
 *
 */
InfraredModel :: InfraredModel(void) :
    mDisabledIsNull(true),
    mDisabled(true)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null disabled state.
 *
 *  The disabled state must be explicitly set with #SetDisabled
 *  before #GetDisabled may be used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetDisabled
 *
 */
Status
InfraredModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mDisabled            = true;
    mDisabledIsNull      = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified disabled state.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled state
 *                         to set.
 *
 *  @retval  kStatus_Success          If successful.
 *
 */
Status
InfraredModel :: Init(const DisabledType &aDisabled)
{
    Status lRetval = kStatus_Success;

    lRetval = SetDisabled(aDisabled);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified infrared remote
 *  control model.
 *
 *  @param[in]  aInfraredModel  An immutable reference to the
 *                              infrared model to initialize
 *                              with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
InfraredModel :: Init(const InfraredModel &aInfraredModel)
{
    Status lRetval = kStatus_Success;

    lRetval = Init(aInfraredModel.mDisabled);
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
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified infrared remote control
 *  model to this one.
 *
 *  @param[in]  aInfraredModel  An immutable reference to the
 *                              infrared model to assign (copy)
 *                              to this one.
 *
 *
 *  @returns
 *    A reference to this infrared remote control model after the
 *    assignment (copy) is complete.
 *
 */
InfraredModel &
InfraredModel :: operator =(const InfraredModel &aInfraredModel)
{
    mDisabledIsNull      = aInfraredModel.mDisabledIsNull;
    mDisabled            = aInfraredModel.mDisabled;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the infrared remote control disabled state.
 *
 *  This attempts to get the infrared remote control disabled state,
 *  if it has been previously initialized or set.
 *
 *  @param[out]  aDisabled  A mutable reference to storage for the
 *                          infrared remote control disabled state,
 *                          if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the infrared model disabled
 *                                  state value has not been
 *                                  initialized with a known value.
 *
 *  @sa Init
 *  @sa SetDisabled
 *
 */
Status
InfraredModel :: GetDisabled(DisabledType &aDisabled) const
{
    Status lRetval = ((mDisabledIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aDisabled = mDisabled;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model remote control disabled state.
 *
 *  This attempts to set the model with the specified disabled state.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled state
 *                         to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aDisabled
 *                                    value has already been set.
 *
 */
Status
InfraredModel :: SetDisabled(const DisabledType &aDisabled)
{
    Status lRetval = kStatus_Success;

    if (mDisabled == aDisabled)
    {
        lRetval = ((mDisabledIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mDisabled = aDisabled;
    }

    mDisabledIsNull = false;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided infrared model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aInfraredModel  An immutable reference to the
 *                              infrared model to compare for
 *                              equality.
 *
 *  @returns
 *    True if this infrared model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool InfraredModel :: operator ==(const InfraredModel &aInfraredModel) const
{
    return ((mDisabledIsNull   == aInfraredModel.mDisabledIsNull) &&
            (mDisabled         == aInfraredModel.mDisabled      ));
}

}; // namespace Model

}; // namespace HLX
