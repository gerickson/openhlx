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
 *      This file defines an object for a HLX server peer-to-peer
 *      network connection.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONBASIS_HPP
#define OPENHLXSERVERCONNECTIONBASIS_HPP

#include <stddef.h>

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionBasis.hpp>
#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HostURLAddress.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Server
{

class ConnectionBasisDelegate;

/**
 *  @brief
 *    An object for a HLX server peer-to-peer network connection.
 *
 *  @ingroup server
 *
 */
class ConnectionBasis :
    public Common::ConnectionBasis
{
public:
    /**
     *  A type for identifying a connection, uniquely when combined
     *  with its protocol scheme (for example, "telnet").
     *
     */
    typedef size_t IdentifierType;

public:
    virtual ~ConnectionBasis(void);

    virtual Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters, const IdentifierType &aIdentifier);
    virtual Common::Status Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress);
    virtual Common::Status Disconnect(void);

    IdentifierType GetIdentifier(void) const;

    Common::Status SetDelegate(ConnectionBasisDelegate *aDelegate);
    ConnectionBasisDelegate *GetDelegate(void) const;

    /**
     *  @brief
     *    Send the specified data to the connection peer.
     *
     *  @param[in]  aBuffer  An immutable shared pointer to the data to
     *                       send to the connection peer.
     *
     *  @retval  kStatus_Success  If successful.
     *
     */
    virtual Common::Status Send(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) = 0;

protected:
    ConnectionBasis(CFStringRef aSchemeRef);

    void OnWillAccept(void);
    void OnIsAccepting(void);
    void OnDidAccept(void);
    void OnDidNotAccept(const Common::Error &aError);

    void OnApplicationDataReceived(Common::ConnectionBuffer::MutableCountedPointer aBuffer);

    void OnWillDisconnect(void);
    void OnDidDisconnect(const Common::Error &aError);
    void OnDidNotDisconnect(const Common::Error &aError);

    void OnError(const Common::Error &aError);

    /**
     *  @brief
     *    Enumeration of connection states.
     *
     */
    enum State
    {
        kState_Unknown       = 0,

        kState_Ready         = 1,

        kState_Listening     = 2,

        kState_Accepting     = 3,
        kState_Accepted      = 4,

        kState_Connected     = kState_Accepted,

        kState_Disconnecting = 5,
        kState_Disconnected  = kState_Listening
    };

    bool IsState(State aState) const;
    State GetState(void) const;

    Common::Status SetState(State aState);

private:
    IdentifierType             mIdentifier;
    State                      mState;
    ConnectionBasisDelegate *  mDelegate;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONBASIS_HPP
