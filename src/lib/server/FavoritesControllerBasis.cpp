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
 *      This file implements an object for...
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

// Request data

Server::Command::Favorites::QueryRequest    FavoritesControllerBasis::kQueryRequest;
Server::Command::Favorites::SetNameRequest  FavoritesControllerBasis::kSetNameRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FavoritesControllerBasis :: FavoritesControllerBasis(Model::FavoritesModel &aFavoritesModel,
                                                     const Model::FavoriteModel::IdentifierType &aFavoritesMax) :
    Server::ControllerBasis(),
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

Status
FavoritesControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
FavoritesControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
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

Status
FavoritesControllerBasis :: HandleQueryReceived(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const FavoriteModel *                     lFavoriteModel;
    const char *                              lName;
    Server::Command::Favorites::NameResponse  lResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lRetval;


    lRetval = mFavoritesModel.GetFavorite(aFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lFavoriteModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lResponse.Init(aFavoriteIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

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
