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
 *      This file...
 *
 */

#include <ListenerBasis.hpp>

#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>

#include <CoreFoundation/CFSocket.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ListenerBasisAcceptDelegate.hpp>
#include <OpenHLX/Server/ListenerBasisDelegate.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


// Preprocessor Defintions

/**
 *  @def SOCK_FLAGS
 *
 *  @brief
 *    A portability mnemonic to address platforms which have or have
 *    not defined SOCK_CLOEXEC.
 *
 */
#ifdef SOCK_CLOEXEC
#define SOCK_FLAGS SOCK_CLOEXEC
#else
#define SOCK_FLAGS 0
#endif


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
 *                          connection listener.
 *
 */
ListenerBasis :: ListenerBasis(CFStringRef aSchemeRef) :
    mSchemeRef(CFURetain(aSchemeRef)),
    mRunLoopParameters(),
    mState(kState_Unknown),
    mDelegate(nullptr),
    mAcceptDelegate(nullptr),
    mSocketRef(nullptr),
    mRunLoopSourceRef(nullptr),
    mHostURLAddress()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ListenerBasis :: ~ListenerBasis(void)
{
    Ignore();

    CFRelease(mSchemeRef);

    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection listener basis on a run loop with the
 *  specified port and run loop parameters.
 *
 *  @param[in]  aDefaultPort        The default port on which the listener
 *                                  should listen if the port is not
 *                                  otherwise specified.
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection listener basis with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ListenerBasis :: Init(in_port_t aDefaultPort, const RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;

    mDefaultPort       = aDefaultPort;
    mRunLoopParameters = aRunLoopParameters;
    mState             = kState_IsReady;

    return (lRetval);
}

/**
 *  @brief
 *    Listen for unsolicited, asynchronous connections from remote
 *    peers at the specified socket address.
 *
 *  This attempts to asynchronously listen for unsolicited connections
 *  from remote peers at the specified socket address and invokes the
 *  specified delegate when a connection is accepted by the listener.
 *
 *  @param[in]  aAcceptDelegate  A pointer to the delegate to invoke
 *                               when a connection is accepted by the
 *                               listener.
 *  @param[in]  aAddress         An immutable reference to the socket
 *                               address to listen on.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EBUSY                   If the listener is already
 *                                    listening.
 *  @retval  -ENOMEM                  Resources could not be allocated
 *                                    to listen.
 *  @retval  -EPFNOSUPPORT            The @a aAddress specifies an
 *                                    unsupported protocol family
 *                                    other than PF_INET or PF_INET6.
 *
 */
Status
ListenerBasis :: Listen(ListenerBasisAcceptDelegate *aAcceptDelegate,
                        const SocketAddress &aAddress)
{
    Status               lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mSocketRef == nullptr, done, lRetval = -EBUSY);
    nlREQUIRE_ACTION(mRunLoopSourceRef == nullptr, done, lRetval = -EBUSY);

    mAcceptDelegate = aAcceptDelegate;

    lRetval = Listen(aAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the connection listener basis.
 *
 *  @returns
 *    A pointer to the delegate for the connection listener basis.
 *
 */
ListenerBasisDelegate *
ListenerBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the connection listener basis.
 *
 *  This attempts to set a delegate for the connection listener basis.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ListenerBasis :: SetDelegate(ListenerBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the connection scheme for the connection listener basis.
 *
 *  @returns
 *    The connection scheme for the connection listener basis.
 *
 */
CFStringRef
ListenerBasis :: GetScheme(void) const
{
    return (mSchemeRef);
}

/**
 *  @brief
 *    Return the listen URL for the connection listener basis.
 *
 *  @returns
 *    The listen URL for the connection listener basis.
 *
 */
const HostURLAddress &
ListenerBasis :: GetAddress(void) const
{
    return (mHostURLAddress);
}

/**
 *  @brief
 *    Callback trampoline to handle listener accept activity.
 *
 *  @param[in]  aSocketRef           A reference to the listener socket
 *                                   that triggered the callback.
 *  @param[in]  aSocketCallBackType  The type of event that triggered
 *                                   the callback.
 *  @param[in]  aAddress             A data object wrapping a SocketAddress
 *                                   corresponding to the socket address of
 *                                   the remote peer the listener is
 *                                   accepting a connection from.
 *  @param[in]  aData                A pointer to a native socket descriptor
 *                                   for the pending accepted connection.
 *  @param[in]  aInfo                A pointer to the listener class
 *                                   instance that registered this
 *                                   trampoline to call back into from
 *                                   the trampoline.
 *
 */
/* static */ void
ListenerBasis :: CFSocketAcceptCallback(CFSocketRef aSocketRef,
                                        CFSocketCallBackType aSocketCallBackType,
                                        CFDataRef aAddress,
                                        const void *aData,
                                        void *aInfo)
{
    ListenerBasis *  lListener = static_cast<ListenerBasis *>(aInfo);

    if (lListener != nullptr)
    {
        lListener->CFSocketAcceptCallback(aSocketRef, aSocketCallBackType, aAddress, aData);
    }
}

// MARK: Listener Delegation Actions

/**
 *  @brief
 *    Signals to listener delegates that a listener will listen.
 *
 *  This is invoked by a listener in response to a pending
 *  listen to signal delegates that the listener will listen.
 *
 */
void
ListenerBasis :: OnWillListen(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ListenerWillListen(*this);
    }
}

/**
 *  @brief
 *    Signals to listener delegates that a listener is in the
 *    process of listening.
 *
 *  This is invoked by a listener in response to a pending
 *  listen to signal delegates that the listener is in the
 *  process of listening.
 *
 */
void
ListenerBasis :: OnIsListening(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ListenerIsListening(*this);
    }
}

/**
 *  @brief
 *    Signals to listener delegates that a listener is now
 *    listening.
 *
 *  This is invoked by a listener in response to a pending
 *  listen to signal delegates that the listener is now
 *  listening.
 *
 */
void
ListenerBasis :: OnDidListen(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ListenerDidListen(*this);
    }
}

/**
 *  @brief
 *    Signals to listener delegates that a listener did not
 *    listen.
 *
 *  This is invoked by a listener in response to a failed
 *  listen to signal delegates that the listener did not
 *  listen.
 *
 *  @param[in]  aError  An immutable reference to the error
 *                      associated with the failed listen.
 *
 */
void
ListenerBasis :: OnDidNotListen(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ListenerDidNotListen(*this, aError);
    }
}

/**
 *  @brief
 *    Signals to listener delegates that a listener experienced an
 *    error.
 *
 *  This is invoked by a listener in response to a listen error
 *  to signal delegates that such a listen error occurred.
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
ListenerBasis :: OnError(const Common::Error &aError)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ListenerError(*this, aError);
    }
}

/**
 *  @brief
 *    Returns whether or not the listener is in the specified state.
 *
 *  @param[in]  aState  The state to confirm.
 *
 *  @returns
 *    True if the listener is in the specified state; otherwise,
 *    false.
 *
 */
bool
ListenerBasis :: IsState(State aState) const
{
    const bool lRetval = (mState == aState);


    return (lRetval);
}

/**
 *  @brief
 *    Returns the current listener state.
 *
 *  @returns
 *    The current listener state.
 *
 */
ListenerBasis::State
ListenerBasis :: GetState(void) const
{
    return (mState);
}

/**
 *  @brief
 *    Sets the current listener state.
 *
 *  @param[in]  aState  The state to set the listener to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the state was already set to
 *                                    the specified value.
 *
 */
Status
ListenerBasis :: SetState(State aState)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(mState != aState, done, lRetval = kStatus_ValueAlreadySet);

    mState = aState;

 done:
    return (lRetval);
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
ListenerBasis :: GetRunLoopParameters(void)
{
    return (mRunLoopParameters);
}

Status
ListenerBasis :: Listen(const SocketAddress &aAddress)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    const sa_family_t  lProtocolFamily = aAddress.uSocketAddress.sa_family;
    const bool lIsHostOrder = true;
    const State lCurrentState = GetState();
    const int lBacklog = 4;
    const CFIndex lOrder = 0;
    const sockaddr *lSocketAddress = nullptr;
    size_t lSocketAddressSize;
    int one = 1;
    Status lRetval = kStatus_Success;
    int lSocket = -1;
    int lStatus;
    int lFlags;
    CFSocketRef lSocketRef = nullptr;
    CFRunLoopSourceRef lRunLoopSourceRef = nullptr;
    const CFSocketContext lSocketContext = { 0, this, nullptr, nullptr, nullptr };


    // Initialize our host address and URL.

    lRetval = mHostURLAddress.Init(GetScheme(), aAddress, !lIsHostOrder);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Determine the socket type to create and the address to bind to
    // based on the protocol family of the specified address

    switch (lProtocolFamily)
    {
    case PF_INET:
        lSocketAddress = reinterpret_cast<const sockaddr *>(&aAddress.uSocketAddressIPv4);
        lSocketAddressSize = sizeof (aAddress.uSocketAddressIPv4);
        break;

    case PF_INET6:
        lSocketAddress = reinterpret_cast<const sockaddr *>(&aAddress.uSocketAddressIPv6);
        lSocketAddressSize = sizeof (aAddress.uSocketAddressIPv6);
        break;

    default:
        lRetval = -EPFNOSUPPORT;
        goto done;
    }

    OnWillListen();

    SetState(kState_WillBeListening);

    OnIsListening();

     // Create the native BSD socket.

    lSocket = socket(lProtocolFamily, SOCK_STREAM | SOCK_FLAGS, IPPROTO_TCP);
    nlREQUIRE_ACTION(lSocket != -1, done, lRetval = -errno);

    LogDebug(lLogIndent,
             lLogLevel,
             "Listening IPv%c Socket: %d\n",
             ((lProtocolFamily == PF_INET) ? '4' : '6'),
             lSocket);

    // When we bind, indicate that we wish to attempt to forcibly bind
    // to a socket already bound to the requested address.

#if defined(SO_REUSEADDR)
    lStatus = setsockopt(lSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one));
    nlREQUIRE_ACTION(lStatus != -1, done, lRetval = -errno);
#endif

    // When we bind, indicate that we wish to attempt to forcibly bind
    // to a socket already bound to the requested port.

#if defined(SO_REUSEPORT)
    lStatus = setsockopt(lSocket, SOL_SOCKET, SO_REUSEPORT, &one, sizeof (one));
    nlREQUIRE_ACTION(lStatus != -1, done, lRetval = -errno);
#endif

    // Bind the BSD socket to the requested address and port
    // combination.

    lStatus = bind(lSocket,
                   lSocketAddress,
                   static_cast<socklen_t>(lSocketAddressSize));
    nlREQUIRE_ACTION(lStatus >= 0, done, lRetval = -errno);

    // Set the BSD socket to non-blocking

    lFlags = fcntl(lSocket, F_GETFL);

    lStatus = fcntl(lSocket, F_SETFL, lFlags | O_NONBLOCK);
    nlREQUIRE_ACTION(lStatus >= 0, done, lRetval = -errno);

    // Starting listening on the BSD socket

    lStatus = listen(lSocket, lBacklog);
    nlREQUIRE_ACTION(lStatus >= 0, done, lRetval = -errno);

    // Create a CoreFoundation socket reference to the BSD socket

    lSocketRef = CFSocketCreateWithNative(kCFAllocatorDefault,
                                          lSocket,
                                          kCFSocketAcceptCallBack,
                                          CFSocketAcceptCallback,
                                          &lSocketContext);
    nlREQUIRE_ACTION(lSocketRef != nullptr, done, lRetval = -ENOMEM);

    // Ensure the accept callback is always re-enabled on trigger and
    // that the underlying BSD socket is always closed on the
    // CoreFoundation socket being invalidated.

    CFSocketSetSocketFlags(lSocketRef, (kCFSocketAutomaticallyReenableAcceptCallBack |
                                        kCFSocketCloseOnInvalidate));

    // Add the listening CoreFoundation socket reference to the
    // configured run loop.

    lRunLoopSourceRef = CFSocketCreateRunLoopSource(kCFAllocatorDefault,
                                                    lSocketRef,
                                                    lOrder);
    nlREQUIRE_ACTION(lRunLoopSourceRef != nullptr, done, lRetval = -ENOMEM);

    CFRunLoopAddSource(mRunLoopParameters.GetRunLoop(),
                       lRunLoopSourceRef,
                       mRunLoopParameters.GetRunLoopMode());

 done:
    if (lRetval == kStatus_Success)
    {
        SetState(kState_IsListening);

        mSocketRef = lSocketRef;
        mRunLoopSourceRef = lRunLoopSourceRef;

        OnDidListen();
    }
    else
    {
        SetState(lCurrentState);

        if ((mSocketRef == nullptr) && (lSocket != -1))
        {
            close(lSocket);
        }
        else
        {
            Ignore(mRunLoopParameters, lSocketRef, lRunLoopSourceRef);
        }

        OnDidNotListen(lRetval);
    }

    return (lRetval);
}

void
ListenerBasis :: Ignore(void)
{
    Ignore(mRunLoopParameters, mSocketRef, mRunLoopSourceRef);
}

void
ListenerBasis :: Ignore(const RunLoopParameters &aRunLoopParameters, CFSocketRef &aSocketRef, CFRunLoopSourceRef &aRunLoopSourceRef)
{
    Boolean lStatus;

    if (aRunLoopSourceRef != nullptr)
    {
        lStatus = CFRunLoopContainsSource(aRunLoopParameters.GetRunLoop(),
                                          aRunLoopSourceRef,
                                          aRunLoopParameters.GetRunLoopMode());

        if (lStatus)
        {
            CFRunLoopRemoveSource(aRunLoopParameters.GetRunLoop(),
                                  aRunLoopSourceRef,
                                  aRunLoopParameters.GetRunLoopMode());
        }

        CFRelease(aRunLoopSourceRef);

        aRunLoopSourceRef = nullptr;
    }

    if (aSocketRef != nullptr)
    {
        lStatus = CFSocketIsValid(aSocketRef);

        if (lStatus)
        {
            CFSocketInvalidate(aSocketRef);

            CFRelease(aSocketRef);

            aSocketRef = nullptr;
        }
    }
}

/**
 *  @brief
 *    Callback to handle listener accept activity.
 *
 *  @param[in]  aSocketRef           A reference to the listener socket
 *                                   that triggered the callback.
 *  @param[in]  aSocketCallBackType  The type of event that triggered
 *                                   the callback.
 *  @param[in]  aAddress             A data object wrapping a SocketAddress
 *                                   corresponding to the socket address of
 *                                   the remote peer the listener is
 *                                   accepting a connection from.
 *  @param[in]  aData                A pointer to a native socket descriptor
 *                                   for the pending accepted connection.
 *
 */
void
ListenerBasis :: CFSocketAcceptCallback(CFSocketRef aSocketRef,
                                        CFSocketCallBackType aSocketCallBackType,
                                        CFDataRef aAddress,
                                        const void *aData)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    const State            lCurrentState    = GetState();
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
    CFSocketNativeHandle   lAcceptingSocket = CFSocketGetNative(aSocketRef);
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
    CFSocketNativeHandle   lConnectedSocket = *static_cast<const CFSocketNativeHandle *>(aData);
    Status                 lStatus          = kStatus_Success;


#if ((!defined(DEBUG) && !DEBUG) || (defined(NDEBUG) && NDEBUG))
	(void)aSocketRef;
#endif // ((!defined(DEBUG) && !DEBUG) || (defined(NDEBUG) && NDEBUG))

    LogDebug(lLogIndent,
             lLogLevel,
             "lAcceptingSocket %d lConnectedSocket %d\n",
             lAcceptingSocket,
             lConnectedSocket);

    nlASSERT(aSocketCallBackType == kCFSocketAcceptCallBack);
    nlASSERT(aData != nullptr);
    nlASSERT(lConnectedSocket != -1);

    SetState(kState_Accepting);

    if (mAcceptDelegate != nullptr)
    {
        const uint8_t *       lData = CFDataGetBytePtr(aAddress);
        const SocketAddress * lSocketAddress = reinterpret_cast<const SocketAddress *>(lData);

        lStatus = mAcceptDelegate->ListenerDidAccept(*this, lConnectedSocket, *lSocketAddress);
    }
    else
    {
        lStatus = -ECONNREFUSED;
    }

    // Regardless of status, we are done accepting at this point, and
    // we unconditionally return to the prior state.

    SetState(lCurrentState);

    // If there was an error from delegation, then clean up and
    // perform general failure delegation.

    if (lStatus != kStatus_Success)
    {
        close(lConnectedSocket);

        //OnDidNotAccept(lStatus);

        OnError(lStatus);
    }
}

}; // namespace Server

}; // namespace HLX
