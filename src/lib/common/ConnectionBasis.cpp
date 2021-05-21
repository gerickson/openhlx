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
 *      This file implements a run loop aware base object for managing a
 *      connection bewteen two IP-based peers.
 *
 */

#include <utility>

#include "ConnectionBasis.hpp"

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;
using namespace std::rel_ops;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is a class default constructor.
 *
 *  This constructs an instance of the class with the specified URL
 *  scheme.
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme supported by the
 *                          connection.
 *
 */
ConnectionBasis :: ConnectionBasis(CFStringRef aSchemeRef) :
    mSchemeRef(CFURetain(aSchemeRef)),
    mRunLoopParameters(),
    mPeerAddress()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConnectionBasis :: ~ConnectionBasis(void)
{
    DeclareScopedFunctionTracer(lTracer);

    if (mSchemeRef != nullptr)
    {
        CFRelease(mSchemeRef);

        mSchemeRef = nullptr;
    }
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection basis on a run loop with the
 *  specified run loop parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection basis with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Init(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    mRunLoopParameters = aRunLoopParameters;

    return (lRetval);
}

/**
 *  @brief
 *    Return the protocol scheme for the connection.
 *
 *  @returns
 *    The protocol scheme for the connection.
 *
 */
CFStringRef
ConnectionBasis :: GetScheme(void) const
{
    return (mSchemeRef);
}

/**
 *  @brief
 *    Return the host address for the connection peer.
 *
 *  @returns
 *    The host address for the connection peer.
 *
 */
const HostURLAddress &
ConnectionBasis :: GetPeerAddress(void) const
{
    return (mPeerAddress);
}

/**
 *  @brief
 *    Return the run loop parameters for the connection.
 *
 *  @returns
 *    The run loop parameters for the connection.
 *
 */
RunLoopParameters &
ConnectionBasis :: GetRunLoopParameters(void)
{
    return (mRunLoopParameters);
}

/**
 *  @brief
 *    Set the host address for the connection peer.
 *
 *  @param[in]  aPeerAddress  An immutable reference to the host address
 *                            to set for the connection peer.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the state was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionBasis :: SetPeerAddress(const HostURLAddress &aPeerAddress)
{
    Status  lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mPeerAddress != aPeerAddress, done, lRetval = kStatus_ValueAlreadySet);

    mPeerAddress = aPeerAddress;

done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
