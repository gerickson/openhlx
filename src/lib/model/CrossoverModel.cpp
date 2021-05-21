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
 *      model high- or lowpass crossover data model.
 *
 */

#include "CrossoverModel.hpp"

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

static const CrossoverModel::FrequencyType kFrequencyDefault = 100;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  @param[in]  aFrequencyLimits  An immutable reference to the
 *                                crossover frequency limits to
 *                                construct the model with.
 *
 *  This constructions a crossover model with the specified frequency
 *  limits.
 *
 */
CrossoverModel :: CrossoverModel(const FrequencyLimits &aFrequencyLimits) :
    mFrequencyLimits(aFrequencyLimits),
    mFrequencyIsNull(true),
    mFrequency(kFrequencyDefault)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null filter crossover frequency.
 *
 *  The frequency must be explicitly set with #SetFrequency before
 *  #GetFrequency may be used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetFrequency
 *
 */
Status
CrossoverModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mFrequency         = kFrequencyDefault;
    mFrequencyIsNull   = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified crossover frequency.
 *
 *  @param[in]  aFrequency  An immutable reference to the crossover
 *                          frequency to initialize the model with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ERANGE                  The specified @a aFrequency value
 *                                    is out of range.
 *
 */
Status
CrossoverModel :: Init(const FrequencyType &aFrequency)
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
 *  This initializes the class with the specified crossover model.
 *
 *  @param[in]  aCrossoverModel  An immutable reference to the crossover
 *                               model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
CrossoverModel :: Init(const CrossoverModel &aCrossoverModel)
{
    Status lRetval = kStatus_Success;

    *this = aCrossoverModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified crossover model to this one.
 *
 *  @param[in]  aCrossoverModel  An immutable reference to the crossover
 *                               model to assign (copy) to this one.
 *
 *
 *  @returns
 *    A reference to this crossover model after the assignment (copy)
 *    is complete.
 *
 */
CrossoverModel &
CrossoverModel :: operator =(const CrossoverModel &aCrossoverModel)
{
    mFrequencyIsNull      = aCrossoverModel.mFrequencyIsNull;
    mFrequency            = aCrossoverModel.mFrequency;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the filter crossover frequency limits.
 *
 *  This attempts to get the filter crossover frequency limits.
 *
 *  @param[out]  aFrequencyLimits  A mutable reference to storage for the
 *                                 filter crossover frequency limits.
 *
 *  @retval  kStatus_Success        If successful.
 *
 */
Status
CrossoverModel :: GetFrequencyLimits(FrequencyLimits &aFrequencyLimits) const
{
    Status  lRetval = kStatus_Success;


    aFrequencyLimits.mMin = mFrequencyLimits.mMin;
    aFrequencyLimits.mMax = mFrequencyLimits.mMax;

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the filter crossover frequency.
 *
 *  This attempts to get the filter crossover frequency, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aFrequency  A mutable reference to storage for the
 *                           filter crossover frequency, if
 *                           successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the crossover model frequency
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetFrequency
 *
 */
Status
CrossoverModel :: GetFrequency(FrequencyType &aFrequency) const
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
 *    This sets the model filter crossover frequency.
 *
 *  This initializes the model with the specified filter crossover
 *  frequency.
 *
 *  @param[in]  aFrequency  An immutable reference to the filter
 *                          crossover frequency to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aFrequency
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aFrequency
 *                                    value is out of range.
 *
 */
Status
CrossoverModel :: SetFrequency(const FrequencyType &aFrequency)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aFrequency >= mFrequencyLimits.mMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aFrequency <= mFrequencyLimits.mMax, done, lRetval = -ERANGE);

    if (mFrequency == aFrequency)
    {
        lRetval = ((mFrequencyIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mFrequency = aFrequency;
    }

    mFrequencyIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided crossover model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aCrossoverModel  An immutable reference to the
 *                               crossover model to compare
 *                               for equality.
 *
 *  @returns
 *    True if this crossover model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool CrossoverModel :: operator ==(const CrossoverModel &aCrossoverModel) const
{
    return ((mFrequencyLimits.mMin == aCrossoverModel.mFrequencyLimits.mMin) &&
            (mFrequencyLimits.mMax == aCrossoverModel.mFrequencyLimits.mMax) &&
            (mFrequencyIsNull      == aCrossoverModel.mFrequencyIsNull     ) &&
            (mFrequency            == aCrossoverModel.mFrequency           ));
}

}; // namespace Model

}; // namespace HLX
