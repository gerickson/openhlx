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
 *      This file implements objects for HLX server favorite data model
 *      commands and their constituent requests and responses.
 *
 */

#include "FavoritesControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Favorites
{

static const char * const kFavoriteObject = "F";

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the favorite query command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite query command response buffer.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the
 *                                   identifier of the favorite to
 *                                   query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier)
{
    return (QueryResponseBasis::Init(kFavoriteObject, aFavoriteIdentifier));
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set name command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite name command response
 *  buffer.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference
 *                                          for the favorite
 *                                          identifier for which to
 *                                          form the name response.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string of the
 *                                          favorite name for
 *                                          which to form the
 *                                          response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                     const char * aName)
{
    return (NameSetResponseBasis::Init(kFavoriteObject,
                                       aFavoriteIdentifier,
                                       aName));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite name command response
 *  buffer.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference
 *                                   for the favorite
 *                                   identifier for which to
 *                                   form the name response.
 *  @param[in]  aName                A pointer to the null-
 *                                   terminated C string of the
 *                                   favorite name for
 *                                   which to form the
 *                                   response.
 *  @param[in]  aNameLength          An immutable reference to
 *                                   the length, in bytes, of
 *                                   @a aName.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kFavoriteObject,
                                       aFavoriteIdentifier,
                                       aName,
                                       aNameLength));
}

}; // namespace Favorites

}; // namespace Command

}; // namespace Server

}; // namespace HLX
