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

#ifndef OPENHLXSERVERLISTENERBASIS_HPP
#define OPENHLXSERVERLISTENERBASIS_HPP

#include <vector>

#include <CoreFoundation/CFSocket.h>
#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HostURLAddress.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>


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

    /**
     *  @brief
     *    This is a class initializer.
     *
     *  This initializes the connection listener basis on a run loop
     *  with the specified run loop parameters.
     *
     *  @param[in]  aRunLoopParameters  An immutable reference to the run
     *                                  loop parameters to initialize the
     *                                  connection listener basis with.
     *
     *  @retval  kStatus_Success  If successful.
     *
     */
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

    /**
     *  @brief
     *    Enumeration of listenerx states.
     *
     */
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

#endif // OPENHLXSERVERLISTENERBASIS_HPP
