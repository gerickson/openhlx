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
 *      This file implements an object for managing an HLX stereophonic
 *      sound mode channel balance data model.
 *
 */

#include "BalanceModel.hpp"

#include <utility>

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The channel type value for indicating a bias towards or for
 *  representing the left stereophonic channel.
 *
 */
const BalanceModel::ChannelType BalanceModel::kChannelLeft   = 'L';

/**
 *  The channel type value for indicating a bias towards or for
 *  representing the right stereophonic channel.
 *
 */
const BalanceModel::ChannelType BalanceModel::kChannelRight  = 'R';

/**
 *  The balance type value for indicating the minimum stereophonic
 *  channel balance that also corresponds to a balance that is fully
 *  biased towards the right channel.
 *
 */
const BalanceModel::BalanceType BalanceModel::kBalanceMax    =  80;

/**
 *  The balance type value for indicating a centered stereophonic
 *  channel balance.
 *
 */
const BalanceModel::BalanceType BalanceModel::kBalanceCenter =   0;

/**
 *  The balance type value for indicating the minimum stereophonic
 *  channel balance that also corresponds to a balance that is fully
 *  biased towards the left channel.
 *
 */
const BalanceModel::BalanceType BalanceModel::kBalanceMin    = -80;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
BalanceModel :: BalanceModel(void) :
    mBalanceIsNull(true),
    mBalance(kBalanceCenter)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null stereophonic channel
 *  balance.
 *
 *  The balance must be explicitly set with #SetBalance before
 *  #GetBalance, #IncreaseBalanceLeft, or #IncreaseBalanceRight may be
 *  used successfully.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetBalance
 *
 */
Status
BalanceModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mBalanceIsNull = true;
    mBalance       = kBalanceCenter;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified stereophonic channel
 *  balance.
 *
 *  @param[in]  aBalance  An immutable reference to the stereophonic
 *                        channel balance to initialize the model with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ERANGE                  The specified @a aBalance value
 *                                    is out of range.
 *
 */
Status
BalanceModel :: Init(const BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = SetBalance(aBalance);
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
 *  This initializes the class with the specified balance model.
 *
 *  @param[in]  aBalanceModel  An immutable reference to the balance
 *                             model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceModel :: Init(const BalanceModel &aBalanceModel)
{
    Status lRetval = kStatus_Success;

    *this = aBalanceModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified balance model to this one.
 *
 *  @param[in]  aBalanceModel  An immutable reference to the balance
 *                             model to assign (copy) to this one.
 *
 *
 *  @returns
 *    A reference to this balance model after the assignment (copy) is
 *    complete.
 *
 */
BalanceModel &
BalanceModel :: operator =(const BalanceModel &aBalanceModel)
{
    mBalanceIsNull = aBalanceModel.mBalanceIsNull;
    mBalance       = aBalanceModel.mBalance;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the stereophonic channel balance.
 *
 *  This attempts to get the stereophonic channel balance, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aBalance  A mutable reference to storage for the
 *                         stereophonic channel balance, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *
 *  @sa Init
 *  @sa SetBalance
 *
 */
Status
BalanceModel :: GetBalance(BalanceType &aBalance) const
{
    Status lRetval = ((mBalanceIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aBalance = mBalance;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Increase the model stereophonic channel balance by one (1)
 *    towards the left channel.
 *
 *  This attempts to increase the model stereophonic channel balance by
 *  one (1) unit towards the left channel.
 *
 *  @param[out]  aOutBalance   A mutable reference to storage for the
 *                             resulting stereophonic channel balance,
 *                             if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a balance that would exceed the
 *                                  maximum left channel bias.
 *
 *  @sa Init
 *  @sa SetBalance
 *
 */
Status
BalanceModel :: IncreaseBalanceLeft(BalanceType &aOutBalance)
{
    static const BalanceType kAdjustment = -1;

    return (AdjustBalance(kAdjustment, aOutBalance));
}

/**
 *  @brief
 *    Increase the model stereophonic channel balance by one (1)
 *    towards the right channel.
 *
 *  This attempts to increase the model stereophonic channel balance by
 *  one (1) unit towards the right channel.
 *
 *  @param[out]  aOutBalance   A mutable reference to storage for the
 *                             resulting stereophonic channel balance,
 *                             if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a balance that would exceed the
 *                                  maximum right channel bias.
 *
 *  @sa Init
 *  @sa SetBalance
 *
 */
Status
BalanceModel :: IncreaseBalanceRight(BalanceType &aOutBalance)
{
    static const BalanceType kAdjustment = 1;

    return (AdjustBalance(kAdjustment, aOutBalance));
}

/**
 *  @brief
 *    Adjust the model stereophonic channel balance by the specified
 *    amount.
 *
 *  This attempts to adjust the model stereophonic channel balance by
 *  by the specified amount.
 *
 *  @param[in]   aAdjustment   The amount to adjust the balance by.
 *                             Positive numbers adjust the balance
 *                             towards the right channel; negative
 *                             values adjust the balance towards the
 *                             left channel.
 *  @param[out]  aOutBalance   A mutable reference to storage for the
 *                             resulting stereophonic channel balance,
 *                             if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The specified @a aBalance value is
 *                                  out of range.
 *
 *  @sa Init
 *  @sa SetBalance
 *
 */
Status
BalanceModel :: AdjustBalance(const BalanceType &aAdjustment, BalanceType &aOutBalance)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!mBalanceIsNull, done, lRetval = kError_NotInitialized);

    lRetval = SetBalance(mBalance + aAdjustment);
    nlEXPECT_SUCCESS(lRetval, done);

    aOutBalance = mBalance;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model stereophonic channel balance.
 *
 *  This initializes the model with the specified stereophonic channel
 *  balance.
 *
 *  @param[in]  aBalance  An immutable reference to the stereophonic
 *                        channel balance to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBalance value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aBalance value
 *                                    is out of range.
 *
 */
Status
BalanceModel :: SetBalance(const BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBalance >= BalanceModel::kBalanceMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aBalance <= BalanceModel::kBalanceMax, done, lRetval = -ERANGE);

    if (mBalance == aBalance)
    {
        lRetval = ((mBalanceIsNull) ? kStatus_Success : kStatus_ValueAlreadySet);
    }
    else
    {
        mBalance = aBalance;
    }

    mBalanceIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided balance model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aBalanceModel  An immutable reference to the balance
 *                             model to compare for equality.
 *
 *  @returns
 *    True if this balance model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool BalanceModel :: operator ==(const BalanceModel &aBalanceModel) const
{
    return ((mBalanceIsNull == aBalanceModel.mBalanceIsNull) &&
            (mBalance       == aBalanceModel.mBalance      ));
}

}; // namespace Model

}; // namespace HLX
