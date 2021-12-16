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

#ifndef OPENHLXSIMULATORNETWORKCONTROLLER_HPP
#define OPENHLXSIMULATORNETWORKCONTROLLER_HPP

#include <OpenHLX/Common/NetworkControllerBasis.hpp>
#include <OpenHLX/Server/NetworkControllerBasis.hpp>
#include <OpenHLX/Server/NetworkControllerCommands.hpp>

#include "ObjectControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a HLX Ethernet network interface.
 *
 *  @ingroup server
 *  @ingroup network
 *
 */
class NetworkController :
    public Common::NetworkControllerBasis,
    public Server::NetworkControllerBasis,
    public Simulator::ObjectControllerBasis
{
public:
    NetworkController(void);
    virtual ~NetworkController(void);

    // Initializer(s)

    Common::Status Init(Server::CommandManager &aCommandManager) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetDHCPv4EnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSDDPEnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status IPv4LoadFromBackupConfiguration(CFDictionaryRef aIPv4Dictionary, Model::NetworkModel &aNetworkModel);
    Common::Status DHCPv4LoadFromBackupConfiguration(CFDictionaryRef aNetworkDictionary, Model::NetworkModel &aNetworkModel);
    Common::Status SDDPLoadFromBackupConfiguration(CFDictionaryRef aNetworkDictionary, Model::NetworkModel &aNetworkModel);

    static Common::Status IPv4SaveToBackupConfiguration(CFMutableDictionaryRef aIPv4Dictionary, const Model::NetworkModel &aNetworkModel);
    static Common::Status DHCPv4SaveToBackupConfiguration(CFMutableDictionaryRef aNetworkDictionary, const Model::NetworkModel &aNetworkModel);
    static Common::Status SDDPSaveToBackupConfiguration(CFMutableDictionaryRef aNetworkDictionary, const Model::NetworkModel &aNetworkModel);

    // Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetDHCPv4EnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSDDPEnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORNETWORKCONTROLLER_HPP
