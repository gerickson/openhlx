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
 *      This file defines objects for HLX client network data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTNETWORKCONTROLLERCOMMANDS_HPP
#define HLXCLIENTNETWORKCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/CommandNetworkRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Network
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client network data model query command request
 *    buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryRequest :
    virtual public Client::Command::RequestBasis,
    public Client::Command::QueryRequestBasis
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
 *    A object for a HLX client network data model query command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::Network::QueryRegularExpressionBasis
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
 *    A object for a HLX client network data model query command request
 *    / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup network
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(void);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTNETWORKCONTROLLERCOMMANDS_HPP
