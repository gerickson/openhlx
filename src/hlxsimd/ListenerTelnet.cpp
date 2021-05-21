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
 *      This file implements a derived object for a HLX server network
 *      connection listener that uses the telnet protocol.
 *
 */

#include <ListenerTelnet.hpp>

#include <stddef.h>
#include <stdint.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>

using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Server
{

// Global Variables

static const uint16_t kTelnetPort = 23;

// Static Class Data Members

CFStringRef ListenerTelnet :: kScheme = CFSTR("telnet");

ListenerTelnet :: ListenerTelnet(void) :
    ListenerBasis(kScheme)
{
    return;
}

ListenerTelnet :: ~ListenerTelnet(void)
{
    return;
}

Status ListenerTelnet :: Init(const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    // Initialize the parent class now that the child intialization is
    // successfully finished.

    lRetval = ListenerBasis::Init(kTelnetPort, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
