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
 *      server command response buffers.
 *
 */

#ifndef HLXSERVERCOMMANDRESPONSEBASIS_HPP
#define HLXSERVERCOMMANDRESPONSEBASIS_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/CommandRoleDelimitedBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    An abstract base object for composing HLX server command
 *    response buffers.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class ResponseBasis :
    public Common::Command::BufferBasis,
    public Common::Command::RoleDelimitedBuffer
{
protected:
    ResponseBasis(void) = default;
    virtual ~ResponseBasis(void) = default;

    virtual Common::Status Init(const char *inBuffer);
    virtual Common::Status Init(const char *inBuffer, const size_t &inSize) final;
    virtual Common::Status Init(const char *inStart, const char *inEnd) final;
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDRESPONSEBASIS_HPP
