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
 *      This file implements derived objects for a HLX client group object
 *      data model state change notifications (SCNs).
 *
 */

#include "GroupsStateChangeNotifications.hpp"

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group state change notification with the
 *  specified type and group identifier.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose state
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsNotificationBasis :: Init(const Type &aType,
                                const IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(aType);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = IdentifierNotificationBasis::Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group volume mute property state change
 *  notification with the specified volume mute state and group
 *  identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose volume
 *                                          mute property state
 *                                          changed.
 *  @param[in]  aMute                       An immutable reference to
 *                                          the volume mute state that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsMuteNotification :: Init(const IdentifierType &aGroupIdentifier, const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupMute, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = MuteNotificationBasis::Init(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group name property state change notification
 *  with the specified name extent and group identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose name
 *                                          property state changed.
 *  @param[in]  aName                       A pointer to the start of
 *                                          the null- terminated C
 *                                          string name that changed.
 *  @param[in]  aNameLength                 An immutable reference to
 *                                          the length, in bytes, of
 *                                          @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsNameNotification :: Init(const IdentifierType &aGroupIdentifier,
                               const char *aName,
                               const size_t &aNameLength)
{
    std::string lName;

    lName.assign(aName, aNameLength);

    return (Init(aGroupIdentifier, lName));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group name property state change notification
 *  with the specified name and group identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose name
 *                                          property state changed.
 *  @param[in]  aName                       An immutable reference to
 *                                          the name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsNameNotification :: Init(const IdentifierType &aGroupIdentifier,
                               const std::string &aName)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupName, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = NameNotificationBasis::Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group sources (inputs) property state change
 *  notification with the specified source (input) identifiers and
 *  count and group identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose sources
 *                                          property state changed.
 *  @param[in]  aSourceIdentifiers          An iterator to an immutable
 *                                          collection of source
 *                                          identifiers that changed.
 *  @param[in]  aCount                      An immutable reference to
 *                                          the number of source
 *                                          identifiers in @a
 *                                          aSourceIdentifiers.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsSourceNotification :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                 const Model::SourceModel::IdentifierType *aSourceIdentifiers,
                                 const size_t &aCount)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupSource, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init(aSourceIdentifiers, aCount);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group sources (inputs) property state change
 *  notification with the specified source identifier and group
 *  identifier.
 *
 *  @note
 *    This may be used when the group sources goes from many sources
 *    to one source.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose sources
 *                                          property state changed.
 *  @param[in]  aSourceIdentifier           An immutable reference to
 *                                          the source identifier that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsSourceNotification :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                 const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    static const size_t lCount = 1;

    return (Init(aGroupIdentifier, &aSourceIdentifier, lCount));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group sources (inputs) property state change
 *  notification with the specified source (input) identifiers and
 *  group identifier.
 *
 *  @note
 *    This may be used when the group sources goes from one set of
 *    arbitrary many sources to another set of arbitrary many sources.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose sources
 *                                          property state changed.
 *  @param[in]  aSourceIdentifiers          An immutable reference to
 *                                          the collection of source
 *                                          identifiers that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsSourceNotification :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                 const Model::GroupModel::Sources &aSourceIdentifiers)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupSource, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mSources = aSourceIdentifiers;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the number of sources associated with the group that changed.
 *
 *  @returns
 *    The number of sources associated with the group.
 *
 */
size_t
GroupsSourceNotification :: GetCount(void) const
{
    Status lStatus;
    size_t lRetval = 0;

    lStatus = mSources.GetCount(lRetval);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the source identifiers associated with the group that changed.
 *
 *  @returns
 *    The source identifiers associated with the group.
 *
 */
const GroupModel::Sources &
GroupsSourceNotification :: GetSources(void) const
{
    return (mSources);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group volume level property state change
 *  notification with the specified volume level state and group
 *  identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose volume
 *                                          level property state
 *                                          changed.
 *  @param[in]  aLevel                      An immutable reference to
 *                                          the volume level state
 *                                          that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsVolumeNotification :: Init(const IdentifierType &aGroupIdentifier,
                                 const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupVolume, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = VolumeNotificationBasis::Init(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group volume level increase property state
 *  change notification with the specified group identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose volume
 *                                          level property was
 *                                          increased.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsIncreaseVolumeNotification :: Init(const IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupIncreaseVolume, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the group volume level decrease property state
 *  change notification with the specified group identifier.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose volume
 *                                          level property was
 *                                          decreased.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsDecreaseVolumeNotification :: Init(const IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(kStateChangeType_GroupDecreaseVolume, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
GroupsZoneNotificationBasis :: GroupsZoneNotificationBasis(void) :
    GroupsNotificationBasis(),
    mZone(IdentifierModel::kIdentifierInvalid)
{
    return;
}

/**
 *  @brief
 *    Return the zone identifier associated with that group that changed.
 *
 *  @returns
 *    The zone identifier.
 *
 */
GroupsZoneNotificationBasis::IdentifierType
GroupsZoneNotificationBasis :: GetZone(void) const
{
    return (mZone);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership state change
 *  notification with the specified type and group and zone
 *  identifiers.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose zone
 *                                          membership state changed.
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object that was added
 *                                          to or removed from the
 *                                          identified group.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsZoneNotificationBasis :: Init(const Type &aType,
                                    const IdentifierType &aGroupIdentifier,
                                    const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsNotificationBasis::Init(aType, aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mZone = aZoneIdentifier;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone added membership state change
 *  notification with the specified group and zone identifiers.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose zone
 *                                          membership state changed.
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object that was added
 *                                          to the identified group.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsZoneAddedNotification :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                    const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsZoneNotificationBasis::Init(kStateChangeType_GroupZoneAdded, aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone removed membership state change
 *  notification with the specified group and zone identifiers.
 *
 *  @param[in]  aGroupIdentifier            An immutable reference
 *                                          to the identifier of the
 *                                          group object whose zone
 *                                          membership state changed.
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object that was
 *                                          removed from the
 *                                          identified group.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsZoneRemovedNotification :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                      const GroupsZoneNotificationBasis::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = GroupsZoneNotificationBasis::Init(kStateChangeType_GroupZoneRemoved, aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
