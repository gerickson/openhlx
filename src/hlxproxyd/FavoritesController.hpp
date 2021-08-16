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

#ifndef OPENHLXPROXYFAVORITESCONTROLLER_HPP
#define OPENHLXPROXYFAVORITESCONTROLLER_HPP

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/FavoritesControllerBasis.hpp>
#include <OpenHLX/Client/FavoritesControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/FavoritesControllerBasis.hpp>
#include <OpenHLX/Server/FavoritesControllerBasis.hpp>
#include <OpenHLX/Server/FavoritesControllerCommands.hpp>

#include "ControllerBasis.hpp"


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
 *  @ingroup favorites
 *
 */
class FavoritesController :
    public Proxy::ControllerBasis,
    public Common::FavoritesControllerBasis,
    public Client::FavoritesControllerBasis,
    public Server::FavoritesControllerBasis
{
public:
    FavoritesController(void);
    virtual ~FavoritesController(void);

    Common::Status Init(Client::CommandManager &aCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const IdentifierType &aFavoriteIdentifier);

    // Server-facing Client Command Completion Handler Trampolines

    static void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Server-facing Client Notification Handler Trampolines

    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

    // Client-facing Server Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    // Proxy Handlers

public:
    // Proxy Handler Trampolines

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status DoRequestHandlers(const bool &aRegister);

    // Server-facing Client Command Completion Handlers

    void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Server-facing Client Notification Handlers

    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Server-facing Client Implementation

private:
    // Client-facing Server Implementation

    Common::Status HandleQueryReceived(const IdentifierType &aFavoriteIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYFAVORITESCONTROLLER_HPP
