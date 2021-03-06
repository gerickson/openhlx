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
 *      This file defines objects for HLX server favorite data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef OPENHLXSERVERFAVORITESCONTROLLERCOMMANDS_HPP
#define OPENHLXSERVERFAVORITESCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandFavoritesRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>

#include "CommandNameSetResponseBasis.hpp"
#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Favorites
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server favorite data model query command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Favorites::QueryRegularExpressionBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server favorite data model query command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class QueryResponse :
    public QueryResponseBasis
{
public:
    QueryResponse(void) = default;
    virtual ~QueryResponse(void) = default;

    // Allow both the base and derived class initializers

    using QueryResponseBasis::Init;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier);
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server favorite name data model property mutation
 *    command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class SetNameRequest :
    public RequestBasis,
    public Common::Command::Favorites::NameRegularExpressionBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server favorite name data model property mutation
 *    command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class NameResponse :
    public NameSetResponseBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                        const char * aName);
    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier,
                        const char * aName,
                        const size_t &aNameLength);
};

}; // namespace Favorites

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERFAVORITESCONTROLLERCOMMANDS_HPP
