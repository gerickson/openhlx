/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a collection of HLX groups.
 *
 */

#ifndef HLXCLIENTGROUPSCONTROLLER_HPP
#define HLXCLIENTGROUPSCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/ControllerBasis.hpp>
#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Common/GroupsControllerBasis.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX groups.
 *
 *  @ingroup client
 *  @ingroup group
 *
 */
class GroupsController :
    public ControllerBasis,
    public Common::GroupsControllerBasis
{
public:
    GroupsController(void);
    ~GroupsController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const IdentifierType &aGroupIdentifier);

    Common::Status GetGroupsMax(size_t &aGroups) const;

    Common::Status GetGroup(const IdentifierType &aIdentifier, const Model::GroupModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aGroupIdentifier) const;

    // Mutator Methods

    Common::Status ClearMute(const IdentifierType &aGroupIdentifier);
    Common::Status SetMute(const IdentifierType &aGroupIdentifier);
    Common::Status SetMute(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status ToggleMute(const IdentifierType &aGroupIdentifier);

    Common::Status SetName(const IdentifierType &aGroupIdentifier, const char *aName);

    Common::Status SetSource(const IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

    Common::Status SetVolume(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume);
    Common::Status IncreaseVolume(const IdentifierType &aGroupIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aGroupIdentifier);

    Common::Status AddZone(const IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status RemoveZone(const IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToggleMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ChangeZoneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ZoneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status ResponseInit(void);
    Common::Status DoNotificationHandlers(bool aRegister);

    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void ToggleMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void DecreaseVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void ChangeZoneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ZoneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    void HandleUnmuteChange(const IdentifierType &aGroupIdentifier);
    void HandleUnmuteChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel);
    void HandleVolumeChange(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute);
    void HandleVolumeChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute);

private:
    friend class Controller;

    void HandleMuteChange(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute);
    void HandleVolumeChange(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume);
    void HandleSourceChange(const IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleSourcesChange(const IdentifierType &aGroupIdentifier, const Model::GroupModel::Sources &aSourceIdentifierCollection);

    void HandleMuteChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute);
    void HandleVolumeChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume);
    void HandleSourceChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleSourcesChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::GroupModel::Sources &aSourceIdentifierCollection);

private:
    size_t                                          mGroupsDidRefreshCount;
    Model::GroupsModel                              mGroups;

private:
    static Command::Groups::SetMuteResponse         kSetMuteResponse;
    static Command::Groups::NameResponse            kNameResponse;
    static Command::Groups::QueryResponse           kQueryResponse;
    static Command::Groups::SourceResponse          kSourceResponse;
    static Command::Groups::SetVolumeResponse       kSetVolumeResponse;
    static Command::Groups::ZoneResponse            kZoneResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTGROUPSCONTROLLER_HPP
