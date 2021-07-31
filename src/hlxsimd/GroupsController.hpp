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
 *      observation and mutation of a collection of HLX groups.
 *
 */

#ifndef HLXSERVERGROUPSCONTROLLER_HPP
#define HLXSERVERGROUPSCONTROLLER_HPP

#include <OpenHLX/Common/GroupsControllerBasis.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>
#include <OpenHLX/Server/GroupsControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ControllerBasis.hpp"


namespace HLX
{

namespace Server
{

class GroupsControllerDelegate;

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a collection of HLX groups.
 *
 *  @ingroup server
 *  @ingroup group
 *
 */
class GroupsController :
    public Simulator::ControllerBasis,
    public ContainerControllerBasis,
    public Common::GroupsControllerBasis
{
public:
    GroupsController(void);
    virtual ~GroupsController(void);

    Common::Status Init(CommandManager &aCommandManager) final;

    // Delegate Management

    Common::Status SetDelegate(GroupsControllerDelegate *aDelegate);
    GroupsControllerDelegate *GetDelegate(void) const;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void AddZoneRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ClearZonesRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void RemoveZoneRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToggleMuteRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status GroupZonesLoadFromBackupConfiguration(CFDictionaryRef aGroupDictionary, Model::GroupModel &aGroupModel);
    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aGroupsDictionary, const IdentifierType &aGroupIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aGroupsDictionary, const IdentifierType &aZoneIdentifier) const final;
    static Common::Status GroupZonesSaveToBackupConfiguration(CFMutableDictionaryRef aGroupDictionary, const Model::GroupModel &aGroupModel);

    Common::Status HandleQueryReceived(const IdentifierType &aGroupIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    Common::Status HandleSetMute(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    Common::Status HandleAdjustVolumeReceived(const uint8_t *aInputBuffer, const size_t &aInputSize, const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetVolumeReceived(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleAdjustVolumeResponse(const uint8_t *aInputBuffer, const size_t &aInputSize, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer);
    static Common::Status HandleSetMuteResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleSetVolumeResponse(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleToggleMuteResponse(const uint8_t *aInputBuffer, const size_t &aInputSize, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer);

    // Command Completion Handlers

    void AddZoneRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ClearZonesRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void RemoveZoneRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToggleMuteRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Delegate Fanout Methods

    Common::Status OnAdjustVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment);
    Common::Status OnSetMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute);
    Common::Status OnSetSource(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status OnSetVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume);
    Common::Status OnToggleMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel);

private:
    Model::GroupsModel                             mGroups;
    GroupsControllerDelegate *                     mDelegate;

private:
    static Command::Groups::AddZoneRequest         kAddZoneRequest;
    static Command::Groups::ClearZonesRequest      kClearZonesRequest;
    static Command::Groups::DecreaseVolumeRequest  kDecreaseVolumeRequest;
    static Command::Groups::IncreaseVolumeRequest  kIncreaseVolumeRequest;
    static Command::Groups::MuteRequest            kMuteRequest;
    static Command::Groups::QueryRequest           kQueryRequest;
    static Command::Groups::RemoveZoneRequest      kRemoveZoneRequest;
    static Command::Groups::SetNameRequest         kSetNameRequest;
    static Command::Groups::SetSourceRequest       kSetSourceRequest;
    static Command::Groups::SetVolumeRequest       kSetVolumeRequest;
    static Command::Groups::ToggleMuteRequest      kToggleMuteRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERGROUPSCONTROLLER_HPP
