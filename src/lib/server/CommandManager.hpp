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
 *      This file defines an object for managing HLX server commands,
 *      including dispatching incoming requests and handling outgoing
 *      synchronous responses.
 *
 */

#ifndef OPENHLXSERVERCOMMANDMANAGER_HPP
#define OPENHLXSERVERCOMMANDMANAGER_HPP

#include <memory>
#include <set>

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Common/ConnectionManagerApplicationDataDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Server/CommandRequestBasis.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Server/ConnectionManagerDelegate.hpp>


namespace HLX
{

namespace Server
{

class CommandManagerDelegate;
class ConnectionManager;

/**
 *  @brief
 *    An object for managing HLX server commands, including
 *    dispatching incoming requests and handling outgoing synchronous
 *    responses.
 *
 *  @ingroup server
 *  @ingroup command
 *
 */
class CommandManager :
    public ConnectionManagerDelegate,
    public Common::ConnectionManagerApplicationDataDelegate
{
public:
    typedef void (* OnRequestReceivedFunc)(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

public:
    CommandManager(void);
    ~CommandManager(void);

    Common::Status Init(ConnectionManager &aConnectionManager, const Common::RunLoopParameters &aRunLoopParameters);

    Common::Status SendResponse(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) const;
    Common::Status SendResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) const;
    Common::Status SendErrorResponse(ConnectionBasis &aConnection) const;
    Common::Status SendErrorResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    CommandManagerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(CommandManagerDelegate *aDelegate);

    Common::Status RegisterRequestHandler(Server::Command::RequestBasis &aRequest, void *aContext, OnRequestReceivedFunc aOnRequestReceivedHandler);
    Common::Status UnregisterRequestHandler(const Server::Command::RequestBasis &aRequest, void *aContext);

    // Connection Manager Delegate Methods

    // Resolve Methods

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError) final;

    // Listen Methods

    void ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Accept Methods

    void ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Application Data Method

    void ConnectionManagerDidReceiveApplicationData(Common::ConnectionManagerBasis &aConnectionManager,
                                                    Common::ConnectionBasis &aConnection,
                                                    Common::ConnectionBuffer::MutableCountedPointer aBuffer) final;

    // Disconnect Methods

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error Method

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError) final;

private:
    Common::Status DispatchRequest(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize) const;
    Common::Status DispatchRequest(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, size_t &aOutDispatchedSize) const;
    Common::Status DispatchRequest(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer aBuffer) const;

private:
    class RequestHandlerState
    {
    public:
        RequestHandlerState(void);
        RequestHandlerState(const RequestHandlerState &aRequestHandlerState);
        ~RequestHandlerState(void);

        Common::Status Init(HLX::Server::Command::RequestBasis &aRequest, void *aContext, OnRequestReceivedFunc aOnNotificationReceivedFunc);

        RequestHandlerState operator =(const RequestHandlerState &aRequestHandlerState);

        bool operator <(const RequestHandlerState &aRequestHandlerState) const;

        Command::RequestBasis *               mRequest;
        OnRequestReceivedFunc                 mOnRequestReceivedHandler;
        void *                                mContext;
    };

    Common::RunLoopParameters             mRunLoopParameters;
    CommandManagerDelegate *              mDelegate;
    ConnectionManager *                   mConnectionManager;
    std::set<RequestHandlerState>         mRequestHandlers;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCOMMANDMANAGER_HPP
