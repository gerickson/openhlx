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
 *      This file defines an object for a HLX client peer-to-peer
 *      network connection.
 *
 */

#ifndef OPENHLXCLIENTCONNECTIONBASIS_HPP
#define OPENHLXCLIENTCONNECTIONBASIS_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionBasis.hpp>
#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Timeout.hpp>


namespace HLX
{

namespace Client
{

class ConnectionBasisDelegate;

/**
 *  @brief
 *    An object for a HLX client peer-to-peer network connection.
 *
 *  @ingroup client
 *
 */
class ConnectionBasis :
    public Common::ConnectionBasis
{
public:
    virtual ~ConnectionBasis(void);

public:
    virtual Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);
    virtual Common::Status Connect(CFURLRef aURLRef, const Common::Timeout &aTimeout);
    virtual Common::Status Disconnect(void);
    virtual Common::Status Disconnect(const Common::Error &aError);

    Common::Status SetDelegate(ConnectionBasisDelegate *aDelegate);
    ConnectionBasisDelegate *GetDelegate(void) const;

    bool IsConnected(void) const;
    bool IsConnecting(void) const;

    bool IsDisconnected(void) const;
    bool IsDisconnecting(void) const;

    CFURLRef GetURL(void) const;
    const Common::Timeout &GetTimeout(void) const;

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
    virtual Common::Status Send(Common::ConnectionBuffer::ImmutableCountedPointer &aBuffer) = 0;

protected:
    ConnectionBasis(CFStringRef aSchemeRef);

    void OnWillConnect(void);
    void OnIsConnecting(void);
    void OnDidConnect(void);
    void OnDidNotConnect(const Common::Error &aError);

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

        kState_Connecting    = 2,
        kState_Connected     = 3,

        kState_Disconnecting = 4,
        kState_Disconnected  = kState_Ready
    };

    bool IsState(State aState) const;
    State GetState(void) const;

    Common::Status SetState(State aState);

private:
    CFURLRef                   mURLRef;
    mutable Common::Timeout    mTimeout;
    State                      mState;
    ConnectionBasisDelegate *  mDelegate;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONNECTIONBASIS_HPP
