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
 *      This file implements objects for HLX server network data model
 *      commands and their constituent requests and responses.
 *
 */

#include "NetworkControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Network
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the network interface query command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the network interface query command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    static const char * const kNetworkObject = "E";

    return (QueryResponseBasis::Init(kNetworkObject));
}

}; // namespace Network

}; // namespace Command

}; // namespace Server

}; // namespace HLX
