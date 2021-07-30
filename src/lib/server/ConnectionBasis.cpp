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
 *      This file implements an object for a HLX server peer-to-peer
 *      network connection.
 *
 */

#include "ConnectionBasis.hpp"

#include <errno.h>

#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ConnectionBasisDelegate.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

ConnectionBasis :: ConnectionBasis(CFStringRef aSchemeRef) :
    Common::ConnectionBasis(aSchemeRef),
    mState(kState_Unknown),
    mDelegate(nullptr)
{
    return;
}

ConnectionBasis :: ~ConnectionBasis(void)
{
    DeclareScopedFunctionTracer(lTracer);

    mDelegate = nullptr;

    SetState(kState_Unknown);

    return;
}

Status ConnectionBasis :: Init(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = Common::ConnectionBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mState             = kState_Ready;

 done:
    return (lRetval);
}

Status ConnectionBasis :: Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress)
{
    const bool      lIsHostOrder = true;
    HostURLAddress  lPeerAddress;
    Status          lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSocket > 0, done, lRetval = -EINVAL);

    // Initialize our peer address.

    lRetval = lPeerAddress.Init(GetScheme(), aPeerAddress, !lIsHostOrder);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SetPeerAddress(lPeerAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ConnectionBasis :: Disconnect(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    return (lRetval);
}

ConnectionBasisDelegate * ConnectionBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

Status ConnectionBasis :: SetDelegate(ConnectionBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

void ConnectionBasis :: OnWillAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillAccept(*this);
    }
}

void ConnectionBasis :: OnIsAccepting(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionIsAccepting(*this);
    }
}

void ConnectionBasis :: OnDidAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidAccept(*this);
    }
}

void ConnectionBasis :: OnDidNotAccept(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotAccept(*this, aError);
    }
}

void ConnectionBasis :: OnApplicationDataReceived(ConnectionBuffer::MutableCountedPointer aBuffer)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidReceiveApplicationData(*this, aBuffer);
    }
}

void ConnectionBasis :: OnWillDisconnect(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillDisconnect(*this, GetPeerAddress().GetURL());
    }
}

void ConnectionBasis :: OnDidDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidDisconnect(*this, GetPeerAddress().GetURL(), aError);
    }
}

void ConnectionBasis :: OnDidNotDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotDisconnect(*this, GetPeerAddress().GetURL(), aError);
    }
}

void ConnectionBasis :: OnError(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionError(*this, aError);
    }
}

bool ConnectionBasis :: IsState(State aState) const
{
    const bool lRetval = (mState == aState);


    return (lRetval);
}

ConnectionBasis::State ConnectionBasis :: GetState(void) const
{
    return (mState);
}

Status ConnectionBasis :: SetState(State aState)
{
    //DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    //Log::Debug().Write("mState %u aState %u\n", mState, aState);

    nlREQUIRE_ACTION(mState != aState, done, lRetval = kStatus_ValueAlreadySet);

    mState = aState;

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
