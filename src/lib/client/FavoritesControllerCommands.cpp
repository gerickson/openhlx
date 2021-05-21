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
 *      This file implements objects for HLX client favorite data model
 *      commands and their constituent requests and responses.
 *
 */

#include "FavoritesControllerCommands.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Favorites
{

static const char * const kFavoriteObject = "F";

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite query command request buffer.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the identifier
 *                                   of the favorite to query.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier)
{
    return (QueryRequestBasis::Init(kFavoriteObject, aFavoriteIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the favorite query command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite query command.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the identifier
 *                                   of the favorite to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
Query :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite set name command request
 *  buffer.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference for the
 *                                   favorite for which to set the
 *                                   name.
 *  @param[in]  aName                A pointer to the null-terminated
 *                                   C string to set the favorite name
 *                                   to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetNameRequest :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                       const char *aName)
{
    return (NameSetRequestBasis::Init(kFavoriteObject,
                                      aFavoriteIdentifier,
                                      aName));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the favorite name command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the favorite set name command.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference for the
 *                                   favorite for which to set the
 *                                   name.
 *  @param[in]  aName                A pointer to the null-terminated
 *                                   C string to set the favorite name
 *                                   to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetName :: Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aFavoriteIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Favorites

}; // namespace Command

}; // namespace Client

}; // namespace HLX
