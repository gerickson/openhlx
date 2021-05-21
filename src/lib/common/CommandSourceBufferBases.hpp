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
 *      This file defines base and derived objects for composing HLX
 *      commands that observe or set source (input) data model
 *      properties.
 *
 */

#ifndef HLXCOMMONCOMMANDSOURCEBUFFERBASES_HPP
#define HLXCOMMONCOMMANDSOURCEBUFFERBASES_HPP

#include <stdint.h>

#include <OpenHLX/Common/Errors.hpp>

#include <CommandPropertyBufferBases.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or mutate
 *    the source (input) data model property for another collection
 *    object, such as a group or zone.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceBufferBasis :
    public PropertyBufferBasis
{
protected:
    SourceBufferBasis(void) = default;
    ~SourceBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or
 *    mutate the source (input) data model level property for all
 *    collection objects.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceAllBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

protected:
    SourceAllBufferBasis(void) = default;
    ~SourceAllBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate the
 *    source (input) set data model property for another collection
 *    object, such as a group or zone.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup source
 *
 */
class SourceSetBufferBasis :
    public PropertySetBufferBasis
{
protected:
    SourceSetBufferBasis(void) = default;
    ~SourceSetBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const IdentifierType &aSourceIdentifier);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDSOURCEBUFFERBASES_HPP
