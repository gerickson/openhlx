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
 *      This file defines a base object for composing HLX commands
 *      that observe (that is, query) a data model object or property
 *      thereof.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDQUERYBUFFERBASIS_HPP
#define OPENHLXCOMMONCOMMANDQUERYBUFFERBASIS_HPP

#include <vector>

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
 *    A base object for composing HLX commands that query a data model
 *    object or property thereof.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class QueryBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType IdentifierType;

protected:
    QueryBufferBasis(void) = default;
    ~QueryBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const char *aProperty, const IdentifierType &aIdentifier);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const char &aProperty, const IdentifierType &aIdentifier);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDQUERYBUFFERBASIS_HPP
