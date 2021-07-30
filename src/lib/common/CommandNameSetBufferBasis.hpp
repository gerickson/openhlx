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
 *      command buffer that performs a name data model set property
 *      mutation.
 *
 */

#ifndef HLXCOMMONCOMMANDNAMESETBUFFERBASIS_HPP
#define HLXCOMMONCOMMANDNAMESETBUFFERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing a HLX command buffer that
 *    performs a name data model set property mutation.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup name
 *
 */
class NameSetBufferBasis :
    public PropertyBufferBasis
{
protected:
    NameSetBufferBasis(void) = default;
    ~NameSetBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aName);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aName, const size_t &aNameLength);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDNAMESETBUFFERBASIS_HPP
