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
 *      This file implements base and derived objects for composing HLX
 *      commands that observe or set HLX physical front panel control
 *      data model properties.
 *
 */

#include "CommandFrontPanelBufferBases.hpp"

#include <string>

#include <OpenHLX/Common/CommandBufferBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace FrontPanel
{

static const char * const kBrightnessObject = "SD";
static const char * const kLockedObject     = "FPL";

/**
 *  @brief
 *    This initializes the front panel query command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the query operation
 *                               into.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryLockedBufferBasis :: Init(Common::Command::BufferBasis &aBuffer)
{
    return (QueryBufferBasis::Init(aBuffer, kLockedObject));
}

/**
 *  @brief
 *    This is the class initializer for a multi-character
 *    operation of an unsigned 8-bit integer value.
 *
 *  This initializes a operation with a specified value into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the property
 *                               mutation operation into.
 *  @param[in]      aOperation   The operation to perform.
 *  @param[in]      aValue       An immutable reference to the unsigned
 *                               8-bit value to use for the operation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                    const char *aOperation,
                    const uint8_t &aValue)
{
    OutputStringStream         lValueStream;
    std::string                lOperationString;

    // Set the width to one (1) while upcasting the value to an
    // unsigned integer to ensure it is interpretted as something to
    // be converted rather than a character literal.

    lValueStream << std::setw(1) << static_cast<unsigned int>(aValue);

    // Compose the buffer with the command and brightness.

    lOperationString = aOperation;
    lOperationString += lValueStream.str();

    return (aBuffer.Init(lOperationString.c_str(), lOperationString.size()));
}

/**
 *  @brief
 *    This is a class initializer for a front panel brightness set
 *    operation.
 *
 *  This initializes the front panel brightness property set operation
 *  of a specified brightness into the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aBrightness  An immutable reference to the
 *                               brightness level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BrightnessBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                              const BrightnessType &aBrightness)
{
    return (BufferBasis::Init(aBuffer,
                              kBrightnessObject,
                              aBrightness));
}

/**
 *  @brief
 *    This is a class initializer for a front panel locked state set
 *    operation.
 *
 *  This initializes the front panel locked state property set operation
 *  of a specified locked state into the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aLocked      An immutable reference to the locked
 *                               state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LockedBufferBasis :: Init(Common::Command::BufferBasis &aBuffer,
                          const LockedType &aLocked)
{
    return (BufferBasis::Init(aBuffer,
                              kLockedObject,
                              aLocked));
}

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Common

}; // namespace HLX
