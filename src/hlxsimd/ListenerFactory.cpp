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

#include <ListenerFactory.hpp>

#include <CFUtilities/CFString.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <ListenerTelnet.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

ListenerFactory :: ListenerFactory(void) :
    mRunLoopParameters(),
    mListeners()
{
    return;
}

ListenerFactory :: ~ListenerFactory(void)
{
    return;
}

Common::Status ListenerFactory :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

bool ListenerFactory :: SupportsScheme(CFStringRef aSchemeRef) const
{
    const CFString lRequestedScheme(aSchemeRef);
    bool lRetval = false;

    if (lRequestedScheme == ListenerTelnet::kScheme)
    {
        lRetval = true;
    }

    return (lRetval);
}

std::unique_ptr<ListenerBasis> ListenerFactory :: CreateListener(CFStringRef aSchemeRef) const
{
    const CFString                  lRequestedScheme(aSchemeRef);
    std::unique_ptr<ListenerBasis>  lRetval;

    if (lRequestedScheme == ListenerTelnet::kScheme)
    {
        lRetval.reset(new ListenerTelnet());
        nlREQUIRE(lRetval != nullptr, done);
    }

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
