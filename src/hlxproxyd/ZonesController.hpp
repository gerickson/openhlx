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

#ifndef OPENHLXPROXYZONESCONTROLLER_HPP
#define OPENHLXPROXYZONESCONTROLLER_HPP

#include <stddef.h>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ZonesControllerBasis.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/ZonesControllerBasis.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>
#include <OpenHLX/Server/ZonesControllerBasis.hpp>
#include <OpenHLX/Server/ZonesControllerCommands.hpp>

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
 *  @ingroup zone
 *
 */
class ZonesController :
    public Proxy::ControllerBasis,
    public Common::ZonesControllerBasis,
    public Client::ZonesControllerBasis,
    public Server::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    Common::Status Init(Client::CommandManager &aCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) final;

    // Server-facing Client Observer Methods

    Common::Status Query(void);
    Common::Status Query(const IdentifierType &aZoneIdentifier);
    Common::Status QueryMute(const IdentifierType &aZoneIdentifier);
    Common::Status QuerySource(const IdentifierType &aZoneIdentifier);
    Common::Status QueryVolume(const IdentifierType &aZoneIdentifier);

    Common::Status GetZonesMax(size_t &aZones) const;

    Common::Status GetZone(const IdentifierType &aIdentifier, const Model::ZoneModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aZoneIdentifier) const;

    // Server-facing Client Mutator Methods

    Common::Status SetVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status IncreaseVolume(const IdentifierType &aZoneIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aZoneIdentifier);


    // Server-facing Client Command Completion Handler Trampolines

    static void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Server-facing Client Notification Handler Trampolines

    static void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

    // Client-facing Server Command Request Handler Trampolines

    static void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status DoRequestHandlers(const bool &aRegister);

    // Server-facing Client Command Completion Handlers

    void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Server-facing Client Notification Handlers

    void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Command Request Completion Handlers

    void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Server-facing Client Implementation

    void HandleMuteChange(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    void HandleSourceChange(const IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleVolumeChange(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);

private:
    // Client-facing Server Implementation

    Common::Status AdjustVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Model::VolumeModel::LevelType &aVolume);
    Common::Status SetMute(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);

    Common::Status HandleQueryReceived(const bool &aIsConfiguration, const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer) const;

    Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMute(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMuteConditionally(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleMuteResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
};

}; // namespace Proxy

}; // namespace HLX

#endif // HLXPROXYZONESCONTROLLER_HPP
