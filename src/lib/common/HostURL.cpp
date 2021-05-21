/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file implements an object for working with and managing an
 *      IETF RFC 1738-style Internet host URL.
 *
 */

#include "HostURL.hpp"

#include <utility>

#include <errno.h>

#include <CoreFoundation/CFSocket.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

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
HostURL :: HostURL(void) :
    mURLRef(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
HostURL :: ~HostURL(void)
{
    CFURLRef  lURLRef = nullptr;

    CFUReferenceSet(mURLRef, lURLRef);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL with the specified URL as a
 *  null-terminated C string.
 *
 *  @param[in]  aURL           A pointer to a null-terminated C
 *                             string of the IETF RFC 1738-style
 *                             Internet host URL to initialize the
 *                             host URL with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aURL is null or if the length
 *                            of @a aURL is zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURL :: Init(const char *aURL)
{
    size_t    lURLLength;
    CFURLRef  lURLRef;
    Status    lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aURL != nullptr, done, lRetval = -EINVAL);

    lURLLength = strlen(aURL);
    nlREQUIRE_ACTION(lURLLength > 0, done, lRetval = -EINVAL);

    lURLRef = CFURLCreateWithBytes(kCFAllocatorDefault,
                                   reinterpret_cast<const UInt8 *>(aURL),
                                   static_cast<CFIndex>(lURLLength),
                                   CFStringGetSystemEncoding(),
                                   nullptr);
    nlREQUIRE_ACTION(lURLRef != nullptr, done, lRetval = -ENOMEM);

    mURLRef = lURLRef;

 done:
    return lRetval;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL with the specified URL as a
 *  CoreFoundation string.
 *
 *  @param[in]  aURLStringRef  A pointer to a CoreFoundation
 *                             string of the IETF RFC 1738-style
 *                             Internet host URL to initialize the
 *                             host URL with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aURLStringRef is null or if
 *                            the length of @a aURLStringRef is
 *                            zero (0).
 *  @retval  -ENOMEM          If memory could not be allocated.
 *
 */
Status
HostURL :: Init(CFStringRef aURLStringRef)
{
    CFIndex   lURLLength;
    CFURLRef  lURLRef;
    Status    lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aURLStringRef != nullptr, done, lRetval = -EINVAL);

    lURLLength = CFStringGetLength(aURLStringRef);
    nlREQUIRE_ACTION(lURLLength > 0, done, lRetval = -EINVAL);

    lURLRef = CFURLCreateWithString(kCFAllocatorDefault,
                                    aURLStringRef,
                                    nullptr);
    nlREQUIRE_ACTION(lURLRef != nullptr, done, lRetval = -ENOMEM);

    mURLRef = lURLRef;

 done:
    return lRetval;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the host URL with the specified URL as a
 *  CoreFoundation URL.
 *
 *  @param[in]  aURLRef        A pointer to a CoreFoundation
 *                             URL of the IETF RFC 1738-style
 *                             Internet host URL to initialize the
 *                             host URL with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aURLStringRef is null or if
 *                            the length of @a aURLStringRef is
 *                            zero (0).
 *
 */
Status
HostURL :: Init(CFURLRef aURLRef)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aURLRef != nullptr, done, lRetval = -EINVAL);

    CFUReferenceSet(mURLRef, aURLRef);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified host URL to this one.
 *
 *  @param[in]  aHostURL       An immutable reference to the host URL
 *                             to assign (copy).
 *
 *  @returns
 *    A reference to this host URL after the assignment (copy) is
 *    complete.
 *
 */
HostURL &
HostURL :: operator =(const HostURL &aHostURL)
{
    Status      lStatus;

    // Avoid a self-copy

    nlEXPECT(&aHostURL != this, done);

    // If the rvalue is non-null, effect a URL copy (there is no
    // CFURLCreateCopy) by getting the string and passing it through
    // the initializer. Otherwise, just swap the URL references.

    if (aHostURL.mURLRef != nullptr)
    {
        lStatus = Init(CFURLGetString(aHostURL.mURLRef));
        nlREQUIRE_SUCCESS(lStatus, done);
    }
    else
    {
        CFUReferenceSet(mURLRef, aHostURL.mURLRef);
    }

done:
    return (*this);
}

/**
 *  @brief
 *    This is a class move (swap) operator.
 *
 *  This moves (swaps) the specified host URL with this one.
 *
 *  @param[in,out]  aHostURL   A mutable reference to the host URL
 *                             to move (swap) with this one.
 *
 *  @returns
 *    A reference to this host URL after the move (swap) is
 *    complete.
 *
 */
HostURL &
HostURL :: operator =(HostURL &&aHostURL)
{
    auto lURLRef = mURLRef;

    // Avoid a self-move

    nlEXPECT(&aHostURL != this, done);

    mURLRef = std::move(aHostURL.mURLRef);

    aHostURL.mURLRef = lURLRef;

 done:
    return (*this);
}

/**
 *  @brief
 *    Return the CoreFoundation URL for the host URL.
 *
 *  @returns
 *    The CoreFoundation URL for the host URL.
 *
 */
CFURLRef
HostURL :: GetURL(void) const
{
    return (mURLRef);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided host URL against this one to determine
 *  if they are equal to one another.
 *
 *  @param[in]  aHostURL       An immutable reference to the host URL
 *                             to compare for equality.
 *
 *  @returns
 *    True if this host URL is equal to the specified one; otherwise,
 *    false.
 *
 */
bool
HostURL :: operator ==(const HostURL &aHostURL) const
{
    bool lRetval = false;

    if (this == &aHostURL)
    {
        lRetval = true;
    }
    else if ((mURLRef == nullptr) && (aHostURL.mURLRef == nullptr))
    {
        lRetval = true;
    }
    else if ((mURLRef != nullptr) && (aHostURL.mURLRef != nullptr))
    {
        lRetval = CFEqual(mURLRef, aHostURL.mURLRef);
    }

    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
