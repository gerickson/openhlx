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
 *      This file implements a derived object for composing HLX command
 *      buffers that observe or set the stereophonic channel balance
 *      data model property.
 *
 */

#include "CommandBalanceBufferBasis.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

namespace Command
{

static const char kBalanceProperty = 'B';

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes a stereophonic channel balance operation (for
 *  example, increment left) against a specific object identifier into
 *  the specified command buffer.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance
 *                               operation is to be made against.
 *  @param[in]      aOperation   A pointer to a null-terminated C
 *                               string representing the operation to
 *                               perform. This might be an increment
 *                               left, increment right, center, or set
 *                               to a specific level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceBufferBasis :: Init(BufferBasis &aBuffer,
                           const char *aObject,
                           const IdentifierType &aIdentifier,
                           const char *aOperation)
{
    return (PropertyBufferBasis::Init(aBuffer,
                                      kBalanceProperty,
                                      aObject,
                                      aIdentifier,
                                      aOperation));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes a stereophonic channel balance operation (for
 *  example, increment left) against a specific object identifier into
 *  the specified command buffer.
 *
 *  @note
 *    This operates with an interface that uses the HLX's native
 *    L:{max, min} to {min, max}:R tagged discontinuous balance bias
 *    model.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance
 *                               operation is to be made against.
 *  @param[in]      aChannel     An immutable reference to the
 *                               stereophonic channel for the
 *                               operation.
 *  @param[in]      aBalance     An immutable reference to the
 *                               stereophonic channel balance bias for
 *                               the operation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceBufferBasis :: Init(BufferBasis &aBuffer,
                           const char *aObject,
                           const IdentifierType &aIdentifier,
                           const ChannelType &aChannel,
                           const BalanceType &aBalance)
{
    OutputStringStream  lBalanceStream;
    std::string         lOperation;


    // Upcast the balance to a signed integer to ensure it is
    // interpretted as something to be converted rather than a
    // character literal.

    lBalanceStream << static_cast<unsigned int>(aBalance);

    lOperation  = aChannel;
    lOperation += lBalanceStream.str();

    return (Init(aBuffer, aObject, aIdentifier, lOperation.c_str()));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes a stereophonic channel balance operation (for
 *  example, increment left) against a specific object identifier into
 *  the specified command buffer.
 *
 *  @note
 *    This operates with an interface that uses this stack's non-HLX
 *    native L:{-max, max}:R non-tagged continuous balance bais model
 *    that converts it into the HLX's native tagged discontinuous
 *    model.
 *
 *  @param[in,out]  aBuffer      A mutable reference to the command
 *                               buffer to compose the operation into.
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance
 *                               operation is to be made against.
 *  @param[in]      aBalance     An immutable reference to the
 *                               stereophonic channel balance bias for
 *                               the operation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceBufferBasis :: Init(BufferBasis &aBuffer,
                           const char *aObject,
                           const IdentifierType &aIdentifier,
                           const BalanceType &aBalance)
{
    ChannelType  lChannel;
    BalanceType  lBalance;

    // Adjust the balance from this stack's non-tagged, continuous
    // L:{-80, 80}:R model to the HLX's L:{80, 0} to {0, 80}:R tagged
    // discontinuous model.

    if (aBalance <= BalanceModel::kBalanceCenter)
    {
        lChannel = BalanceModel::kChannelLeft;
        lBalance = -aBalance;
    }
    else
    {
        lChannel = BalanceModel::kChannelRight;
        lBalance = aBalance;
    }

    return (Init(aBuffer, aObject, aIdentifier, lChannel, lBalance));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
