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
 *      This file implements derived objects for composing HLX client
 *      command request buffers for the stereophonic channel balance
 *      data model property.
 *
 */

#include "CommandBalanceRequestBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/CommandBuffer.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a stereophonic channel balance mutation request
 *  operation (for example, adjust left) against a specific object
 *  identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance mutation request is to be made
 *                           against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance mutation
 *                           request is to be made against.
 *  @param[in]  aOperation   A pointer to a null-terminated C string
 *                           representing the operation to perform on
 *                           the mutation request. This might be an
 *                           adjust left, adjust right, center,
 *                           or set to a specific level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceRequestBasis :: Init(const char *aObject,
                            const IdentifierModel::IdentifierType &aIdentifier,
                            const char *aOperation)
{
    return (BalanceBufferBasis::Init(*this, aObject, aIdentifier, aOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a stereophonic channel balance adjustment request
 *  operation (for example, adjust left) against a specific object
 *  identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance request is to be made
 *                           against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance request is
 *                           to be made against.
 *  @param[in]  aChannel     An immutable reference to the channel to
 *                           adjust the balance towards.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceAdjustRequestBasis :: Init(const char *aObject,
                                  const IdentifierModel::IdentifierType &aIdentifier,
                                  const BalanceModel::ChannelType &aChannel)
{
    static const char * const kBalanceAdjustOperation = "U";
    std::string lOperation;


    lOperation  = aChannel;
    lOperation += kBalanceAdjustOperation;

    return (BalanceRequestBasis::Init(aObject, aIdentifier, lOperation.c_str()));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a stereophonic channel balance adjust left
 *  request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance request is to be made
 *                           against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance request is
 *                           to be made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceAdjustLeftRequestBasis :: Init(const char *aObject,
                                      const IdentifierModel::IdentifierType &aIdentifier)
{
    static const BalanceModel::ChannelType kChannel = BalanceModel::kChannelLeft;


    return (BalanceAdjustRequestBasis::Init(aObject, aIdentifier, kChannel));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a stereophonic channel balance adjust right
 *  request operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           balance request is to be made
 *                           against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the balance request is
 *                           to be made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceAdjustRightRequestBasis :: Init(const char *aObject,
                                       const IdentifierModel::IdentifierType &aIdentifier)
{
    static const BalanceModel::ChannelType kChannel = BalanceModel::kChannelRight;


    return (BalanceAdjustRequestBasis::Init(aObject, aIdentifier, kChannel));
}

/**
 *  @brief
 *    This is the class initializer for a stereophonic channel balance
 *    set operation.
 *
 *  This initializes the sterephonic channel balance property set
 *  operation of a specified value against a specific object and
 *  identifier.
 *
 *  @note
 *    This operates with an interface that uses the HLX's native
 *    L:{max, min} to {min, max}:R tagged discontinuous balance bias
 *    model.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance set
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
BalanceSetRequestBasis :: Init(const char *aObject,
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
 *    set operation.
 *
 *  This initializes the sterephonic channel balance property set
 *  operation of a specified value against a specific object and
 *  identifier.
 *
 *  @note
 *    This operates with an interface that uses this stack's non-HLX
 *    native L:{-max, max}:R non-tagged continuous balance bais model
 *    that converts it into the HLX's native tagged discontinuous
 *    model.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance set
 *                               operation is to be made against.
 *  @param[in]      aBalance     An immutable reference to the
 *                               stereophonic channel balance bias for
 *                               the operation.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceSetRequestBasis :: Init(const char *aObject,
                               const IdentifierModel::IdentifierType &aIdentifier,
                               const BalanceModel::BalanceType &aBalance)
{
    return (BalanceBufferBasis::Init(*this,
                                     aObject,
                                     aIdentifier,
                                     aBalance));
}

/**
 *  @brief
 *    This is the class initializer for a stereophonic channel balance
 *    set-to-center operation.
 *
 *  This initializes the sterephonic channel balance property set
 *  operation to the center value against a specific object and
 *  identifier.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the balance operation is to be
 *                               made against. For example, "O" for a
 *                               zone object.
 *  @param[in]      aIdentifier  A reference to the specific object
 *                               identifier which the balance
 *                               set-to-center operation is to be made
 *                               against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
BalanceCenterRequestBasis :: Init(const char *aObject,
                                  const IdentifierModel::IdentifierType &aIdentifier)
{
    static const BalanceModel::BalanceType kBalance = BalanceModel::kBalanceCenter;


    return (BalanceSetRequestBasis::Init(aObject,
                                         aIdentifier,
                                         kBalance));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
