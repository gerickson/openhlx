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
 *      This file implements a collection object for managing HLX favorite
 *      objects.
 *
 */

#include "FavoritesModel.hpp"

#include <utility>

#include <errno.h>
#include <string.h>

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
FavoritesModel :: FavoritesModel(void) :
    mFavoritesMax(0),
    mFavorites()
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aFavoritesModel  An immutable reference to the
 *                               favorites model to copy from.
 *
 */
FavoritesModel :: FavoritesModel(const FavoritesModel &aFavoritesModel) :
    mFavorites(aFavoritesModel.mFavorites)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified number of default
 *  favorites.
 *
 *  @param[in]  aFavoritesMax  An immutable reference to the maximum
 *                             number of favorites to allow in the
 *                             collection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
FavoritesModel :: Init(const size_t &aFavoritesMax)
{
    FavoriteModel lFavoriteModel;
    Status        lRetval = kStatus_Success;


    mFavoritesMax = aFavoritesMax;

    lRetval = lFavoriteModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    for (IdentifierType lFavorite = IdentifierModel::kIdentifierMin; (lFavorite <= aFavoritesMax); lFavorite++)
    {
        lRetval = lFavoriteModel.SetIdentifier(lFavorite);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetFavorite(lFavorite, lFavoriteModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified favorites
 *  model.
 *
 *  @param[in]  aFavoritesModel  An immutable reference to the
 *                               favorites model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of favorites in @a
 *                            aFavoritesModel exceeds kFavoritesMax.
 *
 */
Status
FavoritesModel :: Init(const FavoritesModel &aFavoritesModel)
{
    Status lRetval = kStatus_Success;


    *this = aFavoritesModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified favorites model to this
 *  one.
 *
 *  @param[in]  aFavoritesModel  An immutable reference to the
 *                               favorites model to assign (copy) to
 *                               this one.
 *
 *
 *  @returns
 *    A reference to this favorites model after the assignment
 *    (copy) is complete.
 *
 */
FavoritesModel &
FavoritesModel :: operator =(const FavoritesModel &aFavoritesModel)
{
    mFavorites = aFavoritesModel.mFavorites;

    return (*this);
}

/**
 *  @brief
 *    Validate an favorite identifier.
 *
 *  This validates the specified favorite identifier.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to
 *                                   the favorite identifer to
 *                                   validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the favorite identifier is
 *                            smaller or larger than supported.
 *
 */
Status
FavoritesModel :: ValidateIdentifier(const IdentifierType &aFavoriteIdentifier) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aFavoriteIdentifier != IdentifierModel::kIdentifierInvalid, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aFavoriteIdentifier <= mFavoritesMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the favorite model associated with the specified
 *    favorite identifier.
 *
 *  This attempts to get the favorite model associated with the
 *  specified favorite identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aFavoriteIdentifier  An immutable reference to
 *                                    the favorite identifier
 *                                    corresponding to the favorite
 *                                    model to get.
 *  @param[out]  aFavoriteModel       A reference to a pointer
 *                                    for a mutable favorite model,
 *                                    set to the requested favorite
 *                                    model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorites model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
FavoritesModel :: GetFavorite(const IdentifierType &aFavoriteIdentifier, FavoriteModel *&aFavoriteModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aFavoriteModel = &mFavorites.at(aFavoriteIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the favorite model associated with the specified
 *    favorite identifier.
 *
 *  This attempts to get the favorite model associated with the
 *  specified favorite identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aFavoriteIdentifier  An immutable reference to
 *                                    the favorite identifier
 *                                    corresponding to the favorite
 *                                    model to get.
 *  @param[out]  aFavoriteModel       A reference to a pointer for
 *                                    for an imutable favorite model,
 *                                    set to the requested favorite
 *                                    model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorites model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
FavoritesModel :: GetFavorite(const IdentifierType &aFavoriteIdentifier, const FavoriteModel *&aFavoriteModel) const
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aFavoriteModel = &mFavorites.at(aFavoriteIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the favorite identifier with the specified name.
 *
 *  This attempts to get the favorite model for the favorite with the
 *  specified name.
 *
 *  @param[in]   aName           A pointer to a null-terminated C
 *                               string of the name of the favorite to
 *                               get.
 *  @param[out]  aFavoriteModel  A reference to storage by which to
 *                               return a pointer to the model, if
 *                               successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No favorite could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
FavoritesModel :: GetFavorite(const char *aName, const FavoriteModel *&aFavoriteModel) const
{
    Status lRetval = -ENOENT;
    Favorites::const_iterator current, end;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    current = mFavorites.begin();
    end     = mFavorites.end();

    while (current != end)
    {
        const char *  lName;
        Status        lStatus;


        lStatus = current->second.GetName(lName);
        nlREQUIRE_SUCCESS(lStatus, next);

        if (strcmp(lName, aName) == 0)
        {
            aFavoriteModel = &current->second;
            lRetval        = kStatus_Success;
            break;
        }

    next:
        current++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model favorite for the specified identifier.
 *
 *  This initializes the model with the specified favorite model
 *  at the specified identifier.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to
 *                                   the favorite identifier
 *                                   corresponding to the favorite
 *                                   model to set.
 *  @param[in]  aFavoriteModel       An immutable reference to
 *                                   the favorite model to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aFavorite
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aFavoriteIdentifier value
 *                                    is out of range.
 *
 */
Status
FavoritesModel :: SetFavorite(const IdentifierType &aFavoriteIdentifier, const FavoriteModel &aFavoriteModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mFavorites[aFavoriteIdentifier] == aFavoriteModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mFavorites[aFavoriteIdentifier] = aFavoriteModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided favorites model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aFavoritesModel  An immutable reference to the
 *                               favorites model to compare
 *                               for equality.
 *
 *  @returns
 *    True if this favorites model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
FavoritesModel :: operator ==(const FavoritesModel &aFavoritesModel) const
{
    return (mFavorites == aFavoritesModel.mFavorites);
}

}; // namespace Model

}; // namespace HLX
