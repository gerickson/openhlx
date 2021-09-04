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
 *      network connection that uses the telnet protocol.
 *
 */

#include <ConnectionTelnet.hpp>

#include <errno.h>
#include <stdint.h>

#include <ConnectionBuffer.hpp>
#include <Timeout.hpp>

#include <CoreFoundation/CFStream.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Client
{

// Global Variables

static const telnet_telopt_t sTelnetOptions[] = {
    { -1, 0, 0 }
};

static const uint16_t kTelnetPort = 23;

static const char * const kClientConfirmationRegexp = "^telnet_client_[[:digit:]]+: connected\r\n$";

// Static Class Data Members

/**
 *  @brief
 *    A CoreFoundation string constant for the URL protocol scheme
 *    supported by this connection.
 *
 */
CFStringRef ConnectionTelnet :: kScheme = CFSTR("telnet");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionTelnet :: ConnectionTelnet(void) :
    ConnectionBasis(kScheme),
    mTelnet(nullptr),
    mReadStreamRef(nullptr),
    mWriteStreamRef(nullptr),
    mReadStreamReady(false),
    mWriteStreamReady(false),
    mReceiveBuffer(),
    mWaitingForClientConfirmation(true),
    mClientConfirmationRegexp()
{
    DeclareScopedFunctionTracer(lTracer);

    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConnectionTelnet :: ~ConnectionTelnet(void)
{
    DeclareScopedFunctionTracer(lTracer);

    if (mTelnet != nullptr)
    {
        telnet_free(mTelnet);
        mTelnet = nullptr;
    }

    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the connection on a run loop with the specified
 *  run loop parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  connection with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If the underlying telnet library
 *                            instance could not be allocated.
 *
 */
Status
ConnectionTelnet :: Init(const RunLoopParameters &aRunLoopParameters)
{
    DeclareScopedFunctionTracer(lTracer);
    const size_t lExpectedMatchCount = 0;
    const int    lFlags = REG_NOSUB;
    Status       lRetval = kStatus_Success;

    // Initialize the telnet library.

    mTelnet = telnet_init(sTelnetOptions, ConnectionTelnet::TelnetEventHandler, 0, this);
    nlREQUIRE_ACTION(mTelnet != nullptr, done, lRetval = -ENOMEM);

    lRetval = mClientConfirmationRegexp.Init(kClientConfirmationRegexp, lExpectedMatchCount, lFlags);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Initialize the parent class now that the child intialization is
    // successfully finished.

    lRetval = ConnectionBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a telnet peer.
 *
 *  This attempts to asynchronously connect via telnet to the peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aURLRef   A reference to a CoreFoundation URL for the
 *                        peer to connect to.
 *  @param[in]  aTimeout  An immutable reference to the timeout by
 *                        which the connection should complete.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The port number for the URL was
 *                            invalid.
 *  @retval  -ECONNREFUSED    If the peer refused the connection.
 *  @retval  -EIO             If read and write streams could not
 *                            be opened for the connected peer.
 *
 */
Status
ConnectionTelnet :: Connect(CFURLRef aURLRef, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const State            lCurrentState = GetState();
    CFString               lHost;
    SInt32                 lPossiblePort;
    uint16_t               lPort;
    const CFOptionFlags    kCommonStreamEvents = (kCFStreamEventErrorOccurred | kCFStreamEventEndEncountered);
    const CFOptionFlags    kReadStreamEvents   = (kCommonStreamEvents | kCFStreamEventHasBytesAvailable);
    const CFOptionFlags    kWriteStreamEvents  = (kCommonStreamEvents | kCFStreamEventCanAcceptBytes);
    CFStreamClientContext  lStreamClientContext;
    CFRunLoopRef           lRunLoop = nullptr;
    CFRunLoopMode          lRunLoopMode;
    bool                   lStatus;
    Status                 lRetval = kStatus_Success;


    // Take care of invoking the super class Connect method first.

    lRetval = ConnectionBasis::Connect(aURLRef, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Now, take care of telnet-specific work.

    // If no port was specified, CFURLGetPortNumber will return -1. In
    // such a case, we default to the default telnet port.

    lPossiblePort = CFURLGetPortNumber(aURLRef);
    nlREQUIRE_ACTION(lPossiblePort <= UINT16_MAX, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(lPossiblePort >= -1, done, lRetval = -EINVAL);

    if (lPossiblePort == -1)
        lPort = kTelnetPort;
    else
        lPort = static_cast<uint16_t>(lPossiblePort);

    lHost = CFURLCopyHostName(aURLRef);

    // Signal delegates that the connection will begin.

    OnWillConnect();

    SetState(kState_Connecting);

    OnIsConnecting();

    CFStreamCreatePairWithSocketToHost(kCFAllocatorDefault,
                                       lHost.GetString(),
                                       lPort,
                                       &mReadStreamRef,
                                       &mWriteStreamRef);

    if ((mReadStreamRef == nullptr) || (mWriteStreamRef == nullptr))
    {
        if (mReadStreamRef)
        {
            Log::Error().Write("Failed to create read stream.\n");

            CFReadStreamClose(mReadStreamRef);
            CFRelease(mReadStreamRef);
            mReadStreamRef = nullptr;
        }

        if (mWriteStreamRef)
        {
            Log::Error().Write("Failed to create write stream.\n");

            CFWriteStreamClose(mWriteStreamRef);
            CFRelease(mWriteStreamRef);
            mWriteStreamRef = nullptr;
        }

        lRetval = -ECONNREFUSED;
        goto done;
    }

    lRunLoop = GetRunLoopParameters().GetRunLoop();
    lRunLoopMode = GetRunLoopParameters().GetRunLoopMode();

    lStreamClientContext.version         = 0;
    lStreamClientContext.info            = this;
    lStreamClientContext.retain          = nullptr;
    lStreamClientContext.release         = nullptr;
    lStreamClientContext.copyDescription = nullptr;

    if (mReadStreamRef != nullptr)
    {
#if USE_kCFStreamPropertyShouldCloseNativeSocket
        CFReadStreamSetProperty(mReadStreamRef,
                                kCFStreamPropertyShouldCloseNativeSocket,
                                kCFBooleanFalse);
#endif

        lStatus = CFReadStreamSetClient(mReadStreamRef,
                                        kReadStreamEvents,
                                        &CFReadStreamCallback,
                                        &lStreamClientContext);

        if (!lStatus)
        {
            Log::Error().Write("Failed to set read stream client.\n");

            lRetval = -EINVAL;
            goto done;
        }

        CFReadStreamScheduleWithRunLoop(mReadStreamRef,
                                        lRunLoop,
                                        lRunLoopMode);
    }

    if (mWriteStreamRef != nullptr)
    {
#if USE_kCFStreamPropertyShouldCloseNativeSocket
        CFWriteStreamSetProperty(mWriteStreamRef,
                                 kCFStreamPropertyShouldCloseNativeSocket,
                                 kCFBooleanFalse);
#endif

        lStatus = CFWriteStreamSetClient(mWriteStreamRef,
                                         kWriteStreamEvents,
                                         &CFWriteStreamCallback,
                                         &lStreamClientContext);

        if (!lStatus)
        {
            Log::Error().Write("Failed to set write stream client.\n");

            lRetval = -EINVAL;
            goto done;
        }

        CFWriteStreamScheduleWithRunLoop(mWriteStreamRef,
                                         lRunLoop,
                                         lRunLoopMode);
    }

    lStatus = CFReadStreamOpen(mReadStreamRef);

    if (!lStatus)
    {
        Log::Error().Write("Failed to open the read stream.\n");

        lRetval = -EIO;
        goto done;
    }

    lStatus = CFWriteStreamOpen(mWriteStreamRef);

    if (!lStatus)
    {
        Log::Error().Write("Failed to open the write stream.\n");

        lRetval = -EIO;
        goto done;
    }

 done:
    // The kState_Connected state should / will only be reached once
    // we get a callback that we are actually connected and ready for
    // reading and writing.

    if (lRetval != kStatus_Success)
    {
        SetState(lCurrentState);

        OnDidNotConnect(lRetval);

        OnError(lRetval);
    }

    return (lRetval);
}

/**
 *  @brief
 *    Close the read and write stream associated with a connected peer.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
ConnectionTelnet :: CloseStreams(void)
{
    CFRunLoopRef  lRunLoop = GetRunLoopParameters().GetRunLoop();
    CFRunLoopMode lRunLoopMode = GetRunLoopParameters().GetRunLoopMode();
    Status        lRetval = kStatus_Success;


    if (mReadStreamRef != nullptr || mWriteStreamRef != nullptr)
    {
        if (mReadStreamRef != nullptr)
        {
            CFReadStreamUnscheduleFromRunLoop(mReadStreamRef, lRunLoop, lRunLoopMode);
            CFReadStreamSetClient(mReadStreamRef, kCFStreamEventNone, nullptr, nullptr);
            CFReadStreamClose(mReadStreamRef);
            CFRelease(mReadStreamRef);
            mReadStreamRef = nullptr;
            mReadStreamReady = false;
        }

        if (mWriteStreamRef != nullptr)
        {
            CFWriteStreamUnscheduleFromRunLoop(mWriteStreamRef, lRunLoop, lRunLoopMode);
            CFWriteStreamSetClient(mWriteStreamRef, kCFStreamEventNone, nullptr, nullptr);
            CFWriteStreamClose(mWriteStreamRef);
            CFRelease(mWriteStreamRef);
            mWriteStreamRef = nullptr;
            mWriteStreamReady = false;
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    Disconnect from the peer.
 *
 *  This attempts to asynchronously disconnect from the
 *  currently-connected peer, if any.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionTelnet :: Disconnect(const Common::Error &aError)
{
    DeclareScopedFunctionTracer(lTracer);
    const State  lCurrentState = GetState();
    Status       lRetval = kStatus_Success;

    OnWillDisconnect();

    SetState(kState_Disconnecting);

    lRetval = CloseStreams();

    if (lRetval == kStatus_Success)
    {
        if (mReceiveBuffer != nullptr)
        {
            mReceiveBuffer->Flush();
        }

        mWaitingForClientConfirmation = true;

        SetState(kState_Disconnected);

        OnDidDisconnect(aError);

        lRetval = ConnectionBasis::Disconnect(aError);
    }
    else
    {
        SetState(lCurrentState);

        OnDidNotDisconnect(lRetval);

        OnError(lRetval);
    }

    return (lRetval);
}

/**
 *  @brief
 *    Send the specified data to the connection peer.
 *
 *  @param[in]  aBuffer  An immutable shared pointer to the data to send to
 *                       the connection peer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConnectionTelnet :: Send(ConnectionBuffer::ImmutableCountedPointer &aBuffer)
{
    const uint8_t *  lBuffer = aBuffer->GetHead();
    const size_t     lSize = aBuffer->GetSize();
    Status           lRetval = kStatus_Success;

    //Log::Debug().Write("Would send %zu bytes at %p...\n", lSize, lBuffer);
    //Log::Utilities::Memory::Write(lBuffer, lSize, sizeof (uint8_t));

    telnet_send(mTelnet, (const char *)lBuffer, lSize);

    return (lRetval);
}

static void
LogStreamError(const CFStreamEventType &aType, const CFStreamError &aStreamError, const char *aStreamDescription)
{
    Log::Error().Write("%s: received %s event type 0x%lx w/ error domain %lu code %d\n",
                       __func__,
                       aStreamDescription,
                       aType,
                       aStreamError.domain,
                       static_cast<int>(aStreamError.error));
}

/**
 *  @brief
 *    Handle an error associated with a connection stream.
 *
 *  This handles any errors associated with either a read or write
 *  connection stream.
 *
 *  @param[in]  aType               The type of stream error that
 *                                  triggered the error.
 *  @param[in]  aStreamError        An immutable reference to the
 *                                  stream error.
 *  @param[in]  aStreamDescription  An optional pointer to a null-
 *                                  terminated C string describing
 *                                  the stream on which the error
 *                                  occurred.
 *
 */
void
ConnectionTelnet :: HandleStreamError(const CFStreamEventType &aType, const CFStreamError &aStreamError, const char *aStreamDescription)
{
    const State  lState = GetState();
    Error        lError;

    LogStreamError(aType, aStreamError, aStreamDescription);

    Log::Debug().Write("%s: state is %d\n", __FUNCTION__, lState);

    switch (aStreamError.domain)
    {

    case kCFStreamErrorDomainPOSIX:
        lError = -aStreamError.error;
        break;

    default:
        lError = kError_Unknown;
        break;

    }

    switch (lState)
    {

    case kState_Connecting:
        {
            SetState(kState_Disconnected);

            OnDidNotConnect(lError);

            OnError(lError);
        }
        break;

     case kState_Unknown:
         break;

     case kState_Connected:
         {
             CloseStreams();

             mReceiveBuffer->Flush();

             mWaitingForClientConfirmation = true;

             SetState(kState_Disconnected);

             OnDidDisconnect(lError);

             OnError(lError);
         }
         break;

    case kState_Disconnecting:
    case kState_Disconnected:
    default:
        break;

    }
}

/**
 *  @brief
 *    Callback to handle connection read stream activity.
 *
 *  This handles any read stream activity associated with the
 *  connected peer.
 *
 *  @param[in]  aStream  A reference to the read stream that
 *                       triggered the callback.
 *  @param[in]  aType    The type of event that triggered the
 *                       callback.
 *
 */
void
ConnectionTelnet :: CFReadStreamCallback(CFReadStreamRef aStream, CFStreamEventType aType)
{
    //DeclareScopedFunctionTracer(lTracer);
    Boolean lStatus;

    //Log::Debug().Write("Received read event 0x%08lx\n", aType);

    switch (aType)
    {

    case kCFStreamEventHasBytesAvailable:
        {
            //Log::Debug().Write("Could read data!\n");

            if (!mReadStreamReady)
            {
                mReadStreamReady = true;

                if (mWriteStreamReady && !mWaitingForClientConfirmation)
                {
                    SetState(kState_Connected);

                    OnDidConnect();
                }
            }

            lStatus = CFReadStreamHasBytesAvailable(aStream);
            if (lStatus)
            {
                const CFIndex lRequestedBytes = 4096;
                uint8_t       lBuffer[lRequestedBytes];
                CFIndex       lResult;


                lResult = CFReadStreamRead(aStream, lBuffer, lRequestedBytes);

                if (lResult > 0)
                {
                    DeclareLogIndentWithValue(lLogIndent, 0);
                    DeclareLogLevelWithValue(lLogLevel, 1);

                    LogDebug(lLogIndent,
                             lLogLevel,
                             "Read the following %zu bytes into %p...\n",
                             lResult,
                             lBuffer);
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
                    Log::Utilities::Memory::Write(lLogIndent,
                                                  lLogLevel,
                                                  lBuffer,
                                                  static_cast<size_t>(lResult));
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

                    telnet_recv(mTelnet,
                                reinterpret_cast<const char *>(lBuffer),
                                static_cast<size_t>(lResult));
                }
            }
        }
        break;

    case kCFStreamEventErrorOccurred:
        {
            const CFStreamError lStreamError = CFReadStreamGetError(aStream);

            HandleStreamError(aType, lStreamError, "read");
        }
        break;

    case kCFStreamEventEndEncountered:
        {
            const CFStreamError lStreamError = { kCFStreamErrorDomainPOSIX, ECONNRESET };

            HandleStreamError(aType, lStreamError, "read");
        }
        break;

    default:
        {
            Log::Error().Write("%s: read event type 0x%lx unhandled\n", __func__, aType);
        }
        break;
    }

    return;
}

/**
 *  @brief
 *    Callback to handle connection read stream activity.
 *
 *  This handles any write stream activity associated with the
 *  connected peer.
 *
 *  @param[in]  aStream  A reference to the write stream that
 *                       triggered the callback.
 *  @param[in]  aType    The type of event that triggered the
 *                       callback.
 *
 */
void
ConnectionTelnet :: CFWriteStreamCallback(CFWriteStreamRef aStream, CFStreamEventType aType)
{
    Boolean lStatus;

    //Log::Debug().Write("Received write event 0x%08lx\n", aType);

    switch (aType)
    {

    case kCFStreamEventCanAcceptBytes:
        {
            //Log::Debug().Write("Could write data!\n");

            if (!mWriteStreamReady)
            {
                mWriteStreamReady = true;

                if (mReadStreamReady && !mWaitingForClientConfirmation)
                {
                    SetState(kState_Connected);

                    OnDidConnect();
                }
            }

            lStatus = CFWriteStreamCanAcceptBytes(aStream);
            if (lStatus)
            {
                //telnet_send(mTelnet, (const char *)&lBuffer[0], lRequestedBytes);
            }
        }
        break;

    case kCFStreamEventErrorOccurred:
        {
            const CFStreamError lStreamError = CFWriteStreamGetError(aStream);

            HandleStreamError(aType, lStreamError, "write");
        }
        break;

    default:
        {
            Log::Error().Write("%s: write event type 0x%lx unhandled\n", __func__, aType);
        }
        break;
    }

    return;
}

/**
 *  @brief
 *    Callback trampoline to handle connection read stream activity.
 *
 *  @param[in]  aStream   A reference to the read stream that
 *                        triggered the callback.
 *  @param[in]  aType     The type of event that triggered the
 *                        callback.
 *  @param[in]  aContext  A pointer to the connection class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 */
void ConnectionTelnet :: CFReadStreamCallback(CFReadStreamRef aStream, CFStreamEventType aType, void *aContext)
{
    ConnectionTelnet *lConnection = static_cast<ConnectionTelnet *>(aContext);

    if (lConnection != nullptr)
    {
        lConnection->CFReadStreamCallback(aStream, aType);
    }

    return;
}

/**
 *  @brief
 *    Callback trampoline to handle connection write stream activity.
 *
 *  @param[in]  aStream   A reference to the write stream that
 *                        triggered the callback.
 *  @param[in]  aType     The type of event that triggered the
 *                        callback.
 *  @param[in]  aContext  A pointer to the connection class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 */
void ConnectionTelnet :: CFWriteStreamCallback(CFWriteStreamRef aStream, CFStreamEventType aType, void *aContext)
{
    ConnectionTelnet *lConnection = static_cast<ConnectionTelnet *>(aContext);

    if (lConnection != nullptr)
    {
        lConnection->CFWriteStreamCallback(aStream, aType);
    }

    return;
}

void ConnectionTelnet :: TryClientConfirmationDataReceived(void)
{
    const char *  lBuffer = reinterpret_cast<const char *>(mReceiveBuffer->GetHead());
    const size_t  lSize = mReceiveBuffer->GetSize();
    Status        lMatchStatus;

    lMatchStatus = mClientConfirmationRegexp.Match(lBuffer, lSize);

    if (lMatchStatus == 0)
    {
        Log::Debug().Write("Received client session confirmation!\n");

        // We received the client session confirmation in full, flush
        // the buffer contents for subsequent end-to-end application
        // data.

        mReceiveBuffer->Flush();

        mWaitingForClientConfirmation = false;

        if (mReadStreamReady && mWriteStreamReady)
        {
            SetState(kState_Connected);

            OnDidConnect();
        }
    }
    else
    {
        Log::Debug().Write("Still waiting for client session confirmation...\n");
    }
}

void ConnectionTelnet :: DidReceiveDataHandler(const uint8_t *aBuffer, const size_t &aSize)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    Status         lStatus;

    LogDebug(lLogIndent,
             lLogLevel,
             "Received %zu bytes of telnet user data.\n",
             aSize);

    // Allocate and initialize the receive buffer on-demand, if one is
    // not already in use.

    if (!mReceiveBuffer)
    {
        mReceiveBuffer.reset(new ConnectionBuffer());
        nlREQUIRE_ACTION(mReceiveBuffer != nullptr, done, lStatus = -ENOMEM);

        lStatus = mReceiveBuffer->Init();
        nlREQUIRE_SUCCESS(lStatus, done);
    }

    // Push the received user data from the input stream into
    // the receive buffer.

    lStatus = Common::Utilities::Put(*mReceiveBuffer.get(), aBuffer, aSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    LogDebug(lLogIndent,
             lLogLevel,
             "Reading the following %zu bytes from %p...\n",
             aSize,
             aBuffer);
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
    Log::Utilities::Memory::Write(lLogIndent,
                                  lLogLevel,
                                  aBuffer,
                                  aSize);
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

    LogDebug(lLogIndent,
             lLogLevel,
             "Receive buffer now contains...\n");
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
    Log::Utilities::Memory::Write(lLogIndent,
                                  lLogLevel,
                                  mReceiveBuffer->GetHead(),
                                  mReceiveBuffer->GetSize());
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

    // If we are waiting for "telnet_client_[[:digit:]]+:
    // connected\r\n", then we have connected at the network transport
    // layer (TCP) and application transport layer (telnet); however,
    // not at the application session layer. Consequently, consume the
    // data to a local buffer and wait until we have received session
    // layer confirmation.
    //
    // Otherwise, the user data is a either an unsolicited
    // notification or a solicited command response both of which need
    // to be buffered and dispatched upwards.

    if (mWaitingForClientConfirmation)
    {
        TryClientConfirmationDataReceived();
    }
    else
    {
        OnApplicationDataReceived(mReceiveBuffer);
    }

 done:
    return;
}

void
ConnectionTelnet :: ShouldTransmitDataHandler(const uint8_t *aBuffer, const size_t &aSize)
{
    CFIndex lResult = 0;
    CFIndex lStatus = 0;

    //Log::Debug().Write("Should send %zu bytes of data\n", aSize);

    lStatus = CFWriteStreamCanAcceptBytes(mWriteStreamRef);
    if (lStatus)
    {
        //Log::Debug().Write("Write stream is ready for the following data at %p...\n", aBuffer);

        //Log::Utilities::Memory::Write(Log::Debug(), aBuffer, aSize, sizeof (uint8_t));

        lResult = CFWriteStreamWrite(mWriteStreamRef,
                                     aBuffer,
                                     static_cast<CFIndex>(aSize));

        //Log::Debug().Write("Wrote %zu bytes\n", lResult);

        if (static_cast<size_t>(lResult) != aSize)
        {
            Log::Debug().Write("Only wrote %zu of %zu bytes!\n", lResult, aSize);
        }
    }
    else
    {
        Log::Debug().Write("Write stream cannot accept data!\n");
    }
}

/**
 *  @brief
 *    Callback to handle connection telnet activity.
 *
 *  @param[in]  aTelnet   A pointer to the telnet instance that
 *                        triggered the callback.
 *  @param[in]  aEvent    A pointer to the type of event that
 *                        triggered the callback.
 *
 */
void
ConnectionTelnet :: TelnetEventHandler(telnet_t *aTelnet, telnet_event_t *aEvent)
{
    nlREQUIRE(aTelnet != nullptr, done);
    nlREQUIRE(aEvent != nullptr, done);

    switch (aEvent->type)
    {

    // This event is generated when there is end-to-end application
    // data that has been pulled in over the telnet channel.

    case TELNET_EV_DATA:
        {
            nlEXPECT(aEvent->data.size > 0, done);

            DidReceiveDataHandler(reinterpret_cast<const uint8_t *>(aEvent->data.buffer),
			                      aEvent->data.size);
        }
        break;

    // This event is generated when there is end-to-end application
    // data to push out over the telnet channel.

    case TELNET_EV_SEND:
        {
            nlEXPECT(aEvent->data.size > 0, done);

            ShouldTransmitDataHandler(reinterpret_cast<const uint8_t *>(aEvent->data.buffer),
                                      aEvent->data.size);
        }
        break;

    default:
        Log::Error().Write("%s: Event %d unhandled\n", __func__, aEvent->type);
        break;

    }

 done:
    return;
}

/**
 *  @brief
 *    Callback trampoline to handle connection telnet activity.
 *
 *  @param[in]  aTelnet   A pointer to the telnet instance that
 *                        triggered the callback.
 *  @param[in]  aEvent    A pointer to the type of event that
 *                        triggered the callback.
 *  @param[in]  aContext  A pointer to the connection class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 */
void
ConnectionTelnet :: TelnetEventHandler(telnet_t *aTelnet, telnet_event_t *aEvent, void *aContext)
{
    ConnectionTelnet *lConnection = static_cast<ConnectionTelnet *>(aContext);

    if (lConnection != nullptr)
    {
        lConnection->TelnetEventHandler(aTelnet, aEvent);
    }

    return;
}

}; // namespace Client

}; // namespace HLX
