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
 *      This file implements an object for effecting an HLX client
 *      controller for any HLX client application, whether a command
 *      line utility or a mobile app.
 *
 * .-------------------------------------------------------------------------.
 * |                                                                         |
 * |                             HLXC Controller                             |
 * |                                                                         |
 * | .---------------------------------------------------------------------. |
 * | |  Controller Basis                                                   | |
 * | '---------------------------------------------------------------------' |
 * | .---------------. .---------------. .---------------. .---------------. |
 * | |    Zones      | |     Groups    | |   EQ Preset   | |   Favorites   | |
 * | |  Controller   | |  Controller   | |  Controller   | |  Controller   | |
 * | '---------------' '---------------' '---------------' '---------------' |
 * | .---------------. .---------------. .---------------. .---------------. |
 * | |  Front Panel  | |   Infrared    | |    Network    | | Configuration | |
 * | |  Controller   | |  Controller   | |  Controller   | |  Controller   | |
 * | '---------------' '---------------' '---------------' '---------------' |
 * | .---------------.                                                       |
 * | |    Sources    |                                                       |
 * | |  Controller   |                                                       |
 * | '---------------'                                                       |
 * | .---------------------------------------------------------------------. |
 * | |                         Command Manager                             | |
 * | |                             Delegate                                | |
 * | '---------------------------------------------------------------------' |
 * |                                                                         |
 * | .---------------------------------------------------------------------. |
 * | |                                                                     | |
 * | |                           Command Manager                           | |
 * | |                                                                     | |
 * | | .-------------------------------. .-------------------------------. | |
 * | | | Response                      | | Request                       | | |
 * | | | Handler                       | | Dispatcher                    | | |
 * | | '-------------------------------' '-------------------------------' | |
 * | | .-----------------------------------------------------------------. | |
 * | | |                          Connection Manager                     | | |
 * | | |                               Delegate                          | | |
 * | | '-----------------------------------------------------------------' | |
 * | '---------------------------------------------------------------------' |
 * | .---------------------------------------------------------------------. |
 * | |                                                                     | |
 * | |                          Connection Manager                         | |
 * | |                                                                     | |
 * | | .-----------------------------------------------------------------. | |
 * | | |                         Connection Basis                        | | |
 * | | '-----------------------------------------------------------------' | |
 * | | .-------------------. .--------------------. .--------------------. | |
 * | | |    Connection     | |    Connection      | |    Connection      | | |
 * | | |      Telnet       | |      Serial        | |       File         | | |
 * | | '-------------------' '--------------------' '--------------------' | |
 * | '---------------------------------------------------------------------' |
 * '-------------------------------------------------------------------------'
 *
 */

#include "ApplicationController.hpp"

#include <vector>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


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

// MARK: Client Controller

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
Controller :: Controller(void) :
    Common::Application::ControllerBasis(),
    Client::Application::ControllerBasis(mGroupsController,
                                         mZonesController),
    ConnectionManagerDelegate(),
    CommandManagerDelegate(),
    ObjectControllerBasisErrorDelegate(),
    mConfigurationController(),
    mEqualizerPresetsController(),
    mFavoritesController(),
    mFrontPanelController(),
    mGroupsController(),
    mInfraredController(),
    mNetworkController(),
    mSourcesController(),
    mZonesController(),
    mDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
Controller :: ~Controller(void)
{
    return;
}

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
Controller :: Init(const RunLoopParameters &aRunLoopParameters)
{
    Controllers::iterator  lCurrent;
    Controllers::iterator  lEnd;
    Status                 lRetval;


    lRetval = Common::Application::ControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::Application::ControllerBasis::Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = GetConnectionManager().AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = GetCommandManager().SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Place the various controllers into the controller
    // container. Order is important since this is the priority we
    // want to run operations like refresh.

    AddController(mConfigurationController);
    AddController(mSourcesController);
    AddController(mZonesController);
    AddController(mGroupsController);
    AddController(mFavoritesController);
    AddController(mEqualizerPresetsController);
    AddController(mInfraredController);
    AddController(mFrontPanelController);
    AddController(mNetworkController);

    // Intialize the controllers.

    lCurrent = GetControllers().begin();
    lEnd     = GetControllers().end();

    while (lCurrent != lEnd)
    {
        lRetval = lCurrent->second.mController->Init(GetCommandManager());
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lCurrent->second.mController->SetErrorDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lCurrent->second.mController->SetRefreshDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lCurrent->second.mController->SetStateChangeDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the delegate for the client controller.
 *
 *  @returns
 *    A pointer to the delegate for the client controller.
 *
 */
Client::Application::ControllerDelegate *
Controller :: GetDelegate(void) const
{
    return (mDelegate);
}

/**
 *  @brief
 *    Set the delegate for the client controller.
 *
 *  This attempts to set a delegate for the client controller.
 *
 *  @param[in]  aDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
Controller :: SetDelegate(Client::Application::ControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    if (aDelegate == mDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    lRetval = Client::Application::ControllerBasis::SetRefreshDelegate(aDelegate);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::Application::ControllerBasis::SetStateChangeDelegate(aDelegate);
    nlREQUIRE_SUCCESS(lRetval, done);

    mDelegate        = aDelegate;

done:
    return (lRetval);
}

// MARK: Equalizer Preset Commands

/**
 *  @brief
 *    Get the maximum number of supported HLX equalizer presets.
 *
 *  @param[out]  aEqualizerPresets  The maximum number of HLX equalizer
 *                                  presets on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
Controller :: EqualizerPresetsGetMax(Model::EqualizerPresetModel::IdentifierType &aEqualizerPresets) const
{
    Status lRetval;

    lRetval = mEqualizerPresetsController.GetEqualizerPresetsMax(aEqualizerPresets);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset model associated with specified
 *    equalizer preset identifier.
 *
 *  @param[in]   aEqualizerPresetIdentifier  An immutable reference
 *                                           to the equalizer preset
 *                                           model to obtain.
 *  @param[out]  aModel                      A reference to an immutable
 *                                           pointer by which to
 *                                           return the equalizer
 *                                           preset model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than supported.
 *
 */
Status
Controller :: EqualizerPresetGet(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const EqualizerPresetModel *&aModel) const
{
    Status lRetval;

    lRetval = mEqualizerPresetsController.GetEqualizerPreset(aEqualizerPresetIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset identifier with the specified name.
 *
 *  This attempts to lookup the equalizer preset identifier for the
 *  equalizer preset with the specified name.
 *
 *  @param[in]   aName                       A pointer to a null-
 *                                           terminated C string of
 *                                           the name of the equalizer
 *                                           preset to find an
 *                                           identifier for.
 *  @param[out]  aEqualizerPresetIdentifier  A reference to storage by
 *                                           which to return the
 *                                           identifier if successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No equalizer preset could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
Controller :: EqualizerPresetLookupIdentifier(const char *aName,
                                              EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresetsController.LookupIdentifier(aName,
                                                           aEqualizerPresetIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the equalizer preset to the specified name.
 *
 *  This attempts to set the equalizer preset with the provided identifier
 *  to the specified name on the peer HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          for which to set the name.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string to set
 *                                          the equalizer preset name
 *                                          to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
Controller :: EqualizerPresetSetName(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresetsController.SetName(aEqualizerPresetIdentifier, aName);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to decrease the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: EqualizerPresetDecreaseBand(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresetsController.DecreaseEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to increase the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: EqualizerPresetIncreaseBand(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresetsController.IncreaseEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the level of an equalizer band of an equalizer preset.
 *
 *  This attempts to set the level of the specified equalizer band of
 *  the specified equalizer preset to the provided value on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to set the band
 *                                          level.
 *  @param[in] aEqualizerBandLevel          An immutable reference to
 *                                          the band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: EqualizerPresetSetBand(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerPresetsController.SetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aEqualizerBandLevel);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Group Commands

/**
 *  @brief
 *    Get the maximum number of supported HLX groups.
 *
 *  @param[out]  aGroups  The maximum number of HLX groups on
 *                        success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
Controller :: GroupsGetMax(Model::GroupModel::IdentifierType &aGroups) const
{
    Status lRetval;

    lRetval = mGroupsController.GetGroupsMax(aGroups);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the group model associated with specified group
 *    identifier.
 *
 *  @param[in]   aGroupIdentifier  An immutable reference to the group
 *                            model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the group model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the group identifier is smaller
 *                            or larger than supported.
 *
 */
Status
Controller :: GroupGet(const GroupModel::IdentifierType &aGroupIdentifier, const GroupModel *&aModel) const
{
    Status lRetval;

    lRetval = mGroupsController.GetGroup(aGroupIdentifier, aModel);
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
Controller :: GroupLookupIdentifier(const char *aName, GroupModel::IdentifierType &aGroupIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.LookupIdentifier(aName, aGroupIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

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
Controller :: GroupSetName(const GroupModel::IdentifierType &aGroupIdentifier, const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.SetName(aGroupIdentifier, aName);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

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
Controller :: GroupClearMute(const GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.ClearMute(aGroupIdentifier);
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
Controller :: GroupSetMute(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.SetMute(aGroupIdentifier);
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
Controller :: GroupSetMute(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.SetMute(aGroupIdentifier, aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

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
Controller :: GroupToggleMute(const GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.ToggleMute(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

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
Controller :: GroupSetSource(const GroupModel::IdentifierType &aGroupIdentifier, const SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.SetSource(aGroupIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

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
 *  @param[in]  aLevel            An immutable reference to the
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
Controller :: GroupSetVolume(const GroupModel::IdentifierType &aGroupIdentifier, const VolumeModel::LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.SetVolume(aGroupIdentifier, aLevel);
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
Controller :: GroupIncreaseVolume(const GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.IncreaseVolume(aGroupIdentifier);
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
Controller :: GroupDecreaseVolume(const GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroupsController.DecreaseVolume(aGroupIdentifier);
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
Controller :: GroupAddZone(const GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;


    lRetval = mGroupsController.AddZone(aGroupIdentifier, aZoneIdentifier);
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
Controller :: GroupRemoveZone(const GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;


    lRetval = mGroupsController.RemoveZone(aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Network Commands

/**
 *  @brief
 *    Attempt to get the HLX server IPv4 Dynamic Host Control Protocol
 *    (DHCP) enabled state.
 *
 *  This attempts to get the HLX server IPv4 Dynamic Host Control
 *  Protocol (DHCP) enabled state, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aDHCPv4Enabled  A mutable reference to storage for
 *                               the IPv4 Dynamic Host Control
 *                               Protocol (DHCP) enabled state, if
 *                               successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IPv4 Dynamic Host Control
 *                                  Protocol (DHCP) enabled state has
 *                                  not been initialized with a known
 *                                  value.
 *
 */
Status
Controller :: NetworkGetDHCPv4Enabled(Model::NetworkModel::EnabledType &aDHCPv4Enabled) const
{
    Status lRetval;

    lRetval = mNetworkController.GetDHCPv4Enabled(aDHCPv4Enabled);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server network interface hardware
 *    address.
 *
 *  This attempts to get the HLX server network interface hardware
 *  address, if it has been previously initialized or set.
 *
 *  @param[out]  aEthernetEUI48    A mutable reference to storage for
 *                                 the network interface EUI-48
 *                                 address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the network interface
 *                                  hardware address has not been
 *                                  initialized with a known value.
 *
 */
Status
Controller :: NetworkGetEthernetEUI48(Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48) const
{
    Status lRetval;

    lRetval = mNetworkController.GetEthernetEUI48(aEthernetEUI48);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server default router (that is, gateway)
 *    IP address.
 *
 *  This attempts to get the HLX server default router (that is,
 *  gateway) IP address, if it has been previously initialized or set.
 *
 *  @param[out]  aDefaultRouterAddress  A mutable reference to storage
 *                                      for the default router IP
 *                                      address, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the default router IP address
 *                                  has not been initialized with a
 *                                  known value.
 *
 */
Status
Controller :: NetworkGetDefaultRouterIPAddress(Model::NetworkModel::IPAddressType &aDefaultRouterAddress) const
{
    Status lRetval;

    lRetval = mNetworkController.GetDefaultRouterIPAddress(aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server host IP address.
 *
 *  This attempts to get the HLX server host IP address, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aHostAddress           A mutable reference to storage
 *                                      for the host IP address, if
 *                                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the host IP address has not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
Controller :: NetworkGetHostIPAddress(Model::NetworkModel::IPAddressType &aHostAddress) const
{
    Status lRetval;

    lRetval = mNetworkController.GetHostIPAddress(aHostAddress);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server IP netmask.
 *
 *  This attempts to get the HLX server IP netmask, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aNetmask               A mutable reference to storage
 *                                      for the IP netmask, if
 *                                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the IP netmask has not been
 *                                  initialized with a known value.
 *
 */
Status
Controller :: NetworkGetIPNetmask(Model::NetworkModel::IPAddressType &aNetmask) const
{
    Status lRetval;

    lRetval = mNetworkController.GetIPNetmask(aNetmask);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the HLX server Control4 Simple Device Discovery
 *    Protocol (SDDP) enabled state.
 *
 *  This attempts to get the HLX server Control4 Simple Device
 *  Discovery Protocol (SDDP) enabled state, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aSDDPEnabled  A mutable reference to storage for
 *                             the Control4 Simple Device Discovery
 *                             Protocol (SDDP) enabled state, if
 *                             successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the Control4 Simple Device
 *                                  Discovery Protocol (SDDP) enabled
 *                                  state has not been initialized
 *                                  with a known value.
 *
 */
Status
Controller :: NetworkGetSDDPEnabled(Model::NetworkModel::EnabledType &aSDDPEnabled) const
{
    Status lRetval;

    lRetval = mNetworkController.GetSDDPEnabled(aSDDPEnabled);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Source Commands

/**
 *  @brief
 *    Get the maximum number of supported HLX sources.
 *
 *  @param[out]  aSources  The maximum number of HLX sources on
 *                         success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
Controller :: SourcesGetMax(Model::SourceModel::IdentifierType &aSources) const
{
    Status lRetval;

    lRetval = mSourcesController.GetSourcesMax(aSources);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the source model associated with specified source
 *    identifier.
 *
 *  @param[in]   aSourceIdentifier  An immutable reference to the source
 *                            model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the source model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the source identifier is smaller
 *                            or larger than supported.
 *
 */
Status
Controller :: SourceGet(const SourceModel::IdentifierType &aSourceIdentifier, const SourceModel *&aModel) const
{
    Status lRetval;

    lRetval = mSourcesController.GetSource(aSourceIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the source identifier with the specified name.
 *
 *  This attempts to lookup the source identifier for the source
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    source to find an identifier
 *                                    for.
 *  @param[out]  aSourceIdentifier    A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No source could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
Controller :: SourceLookupIdentifier(const char *aName, SourceModel::IdentifierType &aSourceIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSourcesController.LookupIdentifier(aName, aSourceIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the source to the specified name.
 *
 *  This attempts to set the source with the provided identifier to the
 *  specified name on the peer HLX server controller.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference for the
 *                                 source for which to set the
 *                                 name.
 *  @param[in]  aName              A pointer to the null-terminated
 *                                 C string to set the source name
 *                                 to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the source identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
Controller :: SourceSetName(const SourceModel::IdentifierType &aSourceIdentifier, const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mSourcesController.SetName(aSourceIdentifier, aName);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Commands

/**
 *  @brief
 *    Get the maximum number of supported HLX zones.
 *
 *  @param[out]  aZones  The maximum number of HLX zones on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
Controller :: ZonesGetMax(Model::ZoneModel::IdentifierType &aZones) const
{
    Status lRetval;

    lRetval = mZonesController.GetZonesMax(aZones);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of a zone.
 *
 *  This queries the current state of the specified HLX server
 *  zone.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the identifier
 *                               of the zone to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneQuery(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval;

    lRetval = mZonesController.Query(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the zone model associated with specified zone
 *    identifier.
 *
 *  @param[in]   aZoneIdentifier  An immutable reference to the zone
 *                                model to obtain.
 *  @param[out]  aModel           A reference to an immutable pointer
 *                                by which to return the zone model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is smaller
 *                            or larger than supported.
 *
 */
Status
Controller :: ZoneGet(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ZoneModel *&aModel) const
{
    Status lRetval;

    lRetval = mZonesController.GetZone(aZoneIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the zone identifier with the specified name.
 *
 *  This attempts to lookup the zone identifier for the zone
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    zone to find an identifier
 *                                    for.
 *  @param[out]  aZoneIdentifier      A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No zone could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
Controller :: ZoneLookupIdentifier(const char *aName, ZoneModel::IdentifierType &aZoneIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.LookupIdentifier(aName, aZoneIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Balance Mutator Commands

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the left channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the left channel on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
Controller :: ZoneIncreaseBalanceLeft(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseBalanceLeft(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the right channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the right channel on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
Controller :: ZoneIncreaseBalanceRight(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseBalanceRight(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the zone stereophonic channel balance state.
 *
 *  This attempts to set the zone stereophonic channel balance state
 *  on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aBalance          An immutable reference to the
 *                                balance state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup balance
 *
 */
Status
Controller :: ZoneSetBalance(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                             const BalanceModel::BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetBalance(aZoneIdentifier, aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Equalizer Band Mutator Commands

/**
 *  @brief
 *    Set the level of an equalizer band of an zone equalizer.
 *
 *  This attempts to set the level of the specified equalizer band of
 *  the specified zone equalizer to the provided value on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                   const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                   const EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier, aEqualizerBandLevel);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the level of an equalizer band of an zone equalizer
 *    by one (1) unit.
 *
 *  This attempts to increase the level of the specified equalizer
 *  band of the specified zone equalizer by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneIncreaseEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                        const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the level of an equalizer band of an zone equalizer
 *    by one (1) unit.
 *
 *  This attempts to decrease the level of the specified equalizer
 *  band of the specified zone equalizer by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the zone equalizer
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the zone equalizer for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone equalizer or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneDecreaseEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                        const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.DecreaseEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Equalizer Preset Mutator Commands

/**
 *  @brief
 *    Set the zone equalizer preset state.
 *
 *  This attempts to set the zone equalizer preset state on the peer
 *  HLX server controller.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          for the zone for which
 *                                          to set the equalizer
 *                                          preset.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone or equalizer preset
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
Controller :: ZoneSetEqualizerPreset(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                     const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetEqualizerPreset(aZoneIdentifier, aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Tone Mutator Commands

/**
 *  @brief
 *    Decrease the bass level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the bass level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneDecreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.DecreaseBass(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the treble level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the treble level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to decrease the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneDecreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.DecreaseTreble(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the bass level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the bass level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the bass
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneIncreaseBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseBass(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the treble level of the zone tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the treble level of the zone tone
 *  equalizer by one (1) unit on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to increase the treble
 *                                        level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneIncreaseTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseTreble(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the bass level of the zone tone equalizer to the specified
 *    level.
 *
 *  This attempts to set the bass level of the zone tone equalizer to
 *  the specified level on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the bass level.
 *  @param[in]  aBass                     An immutable reference to the
 *                                        bass level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetBass(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const ToneModel::LevelType &aBass)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetBass(aZoneIdentifier, aBass);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the treble level of the zone tone equalizer to the specified
 *    level.
 *
 *  This attempts to set the treble level of the zone tone equalizer to
 *  the specified level on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the treble level.
 *  @param[in]  aTreble                   An immutable reference to the
 *                                        treble level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetTreble(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const ToneModel::LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetTreble(aZoneIdentifier, aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone High- and Low-pass Crossover Frequency Mutator Commands

/**
 *  @brief
 *    Set the crossover frequency of the zone highpass filter to the
 *    specified frequency.
 *
 *  This attempts to set the crossover frequency of the zone highpass
 *  filter to the specified frequency on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the highpass
 *                                        crossover frequency.
 *  @param[in]  aHighpassFrequency        An immutable reference to the
 *                                        highpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetHighpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                       const CrossoverModel::FrequencyType &aHighpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetHighpassCrossover(aZoneIdentifier, aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the crossover frequency of the zone lowpass filter to the
 *    specified frequency.
 *
 *  This attempts to set the crossover frequency of the zone lowpass
 *  filter to the specified frequency on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the lowpass
 *                                        crossover frequency.
 *  @param[in]  aLowpassFrequency         An immutable reference to the
 *                                        lowpass crossover frequency
 *                                        to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetLowpassCrossover(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                      const CrossoverModel::FrequencyType &aLowpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetLowpassCrossover(aZoneIdentifier, aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Mute Mutator Commands

/**
 *  @brief
 *    Clear (deassert) the zone volume mute state.
 *
 *  This attempts to clear (deassert) the zone volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to clear (deassert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
Controller :: ZoneClearMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.ClearMute(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set (assert) the zone volume mute state.
 *
 *  This attempts to set (assert) the zone volume mute state on
 *  the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set (assert)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
Controller :: ZoneSetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetMute(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the zone volume mute state to the specified state.
 *
 *  This attempts to set the zone volume mute state to the specified
 *  state on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                               zone for which to set the
 *                               volume mute state.
 *  @param[in]  aMute            An immutable reference to the
 *                               volume mute state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
Controller :: ZoneSetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const Model::VolumeModel::MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetMute(aZoneIdentifier, aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Toggle (flip) the zone volume mute state.
 *
 *  This attempts to toggle (flip) the zone volume mute state on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to toggle (flip)
 *                                the volume mute state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
Controller :: ZoneToggleMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.ToggleMute(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Name Mutator Commands

/**
 *  @brief
 *    Set the zone to the specified name.
 *
 *  This attempts to set the zone with the provided identifier to the
 *  specified name on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                               zone for which to set the
 *                               name.
 *  @param[in]  aName            A pointer to the null-terminated
 *                               C string to set the zone name
 *                               to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
Controller :: ZoneSetName(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                          const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetName(aZoneIdentifier, aName);
    nlEXPECT_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Sound Mode Mutator Commands

/**
 *  @brief
 *    Set the zone equalizer to the specified sound mode.
 *
 *  This attempts to set the zone equalizer to the specified sound
 *  mode on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier           An immutable reference to
 *                                        the zone identifier for
 *                                        which to set the equalizer
 *                                        sound mode.
 *  @param[in]  aSoundMode                An immutable reference to the
 *                                        equalizer sound mode to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
Controller :: ZoneSetSoundMode(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                               const SoundModel::SoundMode &aSoundMode)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetSoundMode(aZoneIdentifier, aSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Source Mutator Commands

/**
 *  @brief
 *    Set the zone source (input) state to the specified source.
 *
 *  This attempts to set the zone source (input) state to the
 *  specified source on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier    An immutable reference for the
 *                                 zone for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone or source (input)
 *                            identifiers are smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
Controller :: ZoneSetSource(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                            const SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetSource(aZoneIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Zone Volume Mutator Commands

/**
 *  @brief
 *    Set the zone volume level.
 *
 *  This attempts to set the zone volume level on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
Controller :: ZoneSetVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                            const VolumeModel::LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetVolume(aZoneIdentifier, aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone volume level by one (1) unit.
 *
 *  This attempts to increase the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                                zone for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
Controller :: ZoneIncreaseVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.IncreaseVolume(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the zone volume level by one (1) unit.
 *
 *  This attempts to decrease the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
Controller :: ZoneDecreaseVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.DecreaseVolume(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the zone volume locked state.
 *
 *  This attempts to set the zone volume locked state to the specified
 *  value on the peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                locked state.
 *  @param[in]  aLocked           An immutable reference to the volume
 *                                locked state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
Controller :: ZoneSetVolumeLocked(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                  const VolumeModel::FixedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = mZonesController.SetVolumeLocked(aZoneIdentifier, aLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Manager Delegate Methods

// MARK: Connection Manager Delegate Methods

// MARK: Connection Manager Resolve Methods

/**
 *  @brief
 *    Delegation from the connection manager that a host name will
 *    resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that will resolve.
 *
 */
void
Controller :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillResolve(*this, aHost);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name is
 *    resolving.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that is resolving.
 *
 */
void
Controller :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsResolving(*this, aHost);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name has
 *    resolved to an IP address.
 *
 *  @note
 *    This delegation may be called more than once for a
 *    resolution, once for each IP address the host name resolves
 *    to.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did resolve.
 *  @param[in]  aIPAddress          An immutable reference to an IP
 *                                  address that the host name
 *                                  resolved to.
 *
 */
void
Controller :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidResolve(*this, aHost, aIPAddress);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a host name did
 *    not resolve.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aHost               A pointer to a null-terminated C
 *                                  string containing the host
 *                                  name that did not resolve.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  resolution.
 *
 */
void
Controller :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotResolve(*this, aHost, aError);
    }
}

// MARK: Connection Manager Connect Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
Controller :: ConnectionManagerWillConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillConnect(*this, aURLRef, aTimeout);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server is connecting.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aTimeout            The timeout for the connection.
 *
 */
void
Controller :: ConnectionManagerIsConnecting(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsConnecting(*this, aURLRef, aTimeout);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *
 */
void
Controller :: ConnectionManagerDidConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidConnect(*this, aURLRef);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not connect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed connection.
 *
 */
void
Controller :: ConnectionManagerDidNotConnect(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotConnect(*this, aURLRef, aError);
    }
}

// MARK: Connection Manager Disconnect Methods

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server will disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *
 */
void
Controller :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillDisconnect(*this, aURLRef);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the disconnection.
 *
 */
void
Controller :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidDisconnect(*this, aURLRef, aError);
    }
}

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server did not disconnect.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aURLRef             The URL associated with the peer
 *                                  server.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the failed
 *                                  disconnection.
 *
 */
void
Controller :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotDisconnect(*this, aURLRef, aError);
    }
}

// MARK: Connection Manager Error Method

/**
 *  @brief
 *    Delegation from the connection manager that a connection to a
 *    peer server experienced an error.
 *
 *  @note
 *    This delegation may occur along with other delegations with
 *    respect to the same underlying event or cause.
 *
 *  @param[in]  aConnectionManager  A reference to the connection
 *                                  manager that issued the delegation.
 *  @param[in]  aRoles              An immutable reference to the roles
 *                                  in which the connection manager
 *                                  that issued the delegation is
 *                                  acting.
 *  @param[in]  aError              An immutable reference to the error
 *                                  associated with the event.
 *
 */
void
Controller :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerError(*this, aError);
    }
}

// MARK: Object Controller Basis Delegate Methods

// MARK: Object Controller Basis Error Delegate Methods

void
Controller :: ControllerError(Client::ObjectControllerBasis &aController, const Common::Error &aError)
{
    (void)aController;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerError(*this, aError);
    }
}

}; // namespace Application

}; // namespace Client

}; // namespace HLX
