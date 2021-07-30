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
 *      This file defines a derived object for composing a HLX
 *      client command request buffer that performs a name data model
 *      set property mutation.
 *
 */

#ifndef HLXCLIENTCOMMANDNAMESETREQUESTBASIS_HPP
#define HLXCLIENTCOMMANDNAMESETREQUESTBASIS_HPP

#include <stdint.h>

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandNameSetBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing a HLX client command request
 *    buffer that performs a name data model set property mutation.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class NameSetRequestBasis :
    public RequestBasis,
    public Common::Command::NameSetBufferBasis
{
protected:
    NameSetRequestBasis(void) = default;
    virtual ~NameSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aName);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDNAMESETREQUESTBASIS_HPP
