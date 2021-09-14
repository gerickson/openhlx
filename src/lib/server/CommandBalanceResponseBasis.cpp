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
 *      This file implements a derived object for composing HLX server
 *      command response buffers for the stereophonic channel balance
 *      data model property.
 *
 */

#include "CommandBalanceResponseBasis.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/CommandBuffer.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer for a stereophonic channel balance
 *    command response.
 *
 *  This initializes the sterephonic channel balance property command
 *  response of a specified value against a specific object and
 *  identifier.
 *
 *  @note
 *    This operates with an interface that uses the HLX's native
 *    L:{max, min} to {min, max}:R tagged discontinuous balance bias
 *    model.
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance response is to be formed. For
 *                           example, "O" for a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance response is
 *                           to be formed.
 *  @param[in]  aChannel     An immutable reference to the
 *                           stereophonic channel for the response.
 *  @param[in]  aBalance     An immutable reference to the
 *                           stereophonic channel balance bias for the
 *                           response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceResponseBasis :: Init(const char *aObject,
                             const IdentifierModel::IdentifierType &aIdentifier,
                             const BalanceModel::ChannelType &aChannel,
                             const BalanceModel::BalanceType &aBalance)
{
    return (BalanceBufferBasis::Init(*this,
                                     aObject,
                                     aIdentifier,
                                     aChannel,
                                     aBalance));
}

/**
 *  @brief
 *    This is the class initializer for a stereophonic channel balance
 *    command response.
 *
 *  This initializes the sterephonic channel balance property command
 *  response of a specified value against a specific object and
 *  identifier.
 *
 *  @note
 *    This operates with an interface that uses this stack's non-HLX
 *    native L:{-max, max}:R non-tagged continuous balance bais model
 *    that converts it into the HLX's native tagged discontinuous
 *    model.
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance response is to be formed. For
 *                           example, "O" for a zone object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance response is
 *                           to be formed.
 *  @param[in]  aBalance     An immutable reference to the
 *                           stereophonic channel balance bias for the
 *                           response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceResponseBasis :: Init(const char *aObject,
                             const IdentifierModel::IdentifierType &aIdentifier,
                             const BalanceModel::BalanceType &aBalance)
{
    return (BalanceBufferBasis::Init(*this,
                                     aObject,
                                     aIdentifier,
                                     aBalance));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
