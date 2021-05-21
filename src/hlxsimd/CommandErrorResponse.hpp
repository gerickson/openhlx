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
 *      This file defines an object for a HLX server command error
 *      response buffer.
 *
 */

#ifndef HLXSERVERCOMMANDERRORRESPONSE_HPP
#define HLXSERVERCOMMANDERRORRESPONSE_HPP

#include <stddef.h>

#include <CommandResponseBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    An object for a HLX server command error response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class ErrorResponse :
    public ResponseBasis
{
public:
    ErrorResponse(void) = default;
    virtual ~ErrorResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDERRORRESPONSE_HPP
