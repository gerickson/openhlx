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
 *      observation and mutation of a collection of HLX favorites.
 *
 */

#ifndef OPENHLXSIMULATORFAVORITESCONTROLLER_HPP
#define OPENHLXSIMULATORFAVORITESCONTROLLER_HPP

#include <OpenHLX/Common/FavoritesControllerBasis.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>
#include <OpenHLX/Model/FavoritesModel.hpp>
#include <OpenHLX/Server/FavoritesControllerBasis.hpp>
#include <OpenHLX/Server/FavoritesControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ObjectControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a collection of HLX favorites.
 *
 *  @ingroup server
 *  @ingroup favorite
 *
 */
class FavoritesController :
    public Common::FavoritesControllerBasis,
    public Server::FavoritesControllerBasis,
    public Simulator::ContainerControllerBasis,
    public Simulator::ObjectControllerBasis
{
public:
    FavoritesController(void);
    virtual ~FavoritesController(void);

    // Initializer(s)

    Common::Status Init(Server::CommandManager &aCommandManager) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aFavoritesDictionary, const IdentifierType &aFavoriteIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aFavoritesDictionary, const IdentifierType &aFavoriteIdentifier) const final;

    // Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORFAVORITESCONTROLLER_HPP
