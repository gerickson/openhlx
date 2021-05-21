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
 *      This file defines an abstract base object for composing HLX
 *      command buffers.
 *
 */

#ifndef HLXCOMMONCOMMANDBUFFERBASIS_HPP
#define HLXCOMMONCOMMANDBUFFERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    An abstract base object for composing HLX command buffers.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class BufferBasis
{
public:
    virtual ~BufferBasis(void) = default;

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
    virtual Common::Status Init(const char *inBuffer, const size_t &inSize) = 0;

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
    virtual Common::Status Init(const char *inStart, const char *inEnd) = 0;

protected:
    BufferBasis(void) = default;
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDBUFFERBASIS_HPP
