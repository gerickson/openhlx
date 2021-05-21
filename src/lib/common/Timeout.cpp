/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file implements types and objects for specifying timeouts.
 *
 */

#include "Timeout.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


namespace HLX
{

namespace Common
{

const struct TimeoutBasis kTimeoutDefault = { TimeoutBasis::kType_Default, 0 };
const struct TimeoutBasis kTimeoutForever = { TimeoutBasis::kType_Forever, 0 };

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
TimeoutBasis :: TimeoutBasis(void) :
    mType(kType_Default),
    mMilliseconds(0)
{
    return;
}

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructions a timeout of the specified type and with the
 *  specified number of milliseconds.
 *
 *  @param[in]  aType          An immutable reference to the type
 *                             to construct the timeout with.
 *  @param[in]  aMilliseconds  An immutable reference to the timeout
 *                             duration, in milliseconds. This is only
 *                             applicable for millisecond types and is
 *                             otherwise ignored.
 *
 */
TimeoutBasis :: TimeoutBasis(const Type &aType,
                             const Value &aMilliseconds) :
    mType(aType),
    mMilliseconds(aMilliseconds)
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aTimeoutBasis  An immutable reference to the
 *                             timeout basis to copy from.
 *
 */
TimeoutBasis :: TimeoutBasis(const TimeoutBasis &aTimeoutBasis) :
    mType(aTimeoutBasis.mType),
    mMilliseconds(aTimeoutBasis.mMilliseconds)
{
    return;
}

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructions a millisecond timeout with the
 *  specified number of milliseconds as its duration.
 *
 *  @param[in]  aMilliseconds  An immutable reference to the timeout
 *                             duration, in milliseconds.
 *
 */
Timeout :: Timeout(const Value &aMilliseconds) :
    TimeoutBasis(kType_Milliseconds, aMilliseconds)
{
    return;
}

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructions a timeout of the specified type and with a
 *  timeout duration of zero (0) milliseconds.
 *
 *  @param[in]  aType          An immutable reference to the type
 *                             to construct the timeout with.
 *
 */
Timeout :: Timeout(Type &aType) :
    TimeoutBasis(aType, 0)
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aTimeout  An immutable reference to the
 *                        time basis to copy from.
 *
 */
Timeout :: Timeout(const Timeout &aTimeout) :
    TimeoutBasis(aTimeout)
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aTimeoutBasis  An immutable reference to the
 *                             timeout basis to copy from.
 *
 */
Timeout :: Timeout(const TimeoutBasis &aTimeoutBasis) :
    TimeoutBasis(aTimeoutBasis)
{
    return;
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified timeout to this one.
 *
 *  @param[in]  aTimeout  An immutable reference to the timeout
 *                        to assign (copy).
 *
 *  @returns
 *    A reference to this timeout after the assignment (copy) is
 *    complete.
 *
 */
Timeout &
Timeout :: operator =(const Timeout &aTimeout)
{
    // Avoid a self-copy

    nlEXPECT(&aTimeout != this, done);

    mType         = aTimeout.mType;
    mMilliseconds = aTimeout.mMilliseconds;

 done:
    return (*this);
}

/**
 *  @brief
 *    Return whether the timeout is of the specified type.
 *
 *  @param[in]  aType  An immutable reference to the timeout type
 *                     to introspect against.
 *
 *  @returns
 *    True if the timeout is of the specified type; otherwise, false.
 *
 */
bool
Timeout :: IsType(const Type &aType) const
{
    return (mType == aType);
}

/**
 *  @brief
 *    Return whether the timeout is of the default type.
 *
 *  @returns
 *    True if the timeout is of the default type; otherwise, false.
 *
 */
bool
Timeout :: IsDefault(void) const
{
    return (IsType(kType_Default));
}

/**
 *  @brief
 *    Return whether the timeout is of the forever type.
 *
 *  @returns
 *    True if the timeout is of the forever type; otherwise, false.
 *
 */
bool
Timeout :: IsForever(void) const
{
    return (IsType(kType_Forever));
}

/**
 *  @brief
 *    Return whether the timeout is of the milliseconds type.
 *
 *  @returns
 *    True if the timeout is of the milliseconds type; otherwise,
 *    false.
 *
 */
bool
Timeout :: IsMilliseconds(void) const
{
    return (IsType(kType_Milliseconds));
}

/**
 *  @brief
 *    Return whether the timeout is of the never type.
 *
 *  @returns
 *    True if the timeout is of the never type; otherwise, false.
 *
 */
bool
Timeout :: IsNever(void) const
{
    return (IsType(kType_Never));
}

/**
 *  @brief
 *    Return the timeout duration, in milliseconds.
 *
 *  @returns
 *    The timeout duration, in milliseconds.
 *
 */
Timeout::Value
Timeout :: GetMilliseconds(void) const
{
    return (mMilliseconds);
}

}; // namespace Common

}; // namespace HLX
