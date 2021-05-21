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
 *      This file implements a run loop-aware queue for managing a
 *      queue of non-retained and unmanaged object pointers.
 *
 */

#include "RunLoopQueue.hpp"

#include <errno.h>

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/RunLoopQueueDelegate.hpp>

using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
RunLoopQueue :: RunLoopQueue(void) :
    mRunLoopParameters(),
    mDelegate(nullptr),
    mRunLoopSourceRef(nullptr),
    mQueue()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
RunLoopQueue :: ~RunLoopQueue(void)
{
    CFRunLoopRemoveSource(mRunLoopParameters.GetRunLoop(), mRunLoopSourceRef, mRunLoopParameters.GetRunLoopMode());

    CFURelease(mRunLoopSourceRef);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the run loop queue on a run loop with the
 *  specified run loop parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  queue with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          Resources for the run loop source could
 *                            not be allocated.
 *
 */
Status
RunLoopQueue :: Init(const RunLoopParameters &aRunLoopParameters)
{
    Status                  lRetval = kStatus_Success;
    CFRunLoopSourceContext  lContext;
    CFRunLoopSourceRef      lRunLoopSourceRef;

    lContext.version         = 0;
    lContext.info            = this;
    lContext.retain          = nullptr;
    lContext.release         = nullptr;
    lContext.copyDescription = RunLoopQueue::CopyDescription;
    lContext.equal           = nullptr;
    lContext.hash            = nullptr;
    lContext.schedule        = nullptr;
    lContext.cancel          = nullptr;
    lContext.perform         = RunLoopQueue::Perform;

    lRunLoopSourceRef = CFRunLoopSourceCreate(kCFAllocatorDefault,
                                              0,
                                              &lContext);
    nlREQUIRE_ACTION(lRunLoopSourceRef != nullptr, done, lRetval = -ENOMEM);

    CFRunLoopAddSource(aRunLoopParameters.GetRunLoop(),
                       lRunLoopSourceRef,
                       aRunLoopParameters.GetRunLoopMode());

    mRunLoopParameters = aRunLoopParameters;
    mRunLoopSourceRef  = lRunLoopSourceRef;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the run loop queue.
 *
 *  @returns
 *    A pointer to the delegate for the run loop queue.
 *
 */
RunLoopQueueDelegate *
RunLoopQueue :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the run loop queue.
 *
 *  This attempts to set a delegate for the run loop queue.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
RunLoopQueue :: SetDelegate(RunLoopQueueDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the depth, in number of elements, of the run loop queue.
 *
 *  @returns
 *    The depth, in number of delements, of the run loop queue.
 *
 */
RunLoopQueue::queue_type::size_type
RunLoopQueue :: GetSize(void) const
{
    return (mQueue.size());
}

/**
 *  @brief
 *    Return whether or not the run loop queue is empty.
 *
 *  @returns
 *    True if the run loop queue is empty; otherwise, false.
 *
 */
bool
RunLoopQueue :: IsEmpty(void) const
{
    return (mQueue.empty());
}

/**
 *  @brief
 *    Place an element onto the run loop queue.
 *
 *  This places the specified element, by pointer, onto the tail of
 *  the run loop queue.
 *
 *  @note
 *    The caller is responsible for managing the life time of the
 *    object placed onto the queue.
 *
 *  @param[in]  aElement  The element to be added to the queue.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the run loop queue has not been
 *                                  initialized.
 *
 *  @sa Init
 *  @sa Pop
 *
 */
Status
RunLoopQueue :: Push(element_type aElement)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mRunLoopSourceRef != nullptr, done, lRetval = kError_NotInitialized);

    mQueue.push(aElement);

    CFRunLoopSourceSignal(mRunLoopSourceRef);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Removes an element from the run loop queue.
 *
 *  This attempts to remove, if present, an element, from the head of
 *  the run loop queue.
 *
 *  @note
 *    The caller is responsible for managing the life time of the
 *    object removed from the queue.
 *
 *  @returns
 *    A pointer to the element at the head of the run loop queue, if
 *    successful; otherwise, a null pointer.
 *
 *  @sa Push
 *
 */
RunLoopQueue::element_type
RunLoopQueue :: Pop(void)
{
    element_type  lRetval = nullptr;

    nlREQUIRE(mRunLoopSourceRef != nullptr, done);

    lRetval = mQueue.front();

    mQueue.pop();

    CFRunLoopSourceSignal(mRunLoopSourceRef);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This forgets about, or flushes, any elements associated with the
 *    run loop queue.
 *
 *  This flushes any elements associated with the run loop queue such
 *  that the queue size is zero following this call.
 *
 */
void
RunLoopQueue :: Flush(void)
{
    queue_type lEmpty;

    swap(mQueue, lEmpty);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided run loop queue against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aQueue  An immutable reference to the run loop
 *                      queue to compare for equality.
 *
 *  @returns
 *    True if this run loop queue is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
RunLoopQueue :: operator ==(const RunLoopQueue &aQueue) const
{
    const bool lRetval = (this == &aQueue);

    return (lRetval);
}

// MARK: CoreFoundation Run Loop Handlers

/**
 *  @brief
 *    Callback to return a description of this CoreFoundation run loop
 *    source object.
 *
 *  This returns a description of this CoreFoundation run loop source
 *  object in response to CFCopyDescription on the run loop source.
 *
 *  @returns
 *    An CoreFoundation immutable string reference for the run loop
 *    source object description.
 *
 */
CFStringRef
RunLoopQueue :: CopyDescription(void) const
{
    return (CFSTR("Open HLX Run Loop Queue"));
}

/**
 *  @brief
 *    Callback to perform any work associated with this CoreFoundation
 *    run loop source object.
 *
 *  This issues queue status delegations as its primary source of work.
 *
 */
void
RunLoopQueue :: Perform(void)
{
    if (mDelegate != nullptr)
    {
        if (mQueue.empty())
        {
            mDelegate->QueueIsEmpty(*this);
        }
        else
        {
            mDelegate->QueueIsNotEmpty(*this);
        }
    }
}

// MARK: CoreFoundation Run Loop Handler Trampolines

/**
 *  @brief
 *    Callback trampoline to return a description of this
 *    CoreFoundation run loop source object.
 *
 *  @param[in]  aContext  A pointer to the command manager class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 *  @returns
 *    An CoreFoundation immutable string reference for the run loop
 *    source object description.
 *
 */
CFStringRef
RunLoopQueue :: CopyDescription(const void *aContext)
{
    const RunLoopQueue * lRunLoopQueue = static_cast<const RunLoopQueue *>(aContext);
    CFStringRef          lRetval = nullptr;

    if (lRunLoopQueue != nullptr)
    {
        lRetval = lRunLoopQueue->CopyDescription();
    }

    return (lRetval);
}

/**
 *  @brief
 *    Callback trampoline to perform any work associated with this
 *    CoreFoundation run loop source object.
 *
 *  @param[in]  aContext  A pointer to the command manager class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 */
void
RunLoopQueue :: Perform(void *aContext)
{
    RunLoopQueue *lRunLoopQueue = static_cast<RunLoopQueue *>(aContext);

    if (lRunLoopQueue != nullptr)
    {
        lRunLoopQueue->Perform();
    }
}

}; // namespace Client

}; // namespace HLX
