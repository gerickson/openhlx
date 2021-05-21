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
 *      This file defines a collection object for managing HLX favorite
 *      objects.
 *
 */

#ifndef HLXMMODELFAVORITESMODEL_HPP
#define HLXMMODELFAVORITESMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX favorite objects.
 *
 *  @ingroup model
 *
 */
class FavoritesModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  favorite model.
     *
     */
    typedef FavoriteModel::IdentifierType IdentifierType;

public:
    FavoritesModel(void);
    FavoritesModel(const FavoritesModel &aFavoritesModel);
    virtual ~FavoritesModel(void) = default;

    Common::Status Init(const size_t &aFavoritesMax);
    Common::Status Init(const FavoritesModel &aFavoritesModel);

    FavoritesModel &operator =(const FavoritesModel &aFavoritesModel);

    Common::Status GetFavorite(const IdentifierType &aFavoriteIdentifier, FavoriteModel *&aFavoriteModel);
    Common::Status GetFavorite(const IdentifierType &aFavoriteIdentifier, const FavoriteModel *&aFavoriteModel) const;
    Common::Status GetFavorite(const char *aName, const FavoriteModel *&aFavoriteModel) const;

    Common::Status SetFavorite(const IdentifierType &aFavoriteIdentifier, const FavoriteModel &aFavoriteModel);

    bool operator ==(const FavoritesModel &aFavoritesModel) const;

private:
    Common::Status ValidateIdentifier(const IdentifierType &aFavoriteIdentifier) const;

private:
    typedef std::map<IdentifierType, FavoriteModel> Favorites;

    size_t     mFavoritesMax;
    Favorites  mFavorites;
};

}; // namespace Model

}; // namespace HLX

#endif // HLXMMODELFAVORITESMODEL_HPP
