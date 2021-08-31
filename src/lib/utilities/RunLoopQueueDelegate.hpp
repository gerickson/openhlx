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
 *      This file defines an abstract delegate to a run loop queue.
 *
 */

#ifndef OPENHLXUTILITIESRUNLOOPQUEUEDELEGATE_HPP
#define OPENHLXUTILITIESRUNLOOPQUEUEDELEGATE_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include <CoreFoundation/CFURL.h>

namespace HLX
{

namespace Utilities
{

class RunLoopQueue;

/**
 *  @brief
 *    Abstract delegate definition for a run loop queue.
 *
 *  @ingroup utilities
 *
 */
class RunLoopQueueDelegate
{
public:
    RunLoopQueueDelegate(void) = default;
    ~RunLoopQueueDelegate(void) = default;

    /**
     *  @brief
     *    Delegation from a run loop queue that the queue is empty.
     *
     *  @param[in]  aQueue  A reference to the run loop queue that
     *                      issued the delegation.
     *
     */
    virtual void QueueIsEmpty(RunLoopQueue &aQueue) = 0;

    /**
     *  @brief
     *    Delegation from a run loop queue that the queue is not
     *    empty.
     *
     *  @param[in]  aQueue  A reference to the run loop queue that
     *                      issued the delegation.
     *
     */
    virtual void QueueIsNotEmpty(RunLoopQueue &aQueue) = 0;
};

}; // namespace Utilities

}; // namespace HLX

#endif // OPENHLXUTILITIESRUNLOOPQUEUEDELEGATE_HPP
