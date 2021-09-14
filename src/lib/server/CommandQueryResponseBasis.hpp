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
 *      This file defines an object for composing HLX server response
 *      buffers for commands that query a data model object or
 *      property thereof.
 *
 */

#ifndef OPENHLXSERVERCOMMANDQUERYRESPONSEBASIS_HPP
#define OPENHLXSERVERCOMMANDQUERYRESPONSEBASIS_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/CommandQueryBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Server/CommandResponseBasis.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    An object for composing HLX server response buffers for commands
 *    that query a data model object or property thereof.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class QueryResponseBasis :
    public ResponseBasis,
    public Common::Command::QueryBufferBasis
{
protected:
    QueryResponseBasis(void) = default;
    virtual ~QueryResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject);
    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCOMMANDQUERYRESPONSEBASIS_HPP
