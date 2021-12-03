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
 *      This file implements a derivable object for creating HLX
 *      client application controllers.
 *
 */

#include "ApplicationControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/ApplicationControllerRefreshDelegate.hpp>
#include <OpenHLX/Client/ApplicationControllerStateChangeDelegate.hpp>
#include <OpenHLX/Client/GroupsControllerBasis.hpp>
#include <OpenHLX/Client/ZonesControllerBasis.hpp>
#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Common/GroupsControllerBasis.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Percentage.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

namespace Application
{

namespace Detail
{

/**
 *  Internal collection of source identifiers.
 *
 *  @private
 *
 */
typedef std::vector<SourceModel::IdentifierType> SourceIdentifiers;

/**
 *  Internal collection of zone identifiers.
 *
 *  @private
 *
 */
typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

}; // namespace Detail

// MARK: Derived Group State

ControllerBasis :: DerivedGroupState :: DerivedGroupState(void) :
    mZoneCount(0),
    mGroupMute(true),
    mGroupVolume(VolumeModel::kLevelMin),
    mGroupVolumeAccumulator(0),
    mGroupSources()
{
    return;
}

ControllerBasis :: DerivedGroupState :: ~DerivedGroupState(void)
{
    return;
}

Status
ControllerBasis :: DerivedGroupState :: Init(void)
{
    Status lRetval;

    mZoneCount              = 0;
    mGroupMute              = true;
    mGroupVolume            = VolumeModel::kLevelMin;
    mGroupVolumeAccumulator = 0;

    lRetval = mGroupSources.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

const GroupModel::Sources &
ControllerBasis :: DerivedGroupState :: GetSources(void) const
{
    return (mGroupSources);
}

VolumeModel::LevelType
ControllerBasis :: DerivedGroupState :: GetVolume(void) const
{
    if (mZoneCount > 0)
    {
        mGroupVolume = static_cast<VolumeModel::LevelType>((mGroupVolumeAccumulator / static_cast<uint8_t>(mZoneCount)));
    }

    return (mGroupVolume);
}

Status
ControllerBasis :: DerivedGroupState :: AddSource(const SourceModel::IdentifierType &aIdentifier)
{
    return (mGroupSources.AddIdentifier(aIdentifier));
}

void
ControllerBasis :: DerivedGroupState :: UpdateVolume(const VolumeModel::LevelType &aVolume)
{
    mGroupVolumeAccumulator += aVolume;
}

// MARK: Client Controller Basis

/**
 *  @brief
 *    This is a class constructor.
 *
 *  @param[in]  aGroupsControllerBasis  A reference to the client groups
 *                                      object controller basis.
 *  @param[in]  aZonesControllerBasis   A reference to the client zones
 *                                      object controller basis.
 *
 */
ControllerBasis :: ControllerBasis(GroupsControllerBasis &aGroupsControllerBasis,
                                   ZonesControllerBasis &aZonesControllerBasis) :
    ClientObjectControllerContainer(),
    ObjectControllerBasisRefreshDelegate(),
    ObjectControllerBasisStateChangeDelegate(),
    mConnectionManager(),
    mCommandManager(),
    mControllersDidRefreshCount(0),
    mRefreshDelegate(nullptr),
    mStateChangeDelegate(nullptr),
    mGroupsControllerBasis(aGroupsControllerBasis),
    mZonesControllerBasis(aZonesControllerBasis),
    mIsDerivingGroupState(false)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the client controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  client controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the client controller was already
 *                                    added as a delegate to the
 *                                    connection manager, command
 *                                    manager, or child controllers.
 *
 */
Status
ControllerBasis :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;


    lRetval = ClientObjectControllerContainer::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mCommandManager.Init(mConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Determine whether the controller supports connections with
 *    the specified protocol scheme.
 *
 *  @param[in]  aSchemeRef  A reference to a CoreFoundation string
 *                          containing the protocol (for example,
 *                          "telnet") scheme for which to check
 *                          support.
 *
 *  @returns
 *     True if the scheme is supported; otherwise, false.
 *
 */
bool
ControllerBasis :: SupportsScheme(CFStringRef aScheme) const
{
    bool lRetval;

    lRetval = mConnectionManager.SupportsScheme(aScheme);

    return (lRetval);
}

// MARK: Accessors

/**
 *  @brief
 *    Return the command manager for the controller.
 *
 *  @returns
 *    An immutable reference to the command manager for the
 *    controller.
 *
 */
const Client::CommandManager &
ControllerBasis :: GetCommandManager(void) const
{
    return (mCommandManager);
}

/**
 *  @brief
 *    Return the command manager for the controller.
 *
 *  @returns
 *    A mutable reference to the command manager for the
 *    controller.
 *
 */
Client::CommandManager &
ControllerBasis :: GetCommandManager(void)
{
    return (mCommandManager);
}

/**
 *  @brief
 *    Return the connection manager for the controller.
 *
 *  @returns
 *    An immutable reference to the connection manager for the
 *    controller.
 *
 */
const Client::ConnectionManager &
ControllerBasis :: GetConnectionManager(void) const
{
    return (mConnectionManager);
}

/**
 *  @brief
 *    Return the connection manager for the controller.
 *
 *  @returns
 *    A mutable reference to the connection manager for the
 *    controller.
 *
 */
Client::ConnectionManager &
ControllerBasis :: GetConnectionManager(void)
{
    return (mConnectionManager);
}

/**
 *  @brief
 *    Refresh the state of the client controller.
 *
 *  This should be called on first-time client start-up or whenever
 *  the client controller state needs to be forcibly refreshed.
 *
 *  This iterates through each of the sub-controllers, tasking each
 *  with taking care of the refresh activity appropriate for its scope
 *  of concern.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        by a controller to perform the
 *                                        refresh.
 *
 */
Status
ControllerBasis :: Refresh(void)
{
    Status                lRetval = kStatus_Success;
    Controllers::iterator begin, end;

    if (mRefreshDelegate != nullptr)
    {
        mRefreshDelegate->ControllerWillRefresh(*this);
    }

    // Reset the overall refresh count.

    mControllersDidRefreshCount = 0;

    // Begin refreshing each controller.

    begin = GetControllers().begin();
    end = GetControllers().end();

    while (begin != end)
    {
        lRetval = begin->second.mController->Refresh();
        nlREQUIRE_SUCCESS(lRetval, done);

        begin++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the controller is in the middle of a refresh.
 *
 *  This returns a Boolean indicating whether (true) or not (false)
 *  the controller is in the middle of a refresh operation with the
 *  peer server controller for up-to-date state.
 *
 *  @returns
 *    True if the controller is refreshing; otherwise, false.
 *
 */
bool
ControllerBasis :: IsRefreshing(void) const
{
    return (mControllersDidRefreshCount != GetControllers().size());
}

/**
 *  @brief
 *    Return the delegate for the client controller.
 *
 *  @returns
 *    A pointer to the delegate for the client controller.
 *
 */
Client::Application::ControllerRefreshDelegate *
ControllerBasis :: GetRefreshDelegate(void) const
{
    return (mRefreshDelegate);
}

/**
 *  @brief
 *    Return the delegate for the client controller.
 *
 *  @returns
 *    A pointer to the delegate for the client controller.
 *
 */
Client::Application::ControllerStateChangeDelegate *
ControllerBasis :: GetStateChangeDelegate(void) const
{
    return (mStateChangeDelegate);
}

/**
 *  @brief
 *    Set the delegate for the client controller.
 *
 *  This attempts to set a delegate for the client controller.
 *
 *  @param[in]  aRefreshDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ControllerBasis :: SetRefreshDelegate(Client::Application::ControllerRefreshDelegate *aRefreshDelegate)
{
    Status lRetval = kStatus_Success;

    if (aRefreshDelegate == mRefreshDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mRefreshDelegate = aRefreshDelegate;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the delegate for the client controller.
 *
 *  This attempts to set a delegate for the client controller.
 *
 *  @param[in]  aStateChangeDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ControllerBasis :: SetStateChangeDelegate(Client::Application::ControllerStateChangeDelegate *aStateChangeDelegate)
{
    Status lRetval = kStatus_Success;

    if (aStateChangeDelegate == mStateChangeDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mStateChangeDelegate = aStateChangeDelegate;

 done:
    return (lRetval);
}

// MARK: Connection Management

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the default timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL)
{
    Status lRetval = kStatus_Success;

    lRetval = Connect(aMaybeURL, kTimeoutDefault);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL,
                           const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Connect(aMaybeURL, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL using IPv4 or IPv6 resolved addresses as
 *  specified.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the host name.
*
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions)
{
    Status lRetval = kStatus_Success;


    lRetval = GetConnectionManager().Connect(aMaybeURL, aVersions, kTimeoutDefault);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL, host name, or host name and port with the
 *  provided timeout using IPv4 or IPv6 resolved addresses as
 *  specified.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the host name.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL,
                           const ConnectionManager::Versions &aVersions,
                           const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    lRetval = GetConnectionManager().Connect(aMaybeURL, aVersions, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the client controller is connected to a
 *    peer.
 *
 *  @returns
 *    True if the client controller is connected to a peer; otherwise,
 *    false.
 *
 */
bool
ControllerBasis :: IsConnected(void) const
{
    return (GetConnectionManager().IsConnected());
}

/**
 *  @brief
 *    Disconnect from the HLX server peer.
 *
 *  This attempts to disconnect from the connected HLX server peer, if
 *  any.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENXIO                   If there is no allocated connection.
 *  @retval  -EALREADY                If the connection is already
 *                                    disconnected.
 *  @retval  -EINPROGRESS             If the connection is already in
 *                                    the process of disconnecting.
 *
 */
Status
ControllerBasis :: Disconnect(void)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Disconnect();
    nlEXPECT_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Object Controller Basis Delegate Methods

// MARK: Object Controller Basis Refresh Delegate Methods

void
ControllerBasis :: ControllerIsRefreshing(Client::ObjectControllerBasis &aController, const uint8_t &aPercentComplete)
{
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = GetControllers().find(&aController);

    if (lControllerIterator != GetControllers().end())
    {
        static const Percentage kPercentCompletePerController    = CalculatePercentage(1,
                                                                                       static_cast<uint8_t>(GetControllers().size()));
        const Percentage        lOtherControllersPercentComplete = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                                       static_cast<uint8_t>(GetControllers().size()));
        const Percentage        lThisControllerPercentComplete   = ((kPercentCompletePerController * aPercentComplete) / 100);
        const Percentage        lPercentComplete                 = (lOtherControllersPercentComplete + lThisControllerPercentComplete);

        if (mRefreshDelegate != nullptr)
        {
            mRefreshDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }
    }
}

/**
 *  @brief
 *    Delegation from a controller that the specified controller is
 *    done refreshing.
 *
 *  On the refresh completion of any one controller, this refreshes
 *  the overall refresh state of the parent client controller.
 *
 *  @param[in]  aController       A reference to the controller
 *                                that issued the delegation.
 *
 */
void
ControllerBasis :: ControllerDidRefresh(Client::ObjectControllerBasis &aController)
{
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = GetControllers().find(&aController);

    if (lControllerIterator != GetControllers().end())
    {
        mControllersDidRefreshCount++;

        if (mRefreshDelegate != nullptr)
        {
            const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                    static_cast<uint8_t>(GetControllers().size()));

            mRefreshDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }

        if (mControllersDidRefreshCount == GetControllers().size())
        {
            // At this point, all controllers have asynchronously
            // completed their refresh requests. Before notifying the
            // delegate of that fact, derive any necessary group
            // state, dispatching state change notifications in the
            // process such that it appears to the delegate as though
            // that group state came with and was bookended by the
            // overall refresh request.

            DeriveGroupState();

            // Now that group state has been derived and state change
            // notifications dispatched, notify the client that the
            // refresh request is complete.

            if (mRefreshDelegate != nullptr)
            {
                mRefreshDelegate->ControllerDidRefresh(*this);
            }
        }
    }

    return;
}

// MARK: Object Controller Basis State Change Delegate Method

/**
 *  Delegation callback for individual sub-controller state change
 *  notifications.
 *
 *  This is not simply a pass-through of sub-controller state change
 *  delegate to the end client due to the fact that some group
 *  sub-controller state changes need to be fanned out to the zone
 *  sub-controller for the zones that belong to a particular group.
 *
 *  In theory and ideally, Audio Authority would have implemented a
 *  group mute, source, or volume change as follows:
 *
 *    \<Group j Mute or Volume or Source Command Request>
 *    \<Zone i Mute or Volume or Source State Change>
 *    ...
 *    \<Zone n Mute or Volume or Source State Change>
 *    \<Group Mute or Volume or Source Command Response>
 *
 *  However, instead, all that we get in practice is:
 *
 *    \<Group j Mute or Volume or Source Command Request>
 *    \<Group j Mute or Volume or Source Command Response>
 *
 *  Leaving us to extract zone membership for the relevant group from
 *  the group sub-controller and to then intuit and apply the mute,
 *  volume, or source changes to the zone members based on the group
 *  command response.
 *
 *  @param[in]  aController               A reference to the controller
 *                                        that initiated the state change
 *                                        notification.
 *
 *  @param[in]  aStateChangeNotification  A read-only reference to the
 *                                        state change notification.
 *
 */
void
ControllerBasis :: ControllerStateDidChange(Client::ObjectControllerBasis &aController,
                                            const StateChange::NotificationBasis &aStateChangeNotification)
{
    MaybeHandleGroupZoneStateChangeInteractions(aController, aStateChangeNotification);

    if (mStateChangeDelegate != nullptr)
    {
        const StateChange::Type lType = aStateChangeNotification.GetType();

        switch (lType)
        {

        // The state change notification types
        // kStateChangeType_Group{Dec,Inc}reaseVolume are internal-
        // only and are used between the groups controllera and this
        // parent controller to synthesize and fan out other client-
        // visible and relevant state change notifications.
        // Consequently, we filter them out here.

        case StateChange::kStateChangeType_GroupIncreaseVolume:
        case StateChange::kStateChangeType_GroupDecreaseVolume:
            break;

        // If the event was not otherwise filtered out, pass it along
        // to the delegate.

        default:
            mStateChangeDelegate->ControllerStateDidChange(*this, aStateChangeNotification);
            break;

        }
    }
}

// MARK: Group State Derivation Methods

/**
 *  @brief
 *    Heuristically derive the mute, source, and volume state for each
 *    group in the group controller.
 *
 *  The groups controller represents a particular challenge. The group
 *  controller query does not return any state about mute, source, or
 *  volume for each group on query. Nor does any mutating command
 *  return any state change notifications. So, all such state has to
 *  be extracted from the constituent zones. Moreover, unless all
 *  zones in a group are in lock-step state synchrony, the actual
 *  state of any group is ambiguous. Consequently, we have to
 *  heuristically derive it as follows:
 *
 *    * Mute
 *
 *      - Mute State   = !IsAnyNotMuted(\<Group>, \<Zones in Group>)
 *
 *    * Volume
 *
 *      - Volume State = Mean(Volume(\<Group>, \<Zones in Group>))
 *
 *    * Source
 *
 *      - Source State = Union(Sources(\<Group>, \<Zones in Group>))
 *
 */
void
ControllerBasis :: DeriveGroupState(void)
{
    GroupModel::IdentifierType  lGroupsCount;
    Status                      lStatus;
    GroupModel::IdentifierType  lGroupIdentifierCurrent;
    GroupModel::IdentifierType  lGroupIdentifierEnd;


    Log::Debug().Write("Attempting to derive group state\n");

    mIsDerivingGroupState = true;

    lStatus = Common::GroupsControllerBasis::GetGroupsMax(lGroupsCount);
    nlREQUIRE_SUCCESS(lStatus, done);

    lGroupIdentifierCurrent = IdentifierModel::kIdentifierMin;
    lGroupIdentifierEnd     = static_cast<GroupModel::IdentifierType>(lGroupsCount + 1);

    while (lGroupIdentifierCurrent != lGroupIdentifierEnd)
    {
        const GroupModel *lGroupModel;

        lStatus = mGroupsControllerBasis.GetGroup(lGroupIdentifierCurrent, lGroupModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        DeriveGroupStateForGroup(lGroupIdentifierCurrent, *lGroupModel);

        lGroupIdentifierCurrent++;
    }

 done:
    mIsDerivingGroupState = false;

    return;
}

/**
 *  @brief
 *    Heuristically derive the mute, source, and volume state for each
 *    group in the group controller that contains the specified zone
 *    identifier.
 *
 *  @param[in]  aZoneIdentifier  A read-only reference to the zone
 *                               identifier against which group
 *                               membership should be checked before
 *                               attempting to derive group state.
 *
 */
void
ControllerBasis :: DeriveGroupStateForGroupsIncludingZone(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    GroupModel::IdentifierType  lGroupsCount;
    Status                      lStatus;
    GroupModel::IdentifierType  lGroupIdentifierCurrent;
    GroupModel::IdentifierType  lGroupIdentifierEnd;


    Log::Debug().Write("Attempting to derive group state for groups including zone %hhu\n", aZoneIdentifier);

    mIsDerivingGroupState = true;

    lStatus = Common::GroupsControllerBasis::GetGroupsMax(lGroupsCount);
    nlREQUIRE_SUCCESS(lStatus, done);

    lGroupIdentifierCurrent = IdentifierModel::kIdentifierMin;
    lGroupIdentifierEnd     = static_cast<GroupModel::IdentifierType>(lGroupsCount + 1);

    while (lGroupIdentifierCurrent != lGroupIdentifierEnd)
    {
        const GroupModel *lGroupModel;

        lStatus = mGroupsControllerBasis.GetGroup(lGroupIdentifierCurrent, lGroupModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        DeriveGroupStateForGroupIncludingZone(lGroupIdentifierCurrent, *lGroupModel, aZoneIdentifier);

        lGroupIdentifierCurrent++;
    }

 done:
    mIsDerivingGroupState = false;

    return;
}

void
ControllerBasis :: DeriveGroupStateForGroupIncludingZone(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                               const Model::GroupModel &aGroupModel,
                               const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    bool  lGroupHasZone;


    Log::Debug().Write("Attempting to derive group state for group %hhu including zone %hhu\n", aGroupIdentifier, aZoneIdentifier);

    // Check whether the group contains the specified zone.

    lGroupHasZone = aGroupModel.ContainsZone(aZoneIdentifier);

    // If it does, proceed with attempting to derive group state for
    // the group.

    if (lGroupHasZone)
    {
        DeriveGroupStateForGroup(aGroupIdentifier, aGroupModel);
    }

    return;
}

void
ControllerBasis :: DeriveGroupStateForGroup(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                               const Model::GroupModel &aGroupModel)
{
    Status                     lStatus;
    DerivedGroupState          lDerivedGroupState;


    Log::Debug().Write("Attempting to derive group state for group %hhu\n", aGroupIdentifier);

    lStatus = lDerivedGroupState.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // An unused zone may have no zones, so expect a potential failure
    // status here.

    lStatus = aGroupModel.GetZones(lDerivedGroupState.mZoneCount);
    nlEXPECT_SUCCESS(lStatus, done);

    if (lDerivedGroupState.mZoneCount > 0)
    {
        Detail::ZoneIdentifiers                  lZoneIdentifiers;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierCurrent;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierEnd;

        lZoneIdentifiers.resize(lDerivedGroupState.mZoneCount);

        lStatus = aGroupModel.GetZones(&lZoneIdentifiers[0], lDerivedGroupState.mZoneCount);
        nlREQUIRE_SUCCESS(lStatus, done);

        lZoneIdentifierCurrent = lZoneIdentifiers.begin();
        lZoneIdentifierEnd     = lZoneIdentifiers.end();

        while (lZoneIdentifierCurrent != lZoneIdentifierEnd)
        {
            const Model::ZoneModel *                   lZoneModel;
            VolumeModel::MuteType               lZoneMute;
            VolumeModel::LevelType             lZoneVolume;
            SourceModel::IdentifierType         lZoneSource;


            lStatus = mZonesControllerBasis.GetZone(*lZoneIdentifierCurrent, lZoneModel);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lZoneModel->GetMute(lZoneMute);
            nlREQUIRE_SUCCESS(lStatus, done);

            if (!lZoneMute)
            {
                lDerivedGroupState.mGroupMute = lZoneMute;
            }

            lStatus = lZoneModel->GetVolume(lZoneVolume);
            nlREQUIRE_SUCCESS(lStatus, done);

            lDerivedGroupState.UpdateVolume(lZoneVolume);

            lStatus = lZoneModel->GetSource(lZoneSource);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lDerivedGroupState.AddSource(lZoneSource);
            nlEXPECT(lStatus >= kStatus_Success, done);

            lZoneIdentifierCurrent++;
        }
    }

    // Whether a group has no, one, or more than one zone, we need to
    // establish baseline derived mute, volume, and source state for
    // the group.

    // Handle a potential mute change for the group.

    mGroupsControllerBasis.HandleMuteChange(aGroupIdentifier, lDerivedGroupState.mGroupMute);

    // Handle a potential volume change for the group.

    mGroupsControllerBasis.HandleVolumeChange(aGroupIdentifier, lDerivedGroupState.GetVolume());

    // Handle a potential source change for the group.

    mGroupsControllerBasis.HandleSourcesChange(aGroupIdentifier, lDerivedGroupState.GetSources());

 done:
    if (lStatus < kStatus_Success)
    {
        Log::Error().Write("Unable to derive status for group %hhu\n", aGroupIdentifier);
    }

    return;
}

void
ControllerBasis :: MaybeHandleGroupZoneStateChangeInteractions(Client::ObjectControllerBasis &aController, const StateChange::NotificationBasis &aStateChangeNotification)
{
    const StateChange::Type lType = aStateChangeNotification.GetType();


    (void)aController;

    nlEXPECT(mIsDerivingGroupState == false, done);

    switch (lType)
    {

    case StateChange::kStateChangeType_GroupMute:
    case StateChange::kStateChangeType_GroupSource:
    case StateChange::kStateChangeType_GroupVolume:
    case StateChange::kStateChangeType_GroupIncreaseVolume:
    case StateChange::kStateChangeType_GroupDecreaseVolume:
        {
            const StateChange::GroupsNotificationBasis &lSCN = static_cast<const StateChange::GroupsNotificationBasis &>(aStateChangeNotification);

            Log::Debug().Write("NOT deriving group state, handling group %hhu event %d\n",
                               lSCN.GetIdentifier(), lType);

            HandleGroupZoneStateChangeInteractions(lSCN, lType);
        }
        break;

    case StateChange::kStateChangeType_ZoneMute:
    case StateChange::kStateChangeType_ZoneSource:
    case StateChange::kStateChangeType_ZoneVolume:
        {
            const StateChange::ZonesNotificationBasis &  lSCN = static_cast<const StateChange::ZonesNotificationBasis &>(aStateChangeNotification);
            const Model::ZoneModel::IdentifierType              lZone = lSCN.GetIdentifier();

            if (!IsRefreshing())
            {
                Log::Debug().Write("NOT deriving group state and NOT refreshing, handling zone %hhu event %d\n",
                                   lZone, lType);

                DeriveGroupStateForGroupsIncludingZone(lZone);
            }
        }
        break;

    default:
        break;

    }

 done:
    return;
}

void
ControllerBasis :: HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType)
{
    switch (aType)
    {

    case StateChange::kStateChangeType_GroupMute:
    case StateChange::kStateChangeType_GroupSource:
    case StateChange::kStateChangeType_GroupVolume:
    case StateChange::kStateChangeType_GroupIncreaseVolume:
    case StateChange::kStateChangeType_GroupDecreaseVolume:
        {
            const GroupModel::IdentifierType  lGroupIdentifier = aGroupStateChangeNotification.GetIdentifier();
            const GroupModel *                lGroupModel;
            Status                            lStatus;
            DerivedGroupState                 lDerivedGroupState;


            lStatus = lDerivedGroupState.Init();
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = mGroupsControllerBasis.GetGroup(lGroupIdentifier, lGroupModel);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lGroupModel->GetZones(lDerivedGroupState.mZoneCount);
            nlREQUIRE_SUCCESS(lStatus, done);

            HandleGroupZoneStateChangeInteractions(aGroupStateChangeNotification, aType, *lGroupModel, lDerivedGroupState);
        }
        break;

    default:
        break;

    }

 done:
    return;
}

void
ControllerBasis :: HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType, const GroupModel &aGroupModel, DerivedGroupState &aDerivedGroupState)
{
    Status lStatus;

    if (aDerivedGroupState.mZoneCount > 0)
    {
        const GroupModel::IdentifierType         lGroupIdentifier = aGroupStateChangeNotification.GetIdentifier();
        Detail::ZoneIdentifiers                  lZoneIdentifiers;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierCurrent;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierEnd;

        lZoneIdentifiers.resize(aDerivedGroupState.mZoneCount);

        lStatus = aGroupModel.GetZones(&lZoneIdentifiers[0], aDerivedGroupState.mZoneCount);
        nlREQUIRE_SUCCESS(lStatus, done);

        lZoneIdentifierCurrent = lZoneIdentifiers.begin();
        lZoneIdentifierEnd     = lZoneIdentifiers.end();

        while (lZoneIdentifierCurrent != lZoneIdentifierEnd)
        {
            HandleGroupZoneStateChangeInteractions(aGroupStateChangeNotification, aType, aDerivedGroupState, *lZoneIdentifierCurrent);

            lZoneIdentifierCurrent++;
        }

        switch (aType)
        {

        case StateChange::kStateChangeType_GroupIncreaseVolume:
        case StateChange::kStateChangeType_GroupDecreaseVolume:
            mIsDerivingGroupState = true;
            mGroupsControllerBasis.HandleVolumeChange(lGroupIdentifier, aDerivedGroupState.GetVolume());
            mIsDerivingGroupState = false;
            break;

        default:
            break;
        }
    }

 done:
    return;
}

void
ControllerBasis :: HandleGroupZoneStateChangeInteractions(const StateChange::GroupsNotificationBasis &aGroupStateChangeNotification, const StateChange::Type &aType, DerivedGroupState &aDerivedGroupState, const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    switch (aType)
    {

    case StateChange::kStateChangeType_GroupMute:
        {
            const StateChange::GroupsMuteNotification &lMuteSCN = static_cast<const StateChange::GroupsMuteNotification &>(aGroupStateChangeNotification);
            const VolumeModel::MuteType lMute = lMuteSCN.GetMute();

            mZonesControllerBasis.HandleMuteChange(aZoneIdentifier, lMute);
        }
        break;

    case StateChange::kStateChangeType_GroupSource:
        {
            const StateChange::GroupsSourceNotification &lSourceSCN = static_cast<const StateChange::GroupsSourceNotification &>(aGroupStateChangeNotification);
            size_t lGroupSourceCount = lSourceSCN.GetCount();

            // The zone controller should only be handling this if
            // there is one and only one source for the group;
            // otherwise, the group is "split" and zone state SHOULD
            // already be up to date.

            if (lGroupSourceCount == 1)
            {
                const GroupModel::Sources &  lGroupSources = lSourceSCN.GetSources();
                Detail::SourceIdentifiers    lGroupSourceIdentifiers;
                Status                       lStatus;

                lGroupSourceIdentifiers.resize(lGroupSourceCount);

                lStatus = lGroupSources.GetIdentifiers(&lGroupSourceIdentifiers[0], lGroupSourceCount);
                nlREQUIRE_SUCCESS(lStatus, done);

                mZonesControllerBasis.HandleSourceChange(aZoneIdentifier, lGroupSourceIdentifiers[0]);
            }
        }
        break;

    case StateChange::kStateChangeType_GroupVolume:
        {
            const StateChange::GroupsVolumeNotification &lVolumeSCN = static_cast<const StateChange::GroupsVolumeNotification &>(aGroupStateChangeNotification);
            const VolumeModel::LevelType lVolume = lVolumeSCN.GetVolume();

            mZonesControllerBasis.HandleVolumeChange(aZoneIdentifier, lVolume);
        }
        break;

    // The state change notification types
    // kStateChangeType_Group{Dec,Inc}reaseVolume are internal-
    // only and are used between the groups controller and this parent
    // controller to synthesize and fan out other client-visible and
    // -relevant state change notifications.
    //
    // On receipt of these events, we only know that group volume has
    // either increased or decreased succcessfully. An individual zone
    // volume state change notification must be fanned out from here.

    case StateChange::kStateChangeType_GroupIncreaseVolume:
    case StateChange::kStateChangeType_GroupDecreaseVolume:
        {
            const ZoneModel *       lZoneModel;
            VolumeModel::LevelType  lZoneVolume;
            Status                  lStatus;

            lStatus = mZonesControllerBasis.GetZone(aZoneIdentifier, lZoneModel);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lZoneModel->GetVolume(lZoneVolume);
            nlREQUIRE_SUCCESS(lStatus, done);

            switch (aType)
            {

            case StateChange::kStateChangeType_GroupIncreaseVolume:
                lZoneVolume += 1;
                break;

            case StateChange::kStateChangeType_GroupDecreaseVolume:
                lZoneVolume -= 1;
                break;

            default:
                goto done;
                break;

            }

            aDerivedGroupState.UpdateVolume(lZoneVolume);

            mZonesControllerBasis.HandleVolumeChange(aZoneIdentifier, lZoneVolume);
        }
        break;

    default:
        break;

    }

 done:
    return;
}

}; // namespace Application

}; // namespace Client

}; // namespace HLX
