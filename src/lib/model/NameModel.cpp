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
 *      This file implements an object for managing a HLX object name
 *      data model.
 *
 */

#include "NameModel.hpp"

#include <errno.h>
#include <string.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The maximum allowed length, in bytes, of a name.
 *
 */
const size_t NameModel::kNameLengthMax = 16;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NameModel :: NameModel(void) :
    mNameIsNull(true),
    mName()
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 *  @sa SetName
 *
 */
Status
NameModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    mName.clear();

    mNameIsNull = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the name with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
NameModel :: Init(const char *aName)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = Init(aName, strlen(aName));

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
NameModel :: Init(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;
    std::string lName;

    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aNameLength <= NameModel::kNameLengthMax, done, lRetval = -ENAMETOOLONG);

    mName.assign(aName, aNameLength);

    mNameIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           name with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
NameModel :: Init(const std::string &aName)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aName.size() <= NameModel::kNameLengthMax, done, lRetval = -ENAMETOOLONG);

    mName       = aName;
    mNameIsNull = false;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified name model.
 *
 *  @param[in]  aNameModel  An immutable reference to the
 *                          name model to initialize with.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
NameModel :: Init(const NameModel &aNameModel)
{
    Status lRetval = kStatus_Success;

    *this = aNameModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified name model to this one.
 *
 *  @param[in]  aNameModel  An immutable reference to the
 *                          name model to assign (copy)
 *                          to this one.
 *
 *
 *  @returns
 *    A reference to this name model after the assignment (copy)
 *    is complete.
 *
 */
NameModel &
NameModel :: operator =(const NameModel &aNameModel)
{
    mName       = aNameModel.mName;
    mNameIsNull = aNameModel.mNameIsNull;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the model name
 *
 *  This attempts to get the model name, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the model
 *                      name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model name value has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetName
 *
 */
Status
NameModel :: GetName(const char *&aName) const
{
    Status lRetval = ((mNameIsNull) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aName = mName.c_str();
    }

    return (lRetval);
}

/**
 *  @brief
 *    This sets the model name.
 *
 *  This attempts to set the model with the specified name.
 *
 *  @param[in]  aName        A pointer to the start of the null-
 *                           terminated C string name to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aName was too long.
 *
 */
Status
NameModel :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = SetName(aName, strlen(aName));

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model name.
 *
 *  This attempts to set the model with the specified name extent.
 *
 *  @param[in]  aName        A pointer to the start of the string name
 *                           to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aNameLength was too long.
 *
 */
Status
NameModel :: SetName(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    if (strncmp(mName.c_str(), aName, aNameLength) == 0)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        lRetval = Init(aName, aNameLength);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  @param[in]  aName  A pointer to a null-terminated C string
 *                     to compare against this model name for
 *                     equality.
 *
 *  @returns
 *    True if this name is equal to the specified name;
 *    otherwise, false.
 *
 */
bool
NameModel :: operator ==(const char *aName) const
{
    return (!mNameIsNull && (mName == aName));
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  @param[in]  aName  An immutable reference to a C++ Standard
 *                     Library string to compare against this
 *                     model name for equality.
 *
 *  @returns
 *    True if this name is equal to the specified name;
 *    otherwise, false.
 *
 */
bool
NameModel :: operator ==(const std::string &aName) const
{
    return (!mNameIsNull && (mName == aName));
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided name model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aNameModel  An immutable reference to the name
 *                          model to compare for equality.
 *
 *  @returns
 *    True if this name model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
NameModel :: operator ==(const NameModel &aNameModel) const
{
    return ((mNameIsNull == aNameModel.mNameIsNull) &&
            (mName       == aNameModel.mName));
}

}; // namespace Model

}; // namespace HLX
