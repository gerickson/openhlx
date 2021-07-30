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
 *      This file implements an object for managing a HLX client command
 *      queue, including dispatching outgoing requests and handling
 *      incoming asynchronous state change notifications and
 *      synchronous command responses.
 *
 */

#include <iterator>
#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

#include <CommandErrorResponse.hpp>
#include <CommandExchangeBasis.hpp>
#include <CommandManager.hpp>
#include <CommandResponseBasis.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

// MARK: Command Manager Exchange State

CommandManager :: ExchangeState :: ExchangeState(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                                 const Timeout &aTimeout,
                                                 OnCommandCompleteFunc aOnCommandCompleteHandler,
                                                 OnCommandErrorFunc aOnCommandErrorHandler,
                                                 void *aContext)
 :
    mExchange(aExchange),
    mTimeout(aTimeout),
    mOnCommandCompleteHandler(aOnCommandCompleteHandler),
    mOnCommandErrorHandler(aOnCommandErrorHandler),
    mContext(aContext)
{
    return;
}

CommandManager :: ExchangeState :: ~ExchangeState(void)
{
    return;
}

// MARK: Command Manager Notification Handler State

CommandManager :: NotificationHandlerState :: NotificationHandlerState(void) :
    mResponse(nullptr),
    mOnNotificationReceivedHandler(nullptr),
    mContext(nullptr)
{
    return;
}

CommandManager :: NotificationHandlerState :: NotificationHandlerState(const NotificationHandlerState &aNotificationHandlerState) :
    mResponse(aNotificationHandlerState.mResponse),
    mOnNotificationReceivedHandler(aNotificationHandlerState.mOnNotificationReceivedHandler),
    mContext(aNotificationHandlerState.mContext)
{
    return;
}

CommandManager :: NotificationHandlerState :: ~NotificationHandlerState(void)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
}

Status CommandManager :: NotificationHandlerState :: Init(Command::ResponseBasis &aResponse, void *aContext, OnNotificationReceivedFunc aOnNotificationReceivedHandler)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aOnNotificationReceivedHandler != nullptr, done, lRetval = -EINVAL);

    mResponse = &aResponse;
    mContext = aContext;
    mOnNotificationReceivedHandler = aOnNotificationReceivedHandler;

 done:
    return (lRetval);
}

CommandManager :: NotificationHandlerState CommandManager :: NotificationHandlerState :: operator =(const NotificationHandlerState &aNotificationHandlerState)
{
    mResponse                      = aNotificationHandlerState.mResponse;
    mContext                       = aNotificationHandlerState.mContext;
    mOnNotificationReceivedHandler = aNotificationHandlerState.mOnNotificationReceivedHandler;

    return (*this);
}

bool CommandManager :: NotificationHandlerState :: operator <(const NotificationHandlerState &aNotificationHandlerState) const
{
    return (mResponse->GetRegularExpression() < aNotificationHandlerState.mResponse->GetRegularExpression());
}

// MARK: Command Manager Send Context

CommandManager :: SendContext :: SendContext(void) :
    mRequestBuffer(nullptr),
    mResponseCompletionRegexp(nullptr),
    mResponseCompletionMatches(nullptr),
    mOnResponseCompleteHandler(nullptr),
    mResponseErrorRegexp(nullptr),
    mResponseErrorMatches(nullptr),
    mOnResponseErrorHandler(nullptr),
    mContext(nullptr)
{
    return;
}

CommandManager :: SendContext :: ~SendContext(void)
{
    return;
}

Status
CommandManager :: SendContext :: Init(const SendContext &aSendContext)
{
    return (Init(aSendContext.mRequestBuffer,
                 *aSendContext.mResponseCompletionRegexp,
                 *aSendContext.mResponseCompletionMatches,
                 aSendContext.mOnResponseCompleteHandler,
                 *aSendContext.mResponseErrorRegexp,
                 *aSendContext.mResponseErrorMatches,
                 aSendContext.mOnResponseErrorHandler,
                 aSendContext.mContext));
}

Status CommandManager :: SendContext :: Init(ConnectionBuffer::ImmutableCountedPointer  aRequestBuffer,
                                              const RegularExpression &                  aResponseCompletionRegexp,
                                              RegularExpression::Matches &               aResponseCompletionMatches,
                                              OnResponseCompleteFunc                     aOnResponseCompleteHandler,
                                              const RegularExpression &                  aResponseErrorRegexp,
                                              RegularExpression::Matches &               aResponseErrorMatches,
                                              OnResponseErrorFunc                        aOnResponseErrorHandler,
                                              void *                                     aContext)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(!IsInUse(), done, lRetval = -EBUSY);

    mRequestBuffer             = aRequestBuffer;
    mResponseCompletionRegexp  = &aResponseCompletionRegexp;
    mResponseCompletionMatches = &aResponseCompletionMatches;
    mOnResponseCompleteHandler = aOnResponseCompleteHandler;
    mResponseErrorRegexp       = &aResponseErrorRegexp;
    mResponseErrorMatches      = &aResponseErrorMatches;
    mOnResponseErrorHandler    = aOnResponseErrorHandler;
    mContext                   = aContext;

 done:
    return (lRetval);
}

void
CommandManager :: SendContext :: Reset(void)
{
    mRequestBuffer             = nullptr;
    mResponseCompletionRegexp  = nullptr;
    mResponseCompletionMatches = nullptr;
    mOnResponseCompleteHandler = nullptr;
    mResponseErrorRegexp       = nullptr;
    mResponseErrorMatches      = nullptr;
    mOnResponseErrorHandler    = nullptr;
    mContext                   = nullptr;
}

bool CommandManager :: SendContext :: IsInUse(void) const
{
    const bool lIsInUse = ((mRequestBuffer                    ) &&
                           (mResponseCompletionRegexp  != nullptr) &&
                           (mResponseCompletionMatches != nullptr) &&
                           (mResponseErrorRegexp       != nullptr) &&
                           (mResponseErrorMatches      != nullptr));

    return (lIsInUse);
}

// MARK: Command Manager

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
CommandManager :: CommandManager(void) :
    ConnectionManagerDelegate(),
    RunLoopQueueDelegate(),
    mRunLoopParameters(),
    mDelegate(nullptr),
    mRunLoopSourceRef(nullptr),
    mConnectionManager(nullptr),
    mCommandQueue(),
    mActiveExchangeState(),
    mNotificationHandlers(),
    mErrorResponse(),
    mCurrentSendContext()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
CommandManager :: ~CommandManager(void)
{
    CFRunLoopRemoveSource(mRunLoopParameters.GetRunLoop(),
                          mRunLoopSourceRef,
                          mRunLoopParameters.GetRunLoopMode());

    CFURelease(mRunLoopSourceRef);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the command manager on a run loop with the
 *  specified run loop parameters.
 *
 *  @param[in]  aConnectionManager  A mutable reference to the connection
 *                                  manager to initialize this command
 *                                  manager with as the command
 *                                  manager is to be a delegate of the
 *                                  connection manager.
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  command manager with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          Resources for the run loop source could
 *                            not be allocated.
 *
 */
Status
CommandManager :: Init(ConnectionManager &aConnectionManager,
                       const RunLoopParameters &aRunLoopParameters)
{
    CFRunLoopSourceContext  lContext;
    CFRunLoopSourceRef      lRunLoopSourceRef;
    Status                  lRetval = kStatus_Success;

    lContext.version         = 0;
    lContext.info            = this;
    lContext.retain          = nullptr;
    lContext.release         = nullptr;
    lContext.copyDescription = CommandManager::CopyDescription;
    lContext.equal           = nullptr;
    lContext.hash            = nullptr;
    lContext.schedule        = nullptr;
    lContext.cancel          = nullptr;
    lContext.perform         = CommandManager::Perform;

    lRunLoopSourceRef = CFRunLoopSourceCreate(kCFAllocatorDefault,
                                              0,
                                              &lContext);
    nlREQUIRE_ACTION(lRunLoopSourceRef != nullptr, done, lRetval = -ENOMEM);

    CFRunLoopAddSource(aRunLoopParameters.GetRunLoop(),
                       lRunLoopSourceRef,
                       aRunLoopParameters.GetRunLoopMode());

    mRunLoopSourceRef = lRunLoopSourceRef;

    lRetval = mCommandQueue.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mCommandQueue.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    mConnectionManager = &aConnectionManager;

    // Register one-of-many delegate interest in connections and
    // errors.

    lRetval = mConnectionManager->AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Register one-and-only delegate interest in application data.

    lRetval = mConnectionManager->SetApplicationDataDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

    lRetval = mErrorResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a client command request to the peer connected server.
 *
 *  This attempts to send a client command exchange request and
 *  expected response pair to the peer connected server.
 *
 *  @param[in]  aExchange                  A mutable shared pointer
 *                                         to the exchange on which
 *                                         the client command request
 *                                         should be sent and on which
 *                                         the response should be
 *                                         received.
 *  @param[in]  aTimeout                   An immutable reference to
 *                                         to timeout by which the
 *                                         response should be
 *                                         received.
 *  @param[in]  aOnCommandCompleteHandler  The handler trampoline to
 *                                         invoke when a successful
 *                                         command response is
 *                                         received.
 *  @param[in]  aOnCommandErrorHandler     The handler trampoline to
 *                                         invoke when a command error
 *                                         response is received.
 *  @param[in]  aContext                   A pointer to the caller-
 *                                         specific context that sent
 *                                         the command and that will
 *                                         be provided to the
 *                                         completion or error
 *                                         handler, when invoked.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If there were insufficient resources to
 *                            allocate memory for the state associated
 *                            with the exchange.
 *
 */
Status
CommandManager :: SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                              const Timeout &aTimeout,
                              OnCommandCompleteFunc aOnCommandCompleteHandler,
                              OnCommandErrorFunc aOnCommandErrorHandler,
                              void *aContext)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    ExchangeState * lExchangeState;
    Status          lRetval = kStatus_Success;

    lExchangeState = new ExchangeState(aExchange, aTimeout, aOnCommandCompleteHandler, aOnCommandErrorHandler, aContext);
    nlREQUIRE_ACTION(lExchangeState != nullptr, done, lRetval = -ENOMEM);

    mCommandQueue.Push(lExchangeState);

    LogDebug(lLogIndent,
             lLogLevel,
             "Command queue is now depth %zu\n",
             mCommandQueue.GetSize());

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the command manager.
 *
 *  @returns
 *    A pointer to the delegate for the command manager.
 *
 */
CommandManagerDelegate *
CommandManager :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the command manager.
 *
 *  This attempts to set a delegate for the command manager.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
CommandManager :: SetDelegate(CommandManagerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Register a notification handler.
 *
 *  This registers the specified synchronous (that is, in response to
 *  a command request) or asynchronous (that is, outside of a response
 *  to a command request) state change notification handler and
 *  context for the provided client command response regular
 *  expression.
 *
 *  @param[in]  aResponse                       A mutable reference to
 *                                              the client command
 *                                              response regular
 *                                              expression to register
 *                                              the handler and
 *                                              context for.
 *  @param[in]  aContext                        The context to register
 *                                              which will be passed
 *                                              back via the handler.
 *  @param[in]  aOnNotificationReceivedHandler  The notification handler
 *                                              to register.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If the @a aOnNotificationReceivedHandler
 *                            is null.
 *  @retval  -EEXIST          If a registration already exists.
 *
 */
Status
CommandManager :: RegisterNotificationHandler(Command::ResponseBasis &aResponse,
                                              void *aContext,
                                              OnNotificationReceivedFunc aOnNotificationReceivedHandler)
{
    NotificationHandlerState lNotificationHandlerState;
    std::pair<std::set<NotificationHandlerState>::iterator, bool> lStatus;
    Status lRetval = kStatus_Success;

    lRetval = lNotificationHandlerState.Init(aResponse, aContext, aOnNotificationReceivedHandler);
    nlREQUIRE_SUCCESS(lRetval, done);

    lStatus = mNotificationHandlers.insert(lNotificationHandlerState);
    nlREQUIRE_ACTION(lStatus.second == true, done, lRetval = -EEXIST);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Unregister a notification handler.
 *
 *  This unregisters any state change notification handler and
 *  context for the provided client command response regular
 *  expression.
 *
 *  @param[in]  aResponse                       A mutable reference to
 *                                              the client command
 *                                              response regular
 *                                              expression to unregister
 *                                              the handler and
 *                                              context for.
 *  @param[in]  aContext                        The context to unregister.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOENT          If there was no registration for @a aResponse.
 *
 */
Status
CommandManager :: UnregisterNotificationHandler(const Command::ResponseBasis &aResponse, void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    (void)aResponse;
    (void)aContext;

    return (lRetval);
}

Status
CommandManager :: ServiceCommandQueue(void)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    Status lRetval = kStatus_Success;

    LogDebug(lLogIndent,
             lLogLevel,
             "mConnectionManager %p\n", mConnectionManager);

    if (mConnectionManager != nullptr)
    {
        const bool lConnected = mConnectionManager->IsConnected();
        const bool lCommandQueueEmpty = mCommandQueue.IsEmpty();

        LogDebug(lLogIndent,
                 lLogLevel,
                 "Connected? %u\n"
                 "Command queue empty? %u\n"
                 "Active exchange? %u\n",
                 lConnected,
                 lCommandQueueEmpty,
                 mActiveExchangeState.get() != nullptr);

        if (lConnected)
        {
            if (!lCommandQueueEmpty && !mActiveExchangeState)
            {
                Command::RequestBasis *                  lRequest;
                Command::ResponseBasis *                 lResponse;
                SendContext                              lSendContext;
                ConnectionBuffer::MutableCountedPointer  lConnectionBuffer;
                const uint8_t *                          lBuffer;
                size_t                                   lSize;

                LogDebug(lLogIndent,
                         lLogLevel,
                         "Would send queued command request!\n");

                mActiveExchangeState.reset(static_cast<ExchangeState *>(mCommandQueue.Pop()));

                LogDebug(lLogIndent,
                         lLogLevel,
                         "Command queue is now depth %zu\n",
                         mCommandQueue.GetSize());

                LogDebug(lLogIndent,
                         lLogLevel,
                         "mActiveExchangeState %p\n",
                         mActiveExchangeState.get());

                nlREQUIRE(mActiveExchangeState, done);
                nlREQUIRE(mActiveExchangeState->mExchange != nullptr, done);

                lRequest = mActiveExchangeState->mExchange->GetRequest();
                nlREQUIRE(lRequest != nullptr, done);

                lResponse = mActiveExchangeState->mExchange->GetResponse();
                nlREQUIRE(lResponse != nullptr, done);

                lBuffer = lRequest->GetBuffer();
                lSize = lRequest->GetSize();

                LogDebug(lLogIndent,
                         lLogLevel,
                         "Would send command request of %zu bytes at %p...\n",
                         lSize, lBuffer);

                lConnectionBuffer.reset(new ConnectionBuffer);
                nlREQUIRE(lConnectionBuffer, done);

                lRetval = lConnectionBuffer->Init();
                nlREQUIRE_SUCCESS(lRetval, done);

                lConnectionBuffer->Put(lBuffer, lSize);

                lRetval = lSendContext.Init(lConnectionBuffer,
                                            lResponse->GetRegularExpression(),
                                            lResponse->GetMatches(),
                                            CommandManager::OnResponseCompleteHandler,
                                            mErrorResponse.GetRegularExpression(),
                                            mErrorResponse.GetMatches(),
                                            CommandManager::OnResponseErrorHandler,
                                            this);
                nlREQUIRE_SUCCESS(lRetval, done);

                lRetval = SetCurrentSendContext(lSendContext);
                nlREQUIRE_SUCCESS(lRetval, done);

                lRetval = mConnectionManager->Send(lConnectionBuffer);
                nlREQUIRE_SUCCESS(lRetval, done);
            }
        }
    }

done:
    return (lRetval);
}

Status
CommandManager :: DispatchNotifications(const uint8_t *aBuffer, const size_t &aSize) const
{
    size_t lDispatchedSize;

    return (DispatchNotifications(aBuffer, aSize, lDispatchedSize));
}

Status
CommandManager :: DispatchNotifications(const uint8_t *aBuffer, const size_t &aSize, size_t &aOutDispatchedSize) const
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    static const size_t     kSizeMinimum = 5;
    static const size_t     kEOLSize = 2;
    const uint8_t *         lNotificationStart = aBuffer;
    const uint8_t *         lNotificationEnd;
    size_t                  lNotificationSearchSize = aSize;
    Status                  lRetval = kStatus_Success;


    // For any notification, at minimum, there needs to be, at least,
    // one '(', one "payload" character, one ')', and a carriage
    // return / new line pair for a valid notification we can do
    // something with.

    nlEXPECT(aSize >= kSizeMinimum, done);

    while (static_cast<size_t>(lNotificationStart - aBuffer) < aSize)
    {
        static const uint8_t kEOL[kEOLSize] = { '\r', '\n' };


        // A valid (un)solicited notification will be terminated with
        // a carriage return / new line pair. If we do not find a
        // singe one of those in the buffer, then there is nothing to
        // do.

        lNotificationEnd = static_cast<const uint8_t *>(memmem(lNotificationStart, lNotificationSearchSize, &kEOL[0], kEOLSize));
        nlEXPECT(lNotificationEnd != nullptr, done);

        // Otherwise, we have met the minimum size requirement and
        // found a terminating carriage return / new line pair. Start
        // attempting to match and dispatch notifications.

        if (lNotificationEnd != nullptr)
        {
            const size_t lNotificationSize = static_cast<size_t>(lNotificationEnd + kEOLSize - lNotificationStart);
            const std::set<NotificationHandlerState>::iterator lFirstNotificationHandler = mNotificationHandlers.begin();
            const std::set<NotificationHandlerState>::iterator lLastNotificationHandler = mNotificationHandlers.end();
            std::set<NotificationHandlerState>::iterator lCurrentNotificationHandler = lFirstNotificationHandler;

            LogDebug(lLogIndent,
                     lLogLevel,
                     "Dispatching notification for:\n");
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
            Log::Utilities::Memory::Write(lLogIndent,
                                          lLogLevel,
                                          lNotificationStart,
                                          lNotificationSize);
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

            while (lCurrentNotificationHandler != lLastNotificationHandler)
            {
                const RegularExpression &lRegularExpression = lCurrentNotificationHandler->mResponse->GetRegularExpression();
                RegularExpression::Matches &lMatches = lCurrentNotificationHandler->mResponse->GetMatches();
                Status lStatus;

                lStatus = lRegularExpression.Match((const char *)lNotificationStart, lNotificationSize, lMatches);

                if (lStatus == 0)
                {
                    lCurrentNotificationHandler->mOnNotificationReceivedHandler(lNotificationStart, lNotificationSize, lMatches, lCurrentNotificationHandler->mContext);
                    break;
                }

                ++lCurrentNotificationHandler;
            }

            lNotificationStart += lNotificationSize;
            lNotificationSearchSize -= lNotificationSize;
        }
    }

 done:
    aOutDispatchedSize = static_cast<size_t>(lNotificationStart - aBuffer);

    return (lRetval);
}

Status CommandManager :: DispatchNotifications(ConnectionBuffer::MutableCountedPointer aBuffer) const
{
    const uint8_t *  lBuffer = aBuffer->GetHead();
    const size_t     lSize = aBuffer->GetSize();
    size_t           lDispatchedSize = 0;
    Status           lRetval = kStatus_Success;

    lRetval = DispatchNotifications(lBuffer, lSize, lDispatchedSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    aBuffer->Get(lDispatchedSize);

 done:
    return (lRetval);
}

#if DEBUG
static void LogMatches(Log::Logger &aLog, Log::Level aLevel, const char *aDescription, const RegularExpression::Matches &aMatches)
{
    const RegularExpression::Matches::const_iterator lFirstMatch   = aMatches.begin();
    const RegularExpression::Matches::const_iterator lLastMatch    = aMatches.end();
    RegularExpression::Matches::const_iterator       lCurrentMatch = lFirstMatch;

    while (lCurrentMatch != lLastMatch)
    {
        aLog.Write(aLevel,
                   "%s: Match %02zu: start %" PRId64 " end %" PRId64 "\n",
                   aDescription,
                   std::distance(lFirstMatch, lCurrentMatch),
                   lCurrentMatch->rm_so,
                   lCurrentMatch->rm_eo);

        ++lCurrentMatch;
    }
}
#endif // DEBUG

Status CommandManager :: DispatchResponse(ConnectionBuffer::ImmutableCountedPointer &aResponseBuffer, const RegularExpression::Matches &aResponseMatches) const
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    OnCommandCompleteFunc lOnCommandCompleteHandler = mActiveExchangeState->mOnCommandCompleteHandler;
    Status lRetval = kStatus_Success;

#if DEBUG
    LogMatches(Log::Debug(), lLogLevel, __FUNCTION__, aResponseMatches);
#endif

    mActiveExchangeState->mExchange->GetResponse()->SetBuffer(aResponseBuffer);

    if (lOnCommandCompleteHandler)
    {
        lOnCommandCompleteHandler(mActiveExchangeState->mExchange,
                                  aResponseMatches,
                                  mActiveExchangeState->mContext);

    }

    return (lRetval);
}

// MARK: Connection Manager Delegate Methods

// MARK: Connection Manager Resolve Methods

/**
 *  @brief
 *    Delegation from the connection manager that a host name will
 *    resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that will resolve.
 *
 */
void
CommandManager :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost)
{
    (void)aConnectionManager;
    (void)aHost;
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name is
 *    resolving.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that is resolving.
 *
 */
void
CommandManager :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost)
{
    (void)aConnectionManager;
    (void)aHost;
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name has
 *    resolved to an IP address.
 *
 *  @note
 *    This delegation may be called more than once for a
 *    resolution, once for each IP address the host name resolves
 *    to.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did resolve.
 *  @param[in]  aIPAddress          An immutable reference to an IP
 *                                  address that the host name
 *                                  resolved to.
 *
 */
void
CommandManager :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;
    (void)aHost;
    (void)aIPAddress;
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name did
 *    not resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did not resolve.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  resolution.
 *
 */
void
CommandManager :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aHost;
    (void)aError;
}

// MARK: Connection Manager Connect Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
CommandManager :: ConnectionManagerWillConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aTimeout;
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server is connecting.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
CommandManager :: ConnectionManagerIsConnecting(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aTimeout;
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *
 */
void
CommandManager :: ConnectionManagerDidConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;

    // We have connected to the server; signal to the run loop that we
    // are ready for more work.

    CFRunLoopSourceSignal(mRunLoopSourceRef);
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed connection.
 *
 */
void
CommandManager :: ConnectionManagerDidNotConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aError;
}

// MARK: Connection Manager Application Data Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection has
 *    received application data.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aConnection         A reference to the connection over
 *                                  which the application data was
 *                                  received.
 *  @param[in]  aBuffer             The buffer containing the received
 *                                  application data.
 *
 */
void
CommandManager :: ConnectionManagerDidReceiveApplicationData(Common::ConnectionManagerBasis &aConnectionManager,
                                                             Common::ConnectionBasis &aConnection,
                                                             ConnectionBuffer::MutableCountedPointer aBuffer)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    const char *   lBuffer      = reinterpret_cast<const char *>(aBuffer->GetHead());
    const size_t   lSize        = aBuffer->GetSize();
    SendContext &  lSendContext = GetCurrentSendContext();
    Status         lMatchStatus;


    (void)aConnectionManager;
    (void)aConnection;

    LogDebug(lLogIndent,
             lLogLevel,
             "Processing command response data...\n");

    // If the send context is in use, then we are waiting for
    // solicited command response data, including an error
    // notification. Otherwise, we are receiving unsolicited
    // notifications from another client mutation.

    if (lSendContext.IsInUse())
    {
        LogDebug(lLogIndent,
                 lLogLevel,
                 "Still waiting for command completion...\n");

        // First, try to match the expected command response pattern.

        lMatchStatus = lSendContext.
            mResponseCompletionRegexp->Match(lBuffer,
                                             lSize,
                                             *lSendContext.mResponseCompletionMatches);

        LogDebug(lLogIndent,
                 lLogLevel,
                 "command response regnexec lStatus %d\n", lMatchStatus);

        if (lMatchStatus == 0)
        {
            LogDebug(lLogIndent,
                     lLogLevel,
                     "Received command completion!\n");

            if (lSendContext.mOnResponseCompleteHandler != nullptr)
            {
                lSendContext.mOnResponseCompleteHandler(aBuffer, *lSendContext.mResponseCompletionMatches, lSendContext.mContext);
            }

            // We received the command completion, flush
            // the buffer contents for subsequent
            // end-to-end application data.

            aBuffer->Flush();

            ClearCurrentSendContext();
        }
        else
        {
            // Either the expected command completion is in progress
            // or we have received a command error response.
            //
            // On the assumption of the latter, try to match the command
            // error pattern.

            lMatchStatus = lSendContext.
                mResponseErrorRegexp->Match(lBuffer, lSize, *lSendContext.mResponseErrorMatches);

            if (lMatchStatus == 0)
            {
                Log::Debug().Write("Received command error!\n");

                if (lSendContext.mOnResponseErrorHandler != nullptr)
                {
                    lSendContext.mOnResponseErrorHandler(kError_BadCommand, lSendContext.mContext);
                }

                // We received the command completion, flush
                // the buffer contents for subsequent
                // end-to-end application data.

                aBuffer->Flush();

                ClearCurrentSendContext();
            }
        }
    }
    else
    {
        // If we are here, then there is no active exchange and any
        // application data we are receiving is unsolicited
        // notification data.

        DispatchNotifications(aBuffer);
    }
}

// MARK: Connection Manager Disconnect Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *
 */
void
CommandManager :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the disconnection.
 *
 */
void
CommandManager :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aError;

    ClearCurrentSendContext();

    mCommandQueue.Flush();

    mActiveExchangeState.reset();
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  disconnection.
 *
 */
void
CommandManager :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aError;
}

// MARK: Connection Manager Error Method

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server experienced an error.
 *
 *  @note
 *    This delegation may occur along with other delegations with
 *    respect to the same underlying event or cause.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the event.
 *
 */
void
CommandManager :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aError;
}

// Run Loop Queue Delegate Methods

/**
 *  @brief
 *    Delegation from the run loop queue that the queue is empty.
 *
 *  @param[in]  aQueue  A reference to the run loop queue that
 *                      issued the delegation.
 *
 */
void
CommandManager :: QueueIsEmpty(RunLoopQueue &aQueue)
{
    (void)aQueue;
}

/**
 *  @brief
 *    Delegation from the run loop queue that the queue is not empty.
 *
 *  @param[in]  aQueue  A reference to the run loop queue that
 *                      issued the delegation.
 *
 */
void
CommandManager :: QueueIsNotEmpty(RunLoopQueue &aQueue)
{
    (void)aQueue;

    // The command queue is not empty and contains client command
    // requests to process; signal to the run loop that we are ready
    // for more work.

    CFRunLoopSourceSignal(mRunLoopSourceRef);
}

/**
 *  @brief
 *    This is the client command response successful completion handler.
 *
 *  This is invoked when a prior client command request receives a
 *  successful command response.
 *
 *  @param[in]  aResponseBuffer   A shared pointer to an immutable
 *                                response buffer containing the
 *                                received client command response
 *                                and, potentially, any associated
 *                                state change notifications.
 *  @param[in]  aResponseMatches  An immutable reference to the
 *                                regular expression substring matches
 *                                associated with this client command
 *                                response regular expression pattern.
 *
 */
void
CommandManager :: OnResponseCompleteHandler(ConnectionBuffer::ImmutableCountedPointer aResponseBuffer,
                                            const RegularExpression::Matches &aResponseMatches)
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    const uint8_t * lBuffer  = aResponseBuffer->GetHead();
#if DEBUG
    const size_t    lSize    = aResponseBuffer->GetSize();
    const size_t    lMatches = aResponseMatches.size();
#endif
    const RegularExpression::Matches::const_iterator lFirstMatch = aResponseMatches.begin();
    Status lStatus;


    LogDebug(lLogIndent,
             lLogLevel,
             "Received response of %zu bytes at %p w/ %zu matches...\n",
             lSize, lBuffer, lMatches);
#if DEBUG
    Log::Utilities::Memory::Write(lLogIndent,
                                  lLogLevel,
                                  lBuffer,
                                  lSize);
#endif

    // First, handle the "solicited" notifications, if any within the
    // buffer, and dispatch them.
    //
    // The notifications were not truly unsolicited since they came
    // alongside a command that implicitly asked for them, so they
    // effectively exist on that command exchange. However, they are
    // unsolicited insofar as they look like completions to commands
    // other than the one that initiated them and have no matching
    // request to pair them up with.
    //
    // There are truly unsolicited notifications that may arrive
    // without an active command exchange due to other client
    // mutations. Those are processed elsewhere, but use the same
    // dispatch engine.

    if ((!aResponseMatches.empty()) && (lFirstMatch->rm_so > 0))
    {
        LogDebug(lLogIndent,
                 lLogLevel,
                 "There are solicited notifications to dispatch!\n");

        lStatus = DispatchNotifications(lBuffer, static_cast<size_t>(lFirstMatch->rm_so));
        nlVERIFY_SUCCESS(lStatus);
    }

    // Second, handle and dispatch the command response termination.

    lStatus = DispatchResponse(aResponseBuffer, aResponseMatches);
    nlVERIFY_SUCCESS(lStatus);

    // Finally, reset the active exchange for another exchange and
    // signal the run loop that we are ready to process more work.

    mActiveExchangeState.reset();

    // We are done dispatching this client command response; signal to
    // the run loop that we are ready for more work.

    CFRunLoopSourceSignal(mRunLoopSourceRef);
}

/**
 *  @brief
 *    This is the client command request error handler.
 *
 *  This is invoked when a prior client command request receives an
 *  error response.
 *
 *  @param[in]  aError  An immutable reference to the error associated
 *                      with the response.
 *
 */
void
CommandManager :: OnResponseErrorHandler(const Common::Error &aError)
{
    DeclareScopedFunctionTracer(lTracer);
    OnCommandErrorFunc lOnCommandErrorHandler = mActiveExchangeState->mOnCommandErrorHandler;


    if (lOnCommandErrorHandler)
    {
        lOnCommandErrorHandler(mActiveExchangeState->mExchange,
                               aError,
                               mActiveExchangeState->mContext);

    }

    // Finally, reset the active exchange for another exchange and
    // signal the run loop that we are ready to process more work.

    mActiveExchangeState.reset();

    // Signal to the run loop that we are ready for more work.

    CFRunLoopSourceSignal(mRunLoopSourceRef);
}

/**
 *  @brief
 *    This is the client command response successful completion
 *    handler trampoline.
 *
 *  This is registered and invoked when a prior client command request
 *  receives a successful command response.
 *
 *  @param[in]  aResponseBuffer   A shared pointer to an immutable
 *                                response buffer containing the
 *                                received client command response
 *                                and, potentially, any associated
 *                                state change notifications.
 *  @param[in]  aResponseMatches  An immutable reference to the
 *                                regular expression substring matches
 *                                associated with this client command
 *                                response regular expression pattern.
 *  @param[in]  aContext          A pointer to the command manager class
 *                                instance that registered this
 *                                trampoline to call back into from
 *                                the trampoline.
 *
 */
void
CommandManager :: OnResponseCompleteHandler(ConnectionBuffer::ImmutableCountedPointer aResponseBuffer, const RegularExpression::Matches &aResponseMatches, void *aContext)
{
    CommandManager *lCommandManager = static_cast<CommandManager *>(aContext);

    if (lCommandManager != nullptr)
    {
        lCommandManager->OnResponseCompleteHandler(aResponseBuffer, aResponseMatches);
    }

    return;
}

/**
 *  @brief
 *    This is the client command request error handler trampoline.
 *
 *  This is registered and invoked when a prior client command request
 *  receives an error response.
 *
 *  @param[in]  aError    An immutable reference to the error associated
 *                        with the response.
 *  @param[in]  aContext  A pointer to the command manager class instance
 *                        that registered this trampoline to call back
 *                        into from the trampoline.
 *
 */
void
CommandManager :: OnResponseErrorHandler(const Common::Error &aError, void *aContext)
{
    CommandManager *lCommandManager = static_cast<CommandManager *>(aContext);

    if (lCommandManager != nullptr)
    {
        lCommandManager->OnResponseErrorHandler(aError);
    }

    return;
}

Status CommandManager :: SetCurrentSendContext(SendContext &aSendContext)
{
    return (mCurrentSendContext.Init(aSendContext));
}

void
CommandManager :: ClearCurrentSendContext(void)
{
    mCurrentSendContext.Reset();
}

CommandManager :: SendContext &CommandManager :: GetCurrentSendContext(void)
{
    return (mCurrentSendContext);
}

// MARK: CoreFoundation Run Loop Handlers

/**
 *  @brief
 *    Callback to return a description of this CoreFoundation run loop
 *    source object.
 *
 *  This returns a description of this CoreFoundation run loop source
 *  object in response to CFCopyDescription on the run loop source.
 *
 *  @returns
 *    An CoreFoundation immutable string reference for the run loop
 *    source object description.
 *
 */
CFStringRef
CommandManager :: CopyDescription(void) const
{
    return (CFSTR("Open HLX Client Command Manager"));
}

/**
 *  @brief
 *    Callback to perform any work associated with this CoreFoundation
 *    run loop source object.
 *
 *  This services the client command request queue as its primary
 *  source of work.
 *
 */
void
CommandManager :: Perform(void)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    Status lStatus;

    lStatus = ServiceCommandQueue();
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

// MARK: CoreFoundation Run Loop Handler Trampolines

/**
 *  @brief
 *    Callback trampoline to return a description of this
 *    CoreFoundation run loop source object.
 *
 *  @param[in]  aContext  A pointer to the command manager class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 *  @returns
 *    An CoreFoundation immutable string reference for the run loop
 *    source object description.
 *
 */
CFStringRef
CommandManager :: CopyDescription(const void *aContext)
{
    const CommandManager * lCommandManager = static_cast<const CommandManager *>(aContext);
    CFStringRef            lRetval = nullptr;

    if (lCommandManager != nullptr)
    {
        lRetval = lCommandManager->CopyDescription();
    }

    return (lRetval);
}

/**
 *  @brief
 *    Callback trampoline to perform any work associated with this
 *    CoreFoundation run loop source object.
 *
 *  @param[in]  aContext  A pointer to the command manager class
 *                        instance that registered this
 *                        trampoline to call back into from
 *                        the trampoline.
 *
 */
void
CommandManager :: Perform(void *aContext)
{
    CommandManager *lCommandManager = static_cast<CommandManager *>(aContext);

    if (lCommandManager != nullptr)
    {
        lCommandManager->Perform();
    }
}

}; // namespace Client

}; // namespace HLX
