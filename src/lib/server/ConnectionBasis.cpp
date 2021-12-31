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
#include <OpenHLX/Utilities/Assert.hpp>

#include "ConnectionBasisDelegate.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
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
    mIdentifier(0),
    mConnectedSocket(-1),
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

    mDelegate = nullptr;

    SetState(kState_Unknown);

    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection basis on a run loop with the
 *  specified run loop parameters and connection scheme identifier.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection basis with.
 *  @param[in]  aIdentifier         An immutable reference to the
 *                                  identifier associated with this
 *                                  connection and its protocol scheme
 *                                  (for example, "telnet").
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Init(const RunLoopParameters &aRunLoopParameters, const IdentifierType &aIdentifier)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = Common::ConnectionBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    mIdentifier        = aIdentifier;
    mState             = kState_Ready;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to the HLX client peer.
 *
 *  This establishes connection state for the HLX client peer at the
 *  specified socket and peer address.
 *
 *  @param[in]  aSocket        An immutable reference to the native
 *                             socket descriptor associated with the
 *                             accepted connection.
 *  @param[in]  aPeerAddress   An immutable reference to the socket
 *                             address associated with the peer client
 *                             at the remote end of the accepted
 *                             connection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the peer address was already
 *                                    set.
 *  @retval  -EINVAL                  The socket was invalid or the
 *                                    connection scheme is null or has
 *                                    zero (0) length.
 *  @retval  -ENOMEM                  If memory could not be allocated.
 *
 */
Status
ConnectionBasis :: Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress)
{
    constexpr bool  kIsHostOrder = true;
    HostURLAddress  lPeerAddress;
    Status          lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSocket > 0, done, lRetval = -EINVAL);

    // Initialize our peer address.

    lRetval = lPeerAddress.Init(GetScheme(), aPeerAddress, !kIsHostOrder);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SetPeerAddress(lPeerAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

    mConnectedSocket = aSocket;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the HLX client peer.
 *
 *  This attempts to asynchronously disconnect from the
 *  currently-connected HLX client peer, if any.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionBasis :: Disconnect(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to close the socket associated with the HLX client peer.
 *
 *  If the socket associated with the HLX client peer is open, this
 *  closes it.
 *
 */
void
ConnectionBasis :: Close(void)
{
    if (mConnectedSocket != -1)
    {
        close(mConnectedSocket);

        mConnectedSocket = -1;
    }
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

// MARK: Connection Delegation Actions

/**
 *  @brief
 *    Signals to connection delegates that a connection will accept.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is about to
 *  accept.
 *
 */
void
ConnectionBasis :: OnWillAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionWillAccept(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection is accepting.
 *
 *  This is invoked by a connection in response to a pending
 *  connection to signal delegates that the connection is accepting.
 *
 */
void
ConnectionBasis :: OnIsAccepting(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionIsAccepting(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did accept.
 *
 *  This is invoked by a connection in response to a successful
 *  connection to signal delegates that the connection did accept.
 *
 */
void
ConnectionBasis :: OnDidAccept(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidAccept(*this);
    }
}

/**
 *  @brief
 *    Signals to connection delegates that a connection did not
 *    accept.
 *
 *  This is invoked by a connection in response to a failed
 *  connection to signal delegates that the connection did not
 *  accept.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed connection.
 *
 */
void
ConnectionBasis :: OnDidNotAccept(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ConnectionDidNotAccept(*this, aError);
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
        mDelegate->ConnectionWillDisconnect(*this, GetPeerAddress().GetURL());
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
        mDelegate->ConnectionDidDisconnect(*this, GetPeerAddress().GetURL(), aError);
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
        mDelegate->ConnectionDidNotDisconnect(*this, GetPeerAddress().GetURL(), aError);
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
 *    Returns the current connection scheme identifier.
 *
 *  @returns
 *    The current connection scheme identifier.
 *
 */
ConnectionBasis::IdentifierType
ConnectionBasis :: GetIdentifier(void) const
{
    return (mIdentifier);
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

}; // namespace Server

}; // namespace HLX
