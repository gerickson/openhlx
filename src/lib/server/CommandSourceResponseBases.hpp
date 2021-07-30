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
 *      This file defines derivable objects for HLX server source
 *      (input) data model property mutation command response buffers.
 *
 */

#ifndef HLXSERVERCOMMANDSOURCERESPONSEBASES_HPP
#define HLXSERVERCOMMANDSOURCERESPONSEBASES_HPP

#include <stdint.h>

#include <OpenHLX/Common/CommandSourceBufferBases.hpp>
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
 *    A derivable object for a HLX server source (input) data model
 *    property mutation command response buffers.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup volume
 *
 */
class SourceResponseBasis :
    public ResponseBasis,
    public Common::Command::SourceBufferBasis
{
protected:
    SourceResponseBasis(void) = default;
    virtual ~SourceResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

/**
 *  @brief
 *    A derivable object for a HLX server all-object source (input) data model
 *    set property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceAllResponseBasis :
    public ResponseBasis,
    public Common::Command::SourceAllBufferBasis
{
protected:
    SourceAllResponseBasis(void) = default;
    virtual ~SourceAllResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX server source (input) data model
 *    set property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceSetResponseBasis :
    public ResponseBasis,
    public Common::Command::SourceSetBufferBasis
{
protected:
    SourceSetResponseBasis(void) = default;
    virtual ~SourceSetResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const IdentifierType &aSource);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDSOURCERESPONSEBASES_HPP
