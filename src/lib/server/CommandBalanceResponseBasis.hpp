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
 *      This file defines a derived object for composing HLX server
 *      command response buffers for the stereophonic channel balance
 *      data model property.
 *
 */

#ifndef HLXSERVERCOMMANDBALANCERESPONSEBASIS_HPP
#define HLXSERVERCOMMANDBALANCERESPONSEBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/CommandBalanceBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/CommandResponseBasis.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX server command response
 *    buffers for the stereophonic channel balance data model
 *    property.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceResponseBasis :
    public ResponseBasis,
    public Common::Command::BalanceBufferBasis
{
protected:
    BalanceResponseBasis(void) = default;
    virtual ~BalanceResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const ChannelType &aChannel, const BalanceType &aBalance);
    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const BalanceType &aBalance);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDBALANCERESPONSEBASIS_HPP
