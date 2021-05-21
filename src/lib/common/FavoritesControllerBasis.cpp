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
 *      This file implements a base object for realizing a HLX favorites
 *      controller, either in a client or server context.
 *
 */

#include "FavoritesControllerBasis.hpp"

#include <errno.h>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

/**
 *  The maximum number of favorites supported by the HLX server
 *  controller.
 *
 */
const FavoritesControllerBasis::IdentifierType  FavoritesControllerBasis::kFavoritesMax = 10;

// MARK: Observer Methods

/**
 *  @brief
 *    Determine whether or not an favorite identifier is valid.
 *
 *  This determines whether or not the specified favorite
 *  identifier is valid.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the
 *                                   favorite identifer to validate.
 *
 *  @returns
 *    True if the identifier is valid; otherwise, false.
 *
 */
bool
FavoritesControllerBasis :: IsValidIdentifier(const IdentifierType &aFavoriteIdentifier)
{
    const bool lIsValid = (ValidateIdentifier(aFavoriteIdentifier) == kStatus_Success);


    return (lIsValid);
}

/**
 *  @brief
 *    Validate an favorite identifier.
 *
 *  This validates the specified favorite identifier.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the
 *                                   favorite identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the favorite identifier is
 *                            smaller or larger than supported.
 *
 */
Status
FavoritesControllerBasis :: ValidateIdentifier(const IdentifierType &aFavoriteIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aFavoriteIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aFavoriteIdentifier <= kFavoritesMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
