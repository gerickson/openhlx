/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines derived objects for a HLX client group object
 *      data model state change notifications (SCNs).
 *
 */

#ifndef OPENHLXCLIENTGROUPSSTATECHANGENOTIFICATIONS_HPP
#define OPENHLXCLIENTGROUPSSTATECHANGENOTIFICATIONS_HPP

#include <memory>

#include <OpenHLX/Client/IdentifierStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/NameStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/SourceStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Client/VolumeStateChangeNotificationBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A derivable object for a HLX client group object data model state
 *    change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsNotificationBasis :
    public NotificationBasis,
    public IdentifierNotificationBasis
{
public:
    virtual ~GroupsNotificationBasis(void) = default;

protected:
    GroupsNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group object volume mute data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class GroupsMuteNotification :
    public GroupsNotificationBasis,
    public MuteNotificationBasis
{
public:
    GroupsMuteNotification(void) = default;
    virtual ~GroupsMuteNotification(void) = default;

    Common::Status Init(const IdentifierType &aGroupIdentifier, const MuteType &aMute);
};

/**
 *  @brief
 *    An object for a HLX client group object name data model property
 *    state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsNameNotification :
    public GroupsNotificationBasis,
    public NameNotificationBasis
{
public:
    GroupsNameNotification(void) = default;
    virtual ~GroupsNameNotification(void) = default;

    Common::Status Init(const IdentifierType &aGroupIdentifier, const char *aName, const size_t &aNameLength);
    Common::Status Init(const IdentifierType &aGroupIdentifier, const std::string &aName);
};

/**
 *  @brief
 *    An object for a HLX client group object source data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsSourceNotification :
    public GroupsNotificationBasis
{
public:
    GroupsSourceNotification(void) = default;
    virtual ~GroupsSourceNotification(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType *aSourceIdentifiers, const size_t &aCount);
    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel::Sources &aSourceIdentifiers);

    size_t                       GetCount(void) const;
    const Model::GroupModel::Sources &  GetSources(void) const;

private:
    Model::GroupModel::Sources  mSources;
};

/**
 *  @brief
 *    An object for a HLX client group object volume level data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class GroupsVolumeNotification :
    public GroupsNotificationBasis,
    public VolumeNotificationBasis
{
public:
    GroupsVolumeNotification(void) = default;
    virtual ~GroupsVolumeNotification(void) = default;

    Common::Status Init(const IdentifierType &aGroupIdentifier, const LevelType &aLevel);
};

/**
 *  @brief
 *    An object for a HLX client group object volume level increase
 *    data model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class GroupsIncreaseVolumeNotification :
    public GroupsNotificationBasis
{
public:
    GroupsIncreaseVolumeNotification(void) = default;
    virtual ~GroupsIncreaseVolumeNotification(void) = default;

    Common::Status Init(const IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group object volume level decrease
 *    data model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class GroupsDecreaseVolumeNotification :
    public GroupsNotificationBasis
{
public:
    GroupsDecreaseVolumeNotification(void) = default;
    virtual ~GroupsDecreaseVolumeNotification(void) = default;

    Common::Status Init(const IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    A base object for a HLX client group data model zone membership
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsZoneNotificationBasis :
    public GroupsNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  zone model.
     *
     */
    typedef Model::ZoneModel::IdentifierType IdentifierType;

public:
    virtual ~GroupsZoneNotificationBasis(void) = default;

    IdentifierType GetZone(void) const;

protected:
    GroupsZoneNotificationBasis(void);

    Common::Status Init(const Type &aType, const IdentifierType &aGroupIdentifier, const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier);

private:
    IdentifierType mZone;
};

/**
 *  @brief
 *    A base object for a HLX client group data model zone membership
 *    addition property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsZoneAddedNotification :
    public GroupsZoneNotificationBasis
{
public:
    GroupsZoneAddedNotification(void) = default;
    virtual ~GroupsZoneAddedNotification(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    A base object for a HLX client group data model zone membership
 *    deletion property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup group
 *  @ingroup state-change
 *
 */
class GroupsZoneRemovedNotification :
    public GroupsZoneNotificationBasis
{
public:
    GroupsZoneRemovedNotification(void) = default;
    virtual ~GroupsZoneRemovedNotification(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier);
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTGROUPSSTATECHANGENOTIFICATIONS_HPP
