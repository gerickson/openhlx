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
 *      This file defines return status and error enumerations shared
 *      by all Open HLX interfaces.
 *
 */

#ifndef HLXCOMMONERRORS_HPP
#define HLXCOMMONERRORS_HPP

#include <stdint.h>

namespace HLX
{

namespace Common
{

/**
 *  @typedef int32_t Status
 *
 *  @brief
 *    Type used for all enumerated success or failure status.
 *
 *    The type is used for both return status as well as a status
 *    argument for delegate methods.
 *
 *  @sa Error
 *
 */
typedef int32_t Status;

/**
 *  @typedef Status Error
 *
 *  @brief
 *    Type used for all enumerated failure status.
 *
 *    The type is used for both return failure status as well as a
 *    failure status argument for delegate methods.
 *
 *  @sa Status
 *
 */
typedef Status  Error;

enum
{
    kStatus_Success             = 0,

    kStatus_ValueAlreadySet     = 1
};

enum
{
    kError_NotInitialized       = -(1 << 8),
    kError_InitializationFailed = -(2 << 8),
    kError_BufferNotOwned       = -(3 << 8),
    kError_BadCommand           = -(4 << 8),
    kError_InvalidConfiguration = -(5 << 8),
    kError_MissingConfiguration = -(6 << 8),
    kError_HostNameResolution   = -(7 << 8)
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONERRORS_HPP
