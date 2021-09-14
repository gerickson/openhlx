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
 *      This file defines interfaces for parsing values from
 *      null-terminated and extent-based character buffers.
 *
 */

#ifndef OPENHLXPARSE_HPP
#define OPENHLXPARSE_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Utilities
{

extern Common::Status Parse(const char *aBuffer, const size_t &aBufferLength, bool &aValue);
extern Common::Status Parse(const char *aBuffer, const size_t &aBufferLength, int8_t &aValue);
extern Common::Status Parse(const char *aBuffer, const size_t &aBufferLength, uint8_t &aValue);
extern Common::Status Parse(const char *aBuffer, const size_t &aBufferLength, uint16_t &aValue);
extern Common::Status Parse(const char *aBuffer, const size_t &aBufferLength, uint32_t &aValue);

extern Common::Status Parse(const char *aString, bool &aValue);
extern Common::Status Parse(const char *aString, int8_t &aValue);
extern Common::Status Parse(const char *aString, uint8_t &aValue);
extern Common::Status Parse(const char *aString, uint16_t &aValue);
extern Common::Status Parse(const char *aString, uint32_t &aValue);

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
inline Common::Status Parse(const uint8_t *aBuffer, const size_t &aBufferLength, bool &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aBuffer), aBufferLength, aValue));
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
inline Common::Status Parse(const uint8_t *aBuffer, const size_t &aBufferLength, int8_t &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aBuffer), aBufferLength, aValue));
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
inline Common::Status Parse(const uint8_t *aBuffer, const size_t &aBufferLength, uint8_t &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aBuffer), aBufferLength, aValue));
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
inline Common::Status Parse(const uint8_t *aBuffer, const size_t &aBufferLength, uint16_t &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aBuffer), aBufferLength, aValue));
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
inline Common::Status Parse(const uint8_t *aString, bool &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aString), aValue));
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
inline Common::Status Parse(const uint8_t *aString, int8_t &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aString), aValue));
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
inline Common::Status Parse(const uint8_t *aString, uint8_t &aValue)
{
    return (Parse(reinterpret_cast<const char *>(aString), aValue));
}

}; // namespace Utilities

}; // namespace HLX

#endif // OPENHLXPARSE_HPP
