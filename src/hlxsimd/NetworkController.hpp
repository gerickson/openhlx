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
 *      This file defines an object for managing the server-side
 *      observation and mutation of a HLX Ethernet network interface.
 *
 */

#ifndef HLXSERVERNETWORKCONTROLLER_HPP
#define HLXSERVERNETWORKCONTROLLER_HPP

#include <ControllerBasis.hpp>
#include <NetworkControllerCommands.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>

namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a HLX Ethernet network interface.
 *
 *  @ingroup client
 *  @ingroup network
 *
 */
class NetworkController :
    public ControllerBasis
{
public:
    NetworkController(void);
    ~NetworkController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    void QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    static void QueryHandler(const char *aInputBuffer, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer);

    // Command Completion Handlers

    void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::NetworkModel                   mNetworkModel;

private:
    static Command::Network::QueryRequest  kQueryRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERNETWORKCONTROLLER_HPP
