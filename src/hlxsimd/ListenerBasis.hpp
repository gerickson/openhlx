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
 *      This file defines a base, derivable object for HLX server
 *      network connection listeners.
 *
 */

#ifndef HLXSERVERLISTENERBASIS_HPP
#define HLXSERVERLISTENERBASIS_HPP

#include <vector>

#include <regex.h>

#include <ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <HostURLAddress.hpp>
#include <RegularExpression.hpp>
#include <RunLoopParameters.hpp>
#include <SocketAddress.hpp>
#include <Timeout.hpp>

#include <CoreFoundation/CFSocket.h>
#include <CoreFoundation/CFURL.h>

namespace HLX
{

namespace Server
{

class ListenerBasisDelegate;
class ListenerBasisAcceptDelegate;

/**
 *  @brief
 *    A base, derivable object for HLX server network connection
 *    listeners.
 *
 *  @ingroup server
 *
 */
class ListenerBasis
{

public:
    virtual ~ListenerBasis(void);

    virtual Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters) = 0;
    virtual Common::Status Listen(ListenerBasisAcceptDelegate *aAcceptDelegate, const Common::SocketAddress &aAddress);

    virtual CFStringRef GetScheme(void) const;
    virtual const Common::HostURLAddress &GetAddress(void) const;

    Common::Status SetDelegate(ListenerBasisDelegate *aDelegate);
    ListenerBasisDelegate *GetDelegate(void) const;

    static void CFSocketAcceptCallback(CFSocketRef aSocketRef,
                                       CFSocketCallBackType aSocketCallBackType,
                                       CFDataRef aAddress,
                                       const void *aData,
                                       void *aInfo);

protected:
    ListenerBasis(CFStringRef aScheme);

    Common::Status Init(in_port_t aDefaultPort, const Common::RunLoopParameters &aRunLoopParameters);

    void OnWillListen(void);
    void OnIsListening(void);
    void OnDidListen(void);
    void OnDidNotListen(const Common::Error &aError);

    void OnWillAccept(void);
    void OnIsAccepting(void);
    void OnDidAccept(void);
    void OnDidNotAccept(const Common::Error &aError);

    void OnError(const Common::Error &aError);

    enum State
    {
        kState_Unknown          = 0,

        kState_IsReady          = 1,

        kState_WillBeListening  = 2,
        kState_IsListening      = 3,

        kState_Accepting        = 4
    };

    bool IsState(State aState) const;
    State GetState(void) const;

    Common::Status SetState(State aState);

    Common::RunLoopParameters &GetRunLoopParameters(void);

private:
    Common::Status Listen(const Common::SocketAddress &aAddress);
    void Ignore(void);

    static void Ignore(const Common::RunLoopParameters &aRunLoopParameters, CFSocketRef &aSocketRef, CFRunLoopSourceRef &aRunLoopSourceRef);

    void CFSocketAcceptCallback(CFSocketRef aSocketRef,
                                CFSocketCallBackType aSocketCallBackType,
                                CFDataRef aAddress,
                                const void *aData);

private:
    CFStringRef                    mSchemeRef;
    in_port_t                      mDefaultPort;
    Common::RunLoopParameters      mRunLoopParameters;
    State                          mState;
    ListenerBasisDelegate *        mDelegate;
    ListenerBasisAcceptDelegate *  mAcceptDelegate;
    CFSocketRef                    mSocketRef;
    CFRunLoopSourceRef             mRunLoopSourceRef;
    Common::HostURLAddress         mHostURLAddress;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERLISTENERBASIS_HPP
