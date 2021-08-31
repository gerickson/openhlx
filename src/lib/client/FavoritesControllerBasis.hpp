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

#ifndef OPENHLXCLIENTFAVORITESCONTROLLERBASIS_HPP
#define OPENHLXCLIENTFAVORITESCONTROLLERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/FavoritesControllerCommands.hpp>
#include <OpenHLX/Client/ObjectControllerBasis.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>
#include <OpenHLX/Model/FavoritesModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup client
 *  @ingroup favorite
 *
 */
class FavoritesControllerBasis :
    public Client::ObjectControllerBasis
{
public:
    virtual ~FavoritesControllerBasis(void);

protected:
    FavoritesControllerBasis(Model::FavoritesModel &aFavoritesModel,
                             const Model::FavoriteModel::IdentifierType &aFavoritesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const Model::FavoriteModel::IdentifierType &aFavoriteIdentifier);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

protected:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status ResponseInit(void);

private:
    Model::FavoritesModel &                      mFavoritesModel;
    const Model::FavoriteModel::IdentifierType & mFavoritesMax;
    size_t                                       mFavoritesDidRefreshCount;

protected:
    static Command::Favorites::NameResponse  kNameResponse;
    static Command::Favorites::QueryResponse kQueryResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFAVORITESCONTROLLERBASIS_HPP
