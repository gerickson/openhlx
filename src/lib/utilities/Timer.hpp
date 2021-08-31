/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file defines types and objects for specifying timeouts.
 *
 */

#ifndef OPENHLXUTILITIESTIMEOUT_HPP
#define OPENHLXUTILITIESTIMEOUT_HPP


#include <CoreFoundation/CFRunLoop.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Utilities
{

class TimerDelegate;

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup utilities
 *
 */
class Timer
{
public:
    Timer(void);
    ~Timer(void);

    // Initializer(s)

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters,
                        const Common::Timeout &aTimeout);

    // Equality

    bool operator == (const Timer &aTimer) const;

    // Delegate Management

    TimerDelegate * GetDelegate(void) const;
    Common::Status SetDelegate(TimerDelegate *aDelegate);

    // Timer Management

    Common::Status Start(void);
    Common::Status Stop(void);
    void           Destroy(void);

    // Timer Fired Handler Trampoline

    static void TimerFiredCallBack(CFRunLoopTimerRef aTimerRef, void *aContext);

private:
    // Timer Fired Handler

    void TimerFiredCallBack(CFRunLoopTimerRef aTimerRef);

private:
    Common::RunLoopParameters  mRunLoopParameters;
    CFRunLoopTimerRef          mTimerRef;
    TimerDelegate *            mDelegate;
};

}; // namespace Utilities

}; // namespace HLX

#endif // OPENHLXUTILITIESTIMEOUT_HPP
