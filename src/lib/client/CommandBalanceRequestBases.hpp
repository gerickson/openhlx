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
 *      This file defines derivable objects for HLX client
 *      stereophonic channel balance data model property mutation
 *      command request buffers.
 *
 */

#ifndef OPENHLXCLIENTCOMMANDBALANCEREQUESTBASIS_HPP
#define OPENHLXCLIENTCOMMANDBALANCEREQUESTBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandBalanceBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    data model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceRequestBasis :
    public RequestBasis,
    public Common::Command::BalanceBufferBasis
{
protected:
    BalanceRequestBasis(void) = default;
    virtual ~BalanceRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    adjustment data model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceAdjustRequestBasis :
    public BalanceRequestBasis
{
protected:
    BalanceAdjustRequestBasis(void) = default;
    virtual ~BalanceAdjustRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const Model::BalanceModel::ChannelType &aChannel);
};

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    adjust-towards-the-left-channel data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceAdjustLeftRequestBasis :
    public BalanceAdjustRequestBasis
{
protected:
    BalanceAdjustLeftRequestBasis(void) = default;
    virtual ~BalanceAdjustLeftRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    adjust-towards-the-right-channel data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceAdjustRightRequestBasis :
    public BalanceAdjustRequestBasis
{
protected:
    BalanceAdjustRightRequestBasis(void) = default;
    virtual ~BalanceAdjustRightRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    set data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceSetRequestBasis :
    public RequestBasis,
    public Common::Command::BalanceBufferBasis
{
protected:
    BalanceSetRequestBasis(void) = default;
    virtual ~BalanceSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const Model::BalanceModel::ChannelType &aChannel, const Model::BalanceModel::BalanceType &aBalance);
    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const Model::BalanceModel::BalanceType &aBalance);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client stereophonic channel balance
 *    set-to-center data model property mutation command request
 *    buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceCenterRequestBasis :
    public BalanceSetRequestBasis
{
protected:
    BalanceCenterRequestBasis(void) = default;
    virtual ~BalanceCenterRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCOMMANDBALANCEREQUESTBASIS_HPP
