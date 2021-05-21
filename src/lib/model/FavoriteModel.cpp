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
 *      This file implements an object for managing a HLX favorite
 *      data model.
 *
 */

#include "FavoriteModel.hpp"

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The maximum allowed length, in bytes, of a favorite name.
 *
 */
const size_t FavoriteModel::kNameLengthMax = NameModel::kNameLengthMax;

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name and identifier.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *  @sa SetName
 *
 */
Status
FavoriteModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the favorite name with.
 *  @param[in]  aIdentifier  An immutable reference for the favorite
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
FavoriteModel :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent and
 *  identifier.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *  @param[in]  aIdentifier  An immutable reference for the favorite
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
FavoriteModel :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName, aNameLength);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           favorite name with.
 *  @param[in]  aIdentifier  An immutable reference for the favorite
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
FavoriteModel :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified favorite model.
 *
 *  @param[in]  aFavoriteModel  An immutable reference to the
 *                              favorite model to initialize
 *                              with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
FavoriteModel :: Init(const FavoriteModel &aFavoriteModel)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aFavoriteModel.mIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aFavoriteModel.mName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified favorite model to this one.
 *
 *  @param[in]  aFavoriteModel  An immutable reference to the
 *                              favorite model to assign (copy)
 *                              to this one.
 *
 *
 *  @returns
 *    A reference to this favorite model after the assignment (copy)
 *    is complete.
 *
 */
FavoriteModel &
FavoriteModel :: operator =(const FavoriteModel &aFavoriteModel)
{
    mIdentifier = aFavoriteModel.mIdentifier;
    mName       = aFavoriteModel.mName;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the favorite identifier.
 *
 *  This attempts to get the favorite identifier, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            favorite identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorite identifier
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
FavoriteModel :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.GetIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the favorite name
 *
 *  This attempts to get the favorite name, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the favorite
 *                      name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorite name value has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 *  @ingroup name
 *
 */
Status
FavoriteModel :: GetName(const char *&aName) const
{
    Status lRetval = kStatus_Success;

    lRetval = mName.GetName(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model favorite identifier.
 *
 *  This attempts to set the model with the favorite identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference to the favorite
 *                           identifier to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aIdentifier value
 *                                    has already been set.
 *  @retval  -EINVAL                  The specified @a aIdentifier value
 *                                    is invalid.
 *
 */
Status
FavoriteModel :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.SetIdentifier(aIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model favorite name.
 *
 *  This attempts to set the model with the specified favorite name.
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
 *  @ingroup name
 *
 */
Status
FavoriteModel :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model favorite name.
 *
 *  This attempts to set the model with the specified favorite name
 *  extent.
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
 *  @ingroup name
 *
 */
Status
FavoriteModel :: SetName(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName, aNameLength);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided favorite model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aFavoriteModel  An immutable reference to the
 *                              favorite model to compare for
 *                              equality.
 *
 *  @returns
 *    True if this favorite model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool FavoriteModel :: operator ==(const FavoriteModel &aFavoriteModel) const
{
    return ((mName       == aFavoriteModel.mName      ) &&
            (mIdentifier == aFavoriteModel.mIdentifier));
}

}; // namespace Model

}; // namespace HLX
