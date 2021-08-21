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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP
#define OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/FavoriteModel.hpp>
#include <OpenHLX/Model/FavoritesModel.hpp>
#include <OpenHLX/Server/ControllerBasis.hpp>
#include <OpenHLX/Server/FavoritesControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup favorites
 *
 */
class FavoritesControllerBasis :
    public Server::ControllerBasis
{
public:
    virtual ~FavoritesControllerBasis(void);

protected:
    FavoritesControllerBasis(Model::FavoritesModel &aFavoritesModel,
                             const Model::FavoriteModel::IdentifierType &aFavoritesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

private:
    Model::FavoritesModel &                      mFavoritesModel;
    const Model::FavoriteModel::IdentifierType & mFavoritesMax;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status HandleQueryReceived(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

protected:
    static Server::Command::Favorites::QueryRequest    kQueryRequest;
    static Server::Command::Favorites::SetNameRequest  kSetNameRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP
