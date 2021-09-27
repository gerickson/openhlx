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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a collection of HLX groups.
 *
 *      The groups client controller is a bit more of a "thick" client
 *      controller relative to its peers and works in concert with the
 *      zones client controller, orchestrated by the parent client
 *      controller due to the fact that Audio Authority did not
 *      implement the groups server-side controller in the same way as
 *      other server-side controllers.
 *
 *      Specifically, when a groups source select, volume, or mute command
 *      is received, synchronous state change notifications for the
 *      constituent zones in the group would have come along in the command
 *      response along with the command completion itself. For example:
 *
 *        \<Group j Mute or Volume or Source Command Request>
 *        \<Zone i Mute or Volume or Source State Change>
 *        ...
 *        \<Zone n Mute or Volume or Source State Change>
 *        \<Group Mute or Volume or Source Command Response>
 *
 *      However, instead, all that we get in practice is:
 *
 *        \<Group j Mute or Volume or Source Command Request>
 *        \<Group j Mute or Volume or Source Command Response>
 *
 *  Leaving it to the parent client controller to extract zone
 *  membership for the relevant group from the group client controller
 *  and to then intuit and apply the mute, volume, or source changes
 *  to the client side zone data members based on the group command
 *  response.
 *
 */

#include "GroupsController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Client/SourcesController.hpp>
#include <OpenHLX/Client/ZonesController.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
GroupsController :: GroupsController(void) :
    Common::GroupsControllerBasis(),
    Client::GroupsControllerBasis(Common::GroupsControllerBasis::mGroups,
                                  Common::GroupsControllerBasis::kGroupsMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
GroupsController :: ~GroupsController(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
GroupsController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::GroupsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::GroupsControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the group identifier with the specified name.
 *
 *  This attempts to lookup the group identifier for the group
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    group to find an identifier
 *                                    for.
 *  @param[out]  aGroupIdentifier     A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No group could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
GroupsController :: LookupIdentifier(const char *aName, IdentifierType &aGroupIdentifier) const
{
    const GroupModel *  lGroupModel;
    Status              lRetval;


    lRetval = mGroups.GetGroup(aName, lGroupModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lGroupModel->GetIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Mute Mutator Commands

/**
 *  @brief
 *    Clear (deassert) the group volume mute state.
 *
 *  This attempts to clear (deassert) the group volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to clear (deassert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: ClearMute(const IdentifierType &aGroupIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::ClearMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::ClearMute>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::SetMuteCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Set (assert) the group volume mute state.
 *
 *  This attempts to set (assert) the group volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set (assert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: SetMute(const IdentifierType &aGroupIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::SetMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::SetMute>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::SetMuteCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the group volume mute state to the specified state.
 *
 *  This attempts to set the group volume mute state to the specified
 *  state on the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the
 *                                volume mute state.
 *  @param[in]  aMute             An immutable reference to the
 *                                volume mute state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
GroupsController :: SetMute(const IdentifierType &aGroupIdentifier,
                            const Model::VolumeModel::MuteType &aMute)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ((aMute) ? SetMute(aGroupIdentifier) : ClearMute(aGroupIdentifier));
    nlEXPECT(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Toggle (flip) the group volume mute state.
 *
 *  This attempts to toggle (flip) the group volume mute state on the
 *  peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to toggle (flip)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: ToggleMute(const IdentifierType &aGroupIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::ToggleMute());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::ToggleMute>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::ToggleMuteCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Name Mutator Commands

/**
 *  @brief
 *    Set the group to the specified name.
 *
 *  This attempts to set the group with the provided identifier to the
 *  specified name on the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the
 *                                name.
 *  @param[in]  aName             A pointer to the null-terminated
 *                                C string to set the group name
 *                                to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
GroupsController :: SetName(const IdentifierType &aGroupIdentifier,
                            const char *aName)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::SetName());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::SetName>(lCommand)->Init(aGroupIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::SetNameCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Source Mutator Commands

/**
 *  @brief
 *    Set the group source (input) state to the specified source.
 *
 *  This attempts to set the group source (input) state to the
 *  specified source on the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group or source (input)
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: SetSource(const IdentifierType &aGroupIdentifier,
                              const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SourcesController::ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::SetSource());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::SetSource>(lCommand)->Init(aGroupIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::SetSourceCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Volume Mutator Commands

/**
 *  @brief
 *    Set the group volume level.
 *
 *  This attempts to set the group volume level on the peer HLX server
 *  controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the volume
 *                                level state.
 *  @param[in]  aVolume           An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
GroupsController :: SetVolume(const IdentifierType &aGroupIdentifier,
                              const Model::VolumeModel::LevelType &aVolume)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::SetVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::SetVolume>(lCommand)->Init(aGroupIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::SetVolumeCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the group volume level by one (1) unit.
 *
 *  This attempts to increase the group volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
GroupsController :: IncreaseVolume(const IdentifierType &aGroupIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::IncreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::IncreaseVolume>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::IncreaseVolumeCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the group volume level by one (1) unit.
 *
 *  This attempts to decrease the group volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
GroupsController :: DecreaseVolume(const IdentifierType &aGroupIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::DecreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::DecreaseVolume>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::DecreaseVolumeCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Add the provided zone to the specified group
 *
 *  This attempts to add the provided zone to the specified group on
 *  the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to add the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to add to the group.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group or source (input)
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: AddZone(const IdentifierType &aGroupIdentifier, const ZoneModel::IdentifierType &aZoneIdentifier)
{
    DeclareScopedFunctionTracer(lTracer);
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ZonesController::ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::AddZone());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::AddZone>(lCommand)->Init(aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::ChangeZoneCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Remove the provided zone from the specified group
 *
 *  This attempts to remove the provided zone from the specified group
 *  on the peer HLX server controller.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to remove the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to remove fromthe
 *                                group.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group or source (input)
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
GroupsController :: RemoveZone(const IdentifierType &aGroupIdentifier, const ZoneModel::IdentifierType &aZoneIdentifier)
{
    DeclareScopedFunctionTracer(lTracer);
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ZonesController::ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Groups::RemoveZone());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Groups::RemoveZone>(lCommand)->Init(aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::GroupsControllerBasis::ChangeZoneCompleteHandler,
                          Client::GroupsControllerBasis::CommandErrorHandler,
                          static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
