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
 *      This file implements an object for specifying a run loop
 *      timer.
 *
 */

#include "Timer.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

#include "TimerDelegate.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
Timer :: Timer(void) :
    mRunLoopParameters(),
    mTimerRef(nullptr),
    mDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
Timer :: ~Timer(void)
{
    Destroy();
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a repeating interval timer to fire at the
 *  specified millisecond timeout in the future.
 *
 *  @note
 *    This only initializes the timer. The timer must be started with
 *    #Start. Timer expiration events are handled through the delegate
 *    interface by invoking #SetDelegate.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  timer with.
 *  @param[in]  aTimeout            An immutable reference to a milli-
 *                                  second timeout containing the
 *                                  interval at which the timer should
 *                                  fire, in milliseconds.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If resources could not be allocated for
 *                            the timer.
 *
 *  @sa SetDelegate
 *  @sa Start
 *  @sa Destroy
 *
 */
Status
Timer :: Init(const Common::RunLoopParameters &aRunLoopParameters,
              const Timeout &aTimeout)
{
    static constexpr CFOptionFlags  kFlags           = 0;
    static constexpr CFIndex        kOrder           = 0;
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

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  @param[in]  aTimer  An immutable reference to the timer compare
 *                      against this timer for equality.
 *
 *  @returns
 *    True if this timer is equal to the specified timer; otherwise,
 *    false.
 *
 */
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

/**
 *  @brief
 *    Return the delegate for the timer.
 *
 *  @returns
 *    A pointer to the delegate for the timer.
 *
 */
TimerDelegate *
Timer :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the timer.
 *
 *  This attempts to set a delegate for the timer.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
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

/**
 *  @brief
 *    Start the timer.
 *
 *  This starts the timer.
 *    
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the timer has not yet been
 *                                  initialized.
 *
 *  @sa Stop
 *  @sa Destroy
 *
 */
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

/**
 *  @brief
 *    Stop the timer.
 *
 *  This stops the timer.
 *    
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the timer has not yet been
 *                                  initialized.
 *
 *  @sa Start
 *  @sa Destroy
 *
 */
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

/**
 *  @brief
 *    Stop and release all resources associated with the timer.
 *
 *  This stops the timer and releases all resources associated with
 *  the timer. The only usable method for the timer after invoking
 *  this method is #Init.
 *
 *  @sa Init
 */
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

/**
 *  @brief
 *    Callback to handle timer expiration activity.
 *
 *  This handles any activity associated with the timer expiration.
 *
 *  @param[in]  aTimerRef  A reference to the timer that
 *                         triggered the callback.
 *
 */
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

/**
 *  @brief
 *    Callback to handle timer expiration activity.
 *
 *  This handles any activity associated with the timer expiration.
 *
 *  @param[in]  aTimerRef  A reference to the timer that
 *                         triggered the callback.
 *  @param[in]  aContext   A pointer to the timer class
 *                         instance that registered this
 *                         trampoline to call back into from
 *                         the trampoline.
 *
 */
/* static */ void
Timer :: TimerFiredCallBack(CFRunLoopTimerRef aTimerRef, void *aContext)
{
    Timer *lTimer = static_cast<Timer *>(aContext);

    if (lTimer != nullptr)
    {
        lTimer->TimerFiredCallBack(aTimerRef);
    }
}

}; // namespace Common

}; // namespace HLX
