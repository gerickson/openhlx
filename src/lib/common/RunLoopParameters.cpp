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
 *      This file implements an object for managing the parameters common
 *      to all run loop participants.
 *
 */

#include "RunLoopParameters.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


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
RunLoopParameters :: RunLoopParameters(void) :
    mRunLoopRef(nullptr),
    mRunLoopMode(kCFRunLoopDefaultMode)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the class with the specified CoreFoundation run
 *  loop and the default run loop mode.
 *
 *  @param[in]  aRunLoopRef   An immutable reference to the
 *                            CoreFoundation run loop to intialize
 *                            with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RunLoopParameters :: Init(const CFRunLoopRef &aRunLoopRef)
{
    return (Init(aRunLoopRef, kCFRunLoopDefaultMode));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the class with the specified CoreFoundation run
 *  loop and run loop mode.
 *
 *  @param[in]  aRunLoopRef   An immutable reference to the
 *                            CoreFoundation run loop to intialize
 *                            with.
 *  @param[in]  aRunLoopMode  An immutable reference to the
 *                            CoreFoundation run loop mode to
 *                            intialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RunLoopParameters :: Init(const CFRunLoopRef &aRunLoopRef,
                          const CFRunLoopMode &aRunLoopMode)
{
    Status lRetval = kStatus_Success;

    mRunLoopRef  = aRunLoopRef;
    mRunLoopMode = aRunLoopMode;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified run loop parameters.
 *
 *  @param[in]  aRunLoopParameters An immutable reference to the run
 *                                 loop parameters to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RunLoopParameters :: Init(const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    *this = aRunLoopParameters;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified run loop parameters to this
 *  one.
 *
 *  @param[in]  aRunLoopParameters An immutable reference to the run
 *                                 loop parameters to assign (copy).
 *
 *  @returns
 *    A reference to this run loop parameters after the assignment
 *    (copy) is complete.
 *
 */
RunLoopParameters &
RunLoopParameters :: operator =(const RunLoopParameters &aRunLoopParameters)
{
    // Avoid a self-copy

    nlEXPECT(&aRunLoopParameters != this, done);

    mRunLoopRef  = aRunLoopParameters.mRunLoopRef;
    mRunLoopMode = aRunLoopParameters.mRunLoopMode;

 done:
    return (*this);
}

/**
 *  @brief
 *    Return the CoreFoundation run loop.
 *
 *  @returns
 *    The CoreFoundation run loop.
 *
 */
CFRunLoopRef
RunLoopParameters :: GetRunLoop(void) const
{
    return (mRunLoopRef);
}

/**
 *  @brief
 *    Return the CoreFoundation run loop mode.
 *
 *  @returns
 *    The CoreFoundation run loop mode.
 *
 */
CFRunLoopMode
RunLoopParameters :: GetRunLoopMode(void) const
{
    return (mRunLoopMode);
}

}; // namespace Common

}; // namespace HLX
