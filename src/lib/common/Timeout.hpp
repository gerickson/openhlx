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
 *      This file defines types and objects for specifying timeouts.
 *
 */

#ifndef HLXCOMMONTIMEOUT_HPP
#define HLXCOMMONTIMEOUT_HPP

#include <stdint.h>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base structure for specifying a timeout, in milliseconds.
 *
 *  @ingroup common
 *
 */
struct TimeoutBasis
{
public:
    /**
     *  The type of timeout.
     *
     */
    enum Type
    {
        kType_Default      = 0,            //!< Default timeout type.
        kType_Milliseconds = 1,            //!< Milliseconds timeout type.
        kType_Never        = 2,            //!< Never/forever timeout type.
        kType_Forever      = kType_Never   //!< An alias for the never/forever timeout type.
    };

    /**
     *  For the Milliseconds timeout type, the duration of the
     *  timeout, in milliseconds.
     *
     */
    typedef uint32_t Value;

    TimeoutBasis(const Type &aType, const Value &aMilliseconds);

    Type   mType;          //!< The timeout type.
    Value  mMilliseconds;  //!< For the Milliseconds timeout type, the duration of the timeout, in milliseconds.

protected:
    TimeoutBasis(void);

    TimeoutBasis(const TimeoutBasis &aTimeoutBasis);
};

/**
 *  @brief
 *    A derived object for specifying a timeout, in milliseconds.
 *
 *  @ingroup common
 *
 */
class Timeout :
    public TimeoutBasis
{
public:
    Timeout(void) = default;
    Timeout(const Value &aMilliseconds);
    Timeout(Type &aType);
    Timeout(const Timeout &aTimeout);
    Timeout(const TimeoutBasis &aTimeoutBasis);
    ~Timeout(void) = default;

    Timeout &operator =(const Timeout &aTimeout);

    bool      IsType(const Type &aType) const;
    bool      IsDefault(void) const;
    bool      IsForever(void) const;
    bool      IsMilliseconds(void) const;
    bool      IsNever(void) const;

    Value     GetMilliseconds(void) const;
};

/**
 *  The default timeout when none is otherwise specified.
 *
 */
extern const struct TimeoutBasis kTimeoutDefault;

/**
 *  The never or forever timeout value to use when no timeout is
 *  desired.
 *
 */
extern const struct TimeoutBasis kTimeoutForever;

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONTIMEOUT_HPP
