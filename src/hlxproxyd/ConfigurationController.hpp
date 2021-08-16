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

#ifndef OPENHLXPROXYCONFIGURATIONCONTROLLER_HPP
#define OPENHLXPROXYCONFIGURATIONCONTROLLER_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ConfigurationControllerBasis.hpp>
#include <OpenHLX/Client/ConfigurationControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/ConfigurationControllerBasis.hpp>
#include <OpenHLX/Server/ConfigurationControllerBasis.hpp>
#include <OpenHLX/Server/ConfigurationControllerCommands.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Server
{

class ConnectionBasis;

}; // namespace Server

namespace Proxy
{

class ConfigurationControllerDelegate;

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup proxy
 *  @ingroup configuration
 *
 */
class ConfigurationController :
    public Proxy::ControllerBasis,
    public Common::ConfigurationControllerBasis,
    public Client::ConfigurationControllerBasis,
    public Server::ConfigurationControllerBasis
{
public:
    ConfigurationController(void);
    virtual ~ConfigurationController(void);

    Common::Status Init(Client::CommandManager &aCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout) final;

    // Delegate Management

    Common::Status SetDelegate(ConfigurationControllerDelegate *aDelegate);
    ConfigurationControllerDelegate *GetDelegate(void) const;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status QueryCurrent(void);

    // Server-facing Client Command Completion Handler Trampolines

    static void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void LoadFromBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SaveToBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ResetToDefaultsCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Server-facing Client Notification Handler Trampolines

    static void SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

    // Client-facing Server Command Request Handler Trampolines

    static void LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    // Proxy Handlers

    void ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                           const Common::Error &aError,
                           Server::ConnectionBasis &aConnection,
                           Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                           void * aContext);
    void ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                              const Common::RegularExpression::Matches &aClientMatches,
                              Server::ConnectionBasis &aConnection,
                              const uint8_t *aBuffer,
                              const size_t &aSize,
                              const Common::RegularExpression::Matches &aServerMatches,
                              Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                              Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                              void * aContext);

public:
    // Proxy Handler Trampolines

    static void ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);
    static void ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status ProxyCommand(Server::ConnectionBasis &aConnection,
                                const uint8_t *aBuffer,
                                const size_t &aSize,
                                const Common::RegularExpression::Matches &aMatches,
                                const Client::Command::ResponseBasis &aResponse,
                                Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                void *aContext);

    // Server-facing Client Command Completion Handlers

    void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void LoadFromBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SaveToBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void ResetToDefaultsCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Server-facing Client Notification Handlers

    void SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Command Completion Handlers

    void LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Configuration Delegation Fanout Methods

    Common::Status OnQueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

private:
    ConfigurationControllerDelegate *                      mDelegate;
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONFIGURATIONCONTROLLER_HPP
