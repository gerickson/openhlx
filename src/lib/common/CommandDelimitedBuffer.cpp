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
 *      This file implements a derived object for composing delimited
 *      (that is, initiated and terminated by a delimiting pair of
 *      characters) HLX command buffers.
 *
 */

#include "CommandDelimitedBuffer.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified delimiters
 *  and null-terminated C string content.
 *
 *  @param[in]  inDelimiters  An immutable reference to the command
 *                            delimiters to compose the command
 *                            buffer with.
 *  @param[in]  inBuffer      A pointer to the null-terminated C string
 *                            to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DelimitedBuffer :: Init(const Delimiters &inDelimiters, const char *inBuffer)
{
    return (Init(inDelimiters, inBuffer, strlen(inBuffer)));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified delimiters
 *  and string extent.
 *
 *  @param[in]  inDelimiters  An immutable reference to the command
 *                            delimiters to compose the command
 *                            buffer with.
 *  @param[in]  inBuffer      A pointer to the string to initialize
 *                            the command buffer with.
 *  @param[in]  inSize        An immutable reference to the length, in
 *                            bytes, of @a inBuffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DelimitedBuffer :: Init(const Delimiters &inDelimiters, const char *inBuffer, const size_t &inSize)
{
    return (Init(inDelimiters, inBuffer, inBuffer + inSize));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified delimiters
 *  and string extent.
 *
 *  @param[in]  inDelimiters  An immutable reference to the command
 *                            delimiters to compose the command
 *                            buffer with.
 *  @param[in]  inStart       A pointer to the start of the string
 *                            to initialize the command buffer with.
 *  @param[in]  inEnd         A pointer to the end of the string
 *                            to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DelimitedBuffer :: Init(const Delimiters &inDelimiters,
                        const char *inStart,
                        const char *inEnd)
{
    /*
     * Ideally, the underlying buffer would have an initialization
     * interface that would allow initialization through an array of
     * start and end iterators; however, for now, we deal with the
     * slight inefficiency of composiing a temporary string.
     */
    std::string  lTempString;
    Status       lRetval = kStatus_Success;


    lTempString += inDelimiters.mStart;

    lTempString.insert(lTempString.end(), inStart, inEnd);

    lTempString += inDelimiters.mEnd;

    lRetval = Buffer::Init(lTempString.c_str(), lTempString.size());
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
