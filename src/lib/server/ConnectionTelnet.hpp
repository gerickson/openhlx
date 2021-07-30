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
 *      network connection that uses the telnet protocol.
 *
 */

#ifndef HLXSERVERCONNECTIONTELNET_HPP
#define HLXSERVERCONNECTIONTELNET_HPP

#include <sys/socket.h>

#include <CoreFoundation/CFStream.h>
#include <CoreFoundation/CFURL.h>

#include <libtelnet.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/ConnectionBasis.hpp>


namespace HLX
{

namespace Common
{

    class ConnectionBuffer;

}; // namespace Common

namespace Server
{

/**
 *  @brief
 *    An object for a HLX server peer-to-peer network connection that.
 *    uses the telnet protocol.
 *
 *  @ingroup server
 *
 */
class ConnectionTelnet :
    public Server::ConnectionBasis
{
public:
    static CFStringRef kScheme;

public:
    ConnectionTelnet(void);
    virtual ~ConnectionTelnet(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters) final;

    Common::Status Connect(const int &aSocket, const Common::SocketAddress &aPeerAddress) final;
    Common::Status Disconnect(void) final;

    Common::Status Send(Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) final;

    static void CFReadStreamCallback(CFReadStreamRef aStream, CFStreamEventType aType, void *aContext);
    static void CFWriteStreamCallback(CFWriteStreamRef aStream, CFStreamEventType aType, void *aContext);
    static void TelnetEventHandler(telnet_t *aTelnet, telnet_event_t *aEvent, void *aContext);

private:
    Common::Status CloseStreams(void);

    Common::Status Put(Common::ConnectionBuffer &aBuffer, const uint8_t *aData, const size_t &aSize);

    void CFReadStreamCallback(CFReadStreamRef aStream, CFStreamEventType aType);
    void CFWriteStreamCallback(CFWriteStreamRef aStream, CFStreamEventType aType);

    void TryServerConfirmationDataReceived(void);
    void DidReceiveDataHandler(const uint8_t *aBuffer, const size_t &aSize);
    void ShouldTransmitDataHandler(const uint8_t *aBuffer, const size_t &aSize);
    void TelnetEventHandler(telnet_t *aTelnet, telnet_event_t *aEvent);
    void HandleStreamError(const CFStreamEventType &aType, const CFStreamError &aStreamError, const char *aStreamDescription);

private:
    telnet_t *                                       mTelnet;
    CFSocketNativeHandle                             mConnectedSocket;
    CFReadStreamRef                                  mReadStreamRef;
    CFWriteStreamRef                                 mWriteStreamRef;
    bool                                             mReadStreamReady;
    bool                                             mWriteStreamReady;
    Common::ConnectionBuffer::MutableCountedPointer  mReceiveBuffer;
    bool                                             mWaitingForServerConfirmation;
    Common::RegularExpression                        mServerConfirmationRegexp;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONNECTIONTELNET_HPP
