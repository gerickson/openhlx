/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines enumerated HLX control protocol command
 *      roles.
 *
 */

#ifndef HLXCOMMONCOMMANDROLE_HPP
#define HLXCOMMONCOMMANDROLE_HPP

#include <stdint.h>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    Enumerated HLX control protocol command roles.
 *
 *  The encoding semantics of the HLX control protocol differ by
 *  command role. These enumerations may be used and specified where
 *  such distinctions are necessary.
 *
 *  @ingroup command
 *
 */
enum class Role : uint8_t
{
    kRequestor = 0,
    kResponder = 1
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDROLE_HPP
