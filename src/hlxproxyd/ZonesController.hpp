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

    static void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

    // Client-facing Server Command Request Handler Trampolines

    static void AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

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
    void SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Server-facing Client Notification Handlers

    void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Client-facing Server Command Request Completion Handlers

    void AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

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

    Common::Status HandleQueryMuteReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQueryMuteReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleQuerySoundMode(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryEqualizerPreset(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryHighpassCrossover(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryLowpassCrossover(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryTone(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryZoneEqualizer(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleQuerySourceReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQuerySourceReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    static Common::Status HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleQueryVolumeFixed(const IdentifierType &aZoneIdentifier, const Model::ZoneModel &aZoneModel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustBalanceReceived(const IdentifierType &aZoneIdentifier, const Model::BalanceModel::ChannelType &aChannel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetBalanceReceived(const IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustBassReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment);
    Common::Status HandleAdjustBassReceived(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustTrebleReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment);
    Common::Status HandleAdjustTrebleReceived(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleSetMute(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMuteConditionally(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetMuteUnconditionally(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleSetSoundMode(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetSoundModeConditionally(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetSoundModeUnconditionally(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetVolumeReceived(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustEqualizerBandReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment);
    Common::Status HandleAdjustEqualizerBandReceived(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetEqualizerBandReceived(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleEqualizerBandResponse(const IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleEqualizerPresetResponse(const IdentifierType &aZoneIdentifier, const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleHighpassCrossoverResponse(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleLowpassCrossoverResponse(const IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleMuteResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleSoundModeResponse(const IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleToneResponse(const IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, Model::ToneModel::LevelType &aTreble, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleVolumeFixedResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aVolumeFixed, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYZONESCONTROLLER_HPP
