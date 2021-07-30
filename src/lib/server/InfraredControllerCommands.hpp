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
 *      This file defines objects for HLX server infrared remote control
 *      data model commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERINFRAREDCONTROLLERCOMMANDS_HPP
#define HLXSERVERINFRAREDCONTROLLERCOMMANDS_HPP

#include <CommandInfraredBufferBases.hpp>
#include <CommandInfraredRegularExpressionBases.hpp>
#include <CommandRequestBasis.hpp>
#include <CommandResponseBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Infrared
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server infrared remote control data model
 *    query command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class QueryRequest :
    public RequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

// MARK: Infrared Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server infrared remote control disabled data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class SetDisabledRequest :
    public RequestBasis,
    public Common::Command::Infrared::DisabledRegularExpressionBasis
{
public:
    SetDisabledRequest(void) = default;
    virtual ~SetDisabledRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server infrared remote control disabled data
 *    model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup infrared
 *
 */
class DisabledResponse :
    public ResponseBasis,
    public Common::Command::Infrared::DisabledBufferBasis
{
public:
    DisabledResponse(void) = default;
    virtual ~DisabledResponse(void) = default;

    Common::Status Init(const Model::InfraredModel::DisabledType &aDisabled);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

}; // namespace Infrared

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERINFRAREDCONTROLLERCOMMANDS_HPP
