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
 *      This file defines an object for creating HLX server
 *      peer-to-peer network connections.
 *
 */

#ifndef HLXSERVERCONNECTIONFACTORY_HPP
#define HLXSERVERCONNECTIONFACTORY_HPP

#include <map>
#include <memory>

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/Errors.hpp>
#include <RunLoopParameters.hpp>

namespace HLX
{

namespace Server
{

class ConnectionBasis;

/**
 *  @brief
 *    An object for creating HLX server peer-to-peer network
 *    connections.
 *
 *  @ingroup server
 *
 */
class ConnectionFactory
{

public:
    ConnectionFactory(void);
    ~ConnectionFactory(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    bool SupportsScheme(CFStringRef aSchemeRef) const;
    std::unique_ptr<ConnectionBasis> CreateConnection(CFStringRef aSchemeRef) const;

private:
    typedef std::map<CFStringRef, ConnectionBasis *> Connections;

    Common::RunLoopParameters  mRunLoopParameters;
    Connections                mConnections;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONNECTIONFACTORY_HPP
