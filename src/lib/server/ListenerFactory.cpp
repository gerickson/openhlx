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
 *      This file implements an object for creating HLX server
 *      network connection listeners.
 *
 */

#include "ListenerFactory.hpp"

#include <CFUtilities/CFString.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ListenerTelnet.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ListenerFactory :: ListenerFactory(void) :
    mRunLoopParameters(),
    mListeners()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ListenerFactory :: ~ListenerFactory(void)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection listener factory with the
 *  specified run loop parameters.
 *
 *  @note
 *    The connection listener factory itself is not a run loop source;
 *    however, the connection listeners it allocates and instantiates
 *    likely will be.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection listener factory with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ListenerFactory :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

/**
 *  @brief
 *    Determine whether the factory supports creating a connection
 *    listener with the specified protocol scheme.
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme for which to check
 *                          support.
 *
 *  @returns
 *     True if the scheme is supported; otherwise, false.
 *
 */
bool
ListenerFactory :: SupportsScheme(CFStringRef aSchemeRef) const
{
    const CFString lRequestedScheme(aSchemeRef);
    bool lRetval = false;

    if (lRequestedScheme == ListenerTelnet::kScheme)
    {
        lRetval = true;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Create a connection listener with the specified protocol scheme.
 *
 *  This attempts to create a connection listener with the specified
 *  protocol scheme (for example, "telnet").
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme for which to create
 *                          a connection listener.
 *
 *  @returns
 *    A valid unique pointer to the newly-created connection listener
 *    on success; otherwise, an empty, invalid unique pointer.
 *
 */
std::unique_ptr<ListenerBasis>
ListenerFactory :: CreateListener(CFStringRef aSchemeRef) const
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
