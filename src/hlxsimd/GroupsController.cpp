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
 *      This file implements the groups server controller for the
 *      Audio Authority HLX client.
 *
 *      The groups server controller is a somewhat stateless
 *      controller relative to its peers and works in concert with the
 *      zones server controller, orchestrated by the parent server
 *      controller due to the fact that Audio Authority did not
 *      implement the groups server-side controller in the same way as
 *      other server-side controllers.
 *
 *      Specifically, when a group's source select, volume, or mute
 *      command is received, synchronous state change notifications
 *      for the constituent zones in the group would have been sent
 *      in the command response along with the command completion
 *      itself. For example:
 *
 *        <Group j Mute or Volume or Source Command Request>
 *        <Zone i Mute or Volume or Source State Change>
 *        ...
 *        <Zone n Mute or Volume or Source State Change>
 *        <Group Mute or Volume or Source Command Response>
 *
 *      However, instead, all that we get in practice is:
 *
 *        <Group j Mute or Volume or Source Command Request>
 *        <Group j Mute or Volume or Source Command Response>
 *
 *  Leaving it to the parent server controller to extract zone
 *  membership for the relevant group from the group server controller
 *  and to then to apply the mute, volume, or source changes
 *  to the server side zone data members.
 *
 */

#include "GroupsController.hpp"

#include <memory>
#include <vector>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <CommandManager.hpp>
#include <GroupsControllerDelegate.hpp>
#include <NameModelDefaults.hpp>
#include <SourcesController.hpp>
#include <Utilities.hpp>
#include <ZonesController.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

namespace Detail
{

typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

};

/**
 *  @brief
 *    A object for representing default data for a HLX group data
 *    model.
 *
 *  @private
 *
 */
struct GroupModelDefaults
{
    Defaults::NameModel mName;
};

static const GroupModelDefaults kGroupModelDefaults[10] = {
    { "Group Name 1"  },
    { "Group Name 2"  },
    { "Group Name 3"  },
    { "Group Name 4"  },
    { "Group Name 5"  },
    { "Group Name 6"  },
    { "Group Name 7"  },
    { "Group Name 8"  },
    { "Group Name 9"  },
    { "Group Name 10" }
};

static CFStringRef      kGroupsSchemaKey = CFSTR("Groups");
static CFStringRef      kNameSchemaKey   = CFSTR("Name");
static CFStringRef      kZonesSchemaKey  = CFSTR("Zones");


/**
 *  @brief
 *    This is the class default constructor.
 *
 */
GroupsController :: GroupsController(void) :
    Common::GroupsControllerBasis(),
    Server::GroupsControllerBasis(Common::GroupsControllerBasis::mGroups,
                                  Common::GroupsControllerBasis::kGroupsMax),
    Simulator::ContainerControllerBasis(),
    Simulator::ObjectControllerBasis(),
    mDelegate(nullptr)
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

Status
GroupsController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kAddZoneRequest,
            GroupsController::AddZoneRequestReceivedHandler
        },

        {
            kClearZonesRequest,
            GroupsController::ClearZonesRequestReceivedHandler
        },

        {
            kDecreaseVolumeRequest,
            GroupsController::DecreaseVolumeRequestReceivedHandler
        },

        {
            kIncreaseVolumeRequest,
            GroupsController::IncreaseVolumeRequestReceivedHandler
        },

        {
            kMuteRequest,
            GroupsController::MuteRequestReceivedHandler
        },

        {
            kQueryRequest,
            GroupsController::QueryRequestReceivedHandler
        },

        {
            kRemoveZoneRequest,
            GroupsController::RemoveZoneRequestReceivedHandler
        },

        {
            kSetNameRequest,
            GroupsController::SetNameRequestReceivedHandler
        },

        {
            kSetSourceRequest,
            GroupsController::SetSourceRequestReceivedHandler
        },

        {
            kSetVolumeRequest,
            GroupsController::SetVolumeRequestReceivedHandler
        },

        {
            kToggleMuteRequest,
            GroupsController::ToggleMuteRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ObjectControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                               &lRequestHandlers[lRequestHandlerCount],
                                                               this,
                                                               aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
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
GroupsController :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::GroupsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::GroupsControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

GroupsControllerDelegate * GroupsController :: GetDelegate(void) const
{
    return (mDelegate);
}

Status GroupsController :: SetDelegate(GroupsControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

Status GroupsController :: HandleSetMute(const IdentifierType &aGroupIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    GroupModel *                       lGroupModel;
    Status                             lRetval;


    lRetval = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lGroupModel->SetMute(aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = HandleSetMuteResponse(aGroupIdentifier, aMute, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status GroupsController :: HandleAdjustVolumeReceived(const uint8_t *aInputBuffer, const size_t &aInputSize, const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aAdjustment, ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
{
    GroupModel *                       lGroupModel;
    Status                             lRetval;


    nlREQUIRE_ACTION(aAdjustment != 0, done, lRetval = -EINVAL);

    lRetval = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = OnAdjustVolume(aGroupIdentifier, *lGroupModel, aAdjustment);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleAdjustVolumeResponse(aInputBuffer, aInputSize, aOutputBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status GroupsController :: HandleSetVolumeReceived(const IdentifierType &aGroupIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    GroupModel *                       lGroupModel;
    Status                             lRetval;


    lRetval = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = OnSetVolume(aGroupIdentifier, *lGroupModel, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleSetVolumeResponse(aGroupIdentifier, aVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void
GroupsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status          lStatus;


    (void)aConnection;

    lStatus = HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void GroupsController :: ResetToDefaultConfiguration(void)
{
    IdentifierType  lGroupIdentifier;
    GroupModel *    lGroupModel;
    Status          lStatus;


    for (lGroupIdentifier = IdentifierModel::kIdentifierMin; lGroupIdentifier <= kGroupsMax; lGroupIdentifier++)
    {
        const GroupModelDefaults &lGroupModelDefaults = kGroupModelDefaults[lGroupIdentifier - 1];

        lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lGroupModel->SetName(lGroupModelDefaults.mName.mName);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lGroupModel->ClearZones();
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return;
}

Status GroupsController :: GroupZonesLoadFromBackupConfiguration(CFDictionaryRef aGroupDictionary, Model::GroupModel &aGroupModel)
{
    CFArrayRef   lZonesArray = nullptr;
    CFIndex      lZoneCount;
    Status       lRetval     = kStatus_Success;

    lZonesArray = static_cast<CFArrayRef>(CFDictionaryGetValue(aGroupDictionary, kZonesSchemaKey));
    nlREQUIRE_ACTION(lZonesArray != nullptr, done, lRetval = kError_MissingConfiguration);

    lZoneCount = CFArrayGetCount(lZonesArray);
    nlEXPECT(lZoneCount > 0, done);

    for (auto i = 0; i < lZoneCount; i++)
    {
        CFNumberRef               lZoneRef = nullptr;
        ZoneModel::IdentifierType lZoneIdentifier;
        bool                      lStatus;

        lZoneRef = static_cast<CFNumberRef>(CFArrayGetValueAtIndex(lZonesArray, i));
        nlREQUIRE_ACTION(lZoneRef != nullptr, done, lRetval = kError_InvalidConfiguration);

        lStatus = CFUNumberGetValue(lZoneRef, lZoneIdentifier);
        nlREQUIRE_ACTION(lStatus, done, lRetval = kError_InvalidConfiguration);

        lRetval = aGroupModel.AddZone(lZoneIdentifier);
        nlREQUIRE(lRetval >= kStatus_Success, done);

        if (lRetval == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return (lRetval);
}

Status GroupsController :: ElementLoadFromBackupConfiguration(CFDictionaryRef aGroupsDictionary, const IdentifierType &aGroupIdentifier)
{
    CFDictionaryRef                  lGroupDictionary = nullptr;
    CFStringRef                      lGroupIdentifierKey = nullptr;
    CFStringRef                      lGroupName = nullptr;
    GroupModel *                     lGroupModel;
    Status                           lRetval = kStatus_Success;

    // Attempt to form the group identifier key.

    lGroupIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aGroupIdentifier);
    nlREQUIRE_ACTION(lGroupIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    // Attempt to retrieve the group dictionary.

    lGroupDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aGroupsDictionary, lGroupIdentifierKey));
    nlREQUIRE_ACTION(lGroupDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the group name string.

    lGroupName = static_cast<CFStringRef>(CFDictionaryGetValue(lGroupDictionary, kNameSchemaKey));
    nlREQUIRE_ACTION(lGroupName != nullptr, done, lRetval = kError_MissingConfiguration);

    lRetval = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lGroupModel->SetName(CFString(lGroupName).GetCString());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Attempt to retrieve the group zones membership.

    lRetval = GroupZonesLoadFromBackupConfiguration(lGroupDictionary, *lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    CFURelease(lGroupIdentifierKey);

    return (lRetval);
}

Status GroupsController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    ContainerControllerBasis::LoadFromBackupConfiguration(aBackupDictionary,
                                                          kGroupsMax,
                                                          kGroupsSchemaKey);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status GroupsController :: GroupZonesSaveToBackupConfiguration(CFMutableDictionaryRef aGroupDictionary, const GroupModel &aGroupModel)
{
    CFMutableArrayRef   lZonesArray = nullptr;
    size_t              lZoneCount;
    Status              lRetval = kStatus_Success;


    lZonesArray = CFArrayCreateMutable(kCFAllocatorDefault,
                                       0,
                                       &kCFTypeArrayCallBacks);
    nlREQUIRE_ACTION(lZonesArray != nullptr, done, lRetval = -ENOMEM);

    lRetval = aGroupModel.GetZones(lZoneCount);
    nlEXPECT_SUCCESS(lRetval, done);

    if (lZoneCount > 0)
    {
        Detail::ZoneIdentifiers                  lZoneIdentifiers;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierCurrent;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierEnd;

        lZoneIdentifiers.resize(lZoneCount);

        lRetval = aGroupModel.GetZones(&lZoneIdentifiers[0], lZoneCount);
        nlREQUIRE_SUCCESS(lRetval, done);

        lZoneIdentifierCurrent = lZoneIdentifiers.begin();
        lZoneIdentifierEnd     = lZoneIdentifiers.end();

        while (lZoneIdentifierCurrent != lZoneIdentifierEnd)
        {
            CFNumberRef  lZoneIdentifierNumber = nullptr;

            lZoneIdentifierNumber = CFUNumberCreate(kCFAllocatorDefault, *lZoneIdentifierCurrent);
            nlREQUIRE_ACTION(lZoneIdentifierNumber != nullptr, done, lRetval = -ENOMEM);

            CFArrayAppendValue(lZonesArray, lZoneIdentifierNumber);

            CFURelease(lZoneIdentifierNumber);

            lZoneIdentifierCurrent++;
        }
    }

    CFDictionaryAddValue(aGroupDictionary, kZonesSchemaKey, lZonesArray);

 done:
    CFURelease(lZonesArray);

    return (lRetval);
}

Status GroupsController :: ElementSaveToBackupConfiguration(CFMutableDictionaryRef aGroupsDictionary, const IdentifierType &aGroupIdentifier) const
{
    CFStringRef                   lGroupIdentifierKey = nullptr;
    CFMutableDictionaryRef        lGroupDictionary = nullptr;
    const GroupModel *            lGroupModel;
    const char *                  lName;
    Status                        lRetval = kStatus_Success;


    lRetval = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lGroupIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aGroupIdentifier);
    nlREQUIRE_ACTION(lGroupIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    lGroupDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                 0,
                                                 &kCFCopyStringDictionaryKeyCallBacks,
                                                 &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lGroupDictionary != nullptr, done, lRetval = -ENOMEM);

    // Name

    lRetval = lGroupModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetCString(lGroupDictionary, kNameSchemaKey, lName);

    // Zones

    lRetval = GroupZonesSaveToBackupConfiguration(lGroupDictionary, *lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the newly-created group dictionary into the backup
    // configuration dictionary, keyed for this group identifier.

    CFDictionaryAddValue(aGroupsDictionary, lGroupIdentifierKey, lGroupDictionary);

 done:
    CFURelease(lGroupIdentifierKey);
    CFURelease(lGroupDictionary);

    return (lRetval);
}

void GroupsController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    ContainerControllerBasis::SaveToBackupConfiguration(aBackupDictionary,
                                                        kGroupsMax,
                                                        kGroupsSchemaKey);
}

// MARK: Command Request Completion Handlers

void GroupsController :: AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                            lGroupIdentifier;
    ZoneModel::IdentifierType                 lZoneIdentifier;
    GroupModel *                              lGroupModel;
    Server::Command::Groups::AddZoneResponse  lAddZoneResponse;
    ConnectionBuffer::MutableCountedPointer   lResponseBuffer;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::AddZoneRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // GetGroup below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                 Common::Utilities::Distance(aMatches.at(1)),
                                                 lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Zone Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                 Common::Utilities::Distance(aMatches.at(1)),
                                                 lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ZonesController::ValidateIdentifier(lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lGroupModel->AddZone(lZoneIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lAddZoneResponse.Init(lGroupIdentifier, lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lAddZoneResponse.GetBuffer();
    lSize = lAddZoneResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                               lGroupIdentifier;
    GroupModel *                                 lGroupModel;
    Server::Command::Groups::ClearZonesResponse  lClearZonesResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::ClearZonesRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lGroupIdentifier = IdentifierModel::kIdentifierMin; lGroupIdentifier <= kGroupsMax; lGroupIdentifier++)
    {
        lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lGroupModel->ClearZones();
        nlREQUIRE(lStatus >= kStatus_Success, done);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

    lStatus = lClearZonesResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lClearZonesResponse.GetBuffer();
    lSize = lClearZonesResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the decrease volume request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const VolumeModel::LevelType      kAdjustment = -1;
    IdentifierType                           lGroupIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::DecreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleAdjustVolumeReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustVolumeReceived(aBuffer, aSize, lGroupIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the increase volume request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const VolumeModel::LevelType      kAdjustment = 1;
    IdentifierType                           lGroupIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::IncreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleAdjustVolumeReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustVolumeReceived(aBuffer, aSize, lGroupIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the mute request functions quite differently
 * in the server group controller than it does in the server zone
 * controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    const char *                             lMutep;
    IdentifierType                           lGroupIdentifier;
    VolumeModel::MuteType                    lMute;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::MuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleSetMute below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetMute(lGroupIdentifier, lMute, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    Server::Command::Groups::QueryResponse   lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lResponse.Init(lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lGroupIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                               lGroupIdentifier;
    ZoneModel::IdentifierType                    lZoneIdentifier;
    GroupModel *                                 lGroupModel;
    Server::Command::Groups::RemoveZoneResponse  lRemoveZoneResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::RemoveZoneRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // GetGroup below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                 Common::Utilities::Distance(aMatches.at(1)),
                                                 lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Zone Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                 Common::Utilities::Distance(aMatches.at(1)),
                                                 lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ZonesController::ValidateIdentifier(lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lGroupModel->RemoveZone(lZoneIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lRemoveZoneResponse.Init(lGroupIdentifier, lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lRemoveZoneResponse.GetBuffer();
    lSize = lRemoveZoneResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    const char *                             lName;
    size_t                                   lNameSize;
    GroupModel *                             lGroupModel;
    Server::Command::Groups::NameResponse    lNameResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // GetGroup below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the group model associated with the parsed group
    // identifier. This will include a range check on the group
    // identifier.

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lGroupModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lNameResponse.Init(lGroupIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the set source request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    SourceModel::IdentifierType              lSourceIdentifier;
    GroupModel *                             lGroupModel;
    Server::Command::Groups::SourceResponse  lSourceResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::SetSourceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // GetGroup below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SourcesController::ValidateIdentifier(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = OnSetSource(lGroupIdentifier, *lGroupModel, lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lSourceResponse.Init(lGroupIdentifier, lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSourceResponse.GetBuffer();
    lSize = lSourceResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the set volume request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    VolumeModel::LevelType                   lVolume;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::SetVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Level
    //
    // The validity of the volume level will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetVolumeReceived(lGroupIdentifier, lVolume, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the toggle mute request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    GroupModel *                             lGroupModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::ToggleMuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // GetGroup below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = OnToggleMute(lGroupIdentifier, *lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleToggleMuteResponse(aBuffer, aSize, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Delegate Fanout Methods

Status GroupsController :: OnAdjustVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->ShouldAdjustVolume(*this, aGroupIdentifier, aGroupModel, aAdjustment);
    }

    return (lRetval);
}

Status GroupsController :: OnSetMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->ShouldSetMute(*this, aGroupIdentifier, aGroupModel, aMute);
    }

    return (lRetval);
}

Status GroupsController :: OnSetSource(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->ShouldSetSource(*this, aGroupIdentifier, aGroupModel, aSourceIdentifier);
    }

    return (lRetval);
}

Status GroupsController :: OnSetVolume(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->ShouldSetVolume(*this, aGroupIdentifier, aGroupModel, aVolume);
    }

    return (lRetval);
}

Status GroupsController :: OnToggleMute(const IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->ShouldToggleMute(*this, aGroupIdentifier, aGroupModel);
    }

    return (lRetval);
}

// MARK: Command Request Handler Trampolines

void GroupsController :: AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->AddZoneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ClearZonesRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->RemoveZoneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToggleMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
