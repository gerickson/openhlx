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
 *      This file defines a base object for realizing a HLX favorites
 *      controller, either in a client or server context.
 *
 */

#ifndef HLXCOMMONDFAVORITESCONTROLLERBASIS_HPP
#define HLXCOMMONDFAVORITESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/FavoriteModel.hpp>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX favorites controller, either in a
 *    client or server context.
 *
 *  @ingroup common
 *
 */
class FavoritesControllerBasis
{
public:
    /**
     *  A locally-scoped convenience type for a favorite identifier.
     *
     */
    typedef Model::FavoriteModel::IdentifierType IdentifierType;

public:
    FavoritesControllerBasis(void) = default;

    // Observer Methods

    static bool           IsValidIdentifier(const IdentifierType &aFavoriteIdentifier);
    static Common::Status ValidateIdentifier(const IdentifierType &aFavoriteIdentifier);

protected:
    virtual ~FavoritesControllerBasis(void) = default;

protected:
    static const IdentifierType  kFavoritesMax;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONDFAVORITESCONTROLLERBASIS_HPP
