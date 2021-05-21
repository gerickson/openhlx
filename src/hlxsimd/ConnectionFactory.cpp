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
 *      This file...
 *
 */

#include <ConnectionFactory.hpp>

#include <CFUtilities/CFString.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <ConnectionTelnet.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

ConnectionFactory :: ConnectionFactory(void) :
    mRunLoopParameters(),
    mConnections()
{
    return;
}

ConnectionFactory :: ~ConnectionFactory(void)
{
    return;
}

Common::Status ConnectionFactory :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

bool ConnectionFactory :: SupportsScheme(CFStringRef aSchemeRef) const
{
    const CFString lRequestedScheme(aSchemeRef);
    bool lRetval = false;

    if (lRequestedScheme == ConnectionTelnet::kScheme)
    {
        lRetval = true;
    }

    return (lRetval);
}

std::unique_ptr<ConnectionBasis> ConnectionFactory :: CreateConnection(CFStringRef aSchemeRef) const
{
    const CFString                    lRequestedScheme(aSchemeRef);
    std::unique_ptr<ConnectionBasis>  lRetval;

    if (lRequestedScheme == ConnectionTelnet::kScheme)
    {
        lRetval.reset(new ConnectionTelnet());
        nlREQUIRE(lRetval != nullptr, done);
    }

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
