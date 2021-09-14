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
 *      This file defines an abstract delegate to a run loop timer.
 *
 */

#ifndef OPENHLXCOMMONTIMERDELEGATE_HPP
#define OPENHLXCOMMONTIMERDELEGATE_HPP


namespace HLX
{

namespace Common
{

class Timer;

/**
 *  @brief
 *    Abstract delegate definition for a timer.
 *
 *  @ingroup common
 *
 */
class TimerDelegate
{
public:
    TimerDelegate(void) = default;
    ~TimerDelegate(void) = default;

    /**
     *  @brief
     *    Delegation from a timer that the timer fired/expired.
     *
     *  @param[in]  aTimer  A reference to the timer that
     *                      issued the delegation.
     *
     */
    virtual void TimerDidFire(Timer &aTimer) = 0;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONTIMERDELEGATE_HPP
