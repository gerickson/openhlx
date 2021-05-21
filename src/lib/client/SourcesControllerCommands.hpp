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
 *      This file defines objects for HLX client source (input) data
 *      model commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTSOURCESCONTROLLERCOMMANDS_HPP
#define HLXCLIENTSOURCESCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/Errors.hpp>

#include <CommandExchangeBasis.hpp>
#include <CommandNameSetRequestBasis.hpp>
#include <CommandQueryRequestBasis.hpp>
#include <CommandResponseBasis.hpp>
#include <CommandSourcesRegularExpressionBases.hpp>
#include <OpenHLX/Model/SourceModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Sources
{

// MARK: Observer Requests, Responses, and Commands


// MARK: Mutator Requests, Responses, and Commands


// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client source name data model property mutation
 *    command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup source
 *
 */
class NameResponse :
    public ResponseBasis,
    public Common::Command::Sources::NameRegularExpressionBasis
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
 *    A object for a HLX client source name data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup source
 *
 */
class SetNameRequest :
    public NameSetRequestBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(const Model::SourceModel::IdentifierType &aSourceIdentifier, const char *aName);
};

/**
 *  @brief
 *    A object for a HLX client source name data model property mutation
 *    command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup source
 *
 */
class SetName :
    public ExchangeBasis
{
public:
    SetName(void) = default;
    virtual ~SetName(void) = default;

    Common::Status Init(const Model::SourceModel::IdentifierType &aSourceIdentifier, const char *aName);

private:
    SetNameRequest mRequest;
    NameResponse   mResponse;
};

}; // namespace Sources

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTSOURCESCONTROLLERCOMMANDS_HPP
