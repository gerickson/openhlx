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
 *      This file implements a base object for composing HLX commands
 *      that observe (that is, query) a data model object or property
 *      thereof.
 *
 */

#include "CommandQueryBufferBasis.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

namespace Command
{

static const char * const kQueryOperation = "Q";

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an object observation (that is, query) operation
 *  against a specific object into the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the observation
 *                               operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the observation operation is to
 *                               be made against. For example, "O" for
 *                               a zone object.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryBufferBasis :: Init(BufferBasis &aBuffer,
                         const char *aObject)
{
    std::string lBuffer;

    lBuffer = kQueryOperation;
    lBuffer += aObject;

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an object observation (that is, query) operation
 *  against a specific object and identfier into the specified command
 *  buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the observation
 *                               operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the observation operation is to
 *                               be made against. For example, "O" for
 *                               a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the observation
 *                               operation is to be made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryBufferBasis :: Init(BufferBasis &aBuffer,
                         const char *aObject,
                         const IdentifierType &aIdentifier)
{
    OutputStringStream  lIdentifierStream;
    std::string         lBuffer;


    lIdentifierStream << aIdentifier;

    // Compose the buffer.

    lBuffer = kQueryOperation;
    lBuffer += aObject;
    lBuffer += lIdentifierStream.str();

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

Status
QueryBufferBasis :: Init(BufferBasis &aBuffer, const char *aObject, const char *aProperty, const IdentifierType &aIdentifier)
{
    OutputStringStream  lIdentifierStream;
    std::string         lBuffer;


    lIdentifierStream << aIdentifier;

    // Compose the buffer.

    lBuffer = kQueryOperation;
    lBuffer += aProperty;
    lBuffer += aObject;
    lBuffer += lIdentifierStream.str();

    return (aBuffer.Init(lBuffer.c_str(), lBuffer.size()));
}

Status
QueryBufferBasis :: Init(BufferBasis &aBuffer, const char *aObject, const char &aProperty, const IdentifierType &aIdentifier)
{
    const char lProperty[2] = { aProperty, '\0' };

    return (Init(aBuffer, aObject, lProperty, aIdentifier));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
