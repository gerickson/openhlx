/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines an object for managing the parameters common
 *      to all run loop participants.
 *
 */

#ifndef HLXCOMMONRUNLOOPPARAMETERS_HPP
#define HLXCOMMONRUNLOOPPARAMETERS_HPP

#include <CoreFoundation/CFRunLoop.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for managing the parameters common to all run loop
 *    participants.
 *
 *  This defines an object for managing the common paramters for all
 *  run loop participants, including a reference to the run loop
 *  itself as well as the run loop mode.
 *
 *  @ingroup common
 *
 */
class RunLoopParameters
{
public:
    RunLoopParameters(void);
    ~RunLoopParameters(void) = default;

    Status Init(const CFRunLoopRef &aRunLoopRef);
    Status Init(const CFRunLoopRef &aRunLoopRef, const CFRunLoopMode &aRunLoopMode);
    Status Init(const RunLoopParameters &aRunLoopParameters);

    RunLoopParameters &operator =(const RunLoopParameters &aRunLoopParameters);

    CFRunLoopRef GetRunLoop(void) const;
    CFRunLoopMode GetRunLoopMode(void) const;

private:
    CFRunLoopRef            mRunLoopRef;
    CFRunLoopMode           mRunLoopMode;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONRUNLOOPPARAMETERS_HPP
