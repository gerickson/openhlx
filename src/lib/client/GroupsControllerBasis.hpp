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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP
#define OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/ControllerBasis.hpp>
#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup group
 *
 */
class GroupsControllerBasis :
    public Client::ControllerBasis
{
public:
    virtual ~GroupsControllerBasis(void);

protected:
    GroupsControllerBasis(Model::GroupsModel &aGroupsModel,
                          const Model::GroupModel::IdentifierType &aGroupsMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const Model::GroupModel::IdentifierType &aGroupIdentifier);

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
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status ResponseInit(void);
    
private:
    //!< Ensure the base class can access the Handle*Change methods
    //!< for cross zone-to-group and group-to-zone state synthesis.

    friend class Controller;

    // Parent Controller Group / Zone Interaction Handlers

    void HandleMuteChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute);
    void HandleMuteChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute);

    void HandleSourceChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleSourceChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier);

    void HandleSourcesChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel::Sources &aSourceIdentifierCollection);
    void HandleSourcesChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::GroupModel::Sources &aSourceIdentifierCollection);

    void HandleUnmuteChange(const Model::GroupModel::IdentifierType &aGroupIdentifier);
    void HandleUnmuteChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel);

    void HandleVolumeChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume);
    void HandleVolumeChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume);
    void HandleVolumeChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute);
    void HandleVolumeChange(const Model::GroupModel::IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute);

private:
    Model::GroupsModel &                            mGroupsModel;
    const Model::GroupModel::IdentifierType &       mGroupsMax;
    size_t                                          mGroupsDidRefreshCount;

protected:
    static Command::Groups::SetMuteResponse         kSetMuteResponse;
    static Command::Groups::NameResponse            kNameResponse;
    static Command::Groups::QueryResponse           kQueryResponse;
    static Command::Groups::SourceResponse          kSourceResponse;
    static Command::Groups::SetVolumeResponse       kSetVolumeResponse;
    static Command::Groups::ZoneResponse            kZoneResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP
