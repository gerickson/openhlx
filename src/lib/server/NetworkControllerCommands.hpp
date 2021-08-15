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
 *      This file defines objects for HLX server network data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERNETWORKCONTROLLERCOMMANDS_HPP
#define HLXSERVERNETWORKCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandNetworkRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>

#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Network
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server network data model query command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Network::QueryRegularExpressionBasis
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
 *    A object for a HLX server network data model query command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
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

    Common::Status Init(void);
};

}; // namespace Network

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERNETWORKCONTROLLERCOMMANDS_HPP
