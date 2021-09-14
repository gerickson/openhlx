/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a collection of HLX favorites.
 *
 */

#ifndef OPENHLXCLIENTFAVORITESCONTROLLER_HPP
#define OPENHLXCLIENTFAVORITESCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/FavoritesControllerBasis.hpp>
#include <OpenHLX/Client/FavoritesControllerCommands.hpp>
#include <OpenHLX/Common/FavoritesControllerBasis.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>
#include <OpenHLX/Model/FavoritesModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX favorites.
 *
 *  @ingroup client
 *  @ingroup favorite
 *
 */
class FavoritesController :
    public Common::FavoritesControllerBasis,
    public Client::FavoritesControllerBasis
{
public:
    FavoritesController(void);
    virtual ~FavoritesController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status GetFavorite(const IdentifierType &aIdentifier, const Model::FavoriteModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aFavoriteIdentifier) const;

    // Mutator Methods

    Common::Status SetName(const IdentifierType &aFavoriteIdentifier, const char *aName);

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFAVORITESCONTROLLER_HPP
