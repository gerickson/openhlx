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
 *      This file defines derivable objects for HLX client source
 *      (input) data model property mutation command request buffers.
 *
 */

#ifndef HLXCLIENTCOMMANDSOURCEREQUESTBASIS_HPP
#define HLXCLIENTCOMMANDSOURCEREQUESTBASIS_HPP

#include <stdint.h>

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandSourceBufferBases.hpp>
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
 *    A derivable object for a HLX client source (input) data model
 *    property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceRequestBasis :
    public RequestBasis,
    public Common::Command::SourceBufferBasis
{
protected:
    SourceRequestBasis(void) = default;
    virtual ~SourceRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client source (input) data model
 *    set property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceSetRequestBasis :
    public RequestBasis,
    public Common::Command::SourceSetBufferBasis
{
protected:
    SourceSetRequestBasis(void) = default;
    virtual ~SourceSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const IdentifierType &aSourceIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDSOURCEREQUESTBASIS_HPP
