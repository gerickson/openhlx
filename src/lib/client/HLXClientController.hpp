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
 *      This file defines an object for effecting an HLX client
 *      controller for any HLX client application, whether a command
 *      line utility or a mobile app.
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLER_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLER_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/CommandManagerDelegate.hpp>
#include <OpenHLX/Client/ConfigurationController.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/ConnectionManagerDelegate.hpp>
#include <OpenHLX/Client/ObjectControllerBasisDelegate.hpp>
#include <OpenHLX/Client/EqualizerPresetsController.hpp>
#include <OpenHLX/Client/FavoritesController.hpp>
#include <OpenHLX/Client/FrontPanelController.hpp>
#include <OpenHLX/Client/GroupsController.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Client/HLXClientControllerBasis.hpp>
#include <OpenHLX/Client/HLXClientControllerDelegate.hpp>
#include <OpenHLX/Client/InfraredController.hpp>
#include <OpenHLX/Client/NetworkController.hpp>
#include <OpenHLX/Client/SourcesController.hpp>
#include <OpenHLX/Client/ZonesController.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HLXCommonControllerBasis.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>

namespace HLX
{

namespace Common
{

    class RunLoopParameters;

}; // namespace Common

namespace Client
{

namespace Application
{

/**
 *  @brief
 *    An object for effecting an HLX client controller for any HLX
 *    client application, whether a command line utility or a mobile
 *    app.
 *
 *  @ingroup client
 *
 */
class Controller :
    public Common::Application::ControllerBasis,
    public Client::Application::ControllerBasis,
    public ConnectionManagerDelegate,
    public CommandManagerDelegate,
    public ObjectControllerBasisErrorDelegate,
    public ObjectControllerBasisStateChangeDelegate
{
public:
    Controller(void);
    virtual ~Controller(void);

    // Initializer(s)

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    ControllerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(ControllerDelegate *aDelegate);

    // Equalizer Preset Commands

    Common::Status EqualizerPresetsGetMax(Model::EqualizerPresetModel::IdentifierType &aEqualizerPresets) const;

    Common::Status EqualizerPresetGet(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerPresetModel *&aModel) const;

    Common::Status EqualizerPresetLookupIdentifier(const char *aName, Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier) const;
    Common::Status EqualizerPresetSetName(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const char *aName);

    Common::Status EqualizerPresetDecreaseBand(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status EqualizerPresetIncreaseBand(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status EqualizerPresetSetBand(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);

    // Group Commands

    Common::Status GroupsGetMax(Model::GroupModel::IdentifierType &aGroups) const;

    Common::Status GroupGet(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel *&aModel) const;

    Common::Status GroupLookupIdentifier(const char *aName, Model::GroupModel::IdentifierType &aGroupIdentifier) const;
    Common::Status GroupSetName(const Model::GroupModel::IdentifierType &aGroupIdentifier, const char *aName);

    Common::Status GroupClearMute(const Model::GroupModel::IdentifierType &aGroupIdentifier);
    Common::Status GroupSetMute(const Model::GroupModel::IdentifierType &aGroupIdentifier);
    Common::Status GroupSetMute(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status GroupToggleMute(const Model::GroupModel::IdentifierType &aGroupIdentifier);

    Common::Status GroupSetSource(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

    Common::Status GroupSetVolume(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status GroupIncreaseVolume(const Model::GroupModel::IdentifierType &aGroupIdentifier);
    Common::Status GroupDecreaseVolume(const Model::GroupModel::IdentifierType &aGroupIdentifier);

    Common::Status GroupAddZone(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status GroupRemoveZone(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    // Source Commands

    Common::Status SourcesGetMax(Model::SourceModel::IdentifierType &aSources) const;

    Common::Status SourceGet(const Model::SourceModel::IdentifierType &aSourceIdentifier, const Model::SourceModel *&aModel) const;

    Common::Status SourceLookupIdentifier(const char *aName, Model::SourceModel::IdentifierType &aSourceIdentifier) const;

    Common::Status SourceSetName(const Model::SourceModel::IdentifierType &aSourceIdentifier, const char *aName);

    // Zone Commands

    Common::Status ZonesGetMax(Model::ZoneModel::IdentifierType &aZones) const;

    Common::Status ZoneQuery(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    Common::Status ZoneGet(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ZoneModel *&aModel) const;

    Common::Status ZoneLookupIdentifier(const char *aName, Model::ZoneModel::IdentifierType &aZoneIdentifier) const;
    Common::Status ZoneSetName(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char *aName);

    Common::Status ZoneSetBalance(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance);
    Common::Status ZoneIncreaseBalanceLeft(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneIncreaseBalanceRight(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    Common::Status ZoneDecreaseEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status ZoneIncreaseEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status ZoneSetEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);

    Common::Status ZoneSetEqualizerPreset(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    Common::Status ZoneDecreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneDecreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneIncreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneIncreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneSetBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass);
    Common::Status ZoneSetTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aTreble);

    Common::Status ZoneSetHighpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);
    Common::Status ZoneSetLowpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);

    Common::Status ZoneClearMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneSetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneSetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status ZoneToggleMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    Common::Status ZoneSetSoundMode(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode);

    Common::Status ZoneSetSource(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

    Common::Status ZoneSetVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status ZoneIncreaseVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ZoneDecreaseVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    Common::Status ZoneSetVolumeLocked(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);

    // Command Manager Delegate Methods

    // Connection Manager Delegate Methods

    // Resolve

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError) final;

    // Connect

    void ConnectionManagerWillConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerIsConnecting(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Timeout &aTimeout) final;
    void ConnectionManagerDidConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Disconnect

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError) final;

    // Object Controller Basis Delegate Methods

    void ControllerError(Client::ObjectControllerBasis &aController, const Common::Error &aError) final;
    void ControllerStateDidChange(Client::ObjectControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification) final;

private:
    void DeriveGroupState(void) final;
    void DeriveGroupStateForGroupsIncludingZone(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    void DeriveGroupStateForGroupIncludingZone(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                               const Model::GroupModel &aGroupModel,
                                               const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    void DeriveGroupStateForGroup(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                  const Model::GroupModel &aGroupModel);

    void MaybeHandleGroupZoneStateChangeInteractions(Client::ObjectControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification);
    void HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType);

    struct DerivedGroupState
    {
    public:
        DerivedGroupState(void);
        ~DerivedGroupState(void);

        Common::Status Init(void);

        const Model::GroupModel::Sources &GetSources(void) const;
        Model::VolumeModel::LevelType GetVolume(void) const;

        Common::Status AddSource(const Model::SourceModel::IdentifierType &aSourceIdentifier);
        void UpdateVolume(const Model::VolumeModel::LevelType &aVolume);

        size_t                                mZoneCount;
        Model::VolumeModel::MuteType          mGroupMute;
        mutable Model::VolumeModel::LevelType mGroupVolume;
        int16_t                               mGroupVolumeAccumulator;
        Model::GroupModel::Sources            mGroupSources;
    };

    void HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType, const Model::GroupModel &aGroupModel, DerivedGroupState &aDerivedGroupState);
    void HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType, DerivedGroupState &aDerivedGroupState, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    ConfigurationController         mConfigurationController;
    EqualizerPresetsController      mEqualizerPresetsController;
    FavoritesController             mFavoritesController;
    FrontPanelController            mFrontPanelController;
    GroupsController                mGroupsController;
    InfraredController              mInfraredController;
    NetworkController               mNetworkController;
    SourcesController               mSourcesController;
    ZonesController                 mZonesController;
    ControllerDelegate *            mDelegate;
    bool                            mIsDerivingGroupState;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLER_HPP
