/*
 *    Copyright (c) 2021-2021 Grant Erickson
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
 *      This file...
 *
 */

#include "Utilities.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


namespace HLX
{

namespace Simulator
{

namespace Utilities
{

namespace Configuration
{

CFStringRef
CreateCFString(const uint8_t &aValue)
{
    CFStringRef lRetval = nullptr;

    lRetval = CFStringCreateWithFormat(kCFAllocatorDefault,
                                       nullptr,
                                       CFSTR("%hhu"),
                                       aValue);
    nlREQUIRE(lRetval != nullptr, done);

 done:
    return (lRetval);
}

}; // namespace Configuration

}; // namespace Utilities

}; // namespace Simulator

}; // namespace HLX
