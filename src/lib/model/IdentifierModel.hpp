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
 *      This file defines an object for managing HLX object
 *      identifiers.
 *
 */

#ifndef OPENHLXMMODELIDENTIFIERMODEL_HPP
#define OPENHLXMMODELIDENTIFIERMODEL_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing HLX object identifiers.
 *
 *  This defines an object for managing HLX object identiers such as
 *  those used for equalizer bands and presets, favorites, groups,
 *  sources, and zones.
 *
 *  @ingroup model
 *
 */
class IdentifierModel
{
public:
    /**
     *  A type for an object (for example, group or zone) identifier.
     *
     */
    typedef uint8_t IdentifierType;

    static const IdentifierType kIdentifierInvalid;
    static const IdentifierType kIdentifierMin;

public:
    IdentifierModel(void);
    virtual ~IdentifierModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const IdentifierType &Identifier);
    Common::Status Init(const IdentifierModel &aIdentifierModel);

    IdentifierModel &operator =(const IdentifierModel &aIdentifierModel);

    Common::Status GetIdentifier(IdentifierType &aIdentifier) const;

    Common::Status SetIdentifier(const IdentifierType &aIdentifier);

    bool operator ==(const IdentifierModel &aIdentifierModel) const;

private:
    IdentifierType mIdentifier;
};

namespace Utilities
{

/**
 *  @brief
 *    Parse an identifier from the specified string buffer extent.
 *
 *  This attempts to parse an identifier from the specified string
 *  buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aIdentifier    A mutable reference to storage to parse the
 *                              identifier into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
inline Common::Status ParseIdentifier(const char *aBuffer, const size_t &aBufferLength, IdentifierModel::IdentifierType &aIdentifier)
{
    return (::HLX::Utilities::Parse(aBuffer, aBufferLength, aIdentifier));
}

/**
 *  @brief
 *    Parse an identifier from the specified null-terminated C string.
 *
 *  This attempts to parse an identifier from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString      A pointer to the start of the null-terminated
 *                            C string.
 *  @param[out]  aIdentifier  A mutable reference to storage to parse the
 *                            identifier into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
inline Common::Status ParseIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    return (::HLX::Utilities::Parse(aString, aIdentifier));
}

/**
 *  @brief
 *    Parse an identifier from the specified string buffer extent.
 *
 *  This attempts to parse an identifier from the specified string
 *  buffer extent.
 *
 *  @param[in]   aBuffer        A pointer to the start of the string buffer
 *                              extent.
 *  @param[in]   aBufferLength  The length of the string buffer extent.
 *  @param[out]  aIdentifier    A mutable reference to storage to parse the
 *                              identifier into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
inline Common::Status ParseIdentifier(const uint8_t *aBuffer, const size_t &aBufferLength, IdentifierModel::IdentifierType &aIdentifier)
{
    return (::HLX::Utilities::Parse(aBuffer, aBufferLength, aIdentifier));
}

/**
 *  @brief
 *    Parse an identifier from the specified null-terminated C string.
 *
 *  This attempts to parse an identifier from the specified
 *  null-terminated C string.
 *
 *  @param[in]   aString      A pointer to the start of the null-terminated
 *                            C string.
 *  @param[out]  aIdentifier  A mutable reference to storage to parse the
 *                            identifier into.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The parsed value was out of range.
 *  @retval  -EINVAL          No valid parseable characters were encountered.
 *  @retval  -EOVERFLOW       The parsed value was too large to represent.
 *
 */
inline Common::Status ParseIdentifier(const uint8_t *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    return (::HLX::Utilities::Parse(aString, aIdentifier));
}

}; // namespace Utilities

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELIDENTIFIERMODEL_HPP
