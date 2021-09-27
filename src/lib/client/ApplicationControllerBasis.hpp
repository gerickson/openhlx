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
 *      This file defines a derivable object for creating HLX client
 *      application controllers.
 *
 */

#ifndef OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP
#define OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP


#include <map>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ConnectionManager.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Client/ObjectControllerBasis.hpp>
#include <OpenHLX/Client/ObjectControllerBasisRefreshDelegate.hpp>
#include <OpenHLX/Client/ObjectControllerBasisStateChangeDelegate.hpp>
#include <OpenHLX/Common/ApplicationObjectControllerContainerTemplate.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

class Controller;
class GroupsControllerBasis;
class ZonesControllerBasis;

namespace Application
{

class Controller;
class ControllerRefreshDelegate;
class ControllerStateChangeDelegate;

/**
 *  @brief
 *    An object for effecting an HLX client controller for any HLX
 *    client application, whether a command line utility or a mobile
 *    app.
 *
 *  @ingroup client
 *
 */
class ControllerBasis :
    public Common::Application::ObjectControllerContainerTemplate<Client::ObjectControllerBasis>,
    public ObjectControllerBasisRefreshDelegate,
    public ObjectControllerBasisStateChangeDelegate
{
public:
    virtual ~ControllerBasis(void);

    // Initializer(s)

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    // Accessors

    const Client::CommandManager &     GetCommandManager(void) const;
    Client::CommandManager &           GetCommandManager(void);
    const Client::ConnectionManager &  GetConnectionManager(void) const;
    Client::ConnectionManager &        GetConnectionManager(void);

    Client::Application::ControllerRefreshDelegate *GetRefreshDelegate(void) const;
    Client::Application::ControllerStateChangeDelegate *GetStateChangeDelegate(void) const;

    Common::Status SetRefreshDelegate(Client::Application::ControllerRefreshDelegate *aRefreshDelegate);
    Common::Status SetStateChangeDelegate(Client::Application::ControllerStateChangeDelegate *aStateChangeDelegate);

    // Connection Management

    Common::Status Connect(const char *aMaybeURL);
    Common::Status Connect(const char *aMaybeURL, const Common::Timeout &aTimeout);
    Common::Status Connect(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions);
    Common::Status Connect(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions, const Common::Timeout &aTimeout);

    bool           IsConnected(void) const;

    Common::Status Disconnect(void);

    // Refresh

    Common::Status Refresh(void);

    // Object Controller Delegate Methods

    // Object Controller Refresh Delegate Methods

    void ControllerIsRefreshing(Client::ObjectControllerBasis &aController, const uint8_t &aPercentComplete) final;
    void ControllerDidRefresh(Client::ObjectControllerBasis &aController) final;

    // Object Controller State Change Delegate Method

    void ControllerStateDidChange(Client::ObjectControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification) final;

protected:
    /**
     *  A convenience type for an object that manages a container of
     *  client object controllers.
     *
     */
    typedef Common::Application::ObjectControllerContainerTemplate<Client::ObjectControllerBasis> ClientObjectControllerContainer;

protected:
    ControllerBasis(GroupsControllerBasis &aGroupsControllerBasis,
                    ZonesControllerBasis &aZonesControllerBasis);

    bool IsRefreshing(void) const;

private:
    // Group State Derivation Methods

    void DeriveGroupState(void);
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
    Client::ConnectionManager                             mConnectionManager;
    Client::CommandManager                                mCommandManager;
    size_t                                                mControllersDidRefreshCount;
    Client::Application::ControllerRefreshDelegate *      mRefreshDelegate;
    Client::Application::ControllerStateChangeDelegate *  mStateChangeDelegate;
    GroupsControllerBasis &                               mGroupsControllerBasis;
    ZonesControllerBasis &                                mZonesControllerBasis;
    bool                                                  mIsDerivingGroupState;
};

}; // namespace Application

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTAPPLICATIONCONTROLLERBASIS_HPP
