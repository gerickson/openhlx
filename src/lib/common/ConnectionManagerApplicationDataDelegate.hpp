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
 *      This file defines a delegate interface for the client
 *      connection manager object specific to application data
 *      receipt.
 *
 */

#ifndef HLXCOMMONCONNECTIONMANAGERAPPLICATIONDATADELEGATE_HPP
#define HLXCOMMONCONNECTIONMANAGERAPPLICATIONDATADELEGATE_HPP

#include <OpenHLX/Common/ConnectionBuffer.hpp>

namespace HLX
{

namespace Common
{

class ConnectionBasis;
class ConnectionManagerBasis;

/**
 *  @brief
 *    A delegate interface for the connection manager object
 *    specific to application data receipt.
 *
 *  @ingroup common
 *
 */
class ConnectionManagerApplicationDataDelegate
{
public:
    ConnectionManagerApplicationDataDelegate(void) = default;
    ~ConnectionManagerApplicationDataDelegate(void) = default;

    // Application Data

    /**
     *  @brief
     *    Delegation from the connection manager that a connection has
     *    received application data.
     *
     *  @param[in]  aConnectionManager  A reference to the connection manager
     *                                  that issued the delegation.
     *  @param[in]  aConnection         A reference to the connection over
     *                                  which the application data was
     *                                  received.
     *  @param[in]  aBuffer             The buffer containing the received
     *                                  application data.
     *
     */
    virtual void ConnectionManagerDidReceiveApplicationData(ConnectionManagerBasis &aConnectionManager,
                                                            ConnectionBasis &aConnection,
                                                            Common::ConnectionBuffer::MutableCountedPointer aBuffer) = 0;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCONNECTIONMANAGERAPPLICATIONDATADELEGATE_HPP
