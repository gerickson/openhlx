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
 *      This file defines objects for HLX server source data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERSOURCESCONTROLLERCOMMANDS_HPP
#define HLXSERVERSOURCESCONTROLLERCOMMANDS_HPP

#include <CommandNameSetResponseBasis.hpp>
#include <CommandRequestBasis.hpp>
#include <CommandSourcesRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/SourceModel.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Sources
{

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server source name data model property mutation
 *    command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup source
 *
 */
class SetNameRequest :
    public RequestBasis,
    public Common::Command::Sources::NameRegularExpressionBasis
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
 *    A object for a HLX server source name data model property mutation
 *    command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup source
 *
 */
class NameResponse :
    public NameSetResponseBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(const Model::SourceModel::IdentifierType &aSource, const char * aName);
    Common::Status Init(const Model::SourceModel::IdentifierType &aSource, const char * aName, const size_t &aNameLength);
};

}; // namespace Sources

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERSOURCESCONTROLLERCOMMANDS_HPP
