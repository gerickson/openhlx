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
 *      This file implements an abstract base object for composing HLX
 *      client command request buffers.
 *
 */

#include "CommandRequestBasis.hpp"


using namespace HLX::Common;
using namespace HLX::Common::Command;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the client command request buffer with the
 *  specified null-terminated C string content.
 *
 *  @param[in]  inBuffer  A pointer to the null-terminated C string
 *                        to initialize the client command request
 *                        buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RequestBasis :: Init(const char *inBuffer)
{
    return (Init(inBuffer, strlen(inBuffer)));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the client command request buffer with the
 *  specified string extent.
 *
 *  @param[in]  inBuffer  A pointer to the string
 *                        to initialize the client command request
 *                        buffer with.
 *  @param[in]  inSize    An immutable reference to the length, in
 *                        bytes, of @a inBuffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RequestBasis :: Init(const char *inBuffer, const size_t &inSize)
{
    return (Init(inBuffer, inBuffer + inSize));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the client command request buffer with the
 *  specified string extent.
 *
 *  @param[in]  inStart   A pointer to the start of the string
 *                        to initialize the client command request
 *                        buffer with.
 *  @param[in]  inEnd     A pointer to the end of the string
 *                        to initialize the client command request
 *                        buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RequestBasis :: Init(const char *inStart, const char *inEnd)
{
    static constexpr Role kRole = Role::kRequestor;

    return(RoleDelimitedBuffer::Init(kRole, inStart, inEnd));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
