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
 *      This file defines an object for managing a peer-to-peer
 *      connection between an HLX server and its clients.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONMANAGER_HPP
#define OPENHLXSERVERCONNECTIONMANAGER_HPP

#include <memory>
#include <unordered_set>
#include <vector>

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionManagerApplicationDataDelegate.hpp>
#include <OpenHLX/Common/ConnectionManagerBasis.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/ConnectionBasis.hpp>
#include <OpenHLX/Server/ConnectionBasisDelegate.hpp>
#include <OpenHLX/Server/ConnectionFactory.hpp>
#include <OpenHLX/Server/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Server/ConnectionSchemeIdentifierManager.hpp>
#include <OpenHLX/Server/ListenerBasis.hpp>
#include <OpenHLX/Server/ListenerBasisAcceptDelegate.hpp>
#include <OpenHLX/Server/ListenerBasisDelegate.hpp>
#include <OpenHLX/Server/ListenerFactory.hpp>


namespace HLX
{

namespace Common
{

    class RunLoopParameters;

}; // namespace Common

namespace Server
{

/**
 *  @brief
 *    An object for managing a peer-to-peer connection between a HLX
 *    server and its clients.
 *
 *  @ingroup server
 *
 */
class ConnectionManager :
    public Common::ConnectionManagerBasis,
    public ListenerBasisAcceptDelegate,
    public ListenerBasisDelegate,
    public ConnectionBasisDelegate
{
public:
    ConnectionManager(void);
    virtual ~ConnectionManager(void) = default;

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status Listen(void);
    Common::Status Listen(const Versions &aVersions);
    Common::Status Listen(const char *aMaybeURL);
    Common::Status Listen(const char *aMaybeURL, const Versions &aVersions);

    Common::Status AddDelegate(ConnectionManagerDelegate *aDelegate);
    Common::Status RemoveDelegate(ConnectionManagerDelegate *aDelegate);

    Common::Status Send(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer);
    Common::Status Send(ConnectionBasis &aConnection, Common::ConnectionBuffer::ImmutableCountedPointer aBuffer);

    // Listener Basis Delegate Methods

    // Listen

    void ListenerWillListen(ListenerBasis &aListener) final;
    void ListenerIsListening(ListenerBasis &aListener) final;
    void ListenerDidListen(ListenerBasis &aListener) final;
    void ListenerDidNotListen(ListenerBasis &aListener, const Common::Error &aError) final;

    // Error

    void ListenerError(ListenerBasis &aListener, const Common::Error &aError) final;

    // Listener Basis Accept Delegate Method

    Common::Status ListenerDidAccept(ListenerBasis &aListener, const int &aSocket, const Common::SocketAddress &aAddress) final;

    // Connection Basis Delegate Methods

    // Accept Methods

    void ConnectionWillAccept(ConnectionBasis &aConnection) final;
    void ConnectionIsAccepting(ConnectionBasis &aConnection) final;
    void ConnectionDidAccept(ConnectionBasis &aConnection) final;
    void ConnectionDidNotAccept(ConnectionBasis &aConnection, const Common::Error &aError) final;

    // Application Data Method

    void ConnectionDidReceiveApplicationData(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer aBuffer) final;

    // Disconnect Methods

    void ConnectionWillDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef) final;
    void ConnectionDidDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionDidNotDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error Method

    void ConnectionError(ConnectionBasis &aConnection, const Common::Error &aError) final;

private:
    void OnWillResolve(const char *aHost) final;
    void OnIsResolving(const char *aHost) final;
    void OnDidResolve(const char *aHost, const Common::IPAddress &aIPAddress) final;
    void OnDidNotResolve(const char *aHost, const Common::Error &aError) final;

    Common::Status Listen(const Common::SocketAddress *aFirst, const Common::SocketAddress *aLast);

    Common::Status CreateConnection(CFStringRef aScheme, const int &aSocket, const Common::SocketAddress &aPeerAddress);

    Common::Status DisposeInactiveConnection(ConnectionBasis &aConnection);
    void FlushInactiveConnections(void);

private:
    typedef std::unordered_set<ConnectionManagerDelegate *> ConnectionManagerDelegates;
    typedef std::vector<std::unique_ptr<ConnectionBasis>>   Connections;
    typedef std::vector<std::unique_ptr<ListenerBasis>>     Listeners;

    ListenerFactory                             mListenerFactory;
    ConnectionFactory                           mConnectionFactory;
    Listeners                                   mListeners;
    Connections                                 mActiveConnections;
    Connections                                 mInactiveConnections;
    Common::RunLoopParameters                   mRunLoopParameters;
    ConnectionManagerDelegates                  mDelegates;
    ConnectionSchemeIdentifierManager           mSchemeIdentifierManager;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONMANAGER_HPP
