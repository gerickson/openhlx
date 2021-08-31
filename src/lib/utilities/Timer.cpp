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

#include "Timer.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>

#include "TimerDelegate.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Utilities
{

Timer :: Timer(void) :
    mRunLoopParameters(),
    mTimerRef(nullptr),
    mDelegate(nullptr)
{
    return;
}

Timer :: ~Timer(void)
{
    Destroy();
}

// MARK: Initializer(s)

Status Timer :: Init(const Common::RunLoopParameters &aRunLoopParameters,
                     const Timeout &aTimeout)
{
    static constexpr CFOptionFlags  kFlags = 0;
    static constexpr CFIndex        kOrder = 0;
    const CFTimeInterval            lIntervalSeconds = aTimeout.GetMilliseconds() / 1000;
    const CFAbsoluteTime            lFirstFireDate   = CFAbsoluteTimeGetCurrent() + lIntervalSeconds;
    CFRunLoopTimerContext           lTimerContext    = { 0, this, 0, 0, 0 };
    Status                          lRetval          = kStatus_Success;


    mTimerRef = CFRunLoopTimerCreate(kCFAllocatorDefault,
                                     lFirstFireDate,
                                     lIntervalSeconds,
                                     kFlags,
                                     kOrder,
                                     TimerFiredCallBack,
                                     &lTimerContext);
    nlREQUIRE_ACTION(mTimerRef != nullptr, done, lRetval = -ENOMEM);

    mRunLoopParameters = aRunLoopParameters;

 done:
    return (lRetval);
}

// MARK: Equality

bool
Timer :: operator ==(const Timer &aTimer) const
{
    bool lRetval = false;

    if (this == &aTimer)
    {
        lRetval = true;
    }
    else if ((mTimerRef == nullptr) && (aTimer.mTimerRef == nullptr))
    {
        lRetval = true;
    }
    else if ((mTimerRef != nullptr) && (aTimer.mTimerRef != nullptr))
    {
        lRetval = CFEqual(mTimerRef, aTimer.mTimerRef);
    }

    return (lRetval);

}

// MARK: Delegate Management

TimerDelegate *
Timer :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
Timer :: SetDelegate(TimerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

// MARK: Timer Management

Status
Timer :: Start(void)
{
    Status  lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mTimerRef != nullptr, done, lRetval = kError_NotInitialized);

    CFRunLoopAddTimer(mRunLoopParameters.GetRunLoop(),
                      mTimerRef,
                      mRunLoopParameters.GetRunLoopMode());

 done:
    return (lRetval);
}

Status
Timer :: Stop(void)
{
    Status  lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mTimerRef != nullptr, done, lRetval = kError_NotInitialized);

    CFRunLoopRemoveTimer(mRunLoopParameters.GetRunLoop(),
                         mTimerRef,
                         mRunLoopParameters.GetRunLoopMode());

 done:
    return (lRetval);
}

void
Timer :: Destroy(void)
{
    if (mTimerRef != nullptr)
    {
        CFRunLoopTimerInvalidate(mTimerRef);

        CFRelease(mTimerRef);

        mTimerRef = nullptr;
    }

    mDelegate = nullptr;
}

// MARK: Timer Fired Handler

void
Timer :: TimerFiredCallBack(CFRunLoopTimerRef aTimerRef)
{
    (void)aTimerRef;

    if (mDelegate != nullptr)
    {
        mDelegate->TimerDidFire(*this);
    }
}

// MARK: Timer Fired Handler Trampoline

/* static */ void
Timer :: TimerFiredCallBack(CFRunLoopTimerRef aTimerRef, void *aContext)
{
    Timer *lTimer = static_cast<Timer *>(aContext);

    if (lTimer != nullptr)
    {
        lTimer->TimerFiredCallBack(aTimerRef);
    }
}

}; // namespace Utilities

}; // namespace HLX
