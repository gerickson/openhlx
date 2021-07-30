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

#ifndef HLXSERVERCONNECTIONBASIS_HPP
#define HLXSERVERCONNECTIONBASIS_HPP

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
    virtual ~ConnectionBasis(void);

    typedef void (* OnResponseCompleteFunc)(Common::ConnectionBuffer::ImmutableCountedPointer aResponseBuffer, const Common::RegularExpression::Matches &aResponseMatches, void *aContext);
    typedef void (* OnResponseErrorFunc)(const Common::Error &aError, void *aContext);

public:
    virtual Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);
    virtual Common::Status Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress);
    virtual Common::Status Disconnect(void);

    Common::Status SetDelegate(ConnectionBasisDelegate *aDelegate);
    ConnectionBasisDelegate *GetDelegate(void) const;

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

    enum State
    {
        kState_Unknown         = 0,

        kState_Ready           = 1,

        kState_Listening       = 2,

        kState_Accepting       = 3,
        kState_Accepted        = 4,

        kState_Connected       = kState_Accepted,

        kState_Disconnecting   = 5,
        kState_Disconnected    = kState_Listening
    };

    bool IsState(State aState) const;
    State GetState(void) const;

    Common::Status SetState(State aState);

private:
    State                          mState;
    ConnectionBasisDelegate *      mDelegate;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONNECTIONBASIS_HPP