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
 *      This file defines a run loop aware base object for managing a
 *      connection bewteen two IP-based peers.
 *
 */

#ifndef OPENHLXCOMMONCONNECTIONBASIS_HPP
#define OPENHLXCOMMONCONNECTIONBASIS_HPP

#include <CoreFoundation/CFString.h>

#include <OpenHLX/Common/HostURLAddress.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A run loop aware base object for managing a connection bewteen
 *    two IP-based peers.
 *
 *  @ingroup common
 *
 */
class ConnectionBasis
{
public:
    virtual ~ConnectionBasis(void);

    Status Init(const RunLoopParameters &aRunLoopParameters);

    CFStringRef GetScheme(void) const;

    const HostURLAddress & GetPeerAddress(void) const;

    RunLoopParameters &GetRunLoopParameters(void);

    Status SetPeerAddress(const HostURLAddress &aPeerAddress);

protected:
    ConnectionBasis(CFStringRef aSchemeRef);

private:
    CFStringRef                    mSchemeRef;
    RunLoopParameters              mRunLoopParameters;
    HostURLAddress                 mPeerAddress;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCONNECTIONBASIS_HPP
