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
 *      This file defines base objects for composing HLX response
 *      buffers for commands that observe or set a data model object
 *      property.
 *
 */

#ifndef OPENHLXSERVERCOMMANDPROPERTYRESPONSEBASIS_HPP
#define OPENHLXSERVERCOMMANDPROPERTYRESPONSEBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
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
 *    A base object for composing HLX response buffers for commands
 *    that observe or set a data model object property.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class PropertyResponseBasis :
    public ResponseBasis,
    public Common::Command::PropertyBufferBasis
{
protected:
    PropertyResponseBasis(void) = default;
    virtual ~PropertyResponseBasis(void) = default;

    Common::Status Init(char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A base object for composing HLX response buffers for commands
 *    that set a data model object property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class PropertySetResponseBasis :
    public ResponseBasis,
    public Common::Command::PropertySetBufferBasis
{
protected:
    PropertySetResponseBasis(void) = default;
    virtual ~PropertySetResponseBasis(void) = default;

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const bool &aValue);

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const uint8_t &aValue);
    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, char aOperation, const int8_t &aValue);

    Common::Status Init(char aProperty, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aOperation, const uint16_t &aValue);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCOMMANDPROPERTYRESPONSEBASIS_HPP
