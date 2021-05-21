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
 *      This file defines a delegate interface for the HLX server
 *      peer-to-peer network connection basis object.
 *
 */

#ifndef HLXSERVERCONNECTIONBASISDELEGATE_HPP
#define HLXSERVERCONNECTIONBASISDELEGATE_HPP

#include <ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <Timeout.hpp>

#include <CoreFoundation/CFURL.h>

namespace HLX
{

namespace Server
{

class ConnectionBasis;

/**
 *  @brief
 *    A delegate interface for the HLX server peer-to-peer network
 *    connection basis object.
 *
 *  @ingroup server
 *
 */
class ConnectionBasisDelegate
{
public:
    ConnectionBasisDelegate(void);
    ~ConnectionBasisDelegate(void);

    // Accept

    virtual void ConnectionWillAccept(ConnectionBasis &aConnection) = 0;
    virtual void ConnectionIsAccepting(ConnectionBasis &aConnection) = 0;
    virtual void ConnectionDidAccept(ConnectionBasis &aConnection) = 0;
    virtual void ConnectionDidNotAccept(ConnectionBasis &aConnection, const Common::Error &aError) = 0;

    // Application Data

    virtual void ConnectionDidReceiveApplicationData(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer aBuffer) = 0;

    // Disconnect

    virtual void ConnectionWillDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef) = 0;
    virtual void ConnectionDidDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) = 0;
    virtual void ConnectionDidNotDisconnect(ConnectionBasis &aConnection, CFURLRef aURLRef, const Common::Error &aError) = 0;

    // Error

    virtual void ConnectionError(ConnectionBasis &aConnection, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONNECTIONBASISDELEGATE_HPP
