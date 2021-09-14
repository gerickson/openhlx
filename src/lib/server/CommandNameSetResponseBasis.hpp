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
 *      This file defines a derivable object for a HLX server name
 *      data model set property mutation command response buffer.
 *
 */

#ifndef OPENHLXSERVERCOMMANDNAMESETRESPONSEBASIS_HPP
#define OPENHLXSERVERCOMMANDNAMESETRESPONSEBASIS_HPP

#include <stdint.h>

#include <OpenHLX/Common/CommandNameSetBufferBasis.hpp>
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
 *    A derivable object for a HLX server name data model set property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup name
 *
 */
class NameSetResponseBasis :
    public ResponseBasis,
    public Common::Command::NameSetBufferBasis
{
protected:
    NameSetResponseBasis(void) = default;
    virtual ~NameSetResponseBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aName);
    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aName, const size_t &aNameLength);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCOMMANDNAMESETRESPONSEBASIS_HPP
