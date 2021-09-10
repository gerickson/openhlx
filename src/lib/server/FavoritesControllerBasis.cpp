/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements a derivable object for realizing a HLX
 *      favorites controller, in a server.
 *
 */

#include "FavoritesControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Class-scoped Command Request Regular Expression Data

/**
 *  Class-scoped server favorite query command request regular expression.
 *
 */
Server::Command::Favorites::QueryRequest    FavoritesControllerBasis::kQueryRequest;

/**
 *  Class-scoped server favorite set name command request regular
 *  expression.
 *
 */
Server::Command::Favorites::SetNameRequest  FavoritesControllerBasis::kSetNameRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the favorites controller with the specified
 *  favorites collection model and the maximum number of allowed
 *  favorites.
 *
 *  @param[in]  aFavoritesModel  A mutable reference to the
 *                               favorites collection model to
 *                               construct the controller with. This
 *                               is retained by a weak pointer
 *                               reference and, consequently, must
 *                               remain in scope for the lifetime of
 *                               the controller.
 *  @param[in]  aFavoritesMax    An immutable reference to the
 *                               maximum number of allowed favorites
 *                               managed by the controller.
 *
 */
FavoritesControllerBasis :: FavoritesControllerBasis(Model::FavoritesModel &aFavoritesModel,
                                                     const Model::FavoriteModel::IdentifierType &aFavoritesMax) :
    Server::ObjectControllerBasis(),
    mFavoritesModel(aFavoritesModel),
    mFavoritesMax(aFavoritesMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
FavoritesControllerBasis :: ~FavoritesControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FavoritesControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Implementation

Status
FavoritesControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a favorite
 *    query request of all favorites.
 *
 *  This handles and generates the server command response for a
 *  favorite query request of all favorites.
 *
 *  @param[in,out]  aBuffer  A mutable reference to the shared
 *                           pointer into which the response is to be
 *                           generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorites model has not
 *                                  been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a favorite identifier is
 *                                  smaller or larger than supported.
 *
 */
Status
FavoritesControllerBasis :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;


    for (auto lFavoriteIdentifier = IdentifierModel::kIdentifierMin; lFavoriteIdentifier <= mFavoritesMax; lFavoriteIdentifier++)
    {
        lRetval = HandleQueryReceived(lFavoriteIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a favorite
 *    query request of a specific favorite.
 *
 *  This handles and generates the server command response for a
 *  favorite query request of a specific favorite.
 *
 *  @param[in]      aFavoriteIdentifier  An immutable reference
 *                                       to the identifier of
 *                                       the favorite queried.
 *  @param[in,out]  aBuffer              A mutable reference to
 *                                       the shared pointer into
 *                                       which the response is
 *                                       to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the favorites model has not
 *                                  been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a favorite identifier is
 *                                  smaller or larger than supported.
 *
 */
Status
FavoritesControllerBasis :: HandleQueryReceived(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const FavoriteModel *                     lFavoriteModel;
    const char *                              lName;
    Server::Command::Favorites::NameResponse  lNameResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lRetval;


    lRetval = mFavoritesModel.GetFavorite(aFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lFavoriteModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aFavoriteIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

}; // namespace Server

}; // namespace HLX
