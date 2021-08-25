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
 *      This file defines an object for managing a peer-to-peer
 *      connection between an HLX client and server.
 *
 */

#ifndef OPENHLXCLIENTCONNECTIONMANAGER_HPP
#define OPENHLXCLIENTCONNECTIONMANAGER_HPP

#include <unordered_set>

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Client/ConnectionBasis.hpp>
#include <OpenHLX/Client/ConnectionBasisDelegate.hpp>
#include <OpenHLX/Client/ConnectionFactory.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Common/ConnectionManagerApplicationDataDelegate.hpp>
#include <OpenHLX/Common/ConnectionManagerBasis.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Common
{

    class RunLoopParameters;

}; // namespace Common

namespace Client
{

/**
 *  @brief
 *    An object for managing a peer-to-peer connection between a HLX
 *    client and server.
 *
 *  @ingroup client
 *
 */
class ConnectionManager :
    public Common::ConnectionManagerBasis,
    public ConnectionBasisDelegate
{
public:
    ConnectionManager(void);
    virtual ~ConnectionManager(void) = default;

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status Connect(const char *aMaybeURL, const Common::Timeout &aTimeout);
    Common::Status Connect(const char *aMaybeURL, const Versions &aVersions, const Common::Timeout &aTimeout);
    Common::Status Disconnect(void);

    Common::Status AddDelegate(ConnectionManagerDelegate *aDelegate);
    Common::Status RemoveDelegate(ConnectionManagerDelegate *aDelegate);

    bool IsConnected(void) const;

    Common::Status Send(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer);

    // Connection Basis Delegate Methods

    // Connect

    void ConnectionWillConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionIsConnecting(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionDidConnect(ConnectionBasis &aConnection, CFURLRef aURLRef) final;
    void ConnectionDidNotConnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) final;

    // Application Data

    void ConnectionDidReceiveApplicationData(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer aBuffer) final;

    // Disconnect

    void ConnectionWillDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef) final;
    void ConnectionDidDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionDidNotDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error

    void ConnectionError(ConnectionBasis &aConnection, const Common::Error &aError) final;

private:
    void OnWillResolve(const char *aHost) final;
    void OnIsResolving(const char *aHost) final;
    void OnDidResolve(const char *aHost, const Common::IPAddress &aIPAddress) final;
    void OnDidNotResolve(const char *aHost, const Common::Error &aError) final;

    Common::Status Connect(CFURLRef aURLRef, const Common::Timeout &aTimeout);

private:
    typedef std::unordered_set<ConnectionManagerDelegate *> ConnectionManagerDelegates;

    ConnectionFactory                           mConnectionFactory;
    ConnectionBasis *                           mConnection;
    ConnectionManagerDelegates                  mDelegates;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONNECTIONMANAGER_HPP
