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
 *      This file defines a base object for....
 *
 */

#ifndef OPENHLXPROXYCONTROLLERBASIS_HPP
#define OPENHLXPROXYCONTROLLERBASIS_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/CommandManager.hpp>


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    A base object for....
 *
 *  @ingroup proxy
 *
 */
class ControllerBasis
{
public:
    virtual ~ControllerBasis(void);

    // Intializer(s)

    virtual Common::Status Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout);

    // Configuration Management Methods

    virtual Common::Status QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status ProxyObservationCommand(Server::ConnectionBasis &aClientConnection,
                                           const uint8_t *aRequestBuffer,
                                           const size_t &aRequestSize,
                                           const Common::RegularExpression::Matches &aMatches,
                                           const Client::Command::ResponseBasis &aExpectedResponse,
                                           Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                           Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                           Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                           void *aClientContext,
                                           void *aServerContext);
    Common::Status ProxyMutationCommand(Server::ConnectionBasis &aClientConnection,
                                        const uint8_t *aRequestBuffer,
                                        const size_t &aRequestSize,
                                        const Common::RegularExpression::Matches &aMatches,
                                        const Client::Command::ResponseBasis &aExpectedResponse,
                                        Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                        Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                        void *aClientContext);

protected:
    ControllerBasis(void);

private:
    // Proxy Handlers

    void ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                           const Common::Error &aClientError,
                           Server::ConnectionBasis &aClientConnection,
                           Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                           void * aContext);
    void ProxyObservationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                         const Common::RegularExpression::Matches &aClientMatches,
                                         Server::ConnectionBasis &aClientConnection,
                                         const uint8_t *aRequestBuffer,
                                         const size_t &aRequestSize,
                                         const Common::RegularExpression::Matches &aServerMatches,
                                         Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                         Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                         void * aClientContext,
                                         void * aServerContext);
    void ProxyMutationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                      const Common::RegularExpression::Matches &aClientMatches,
                                      Server::ConnectionBasis &aClientConnection,
                                      const uint8_t *aRequestBuffer,
                                      const size_t &aRequestSize,
                                      const Common::RegularExpression::Matches &aServerMatches,
                                      Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                      void * aContext);

public:
    // Proxy Handler Trampolines

    static void ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                  const Common::Error &aClientError,
                                  void *aContext);
    static void ProxyObservationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                                const Common::RegularExpression::Matches &aClientMatches,
                                                void *aContext);
    static void ProxyMutationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                             const Common::RegularExpression::Matches &aClientMatches,
                                             void *aContext);

private:
    Client::CommandManager  * mClientCommandManager;
    Server::CommandManager  * mServerCommandManager;
    Common::Timeout           mTimeout;
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLERBASIS_HPP
