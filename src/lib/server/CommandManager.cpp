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
 *      This file implements an object for managing HLX server commands,
 *      including dispatching incoming requests and handling outgoing
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

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/CommandErrorResponse.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/CommandRequestBasis.hpp>
#include <OpenHLX/Server/CommandResponseBasis.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// MARK: Command Manager Request Handler State

CommandManager :: RequestHandlerState :: RequestHandlerState(void) :
    mRequest(nullptr),
    mOnRequestReceivedHandler(nullptr),
    mContext(nullptr)
{
    return;
}

CommandManager :: RequestHandlerState :: RequestHandlerState(const RequestHandlerState &aRequestHandlerState) :
    mRequest(aRequestHandlerState.mRequest),
    mOnRequestReceivedHandler(aRequestHandlerState.mOnRequestReceivedHandler),
    mContext(aRequestHandlerState.mContext)
{
    return;
}

CommandManager :: RequestHandlerState :: ~RequestHandlerState(void)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
}

Status CommandManager :: RequestHandlerState :: Init(HLX::Server::Command::RequestBasis &aRequest, void *aContext, OnRequestReceivedFunc aOnRequestReceivedHandler)
{
    DeclareLogLevelWithValue(lLogLevel, 1);
    DeclareScopedFunctionTracerAtLevel(lTracer, lLogLevel);
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aOnRequestReceivedHandler != nullptr, done, lRetval = -EINVAL);

    mRequest = &aRequest;
    mContext = aContext;
    mOnRequestReceivedHandler = aOnRequestReceivedHandler;

 done:
    return (lRetval);
}

CommandManager :: RequestHandlerState CommandManager :: RequestHandlerState :: operator =(const RequestHandlerState &aRequestHandlerState)
{
    mRequest                  = aRequestHandlerState.mRequest;
    mContext                  = aRequestHandlerState.mContext;
    mOnRequestReceivedHandler = aRequestHandlerState.mOnRequestReceivedHandler;

    return (*this);
}

bool CommandManager :: RequestHandlerState :: operator <(const RequestHandlerState &aRequestHandlerState) const
{
    return (mRequest->GetRegularExpression() < aRequestHandlerState.mRequest->GetRegularExpression());
}

// MARK: Command Manager

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
CommandManager :: CommandManager(void) :
    ConnectionManagerDelegate(),
    mRunLoopParameters(),
    mDelegate(nullptr),
    mConnectionManager(nullptr),
    mRequestHandlers()
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
    return;
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
    Status            lRetval = kStatus_Success;

    mConnectionManager = &aConnectionManager;

    // Register one-of-many delegate interest in connections and
    // errors.

    lRetval = mConnectionManager->AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Register one-and-only delegate interest in application data.

    lRetval = mConnectionManager->SetApplicationDataDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command response to all connected clients.
 *
 *  This attempts to send a server command response for a
 *  previously-received command request to all connected (and,
 *  consequently, subscribed) clients.
 *
 *  @param[in]  aBuffer  An immutable shared pointer to the
 *                       buffer containing the command response to
 *                       send.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
CommandManager :: SendResponse(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status  lRetval = kStatus_Success;

    lRetval = mConnectionManager->Send(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command response to the specified connected
 *    client.
 *
 *  This attempts to send a server command response for a
 *  previously-received command request from the specified connected
 *  client.
 *
 *  @param[in]  aConnection  A mutable reference to the connection
 *                           over which to to send the response.
 *  @param[in]  aBuffer      An immutable shared pointer to the
 *                           buffer containing the command response to
 *                           send.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
CommandManager :: SendResponse(ConnectionBasis &aConnection, ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status  lRetval = kStatus_Success;

    lRetval = mConnectionManager->Send(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command error response to the specified connected
 *    client.
 *
 *  This attempts to send a server command error response for a
 *  previously-received command request from the specified connected
 *  client.
 *
 *  @param[in]  aConnection  A mutable reference to the connection
 *                           over which to to send the error response.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If resources for the error response
 *                            could not be allocated.
 *
 */
Status
CommandManager :: SendErrorResponse(ConnectionBasis &aConnection) const
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendErrorResponse(aConnection, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a server command error response to the specified connected
 *    client using the provided buffer.
 *
 *  This attempts to send a server command error response for a
 *  previously-received command request from the specified connected
 *  client using the provided buffer.
 *
 *  @param[in]      aConnection  A mutable reference to the connection
 *                               over which to to send the error response.
 *  @param[in,out]  aBuffer      A reference to a shared pointer to the
 *                               buffer in which to put the error
 *                               response.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If resources for the error response
 *                            could not be allocated.
 *
 */
Status
CommandManager :: SendErrorResponse(ConnectionBasis &aConnection,
                                    ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Command::ErrorResponse  lErrorResponse;
    const uint8_t *         lBuffer;
    size_t                  lSize;
    Status                  lRetval;

    lRetval = lErrorResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lErrorResponse.GetBuffer();
    lSize = lErrorResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

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
 *    Register a command request handler.
 *
 *  This registers the specified unsolicited command request handler and
 *  context for the provided client command response regular
 *  expression.
 *
 *  @param[in]  aRequest                        A mutable reference to
 *                                              the server command
 *                                              request regular
 *                                              expression to register
 *                                              the handler and
 *                                              context for.
 *  @param[in]  aContext                        The context to register
 *                                              which will be passed
 *                                              back via the handler.
 *  @param[in]  aOnRequestReceivedHandler       The request handler
 *                                              to register.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If the @a aOnRequestReceivedHandler
 *                            is null.
 *  @retval  -EEXIST          If a registration already exists.
 *
 */
Status
CommandManager :: RegisterRequestHandler(HLX::Server::Command::RequestBasis &aRequest,
                                         void *aContext,
                                         OnRequestReceivedFunc aOnRequestReceivedHandler)
{
    RequestHandlerState                                       lRequestHandlerState;
    std::pair<std::set<RequestHandlerState>::iterator, bool>  lStatus;
    Status                                                    lRetval = kStatus_Success;

    lRetval = lRequestHandlerState.Init(aRequest, aContext, aOnRequestReceivedHandler);
    nlREQUIRE_SUCCESS(lRetval, done);

    lStatus = mRequestHandlers.insert(lRequestHandlerState);
    nlREQUIRE_ACTION(lStatus.second == true, done, lRetval = -EEXIST);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Unregister a command request handler.
 *
 *  This unregisters any command request handler and context for the provided
 *  server command request regular expression.
 *
 *  @param[in]  aRequest                        A mutable reference to
 *                                              the server command
 *                                              request regular
 *                                              expression to unregister
 *                                              the handler and
 *                                              context for.
 *  @param[in]  aContext                        The context to unregister.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOENT          If there was no registration for @a aRequest.
 *
 */
Status
CommandManager :: UnregisterRequestHandler(const HLX::Server::Command::RequestBasis &aRequest, void *aContext)
{
    Status lRetval = kStatus_Success;

    (void)aRequest;
    (void)aContext;

    return (lRetval);
}

Status
CommandManager :: DispatchRequest(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize) const
{
    size_t lDispatchedSize;

    return (DispatchRequest(aConnection, aBuffer, aSize, lDispatchedSize));
}

Status
CommandManager :: DispatchRequest(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, size_t &aOutDispatchedSize) const
{
    DeclareLogIndentWithValue(lLogIndent, 0);
    DeclareLogLevelWithValue(lLogLevel, 1);
    static const size_t     kSizeMinimum = 3;
    static const size_t     kEOLSize = 1;
    const uint8_t *         lRequestStart = aBuffer;
    const uint8_t *         lRequestEnd;
    size_t                  lRequestSearchSize = aSize;
    Status                  lRetval = kStatus_Success;


    // For any notification, at minimum, there needs to be, at least,
    // one '[', one "payload" character, and one ']' for a valid
    // request we can do something with.

    nlEXPECT(aSize >= kSizeMinimum, done);

    while (static_cast<size_t>(lRequestStart - aBuffer) < aSize)
    {
        static const uint8_t kEOL[kEOLSize] = { ']' };


        // A valid unsolicited request will be terminated with
        // a carriage return / new line pair. If we do not find a
        // singe one of those in the buffer, then there is nothing to
        // do.

        lRequestEnd = static_cast<const uint8_t *>(memmem(lRequestStart, lRequestSearchSize, &kEOL[0], kEOLSize));
        nlEXPECT(lRequestEnd != nullptr, done);

        // Otherwise, we have met the minimum size requirement and
        // found a line termination. Start attempting to match and
        // dispatch the request.

        if (lRequestEnd != nullptr)
        {
            const size_t lRequestSize = static_cast<size_t>(lRequestEnd + kEOLSize - lRequestStart);
            const std::set<RequestHandlerState>::iterator lFirstRequestHandler = mRequestHandlers.begin();
            const std::set<RequestHandlerState>::iterator lLastRequestHandler = mRequestHandlers.end();
            std::set<RequestHandlerState>::iterator lCurrentRequestHandler = lFirstRequestHandler;
            Status lStatus = 1;

            LogDebug(lLogIndent,
                     lLogLevel,
                     "Dispatching request for:\n");
#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
            Log::Utilities::Memory::Write(Log::Debug(),
                                          lLogIndent,
                                          lLogLevel,
                                          lRequestStart,
                                          lRequestSize,
                                          sizeof (uint8_t));
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

            while (lCurrentRequestHandler != lLastRequestHandler)
            {
                const RegularExpression &lRegularExpression = lCurrentRequestHandler->mRequest->GetRegularExpression();
                RegularExpression::Matches &lMatches = lCurrentRequestHandler->mRequest->GetMatches();

                lStatus = lRegularExpression.Match((const char *)lRequestStart, lRequestSize, lMatches);

                if (lStatus == 0)
                {
                    lCurrentRequestHandler->mOnRequestReceivedHandler(aConnection, lRequestStart, lRequestSize, lMatches, lCurrentRequestHandler->mContext);
                    break;
                }

                ++lCurrentRequestHandler;
            }

            // If we are here and no handlers matched on the request
            // then we have a request in flight that cannot and will
            // not be responded to. Default to sending an error
            // response in such a case.

            if (lStatus != 0)
            {
                lRetval = SendErrorResponse(aConnection);
                nlREQUIRE_SUCCESS(lRetval, done);
            }

            lRequestStart += lRequestSize;
            lRequestSearchSize -= lRequestSize;
        }
    }

 done:
    aOutDispatchedSize = static_cast<size_t>(lRequestStart - aBuffer);

    LogDebug(lLogIndent,
             lLogLevel,
             "lRetval %d aOutDispatchedSize %zu\n",
             lRetval,
             aOutDispatchedSize);

    return (lRetval);
}

Status
CommandManager :: DispatchRequest(ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer aBuffer) const
{
    const uint8_t *  lBuffer = aBuffer->GetHead();
    const size_t     lSize = aBuffer->GetSize();
    size_t           lDispatchedSize = 0;
    Status           lRetval = kStatus_Success;

    lRetval = DispatchRequest(aConnection, lBuffer, lSize, lDispatchedSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    aBuffer->Get(lDispatchedSize);

 done:
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
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that will resolve.
 *
 */
void
CommandManager :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;
    (void)aHost;
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name is
 *    resolving.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that is resolving.
 *
 */
void
CommandManager :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;
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
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did resolve.
 *  @param[in]  aIPAddress          An immutable reference to an IP
 *                                  address that the host name
 *                                  resolved to.
 *
 */
void
CommandManager :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;
    (void)aRoles;
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
void
CommandManager :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;
    (void)aHost;
    (void)aError;
}

// MARK: Connection Manager Listen Methods

/**
 *  @brief
 *    Delegation from the connection manager that it will listen
 *    for connections from peer clients at the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL at which the server is
 *                                  to listen at.
 *
 */
void
CommandManager :: ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;

    return;
}

/**
 *  @brief
 *    Delegation from the connection manager that it is in the
 *    processing of listening for connections from peer clients at
 *    the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL at which the server is
 *                                  listening at.
 *
 */
void
CommandManager :: ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;

    return;
}

/**
 *  @brief
 *    Delegation from the connection manager that it is listening
 *    for connections from peer clients at the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL at which the server is
 *                                  listening at.
 *
 */
void
CommandManager :: ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;

    return;
}

/**
 *  @brief
 *    Delegation from the connection manager that it did not
 *    listen for connections from peer clients at the specified
 *    URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL at which the server
 *                                  was to listen at.
 *  @param[in]  aError              An immutable reference to the
 *                                  error associated with the
 *                                  failed server listen.
 *
 */
void
CommandManager :: ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aURLRef;
    (void)aError;

    return;
}

// MARK: Connection Manager Accept Methods

/**
 *  @brief
 *    Delegation from the connection manager that it will accept
 *    a connection from the peer client at the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL for the client from
 *                                  which the server will accept a
 *                                  connection.
 *
 */
void
CommandManager :: ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;
}

/**
 *  @brief
 *    Delegation from the connection manager that it is in the
 *    process of accepting a connection from the peer client at
 *    the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL for the client from
 *                                  which the server is accepting
 *                                  a connection.
 *
 */
void
CommandManager :: ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;
}

/**
 *  @brief
 *    Delegation from the connection manager that it did accept
 *    a connection from the peer client at the specified URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL for the client from
 *                                  which the server did accept a
 *                                  connection.
 *
 */
void
CommandManager :: ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aURLRef;
}

/**
 *  @brief
 *    Delegation from the connection manager that it did not
 *    accept a connection from the peer client at the specified
 *    URL.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the
 *                                  delegation.
 *  @param[in]  aURLRef             The URL for the client from
 *                                  which the server did not
 *                                  accept a connection.
 *  @param[in]  aError              An immutable reference to the
 *                                  error associated with the
 *                                  failed server accept.
 *
 */
void
CommandManager :: ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
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
    const uint8_t *  lBuffer = reinterpret_cast<const uint8_t *>(aBuffer->GetHead());
    const size_t     lSize = aBuffer->GetSize();


    (void)aConnectionManager;

#if (defined(DEBUG) && DEBUG) && !defined(NDEBUG)
    Log::Debug().Write(lLogIndent,
                       lLogLevel,
                       "Processing command request data...\n");
#endif // (defined(DEBUG) && DEBUG) && !defined(NDEBUG)

    DispatchRequest(reinterpret_cast<Server::ConnectionBasis &>(aConnection), lBuffer, lSize);

    aBuffer->Flush();
}

// MARK: Connection Manager Disconnect Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection manager
 *                                  that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *
 */
void
CommandManager :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aRoles;
    (void)aURLRef;
}

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
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the disconnection.
 *
 */
void
CommandManager :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;
    (void)aURLRef;
    (void)aError;
}

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
 *  @param[in]  aURLRef             The URL associated with the peer server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  disconnection.
 *
 */
void
CommandManager :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;
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
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the event.
 *
 */
void
CommandManager :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;
    (void)aError;

    return;
}

}; // namespace Server

}; // namespace HLX
