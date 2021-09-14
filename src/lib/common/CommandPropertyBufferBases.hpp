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
 *      This file defines base objects for composing HLX commands that
 *      observe or set a data model object property.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDPROPERTYBUFFERBASIS_HPP
#define OPENHLXCOMMONCOMMANDPROPERTYBUFFERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class BufferBasis;

/**
 *  @brief
 *    A base object for composing HLX commands that observe a data
 *    model object property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class PropertyBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType IdentifierType;

protected:
    PropertyBufferBasis(void) = default;
    ~PropertyBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

/**
 *  @brief
 *    A base object for composing HLX commands that mutate (that is,
 *    set) a data model object property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class PropertySetBufferBasis :
    public PropertyBufferBasis
{
protected:
    PropertySetBufferBasis(void) = default;
    ~PropertySetBufferBasis(void) = default;

    // Single character operations

    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const bool &aValue);
    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const uint8_t &aValue);
    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const int8_t &aValue);

    // Multi-character operations

    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation, const uint16_t &aValue);

private:
    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, char aOperation, const int &aValue);
    static Common::Status Init(BufferBasis &aBuffer, char aProperty, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation, const int &aValue);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDPROPERTYBUFFERBASIS_HPP
