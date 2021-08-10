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

#ifndef OPENHLXSIMULATORGROUPSCONTROLLER_HPP
#define OPENHLXSIMULATORGROUPSCONTROLLER_HPP

#include <OpenHLX/Common/GroupsControllerBasis.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>
#include <OpenHLX/Server/GroupsControllerBasis.hpp>
#include <OpenHLX/Server/GroupsControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
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
    public Server::ContainerControllerBasis,
    public Common::GroupsControllerBasis,
    public Server::GroupsControllerBasis
{
public:
    GroupsController(void);
    virtual ~GroupsController(void);

    Common::Status Init(Server::CommandManager &aCommandManager) final;

    // Delegate Management

    Common::Status SetDelegate(GroupsControllerDelegate *aDelegate);
    GroupsControllerDelegate *GetDelegate(void) const;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
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

    void AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Delegate Fanout Methods

    Common::Status OnAdjustVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment);
    Common::Status OnSetMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute);
    Common::Status OnSetSource(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status OnSetVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume);
    Common::Status OnToggleMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel);

private:
    GroupsControllerDelegate *                     mDelegate;

};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORGROUPSCONTROLLER_HPP
