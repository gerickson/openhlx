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
 *      This file defines objects for HLX client infrared remote control
 *      data model commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTINFRAREDCONTROLLERCOMMANDS_HPP
#define HLXCLIENTINFRAREDCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/CommandInfraredBufferBases.hpp>
#include <OpenHLX/Common/CommandInfraredRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Infrared
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client infrared remote control data model
 *    query command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Infrared::QueryDisabledBufferBasis
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
 *    A object for a HLX client infrared remote control data model
 *    query command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::Infrared::DisabledRegularExpressionBasis
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
 *    A object for a HLX client infrared remote control data model
 *    query command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
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

// MARK: Infrared Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client infrared remote control disabled data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class SetDisabledRequest :
    public RequestBasis,
    public Common::Command::Infrared::DisabledBufferBasis
{
public:
    SetDisabledRequest(void) = default;
    virtual ~SetDisabledRequest(void) = default;

    Common::Status Init(const Model::InfraredModel::DisabledType &aDisabled);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client infrared remote control disabled data
 *    model property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class DisabledResponse :
    public ResponseBasis,
    public Common::Command::Infrared::DisabledRegularExpressionBasis
{
public:
    DisabledResponse(void) = default;
    virtual ~DisabledResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client infrared remote control disabled data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class SetDisabled :
    public ExchangeBasis
{
public:
    SetDisabled(void) = default;
    virtual ~SetDisabled(void) = default;

    Common::Status Init(const Model::InfraredModel::DisabledType &aDisabled);

private:
    SetDisabledRequest  mRequest;
    DisabledResponse    mResponse;
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTINFRAREDCONTROLLERCOMMANDS_HPP
