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
 *      This file defines objects for HLX client favorite data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef OPENHLXCLIENTFAVORITESCONTROLLERCOMMANDS_HPP
#define OPENHLXCLIENTFAVORITESCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandNameSetRequestBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/CommandFavoritesRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Favorites
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client favorite data model query command request
 *    buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class QueryRequest :
    virtual public Client::Command::RequestBasis,
    public Client::Command::QueryRequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client favorite data model query command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::Favorites::QueryRegularExpressionBasis
{
public:
    QueryResponse(void) = default;
    virtual ~QueryResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client favorite data model query command request
 *    / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

// MARK: Mutator Requests, Responses, and Commands


// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client favorite name data model property mutation
 *    command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class NameResponse :
    public ResponseBasis,
    public Common::Command::Favorites::NameRegularExpressionBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client favorite name data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class SetNameRequest :
    public NameSetRequestBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier, const char *aName);
};

/**
 *  @brief
 *    An object for a HLX client favorite name data model property mutation
 *    command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup favorite
 *
 */
class SetName :
    public ExchangeBasis
{
public:
    SetName(void) = default;
    virtual ~SetName(void) = default;

    Common::Status Init(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier, const char *aName);

private:
    SetNameRequest mRequest;
    NameResponse   mResponse;
};

}; // namespace Favorites

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFAVORITESCONTROLLERCOMMANDS_HPP
