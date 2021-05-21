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
 *      This file implements a buffer object for composing HLX commands.
 *
 */

#include "CommandBuffer.hpp"


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
 *  This initializes the command buffer with the specified
 *  null-terminated C string content.
 *
 *  @param[in]  inBuffer  A pointer to the null-terminated C string
 *                        to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
Buffer :: Init(const char *inBuffer)
{
    return (Init(inBuffer, strlen(inBuffer)));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified buffer
 *  extent.
 *
 *  @param[in]  inBuffer  A pointer to the buffer to initialize
 *                        the command buffer with.
 *  @param[in]  inSize    An immutable reference to the length, in
 *                        bytes, of @a inBuffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
Buffer :: Init(const char *inBuffer, const size_t &inSize)
{
    return (Init(inBuffer, inBuffer + inSize));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified buffer
 *  extent.
 *
 *  @param[in]  inStart   A pointer to the start of the buffer
 *                        to initialize the command buffer with.
 *  @param[in]  inEnd     A pointer to the end of the buffer
 *                        to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
Buffer :: Init(const char *inStart, const char *inEnd)
{
    mBuffer.clear();

    AppendBuffer(inStart, inEnd);

    return (kStatus_Success);
}

/**
 *  @brief
 *    Get a pointer to the start of the command buffer contents.
 *
 *  @returns
 *    A pointer to the start of the command buffer * contents.
 *
 */
const uint8_t *
Buffer :: GetBuffer(void) const
{
    return (reinterpret_cast<const uint8_t *>(&mBuffer[0]));
}

/**
 *  @brief
 *    Return the size, in bytes, of the command buffer contents.
 *
 *  @returns
 *    The size, in bytes, of the command buffer contents.
 *
 */
size_t
Buffer :: GetSize(void) const
{
    return (mBuffer.size());
}

/**
 *  @brief
 *    Append content to the command buffer.
 *
 *  This appends to the command buffer the specified buffer extent.
 *
 *  @param[in]  inStart   A pointer to the start of the buffer
 *                        to append to the command buffer.
 *  @param[in]  inEnd     A pointer to the end of the buffer
 *                        to append to the command buffer.
 *
 */
void
Buffer :: AppendBuffer(const char *inStart, const char *inEnd)
{
    mBuffer.insert(mBuffer.end(), inStart, inEnd);
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
