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
 *      This file defines a run loop-aware queue for managing a queue
 *      of non-retained and unmanaged object pointers.
 *
 */

#ifndef HLXCLIENTRUNLOOPQUEUE_HPP
#define HLXCLIENTRUNLOOPQUEUE_HPP

#include <queue>

#include <CoreFoundation/CFRunLoop.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>

namespace HLX
{

namespace Client
{

class RunLoopQueueDelegate;

/**
 *  @brief
 *     A run loop-aware queue for managing a queue of non-retained and
 *     unmanaged object pointers.
 *
 *  @ingroup utilities
 *
 */
class RunLoopQueue
{
public:
    /**
     *  The type of element stored by the run loop queue.
     *
     */
    typedef void * element_type;

    /**
     *  The type of internal queue instantiated by the run loop queue.
     *
     */
    typedef std::queue<element_type> queue_type;

public:
    RunLoopQueue(void);
    ~RunLoopQueue(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    RunLoopQueueDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(RunLoopQueueDelegate *aDelegate);

    queue_type::size_type  GetSize(void) const;
    bool                   IsEmpty(void) const;

    Common::Status  Push(element_type aElement);
    element_type    Pop(void);
    void            Flush(void);

    bool operator ==(const RunLoopQueue &aQueue) const;

    // CFRunLoop Handler Trampolines

    static CFStringRef CopyDescription(const void *aContext);
    static void        Perform(void *aContext);

private:
    // CFRunLoop Handlers

    CFStringRef        CopyDescription(void) const;
    void               Perform(void);

private:
    Common::RunLoopParameters  mRunLoopParameters;
    RunLoopQueueDelegate *     mDelegate;
    CFRunLoopSourceRef         mRunLoopSourceRef;
    queue_type                 mQueue;
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTRUNLOOPQUEUE_HPP
