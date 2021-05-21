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
 *      This file implements an object for managing a HLX group.
 *
 */

#include "GroupModel.hpp"

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The maximum allowed length, in bytes, of a group name.
 *
 */
const size_t GroupModel::kNameLengthMax = NameModel::kNameLengthMax;

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name and identifier.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *  @sa SetName
 *
 */
Status
GroupModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the group name with.
 *  @param[in]  aIdentifier  An immutable reference for the group
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
GroupModel :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent and
 *  identifier.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *  @param[in]  aIdentifier  An immutable reference for the group
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
GroupModel :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName, aNameLength);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           group name with.
 *  @param[in]  aIdentifier  An immutable reference for the group
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
GroupModel :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified group model.
 *
 *  @param[in]  aGroupModel  An immutable reference to the
 *                           group model to initialize with.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
GroupModel :: Init(const GroupModel &aGroupModel)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aGroupModel.mIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aGroupModel.mName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mSources.Init(aGroupModel.mSources);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init(aGroupModel.mVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init(aGroupModel.mZones);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified group model to this one.
 *
 *  @param[in]  aGroupModel  An immutable reference to the
 *                           group model to assign (copy)
 *                           to this one.
 *
 *
 *  @returns
 *    A reference to this group model after the assignment (copy)
 *    is complete.
 *
 */
GroupModel &
GroupModel :: operator =(const GroupModel &aGroupModel)
{
    mIdentifier = aGroupModel.mIdentifier;
    mName       = aGroupModel.mName;
    mSources    = aGroupModel.mSources;
    mVolume     = aGroupModel.mVolume;
    mZones      = aGroupModel.mZones;

    return (*this);
}

/**
 *  @brief
 *    Determine whether or not the specified zone identifier is a
 *    member of the group.
 *
 *  This determines whether or not the specified zone identifier is a
 *  member of the group.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the
 *                               zone identifer to check for group
 *                               membership.
 *
 *  @returns
 *    True if the zone identifier is a member of the group; otherwise,
 *    false.
 *
 *  @sa AddZone
 *  @sa ClearZones
 *  @sa RemoveZone
 *
 */
bool
GroupModel :: ContainsZone(const ZoneModel::IdentifierType &aZoneIdentifier) const
{
    return (mZones.ContainsIdentifier(aZoneIdentifier));
}

/**
 *  @brief
 *    Attempt to get the group identifier.
 *
 *  This attempts to get the group identifier, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            group identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group identifier
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
GroupModel :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.GetIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the group name
 *
 *  This attempts to get the group name, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the group
 *                      name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group name value has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetName
 *
 *  @ingroup name
 *
 */
Status
GroupModel :: GetName(const char *&aName) const
{
    Status lRetval = kStatus_Success;

    lRetval = mName.GetName(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model group volume mute state.
 *
 *  This attempts to get the model group volume mute state, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aMute  A mutable reference to storage for the
 *                      group volume mute state, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa SetMute
 *  @sa ToggleMute
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: GetMute(MuteType &aMute) const
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.GetMute(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the number of source (input) identifiers associated with
 *    this group.
 *
 *  @note
 *    This may be useful to help dynamically-size caller-allocated
 *    storage for the actual source (input) identifiers.
 *
 *  @param[out]  aCount  The number of source (input) identifiers
 *                       associated with this group, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group sources (inputs)
 *                                  have not been initialized with a
 *                                  known value(s).
 *
 */
Status
GroupModel :: GetSources(size_t &aCount) const
{
    return (GetIdentifiers(mSources, aCount));
}

/**
 *  @brief
 *    Get the source (input) identifiers associated with this group
 *    model.
 *
 *  @param[in,out]  aSourceIdentifiers      An iterator to a mutable
 *                                          linear array (that is a
 *                                          standard C-style array or
 *                                          adapter thereto) for
 *                                          source (input) identifiers
 *                                          associated with the group
 *                                          whose size is represented
 *                                          by @a aCount. On success,
 *                                          this will contain the
 *                                          minmum of @a aCount and
 *                                          the actual number of
 *                                          sources (inputs)
 *                                          identifiers associated
 *                                          with the group.
 *  @param[in,out]  aCount                  A mutable reference to
 *                                          the number of elements @a
 *                                          aSourceIdentifiers can
 *                                          support. On success, this
 *                                          will contain the minmum of
 *                                          @a aCount and the actual
 *                                          number of sources (inputs)
 *                                          associated with the group.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group sources (inputs)
 *                                  have not been initialized with a
 *                                  known value(s).
 *
 */
Status
GroupModel :: GetSources(SourceModel::IdentifierType *aSourceIdentifiers, size_t &aCount) const
{
    return (GetIdentifiers(mSources, aSourceIdentifiers, aCount));
}

/**
 *  @brief
 *    Get the source (input) identifiers associated with this group
 *    model.
 *
 *  @param[in,out]  aSourceIdentifiers      A mutable reference to a
 *                                          container collection for
 *                                          source (input) identifiers
 *                                          associated with the group.
 *                                          On success, source (input)
 *                                          identifiers associated
 *                                          with the group.
 *
 *  @retval  kStatus_Success        Unconditionally.
 *
 */
Status
GroupModel :: GetSources(Sources &aSourceIdentifiers) const
{
    Status lRetval = kStatus_Success;

    aSourceIdentifiers = mSources;

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model group volume level.
 *
 *  This attempts to get the model group volume level, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aLevel  A mutable reference to storage for the
 *                       group volume level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: GetVolume(LevelType &aLevel) const
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.GetVolume(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the number of zone identifiers associated with this group model.
 *
 *  @note
 *    This may be useful to help dynamically-size caller-allocated
 *    storage for the actual zone identifiers.
 *
 *  @param[out]  aCount  The number of zone identifiers
 *                       associated with this group, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group zones have not
 *                                  been initialized with a known
 *                                  value(s).
 *
 */
Status
GroupModel :: GetZones(size_t &aCount) const
{
    return (GetIdentifiers(mZones, aCount));
}

/**
 *  @brief
 *    Get the zone identifiers associated with this group model.
 *
 *  @param[in,out]  aZoneIdentifiers      An iterator to a mutable
 *                                        linear array (that is a
 *                                        standard C-style array or
 *                                        adapter thereto) for zone
 *                                        identifiers associated with
 *                                        the group whose size is
 *                                        represented by @a aCount. On
 *                                        success, this will contain
 *                                        the minmum of @a aCount and
 *                                        the actual number of zone
 *                                        identifiers associated with
 *                                        the group.
 *  @param[in,out]  aCount                A mutable reference to
 *                                        the number of elements @a
 *                                        aSourceIdentifiers can
 *                                        support. On success, this
 *                                        will contain the minmum of
 *                                        @a aCount and the actual
 *                                        number of zones associated
 *                                        with the group.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group zones have not been
 *                                  initialized with a known value(s).
 *
 */
Status
GroupModel :: GetZones(ZoneModel::IdentifierType *aZoneIdentifiers, size_t &aCount) const
{
    return (GetIdentifiers(mZones, aZoneIdentifiers, aCount));
}

/**
 *  @brief
 *    Attempt to associate a source (input) identifier with the group
 *    model.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the source
 *                                 (input) identifier to associate
 *                                 with the group model.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the source (input) identifier is
 *                                    already associated with the group.
 *  @retval  kError_NotInitialized    If the group sources (inputs) have
 *                                    not been initialized with a
 *                                    known value(s).
 *
 */
Status
GroupModel :: AddSource(const SourceModel::IdentifierType &aSourceIdentifier)
{
    return (AddIdentifier(mSources, aSourceIdentifier));
}

/**
 *  @brief
 *    Attempt to associate a zone identifier with the group model.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the
 *                               zone identifier to associate
 *                               with the group model.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the zone identifier is already
 *                                    associated with the group.
 *  @retval  kError_NotInitialized    If the group zones have not been
 *                                    initialized with a known
 *                                    value(s).
 *
 */
Status
GroupModel :: AddZone(const ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (AddIdentifier(mZones, aZoneIdentifier));
}

/**
 *  @brief
 *    Clear (remove) all zone identifiers associated with the group
 *    model.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group zones have not been
 *                                  initialized with a known value(s).
 *
 */
Status
GroupModel :: ClearZones(void)
{
    return (mZones.ClearIdentifiers());
}

/**
 *  @brief
 *    Decrease the model group volume level by one (1) unit.
 *
 *  This attempts to decrease the model group volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting group volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  minimum group volume level.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: DecreaseVolume(LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.DecreaseVolume(aOutLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the model group volume level by one (1) unit.
 *
 *  This attempts to increase the model group volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting group volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  maximum group volume level.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: IncreaseVolume(LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.IncreaseVolume(aOutLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to remove (disassociate) a source (input) identifier
 *    from the group model.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) identifier to remove
 *                                 (disassociate) from the group
 *                                 model.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kError_NotInitialized    If the group sources (inputs) have
 *                                    not been initialized with a
 *                                    known value(s).
 *  @retval  -ENOENT                  The source (input) identifier is
 *                                    not associated with the group
 *                                    model.
 *
 */
Status
GroupModel :: RemoveSource(const SourceModel::IdentifierType &aSourceIdentifier)
{
    return (RemoveIdentifier(mSources, aSourceIdentifier));
}

/**
 *  @brief
 *    Attempt to remove (disassociate) a zone identifier from the
 *    group model.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the
 *                               zone identifier to remove
 *                               (disassociate) from the group model.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group zones have not been
 *                                  initialized with a known value(s).
 *  @retval  -ENOENT                The zone identifier is not
 *                                  associated with the group model.
 *
 */
Status
GroupModel :: RemoveZone(const ZoneModel::IdentifierType &aZoneIdentifier)
{
    return (RemoveIdentifier(mZones, aZoneIdentifier));
}

/**
 *  @brief
 *    This sets the model group identifier.
 *
 *  This attempts to set the model with the group identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference to the group
 *                           identifier to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aIdentifier value
 *                                    has already been set.
 *  @retval  -EINVAL                  The specified @a aIdentifier value
 *                                    is invalid.
 *
 */
Status
GroupModel :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.SetIdentifier(aIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model group name.
 *
 *  This attempts to set the model with the specified group name.
 *
 *  @param[in]  aName        A pointer to the start of the null-
 *                           terminated C string name to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aName was too long.
 *
 *  @ingroup name
 *
 */
Status
GroupModel :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model group name.
 *
 *  This attempts to set the model with the specified group name
 *  extent.
 *
 *  @param[in]  aName        A pointer to the start of the string name
 *                           to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aNameLength was too long.
 *
 *  @ingroup name
 *
 */
Status
GroupModel :: SetName(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName, aNameLength);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model group volume mute state.
 *
 *  This attempts to set the model with the specified group volume
 *  mute state.
 *
 *  @param[in]  aMute  An immutable reference to the group
 *                     volume mute state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aMute value
 *                                    has already been set.
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: SetMute(const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.SetMute(aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate a single source (input) identifier with the
 *    group model, removing any other source (input) associations.
 *
 *  This attempts to associate the specified, single source (input)
 *  identifier with the group model, removing any other source (input)
 *  associations, if successful.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the source
 *                                 (input) identifier to associate
 *                                 with the group model.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the source (input) identifier
 *                                    and @b only that source (input)
 *                                    identifier is already associated
 *                                    with the group. If the requested
 *                                    source (input) are one of many
 *                                    already associated with the
 *                                    group, then the return status
 *                                    will be kStatus_Success, if
 *                                    successful.
 *  @retval  kError_NotInitialized    If the group sources (inputs) have
 *                                    not been initialized with a
 *                                    known value(s).
 *
 *  @sa AddSource
 *
 */
Status
GroupModel :: SetSource(const IdentifierType &aSourceIdentifier)
{
    const size_t  lCount = 1;

    return (mSources.SetIdentifiers(&aSourceIdentifier, lCount));
}

/**
 *  @brief
 *    Attempt to associate multiple source (input) identifiers with
 *    the group model, removing any other source (input) associations.
 *
 *  This attempts to associate the specified multiple source (input)
 *  identifiers with the group model, removing any other source (input)
 *  associations, if successful.
 *
 *  @param[in]  aSourceIdentifiers  An iterator to an immutable
 *                                  linear array (that is a standard
 *                                  C-style array or adapter thereto)
 *                                  of source (input) identifiers to
 *                                  associate with the group model
 *                                  whose size is represented by @a
 *                                  aCount.
 *  @param[in]  aCount              A immutable reference to the
 *                                  number of source (input)
 *                                  identifiers in @a
 *                                  aSourceIdentifiers.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the source (input) identifiers
 *                                    and @b only those source (input)
 *                                    identifiers are already
 *                                    associated with the group. If
 *                                    the requested sources (inputs)
 *                                    are some of many already
 *                                    associated with the group, then
 *                                    the return status will be
 *                                    kStatus_Success, if successful.
 *  @retval  kError_NotInitialized    If the group sources (inputs) have
 *                                    not been initialized with a
 *                                    known value(s).
 *
 *  @sa AddSource
 *
 */
Status
GroupModel :: SetSources(const SourceModel::IdentifierType *aSourceIdentifiers, const size_t &aCount)
{
    return (mSources.SetIdentifiers(aSourceIdentifiers, aCount));
}

/**
 *  @brief
 *    Attempt to associate multiple source (input) identifiers with
 *    the group model, removing any other source (input) associations.
 *
 *  This attempts to associate the specified multiple source (input)
 *  identifiers with the group model, removing any other source (input)
 *  associations, if successful.
 *
 *  @param[in]  aSourceIdentifiers  An immutable reference to a
 *                                  container collection of source
 *                                  (input) identifiers to associate
 *                                  with the group.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the source (input) identifiers
 *                                    and @b only those source (input)
 *                                    identifiers are already
 *                                    associated with the group. If
 *                                    the requested sources (inputs)
 *                                    are some of many already
 *                                    associated with the group, then
 *                                    the return status will be
 *                                    kStatus_Success, if successful.
 *  @retval  kError_NotInitialized    If the group sources (inputs) have
 *                                    not been initialized with a
 *                                    known value(s).
 *
 *  @sa AddSource
 *
 */
Status
GroupModel :: SetSources(const Sources &aSourceIdentifiers)
{
    return (mSources.SetIdentifiers(aSourceIdentifiers));
}

/**
 *  @brief
 *    This sets the model group volume level.
 *
 *  This attempts to set the model with the specified group volume
 *  level.
 *
 *  @param[in]  aLevel  An immutable reference to the group volume
 *                      level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLevel value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aLevel value
 *                                    is out of range.
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: SetVolume(const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.SetVolume(aLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to toggle (flip) the model group volume mute state.
 *
 *  This attempts to toggle (flip) the model group volume mute state.
 *
 *  @param[out]  aOutMute  A mutable reference to storage for the
 *                         resulting group volume mute state, if
 *                         successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the group volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @ingroup volume
 *
 */
Status
GroupModel :: ToggleMute(MuteType &aOutMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.ToggleMute(aOutMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided group model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aGroupModel  An immutable reference to the group
 *                           model to compare for equality.
 *
 *  @returns
 *    True if this group model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
GroupModel :: operator ==(const GroupModel &aGroupModel) const
{
    return ((mIdentifier       == aGroupModel.mIdentifier) &&
            (mName             == aGroupModel.mName      ) &&
            (mSources          == aGroupModel.mSources   ) &&
            (mVolume           == aGroupModel.mVolume    ) &&
            (mZones            == aGroupModel.mZones     ));
}

/**
 *  @brief
 *    Get the number of identifiers associated with the specified
 *    identifiers collection.
 *
 *  @note
 *    This may be useful to help dynamically-size caller-allocated
 *    storage for the actual identifiers.
 *
 *  @param[in]   aCollection  An immutable reference to the identifiers
 *                            collection for which to get an
 *                            identifier count.
 *  @param[out]  aCount       The number of identifiers associated
 *                            with the collection, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *
 */
Status
GroupModel :: GetIdentifiers(const IdentifiersCollection &aCollection,
                             size_t &aCount)
{
    return (aCollection.GetCount(aCount));
}

/**
 *  @brief
 *    Get the identifiers from the specified collection.
 *
 *  @param[in]      aCollection   An immutable reference to the
 *                                identifiers collection from which to
 *                                get the identifiers.
 *  @param[in,out]  aIdentifiers  An iterator to a mutable linear
 *                                array (that is a standard C-style
 *                                array or adapter thereto) for
 *                                identifiers in the collection whose
 *                                size is represented by @a aCount. On
 *                                success, this will contain the
 *                                minmum of @a aCount and the actual
 *                                number of identifiers in the
 *                                collection.
 *  @param[in,out]  aCount        A mutable reference to the number
 *                                of elements @a aIdentifiers can
 *                                support. On success, this will
 *                                contain the minmum of @a aCount and
 *                                the actual number of identifiers in
 *                                the collection.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *
 */
Status
GroupModel :: GetIdentifiers(const IdentifiersCollection &aCollection,
                             IdentifiersCollection::IdentifierType *aIdentifiers,
                             size_t &aCount)
{
    return (aCollection.GetIdentifiers(aIdentifiers, aCount));
}

/**
 *  @brief
 *    Attempt to associate an identifier with the specified
 *    identifiers collection.
 *
 *  @param[in,out]   aCollection  A mutable reference to the
 *                                identifiers collection to associate
 *                                the identifier with.
 *  @param[in]       aIdentifier  An immutable reference to the
 *                                identifier to associate with the
 *                                collection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifier is already
 *                                    associated with the collection.
 *  @retval  kError_NotInitialized    If the identifiers have not
 *                                    been initialized with a known
 *                                    value(s).
 *
 */
Status
GroupModel :: AddIdentifier(IdentifiersCollection &aCollection,
                            const IdentifiersCollection::IdentifierType &aIdentifier)
{
    return (aCollection.AddIdentifier(aIdentifier));
}

/**
 *  @brief
 *    Attempt to remove (disassociate) an identifier from the
 *    specified identifiers collection.
 *
 *  @param[in,out]   aCollection  A mutable reference to the
 *                                identifiers collection to remove
 *                                (disassociate) the identifier from.
 *  @param[in]       aIdentifier  An immutable reference to the
 *                                identifier to remove (disassociate)
 *                                from the collection.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *  @retval  -ENOENT                The identifier is not
 *                                  associated with the collection.
 *
 */
Status
GroupModel :: RemoveIdentifier(IdentifiersCollection &aCollection,
                               const IdentifiersCollection::IdentifierType &aIdentifier)
{
    return (aCollection.RemoveIdentifier(aIdentifier));
}

}; // namespace Model

}; // namespace HLX
