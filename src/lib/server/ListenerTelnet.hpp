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
 *      This file defines a derived object for a HLX server network
 *      connection listener that uses the telnet protocol.
 *
 */

#ifndef HLXSERVERLISTENERTELNET_HPP
#define HLXSERVERLISTENERTELNET_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ListenerBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A derived object for a HLX server network connection listener
 *    that uses the telnet protocol.
 *
 *  @ingroup server
 *
 */
class ListenerTelnet :
    public ListenerBasis
{
public:
    static CFStringRef kScheme;

public:
    ListenerTelnet(void);
    virtual ~ListenerTelnet(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters) final;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERLISTENERTELNET_HPP
