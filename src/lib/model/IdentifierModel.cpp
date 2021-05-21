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
 *      This file implements an object for managing HLX object
 *      identifiers.
 *
 */

#include "IdentifierModel.hpp"

#include <utility>

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Model
{

/**
 *  The invalid object identifier.
 *
 *  This may be useful as a "nullable" initializer in some contexts to
 *  indicate a null or unset value. Regardless, it is not a valid
 *  identifier.
 *
 */
const IdentifierModel::IdentifierType IdentifierModel::kIdentifierInvalid = 0;

/**
 *  The minimum or lowest object identifier.
 *
 */
const IdentifierModel::IdentifierType IdentifierModel::kIdentifierMin     = 1;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
IdentifierModel :: IdentifierModel(void) :
    mIdentifier(kIdentifierInvalid)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null or invalid identifier.
 *
 *  The identifier must be explicitly set with #SetIdentifier before
 *  #GetIdentifier may be used successfully.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 *  @sa SetIdentifier
 *
 */
Status
IdentifierModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mIdentifier = kIdentifierInvalid;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference for the
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If an invalid identifier was specified.
 *
 */
Status
IdentifierModel :: Init(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = SetIdentifier(aIdentifier);
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
 *  This initializes the class with the specified identifier model.
 *
 *  @param[in]  aIdentifierModel  An immutable reference to the
 *                                identifier model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IdentifierModel :: Init(const IdentifierModel &aIdentifierModel)
{
    Status lRetval = kStatus_Success;

    *this = aIdentifierModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified identifier model to this one.
 *
 *  @param[in]  aIdentifierModel  An immutable reference to the
 *                                identifier model to assign (copy) to
 *                                this one.
 *
 *
 *  @returns
 *    A reference to this identifier model after the assignment (copy)
 *    is complete.
 *
 */
IdentifierModel &
IdentifierModel :: operator =(const IdentifierModel &aIdentifierModel)
{
    mIdentifier = aIdentifierModel.mIdentifier;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model identifier.
 *
 *  This attempts to get the model identifier, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifier value has not
 *                                  been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
IdentifierModel :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = ((mIdentifier == kIdentifierInvalid) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aIdentifier = mIdentifier;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Set the identifier for the model.
 *
 *  This attempts to set the specified identifier for the model.
 *
 *  @param[in]  aIdentifier  The identifier to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifier was already set to
 *                                    the specified value.
 *  @retval  -EINVAL                  If an invalid identifier was specified.
 *
 */
Status
IdentifierModel :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aIdentifier != kIdentifierInvalid, done, lRetval = -EINVAL);

    if (mIdentifier == aIdentifier)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mIdentifier = aIdentifier;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided identifier model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aIdentifierModel  An immutable reference to the
 *                                identifier model to compare
 *                                for equality.
 *
 *  @returns
 *    True if this identifier model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool IdentifierModel :: operator ==(const IdentifierModel &aIdentifierModel) const
{
    return (mIdentifier == aIdentifierModel.mIdentifier);
}

}; // namespace Model

}; // namespace HLX
