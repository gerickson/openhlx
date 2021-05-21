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
 *      This file implements a derived object for composing HLX commands
 *      that observe or mutate the sound data model tone properties.
 *
 */

#include "CommandToneBufferBasis.hpp"

#include <OpenHLX/Common/OutputStringStream.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

static const char   kToneProperty    = 'T';
static const char   kBassDelimiter   = 'B';
static const char   kTrebleDelimiter = 'T';

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes a tone equalizer operation against a specific
 *  object and identifier into the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the tone equalizer operation is
 *                               to be made against. For example, "O"
 *                               for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the tone equalizer
 *                               operation is to be made against.
 *  @param[in]      aOperation   A pointer to a null-terminated C
 *                               string representing the operation to
 *                               perform.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToneBufferBasis :: Init(BufferBasis &aBuffer,
                        const char *aObject,
                        const IdentifierType &aIdentifier,
                        const char *aOperation)
{
    return (PropertyBufferBasis::Init(aBuffer,
                                      kToneProperty,
                                      aObject,
                                      aIdentifier,
                                      aOperation));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes a tone equalizer bass and treble band levels
 *  operation against a specific object identifier into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the tone equalizer level
 *                               operation is to be made against. For
 *                               example, "O" for a zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the tone equalizer
 *                               level operation is to be made
 *                               against.
 *  @param[in]      aBass        An immutable reference to the
 *                               bass level for the operation.
 *  @param[in]      aTreble      An immutable reference to the
 *                               treble level for the operation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToneBufferBasis :: Init(BufferBasis &aBuffer,
                        const char *aObject,
                        const IdentifierType &aIdentifier,
                        const LevelType &aBass,
                        const LevelType &aTreble)
{
    OutputStringStream  lBassStream;
    OutputStringStream  lTrebleStream;
    std::string         lToneString;


    // Upcast the levels to a signed integer to ensure they are
    // interpretted as something to be converted rather than a
    // character literal.

    lBassStream << static_cast<int>(aBass);
    lTrebleStream << static_cast<int>(aTreble);

    lToneString = kBassDelimiter;
    lToneString += lBassStream.str();
    lToneString += kTrebleDelimiter;
    lToneString += lTrebleStream.str();

    return (PropertyBufferBasis::Init(aBuffer,
                                      kToneProperty,
                                      aObject,
                                      aIdentifier,
                                      lToneString.c_str()));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
