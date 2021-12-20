/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file defines an object for....
 *
 */

#ifndef OPENHLXPROXYNETWORKCONTROLLER_HPP
#define OPENHLXPROXYNETWORKCONTROLLER_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/NetworkControllerBasis.hpp>
#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/NetworkControllerBasis.hpp>
#include <OpenHLX/Server/NetworkControllerBasis.hpp>
#include <OpenHLX/Server/NetworkControllerCommands.hpp>

#include "ObjectControllerBasis.hpp"


namespace HLX
{

namespace Server
{

class ConnectionBasis;

}; // namespace Server

namespace Proxy
{

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup proxy
 *  @ingroup network
 *
 */
class NetworkController :
    public Common::NetworkControllerBasis,
    public Client::NetworkControllerBasis,
    public Server::NetworkControllerBasis,
    public Proxy::ObjectControllerBasis
{
public:
    NetworkController(void);
    virtual ~NetworkController(void);

    // Initializer(s)

    Common::Status Init(Client::CommandManager &aCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) final;

    // Server-facing Client Notification Handler Trampolines

    static void DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EthernetIPv4AddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EthernetIPv4DefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EthernetIPv4NetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

    // Client-facing Server Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status DoRequestHandlers(const bool &aRegister);

    // Server-facing Client Notification Handlers

    void DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EthernetIPv4AddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EthernetIPv4DefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EthernetIPv4NetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Explicitly hide base class initializers

    using Client::NetworkControllerBasis::Init;
    using Server::NetworkControllerBasis::Init;
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYNETWORKCONTROLLER_HPP
