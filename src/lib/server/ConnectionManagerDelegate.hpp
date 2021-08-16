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
 *      This file defines a delegate interface for the server
 *      connection manager object.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP
#define OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP

#include <OpenHLX/Common/ConnectionManagerDelegateBasis.hpp>


namespace HLX
{

namespace Server
{

class ConnectionManager;

/**
 *  @brief
 *    A delegate interface for the server connection manager object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the pending and stable state of a server
 *  connection as it moves through its lifetime.
 *
 *  @ingroup server
 *
 */
class ConnectionManagerDelegate :
    public Common::ConnectionManagerDelegateBasis
{
public:
    ConnectionManagerDelegate(void) = default;
    ~ConnectionManagerDelegate(void) = default;

    // Listen

    virtual void ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Accept

    virtual void ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) = 0;
    virtual void ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) = 0;

};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONMANAGERDELEGATE_HPP
