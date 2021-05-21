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
 *      This file defines a derivable object for a HLX client data
 *      model properties observation (that is, query) command request
 *      buffer.
 *
 */

#ifndef HLXCLIENTCOMMANDQUERYREQUESTBASIS_HPP
#define HLXCLIENTCOMMANDQUERYREQUESTBASIS_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

#include <CommandQueryBufferBasis.hpp>
#include <CommandRequestBasis.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>

namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    A derivable object for a HLX client data model properties
 *    observation command request buffer.
 *
 *  A derivable object for a HLX client data model properties
 *  observation (that is, query) command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class QueryRequestBasis :
    public RequestBasis,
    public Common::Command::QueryBufferBasis
{
protected:
    QueryRequestBasis(void) = default;
    virtual ~QueryRequestBasis(void) = default;

    Common::Status Init(const char *aObject);
    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDQUERYREQUESTBASIS_HPP
