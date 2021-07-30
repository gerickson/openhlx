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

Status BalanceResponseBasis :: Init(const char *aObject, const IdentifierType &aIdentifier, const ChannelType &aChannel, const BalanceType &aBalance)
{
    return (BalanceBufferBasis::Init(*this, aObject, aIdentifier, aChannel, aBalance));
}

Status BalanceResponseBasis :: Init(const char *aObject, const IdentifierModel::IdentifierType &aIdentifier, const BalanceModel::BalanceType &aBalance)
{
    return (BalanceBufferBasis::Init(*this, aObject, aIdentifier, aBalance));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
