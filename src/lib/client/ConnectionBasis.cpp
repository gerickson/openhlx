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
 *      This file implements an object for a HLX client peer-to-peer
 *      network connection.
 *
 */

#include "ConnectionBasis.hpp"

#include <errno.h>

#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

#include "ConnectionBasisDelegate.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Client
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
    Common::ConnectionBasis(aSchemeRef),
    mURLRef(nullptr),
    mTimeout(),
    mState(kState_Unknown),
    mDelegate(nullptr)
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

    if (mURLRef != nullptr)
    {
        CFURelease(mURLRef);
        mURLRef = nullptr;
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

    lRetval = Common::ConnectionBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mState             = kState_Ready;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to the HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aURLRef   A reference to a CoreFoundation URL for the
 *                        HLX server peer to connect to.
 *  @param[in]  aTimeout  An immutable reference to the timeout by
 *                        which the connection should complete.
 *
 *  @note
 *    At present any meaningful work associated with the connection is
 *    handled by a dervied class.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Connect(CFURLRef aURLRef, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    mURLRef  = CFURetain(aURLRef);
    mTimeout = aTimeout;

    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the HLX server peer.
 *
 *  This attempts to asynchronously disconnect from the
 *  currently-connected HLX server peer, if any.
 *
 *  @note
 *    At present any meaningful work associated with the disconnection
 *    is handled by a dervied class.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Disconnect(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval;

    lRetval = Disconnect(kStatus_Success);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the HLX server peer with the specified error.
 *
 *  This attempts to asynchronously disconnect from the
 *  currently-connected HLX server peer, if any, with the specified
 *  error (that is, reason for disconnection), for example -ETIMEDOUT.
 *
 *  @note
 *    At present any meaningful work associated with the disconnection
 *    is handled by a dervied class.
 *
 *  @param[in]  aError  A reference to the error associated with the
 *                      reason for the disconnection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Disconnect(const Error &aError)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    (void)aError;

    if (mURLRef != nullptr)
    {
        CFURelease(mURLRef);
        mURLRef = nullptr;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the connection basis.
 *
 *  @returns
 *    A pointer to the delegate for the connection basis.
 *
 */
ConnectionBasisDelegate *
ConnectionBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the connection basis.
 *
 *  This attempts to set a delegate for the connection basis.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionBasis :: SetDelegate(ConnectionBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the URL for the connection peer.
 *
 *  @returns
 *    The URL for the connection peer.
 *
 */
CFURLRef
ConnectionBasis :: GetURL(void) const
{
    return (mURLRef);
}

/**
 *  @brief
 *    Return the default timeout for the connection.
 *
 *  @returns
 *    The default timeout for the connection.
 *
 */
const Timeout &
ConnectionBasis :: GetTimeout(void) const
{
    return (mTimeout);
}

// MARK: Connection Delegation Actions

/**
 *  @brief
 *    Signals to connection delegates that a connection will connect.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is about to
 *  connect.
 *
 */
void
ConnectionBasis :: OnWillConnect(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillConnect(*this, mURLRef, mTimeout);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection is connecting.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is connecting.
 *
 */
void
ConnectionBasis :: OnIsConnecting(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionIsConnecting(*this, mURLRef, mTimeout);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did connect.
 *
 *  This is invoked by a connection in response to a successful
 *  connection to signal delegates that the connection did connect.
 *
 */
void
ConnectionBasis :: OnDidConnect(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidConnect(*this, mURLRef);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did not
 *    connect.
 *
 *  This is invoked by a connection in response to a failed
 *  connection to signal delegates that the connection did not
 *  connect.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed connection.
 *
 */
void
ConnectionBasis :: OnDidNotConnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotConnect(*this, mURLRef, aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that the connection received
 *    application data.
 *
 *  This is invoked by a connection in response to the receipt of
 *  application data to signal delegates of that data.
 *
 *  @param[in]  aBuffer  A shared pointer to a mutable buffer
 *                       containing the application data.
 *
 */
void
ConnectionBasis :: OnApplicationDataReceived(ConnectionBuffer::MutableCountedPointer aBuffer)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidReceiveApplicationData(*this, aBuffer);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection will
 *    disconnect.
 *
 *  This is invoked by a connection in response to a pending
 *  disconnection to signal delegates that the connection will
 *  disconnect.
 *
 */
void
ConnectionBasis :: OnWillDisconnect(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillDisconnect(*this, mURLRef);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did
 *    disconnect.
 *
 *  This is invoked by a connection in response to a disconnection
 *  to signal delegates that the connection did disconnect.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the disconnection.
 *
 */
void
ConnectionBasis :: OnDidDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidDisconnect(*this, mURLRef, aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did
 *    not disconnect.
 *
 *  This is invoked by a connection in response to a failed
 *  disconnection to signal delegates that the connection did
 *  not disconnect.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed disconnection.
 *
 */
void
ConnectionBasis :: OnDidNotDisconnect(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotDisconnect(*this, mURLRef, aError);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection experienced an
 *    error.
 *
 *  This is invoked by a connection in response to a connection error
 *  to signal delegates that such a connection error occurred.
 *
 *  @note
 *    This action may occur along with other actions with respect to
 *    the same underlying event or cause.
 *
 *  @param[in]  aError  An immutable reference to the error associated
 *                      with the event.
 *
 */
void
ConnectionBasis :: OnError(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionError(*this, aError);
    }
}

/**
 *  @brief
 *    Returns whether or not the connection is connected to a peer.
 *
 *  @returns
 *    True if the connection is connected; otherwise, false.
 *
 */
bool
ConnectionBasis :: IsConnected(void) const
{
    return (IsState(kState_Connected));
}

/**
 *  @brief
 *    Returns whether or not the connection is connecting to a peer.
 *
 *  @returns
 *    True if the connection is connecting; otherwise, false.
 *
 */
bool
ConnectionBasis :: IsConnecting(void) const
{
    return (IsState(kState_Connecting));
}

/**
 *  @brief
 *    Returns whether or not the connection is disconnected from a
 *    peer.
 *
 *  @returns
 *    True if the connection is disconnected; otherwise, false.
 *
 */
bool
ConnectionBasis :: IsDisconnected(void) const
{
    return (IsState(kState_Disconnected));
}

/**
 *  @brief
 *    Returns whether or not the connection is disconnecting from a
 *    peer.
 *
 *  @returns
 *    True if the connection is disconnecting; otherwise, false.
 *
 */
bool
ConnectionBasis :: IsDisconnecting(void) const
{
    return (IsState(kState_Disconnecting));
}

/**
 *  @brief
 *    Returns whether or not the connection is in the specified state.
 *
 *  @param[in]  aState  The state to confirm.
 *
 *  @returns
 *    True if the connection is in the specified state; otherwise,
 *    false.
 *
 */
bool
ConnectionBasis :: IsState(State aState) const
{
    const bool lRetval = (mState == aState);


    return (lRetval);
}

/**
 *  @brief
 *    Returns the current connection state.
 *
 *  @returns
 *    The current connection state.
 *
 */
ConnectionBasis::State
ConnectionBasis :: GetState(void) const
{
    return (mState);
}

/**
 *  @brief
 *    Sets the current connection state.
 *
 *  @param[in]  aState  The state to set the connection to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the state was already set to
 *                                    the specified value.
 *
 */
Status
ConnectionBasis :: SetState(State aState)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mState != aState, done, lRetval = kStatus_ValueAlreadySet);

    mState = aState;

 done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
