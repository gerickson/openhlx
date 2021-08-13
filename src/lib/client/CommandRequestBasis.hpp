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
 *      client command request buffers.
 *
 */

#ifndef HLXCLIENTCOMMANDREQUESTBASIS_HPP
#define HLXCLIENTCOMMANDREQUESTBASIS_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/CommandRoleDelimitedBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    An abstract base object for composing HLX client command request
 *    buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class RequestBasis :
    public Common::Command::BufferBasis,
    public Common::Command::RoleDelimitedBuffer
{
public:
    RequestBasis(void) = default;
    virtual ~RequestBasis(void) = default;

    virtual Common::Status Init(const char *inBuffer);
    virtual Common::Status Init(const char *inBuffer, const size_t &inSize) final;
    virtual Common::Status Init(const char *inStart, const char *inEnd) final;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDREQUESTBASIS_HPP
