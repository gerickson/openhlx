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
 *      characters) HLX command buffer, where the delimiters used are
 *      based on an enumerated role designation.
 *
 */

#include "CommandRoleDelimitedBuffer.hpp"

#include <string.h>

#include <CommandRoleDelimiters.hpp>


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
 *  This initializes the command buffer with the specified role and
 *  null-terminated C string content.
 *
 *  @param[in]  inRole    An immutable reference to the command role
 *                        to compose the command buffer with.
 *  @param[in]  inBuffer  A pointer to the null-terminated C string
 *                        to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RoleDelimitedBuffer :: Init(const Role &inRole, const char *inBuffer)
{
    return (Init(inRole, inBuffer, strlen(inBuffer)));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified role and
 *  string extent.
 *
 *  @param[in]  inRole    An immutable reference to the command role
 *                        to compose the command buffer with.
 *  @param[in]  inBuffer  A pointer to the string to initialize
 *                        the command buffer with.
 *  @param[in]  inSize    An immutable reference to the length, in
 *                        bytes, of @a inBuffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RoleDelimitedBuffer :: Init(const Role &inRole, const char *inBuffer, const size_t &inSize)
{
    return (Init(inRole, inBuffer, inBuffer + inSize));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command buffer with the specified role and
 *  string extent.
 *
 *  @param[in]  inRole    An immutable reference to the command role
 *                        to compose the command buffer with.
 *  @param[in]  inStart   A pointer to the start of the string
 *                        to initialize the command buffer with.
 *  @param[in]  inEnd     A pointer to the end of the string
 *                        to initialize the command buffer with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RoleDelimitedBuffer :: Init(const Role &inRole, const char *inStart, const char *inEnd)
{
    return (DelimitedBuffer::Init(GetRoleBufferDelimiters(inRole), inStart, inEnd));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
