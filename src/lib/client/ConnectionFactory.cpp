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
 *      This file implements an object for creating HLX client
 *      connections.
 *
 */

#include "ConnectionFactory.hpp"

#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>

#include "ConnectionTelnet.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionFactory :: ConnectionFactory(void) :
    mConnections()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConnectionFactory :: ~ConnectionFactory(void)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection factory with the specified run
 *  loop parameters.
 *
 *  @note
 *    The connection factory itself is not a run loop source; however,
 *    the connections it allocates and instantiates likely will be.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection factory with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          Resources for a connection could
 *                            not be allocated.
 *
 */
Status
ConnectionFactory :: Init(const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    static ConnectionTelnet sConnectionTelnet;

    lRetval = sConnectionTelnet.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mConnections[ConnectionTelnet::kScheme] = &sConnectionTelnet;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return a connection for the protocol scheme associated with the
 *    specified peer URL.
 *
 *  This attempts to return a connection suitable for the protocol
 *  scheme associated with the specified peer URL.
 *
 *  @param[in]  aURLRef  A reference to a CoreFoundation URL for which
 *                       the protocol scheme will be used to get a
 *                       suitable connection.
 *
 *  @returns
 *    A pointer to the connection if successful; otherwise null.
 *
 */
ConnectionBasis *
ConnectionFactory :: GetConnection(CFURLRef aURLRef) const
{
    DeclareScopedFunctionTracer(lTracer);
    Connections::const_iterator  lCurrent, lEnd;
    ConnectionBasis *            lConnection = nullptr;
    CFString                     lRequestedScheme;

    nlREQUIRE(aURLRef != nullptr, done);

    lRequestedScheme = CFURLCopyScheme(aURLRef);

    lCurrent = mConnections.begin();
    lEnd = mConnections.end();

    while (lCurrent != lEnd)
    {
        const CFString lCurrentScheme(lCurrent->first);

        if (lRequestedScheme == lCurrentScheme)
        {
            lConnection = lCurrent->second;
            break;
        }

        lCurrent++;
    }

done:
    return (lConnection);
}

}; // namespace Client

}; // namespace HLX
