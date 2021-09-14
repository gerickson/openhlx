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
 *      This file implements...
 *
 */

#include "ProxyCommand.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

namespace Command
{

Status
Proxy :: Init(const uint8_t *aBuffer,
              const size_t &aSize,
              const Client::Command::ResponseBasis &aResponse)
{
    Common::Command::Buffer &            lRequest  = static_cast<Common::Command::Buffer &>(mRequest);
    const char *                         lBuffer   = reinterpret_cast<const char *>(aBuffer);
    Common::Command::RegularExpression & lResponse = static_cast<Common::Command::RegularExpression &>(mResponse);
    Status                               lRetval   = kStatus_Success;

    // Downcast to the command buffer class such that we can
    // initialize the buffer without getting additional command
    // role-specific delimiters added. The request we were passed
    // should already have those.

    lRetval = lRequest.Init(lBuffer, aSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Downcast to the command regular expression class such that
    // we can initialize the regular expression without getting
    // additional command role-specific delimiters added. The
    // response we were passed should already have those.

    lRetval = lResponse.Init(aResponse.GetRegularExpression().GetRegexp(),
                             aResponse.GetRegularExpression().GetExpectedMatchCount());
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::Command::ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
