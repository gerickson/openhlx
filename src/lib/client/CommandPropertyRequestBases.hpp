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
 *      This file defines derivable objects for HLX client data model
 *      property mutation command request buffers.
 *
 */

#ifndef HLXCLIENTCOMMANDPROPERTYREQUESTBASIS_HPP
#define HLXCLIENTCOMMANDPROPERTYREQUESTBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
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
 *    A derivable object for a HLX client data model property mutation
 *    command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class PropertyRequestBasis :
    public RequestBasis,
    public Common::Command::PropertyBufferBasis
{
protected:
    PropertyRequestBasis(void) = default;
    virtual ~PropertyRequestBasis(void) = default;

    Common::Status Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client data model set property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class PropertySetRequestBasis :
    public RequestBasis,
    public Common::Command::PropertySetBufferBasis
{
protected:
    PropertySetRequestBasis(void) = default;
    virtual ~PropertySetRequestBasis(void) = default;

    // Single character operations

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const bool &aValue);

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const uint8_t &aValue);
    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const int8_t &aValue);

    // Multi-character operations

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aOperation, const uint16_t &aValue);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDPROPERTYREQUESTBASIS_HPP
