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
 *      This file defines a delegate interface for the server
 *      connection listener basis object.
 *
 */

#ifndef HLXSERVERLISTENERBASISACCEPTDELEGATE_HPP
#define HLXSERVERLISTENERBASISACCEPTDELEGATE_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/SocketAddress.hpp>


namespace HLX
{

namespace Server
{

class ConnectionManager;

/**
 *  @brief
 *    A delegate interface for the server connection listener basis
 *    object.
 *
 *  @ingroup server
 *
 */
class ListenerBasisAcceptDelegate
{
public:
    ListenerBasisAcceptDelegate(void) = default;
    ~ListenerBasisAcceptDelegate(void) = default;

    // Accept

    virtual Common::Status ListenerDidAccept(ListenerBasis &aListener, const int &aSocket, const Common::SocketAddress &aAddress) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERLISTENERBASISACCEPTDELEGATE_HPP
