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
 *      This file defines an object for managing a HLX client command
 *      queue, including dispatching outgoing requests and handling
 *      incoming asynchronous state change notifications and
 *      synchronous command responses.
 *
 */

#ifndef OPENHLXCLIENTCOMMANDMANAGER_HPP
#define OPENHLXCLIENTCOMMANDMANAGER_HPP

#include <memory>
#include <set>

#include <CoreFoundation/CFRunLoop.h>

#include <OpenHLX/Client/CommandErrorResponse.hpp>
#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Common/ConnectionBasis.hpp>
#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/ConnectionManagerApplicationDataDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/RunLoopQueue.hpp>
#include <OpenHLX/Common/RunLoopQueueDelegate.hpp>


namespace HLX
{

namespace Client
{

class CommandManagerDelegate;
class ConnectionManager;
class ControllerBase;

/**
 *  @brief
 *    An object for managing a HLX client command queue, including
 *    dispatching outgoing requests and handling incoming asynchronous
 *    state change notifications and synchronous command responses.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class CommandManager :
    public ConnectionManagerDelegate,
    public Common::ConnectionManagerApplicationDataDelegate,
    public Common::RunLoopQueueDelegate
{
public:
    /**
     *  @brief
     *    Command completion callback function.
     *
     *  This defines a function to call on successful completion of a
     *  previously-dispatched client command request.
     *
     */
    typedef void (* OnCommandCompleteFunc)(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aResponseMatches, void *aContext);

    /**
     *  @brief
     *    Command error callback function.
     *
     *  This defines a function to call on error for a
     *  previously-dispatched client command request.
     *
     */
    typedef void (* OnCommandErrorFunc)(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    /**
     *  @brief
     *    Notification callback function.
     *
     *  This defines a function to call on receipt of a synchronous
     *  (that is, in response to a command request) or asynchronous
     *  (that is, outside of a response to a command request) state
     *  change notification.
     *
     */
    typedef void (* OnNotificationReceivedFunc)(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

public:
    CommandManager(void);
    ~CommandManager(void);

    Common::Status Init(ConnectionManager &aConnectionManager,
                        const Common::RunLoopParameters &aRunLoopParameters);

    Common::Status SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Timeout &aTimeout, OnCommandCompleteFunc aOnCommandCompleteHandler, OnCommandErrorFunc aOnCommandErrorHandler, void *aContext);

    CommandManagerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(CommandManagerDelegate *aDelegate);

    Common::Status RegisterNotificationHandler(Command::ResponseBasis &aResponse, void *aContext, OnNotificationReceivedFunc aOnNotificationReceivedHandler);
    Common::Status UnregisterNotificationHandler(const Command::ResponseBasis &aResponse, void *aContext);

    // Connection Manager Delegate Methods

    // Resolve Methods

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager,
                                      const Common::ConnectionManagerBasis::Roles &aRoles,
                                      const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager,
                                      const Common::ConnectionManagerBasis::Roles &aRoles,
                                      const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager,
                                     const Common::ConnectionManagerBasis::Roles &aRoles,
                                     const char *aHost, const Common::IPAddress &aIPAddress) final;

    /**
     *  @brief
     *    Delegation from the connection manager that a host name did
     *    not resolve.
     *
     *  @param[in]  aConnectionManager  A reference to the connection
     *                                  manager that issued the delegation.
     *  @param[in]  aRoles              An immutable reference to the roles
     *                                  in which the connection manager
     *                                  that issued the delegation is
     *                                  acting.
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did not resolve.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  resolution.
     *
     */
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager,
                                        const Common::ConnectionManagerBasis::Roles &aRoles,
                                        const char *aHost,
                                        const Common::Error &aError) final;

    // Connect Methods

    void ConnectionManagerWillConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerIsConnecting(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerDidConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Application Data Method

    void ConnectionManagerDidReceiveApplicationData(Common::ConnectionManagerBasis &aConnectionManager,
                                                    Common::ConnectionBasis &aConnection,
                                                    Common::ConnectionBuffer::MutableCountedPointer aBuffer) final;

    // Disconnect Methods

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef) final;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did disconnect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection manager
     *                                  that issued the delegation.
     *  @param[in]  aRoles              An immutable reference to the roles
     *                                  in which the connection manager
     *                                  that issued the delegation is
     *                                  acting.
     *  @param[in]  aURLRef             The URL associated with the peer
     *                                  server.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the disconnection.
     *
     */
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager,
                                        const Common::ConnectionManagerBasis::Roles &aRoles,
                                        CFURLRef aURLRef,
                                        const Common::Error &aError) final;

    /**
     *  @brief
     *    Delegation from the connection manager that a connection to a
     *    peer server did not disconnect.
     *
     *  @param[in]  aConnectionManager  A reference to the connection manager
     *                                  that issued the delegation.
     *  @param[in]  aRoles              An immutable reference to the roles
     *                                  in which the connection manager
     *                                  that issued the delegation is
     *                                  acting.
     *  @param[in]  aURLRef             The URL associated with the peer
                                        server.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  disconnection.
     *
     */
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager,
                                           const Common::ConnectionManagerBasis::Roles &aRoles,
                                           CFURLRef aURLRef,
                                           const Common::Error &aError) final;

    // Error Method

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
     *  @param[in]  aRoles              An immutable reference to the roles
     *                                  in which the connection manager
     *                                  that issued the delegation is
     *                                  acting.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the event.
     *
     */
    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError) final;

    // Run Loop Queue Delegate Methods

    void QueueIsEmpty(Common::RunLoopQueue &aQueue);
    void QueueIsNotEmpty(Common::RunLoopQueue &aQueue);

    // Connection Manager Response Handler Trampolines

    static void OnResponseCompleteHandler(Common::ConnectionBuffer::ImmutableCountedPointer aResponseBuffer, const Common::RegularExpression::Matches &aResponseMatches, void *aContext);
    static void OnResponseErrorHandler(const Common::Error &aError, void *aContext);

    // CFRunLoop Handler Trampolines

    static CFStringRef CopyDescription(const void *aContext);
    static void        Perform(void *aContext);

private:
    // CFRunLoop Handlers

    CFStringRef        CopyDescription(void) const;
    void               Perform(void);

private:
    Common::Status ServiceCommandQueue(void);
    Common::Status DispatchNotifications(const uint8_t *aBuffer, const size_t &aSize) const;
    Common::Status DispatchNotifications(const uint8_t *aBuffer, const size_t &aSize, size_t &aOutDispatchedSize) const;
    Common::Status DispatchNotifications(Common::ConnectionBuffer::MutableCountedPointer aBuffer) const;

    Common::Status DispatchResponse(Common::ConnectionBuffer::ImmutableCountedPointer &aResponseBuffer, const Common::RegularExpression::Matches &aResponseMatches) const;

    // Connection Manager Response Handlers

    void OnResponseCompleteHandler(Common::ConnectionBuffer::ImmutableCountedPointer aResponseBuffer, const Common::RegularExpression::Matches &aResponseMatches);
    void OnResponseErrorHandler(const Common::Error &aError);

    // Send Context Management

    class SendContext;

    Common::Status SetCurrentSendContext(SendContext &aSendContext);
    void           ClearCurrentSendContext(void);
    SendContext &  GetCurrentSendContext(void);

private:
    struct ExchangeState
    {
        typedef std::shared_ptr<ExchangeState> MutableCountedPointer;

        ExchangeState(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                      const Common::Timeout &aTimeout, OnCommandCompleteFunc aOnCommandCompleteHandler, OnCommandErrorFunc aOnCommandErrorHandler, void *aContext);
        ~ExchangeState(void);

        Command::ExchangeBasis::MutableCountedPointer  mExchange;
        const Common::Timeout                          mTimeout;
        OnCommandCompleteFunc                          mOnCommandCompleteHandler;
        OnCommandErrorFunc                             mOnCommandErrorHandler;
        void *                                         mContext;
    };

    class NotificationHandlerState
    {
    public:
        NotificationHandlerState(void);
        NotificationHandlerState(const NotificationHandlerState &aNotificationHandlerState);
        ~NotificationHandlerState(void);

        Common::Status Init(Command::ResponseBasis &aResponse, void *aContext, OnNotificationReceivedFunc aOnNotificationReceivedFunc);

        NotificationHandlerState operator =(const NotificationHandlerState &aNotificationHandlerState);

        bool operator <(const NotificationHandlerState &aNotificationHandlerState) const;

        Command::ResponseBasis *              mResponse;
        OnNotificationReceivedFunc            mOnNotificationReceivedHandler;
        void *                                mContext;
    };

    class SendContext
    {
    public:
        typedef void (* OnResponseCompleteFunc)(Common::ConnectionBuffer::ImmutableCountedPointer aResponseBuffer, const Common::RegularExpression::Matches &aResponseMatches, void *aContext);
        typedef void (* OnResponseErrorFunc)(const Common::Error &aError, void *aContext);

    public:
        SendContext(void);
        ~SendContext(void);

        Common::Status Init(const SendContext &aSendContext);
        Common::Status Init(Common::ConnectionBuffer::ImmutableCountedPointer aRequestBuffer,
                    const Common::RegularExpression &                         aResponseCompletionRegexp,
                    Common::RegularExpression::Matches &                      aResponseCompletionMatches,
                    OnResponseCompleteFunc                                    aOnResponseCompleteHandler,
                    const Common::RegularExpression &                         aResponseErrorRegexp,
                    Common::RegularExpression::Matches &                      aResponseErrorMatches,
                    OnResponseErrorFunc                                       aOnResponseErrorHandler,
                    void *                                                    aContext);

        void Reset(void);
        bool IsInUse(void) const;

        Common::ConnectionBuffer::ImmutableCountedPointer  mRequestBuffer;
        const Common::RegularExpression *                  mResponseCompletionRegexp;
        Common::RegularExpression::Matches *               mResponseCompletionMatches;
        OnResponseCompleteFunc                             mOnResponseCompleteHandler;
        const Common::RegularExpression *                  mResponseErrorRegexp;
        Common::RegularExpression::Matches *               mResponseErrorMatches;
        OnResponseErrorFunc                                mOnResponseErrorHandler;
        void *                                             mContext;
    };

    Common::RunLoopParameters             mRunLoopParameters;
    CommandManagerDelegate *              mDelegate;
    CFRunLoopSourceRef                    mRunLoopSourceRef;
    ConnectionManager *                   mConnectionManager;
    Common::RunLoopQueue                  mCommandQueue;
    ExchangeState::MutableCountedPointer  mActiveExchangeState;
    std::set<NotificationHandlerState>    mNotificationHandlers;
    Command::ErrorResponse                mErrorResponse;
    SendContext                           mCurrentSendContext;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCOMMANDMANAGER_HPP
