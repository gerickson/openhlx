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
 *      This file implements interfaces for parsing values from
 *      null-terminated and extent-based character buffers.
 *
 */

#include "Parse.hpp"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/strntol.h>
#include <OpenHLX/Utilities/strntoul.h>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Utilities
{

/**
 *  @brief
 *    Parse a long value from the specified string buffer extent.
 *
 *  This attempts to parse a long value from the specified string
 *  buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              long value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
static Status
Parse(const char *aBuffer, const size_t &aBufferLength, long &aValue)
{
    Status          lRetval = kStatus_Success;


    (void)aBufferLength;

    errno = 0;

    aValue = strntol(aBuffer, aBufferLength, nullptr, 10);
    nlREQUIRE_ACTION(aValue != LONG_MAX, done, lRetval = -EOVERFLOW);
    nlREQUIRE_ACTION((errno != EINVAL) && (errno != ERANGE), done, lRetval = -errno);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse an unsigned long value from the specified string buffer
 *    extent.
 *
 *  This attempts to parse an unsigned long value from the specified
 *  string buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              unsigned long value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
static Status
Parse(const char *aBuffer, const size_t &aBufferLength, unsigned long &aValue)
{
    Status          lRetval = kStatus_Success;


    (void)aBufferLength;

    errno = 0;

    aValue = strntoul(aBuffer, aBufferLength, nullptr, 10);
    nlEXPECT_ACTION(aValue != ULONG_MAX, done, lRetval = -EOVERFLOW);
    nlEXPECT_ACTION((errno != EINVAL) && (errno != ERANGE), done, lRetval = -errno);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse a Boolean value from the specified string buffer extent.
 *
 *  This attempts to parse a Boolean value from the specified string
 *  buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              Boolean value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aBuffer, const size_t &aBufferLength, bool &aValue)
{
    uint8_t         lTemp;
    Status          lRetval;

    lRetval = Parse(aBuffer, aBufferLength, lTemp);
    nlREQUIRE_SUCCESS(lRetval, done);

    aValue = static_cast<bool>(lTemp);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse an signed 8-bit value from the specified string buffer
 *    extent.
 *
 *  This attempts to parse an signed 8-bit value from the specified
 *  string buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              signed 8-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aBuffer, const size_t &aBufferLength, int8_t &aValue)
{
    long            lTemp;
    Status          lRetval = kStatus_Success;


    lRetval = Parse(aBuffer, aBufferLength, lTemp);
    nlREQUIRE_SUCCESS(lRetval, done);

    aValue = static_cast<int8_t>(lTemp);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse an unsigned 8-bit value from the specified string buffer
 *    extent.
 *
 *  This attempts to parse an unsigned 8-bit value from the specified
 *  string buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              unsigned 8-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aBuffer, const size_t &aBufferLength, uint8_t &aValue)
{
    unsigned long   lTemp;
    Status          lRetval = kStatus_Success;


    lRetval = Parse(aBuffer, aBufferLength, lTemp);
    nlREQUIRE_SUCCESS(lRetval, done);

    aValue = static_cast<uint8_t>(lTemp & UINT8_MAX);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse an unsigned 16-bit value from the specified string buffer
 *    extent.
 *
 *  This attempts to parse an unsigned 16-bit value from the specified
 *  string buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aValue         A mutable reference to storage to parse the
 *                              unsigned 16-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aBuffer, const size_t &aBufferLength, uint16_t &aValue)
{
    unsigned long   lTemp;
    Status          lRetval = kStatus_Success;


    lRetval = Parse(aBuffer, aBufferLength, lTemp);
    nlREQUIRE_SUCCESS(lRetval, done);

    aValue = static_cast<uint16_t>(lTemp);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Parse a Boolean value from the specified null-terminated C
 *    string.
 *
 *  This attempts to parse a Boolean value from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString  A pointer to the start of the null-terminated
 *                        C string.
 *  @param[out]  aValue   A mutable reference to storage to parse the
 *                        Boolean value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aString, bool &aValue)
{
    return (Parse(aString, strlen(aString), aValue));
}

/**
 *  @brief
 *    Parse a signed 8-bit value from the specified null-terminated C
 *    string.
 *
 *  This attempts to parse a signed 8-bit value from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString  A pointer to the start of the null-terminated
 *                        C string.
 *  @param[out]  aValue   A mutable reference to storage to parse the
 *                        signed 8-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aString, int8_t &aValue)
{
    return (Parse(aString, strlen(aString), aValue));
}

/**
 *  @brief
 *    Parse a unsigned 8-bit value from the specified null-terminated C
 *    string.
 *
 *  This attempts to parse a unsigned 8-bit value from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString  A pointer to the start of the null-terminated
 *                        C string.
 *  @param[out]  aValue   A mutable reference to storage to parse the
 *                        unsigned 8-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aString, uint8_t &aValue)
{
    return (Parse(aString, strlen(aString), aValue));
}

/**
 *  @brief
 *    Parse a unsigned 16-bit value from the specified null-terminated C
 *    string.
 *
 *  This attempts to parse a unsigned 16-bit value from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString  A pointer to the start of the null-terminated
 *                        C string.
 *  @param[out]  aValue   A mutable reference to storage to parse the
 *                        unsigned 16-bit value into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
Status
Parse(const char *aString, uint16_t &aValue)
{
    return (Parse(aString, strlen(aString), aValue));
}

}; // namespace Utilities

}; // namespace HLX
