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
 *      This file defines a derivable object for realizing a HLX
 *      Ethernet network interface controller, in a client.
 *
 */

#ifndef OPENHLXCLIENTNETWORKCONTROLLERBASIS_HPP
#define OPENHLXCLIENTNETWORKCONTROLLERBASIS_HPP

#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Client/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    A derivable object for realizing a HLX Ethernet network
 *    interface controller, in a client.
 *
 *  @ingroup client
 *  @ingroup network
 *
 */
class NetworkControllerBasis :
    public Client::ObjectControllerBasis
{
public:
    virtual ~NetworkControllerBasis(void);

protected:
    NetworkControllerBasis(Model::NetworkModel &aNetworkModel);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IPDefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IPHostAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IPNetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

protected:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IPDefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IPHostAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IPNetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status ResponseInit(void);

private:
    Model::NetworkModel & mNetworkModel;

protected:
    static Command::Network::DHCPv4EnabledResponse           kDHCPv4EnabledResponse;
    static Command::Network::EthernetEUI48Response           kEthernetEUI48Response;
    static Command::Network::IPDefaultRouterAddressResponse  kIPDefaultRouterAddressResponse;
    static Command::Network::IPHostAddressResponse           kIPHostAddressResponse;
    static Command::Network::IPNetmaskResponse               kIPNetmaskResponse;
    static Command::Network::QueryResponse                   kQueryResponse;
    static Command::Network::SDDPEnabledResponse             kSDDPEnabledResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTNETWORKCONTROLLERBASIS_HPP
