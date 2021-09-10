/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements a derivable object for realizing a HLX
 *      groups controller, in a server.
 *
 */

#include "GroupsControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Detail
{

/**
 *  Internal collection of zone identifiers.
 *
 *  @private
 *
 */
typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

};

// Class-scoped Command Request Regular Expression Data

/**
 *  Class-scoped server group add zone command request regular
 *  expression.
 *
 */
Server::Command::Groups::AddZoneRequest         GroupsControllerBasis::kAddZoneRequest;

/**
 *  Class-scoped server group remove (clear) all zones command
 *  request regular expression.
 *
 */
Server::Command::Groups::ClearZonesRequest      GroupsControllerBasis::kClearZonesRequest;

/**
 *  Class-scoped server group decrease volume level command request regular
 *  expression.
 *
 */
Server::Command::Groups::DecreaseVolumeRequest  GroupsControllerBasis::kDecreaseVolumeRequest;

/**
 *  Class-scoped server group increase volume level command request regular
 *  expression.
 *
 */
Server::Command::Groups::IncreaseVolumeRequest  GroupsControllerBasis::kIncreaseVolumeRequest;

/**
 *  Class-scoped server group volume mute state command request regular
 *  expression.
 *
 */
Server::Command::Groups::MuteRequest            GroupsControllerBasis::kMuteRequest;

/**
 *  Class-scoped server group query command request regular expression.
 *
 */
Server::Command::Groups::QueryRequest           GroupsControllerBasis::kQueryRequest;

/**
 *  Class-scoped server group remove zone command request regular
 *  expression.
 *
 */
Server::Command::Groups::RemoveZoneRequest      GroupsControllerBasis::kRemoveZoneRequest;

/**
 *  Class-scoped server group set name command request regular
 *  expression.
 *
 */
Server::Command::Groups::SetNameRequest         GroupsControllerBasis::kSetNameRequest;

/**
 *  Class-scoped server group set source (input) command request
 *  regular expression.
 *
 */
Server::Command::Groups::SetSourceRequest       GroupsControllerBasis::kSetSourceRequest;

/**
 *  Class-scoped server group set volume level command request regular
 *  expression.
 *
 */
Server::Command::Groups::SetVolumeRequest       GroupsControllerBasis::kSetVolumeRequest;

/**
 *  Class-scoped server group toggle volume mute state command request regular
 *  expression.
 *
 */
Server::Command::Groups::ToggleMuteRequest      GroupsControllerBasis::kToggleMuteRequest;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the groups controller with the specified groups
 *  collection model and the maximum number of allowed groups.
 *
 *  @param[in]  aGroupsModel  A mutable reference to the groups
 *                            collection model to construct the
 *                            controller with. This is retained by a
 *                            weak pointer reference and,
 *                            consequently, must remain in scope for
 *                            the lifetime of the controller.
 *  @param[in]  aGroupsMax    An immutable reference to the maximum
 *                            number of allowed groups managed by the
 *                            controller.
 *
 */
GroupsControllerBasis :: GroupsControllerBasis(Model::GroupsModel &aGroupsModel,
                                               const Model::GroupsModel::IdentifierType &aGroupsMax) :
    Server::ObjectControllerBasis(),
    mGroupsModel(aGroupsModel),
    mGroupsMax(aGroupsMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
GroupsControllerBasis :: ~GroupsControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager.
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
GroupsControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Implementation

Status
GroupsControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kAddZoneRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kClearZonesRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kDecreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIncreaseVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kRemoveZoneRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSourceRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kToggleMuteRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    query request of all groups.
 *
 *  This handles and generates the server command response for a
 *  group query request of all groups.
 *
 *  @param[in,out]  aBuffer  A mutable reference to the shared
 *                           pointer into which the response is to be
 *                           generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the groups model has
 *                                  not been completely and successfully
 *                                  initialized.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
Status
GroupsControllerBasis :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;


    for (auto lGroupIdentifier = IdentifierModel::kIdentifierMin; lGroupIdentifier <= mGroupsMax; lGroupIdentifier++)
    {
        lRetval = HandleQueryReceived(lGroupIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    query request of a specific group.
 *
 *  This handles and generates the server command response for a
 *  group query request of a specific group.
 *
 *  @param[in]      aGroupIdentifier  An immutable reference
 *                                    to the identifier of the
 *                                    group queried.
 *  @param[in,out]  aBuffer           A mutable reference to
 *                                    the shared pointer into
 *                                    which the response is
 *                                    to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the groups model has
 *                                  not been completely and successfully
 *                                  initialized.
 *  @retval  -ERANGE                If a group identifier is smaller
 *                                  or larger than supported.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
Status
GroupsControllerBasis :: HandleQueryReceived(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const GroupModel *                     lGroupModel;
    const char *                           lName;
    Server::Command::Groups::NameResponse  lNameResponse;
    const uint8_t *                        lBuffer;
    size_t                                 lSize;
    size_t                                 lZoneCount;
    Status                                 lRetval;


    lRetval = mGroupsModel.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Name Response

    lRetval = lGroupModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aGroupIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Zone Membership Response

    lRetval = lGroupModel->GetZones(lZoneCount);
    nlEXPECT_SUCCESS(lRetval, done);

    if (lZoneCount > 0)
    {
        Detail::ZoneIdentifiers                  lZoneIdentifiers;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierCurrent;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierEnd;

        lZoneIdentifiers.resize(lZoneCount);

        lRetval = lGroupModel->GetZones(&lZoneIdentifiers[0], lZoneCount);
        nlREQUIRE_SUCCESS(lRetval, done);

        lZoneIdentifierCurrent = lZoneIdentifiers.begin();
        lZoneIdentifierEnd     = lZoneIdentifiers.end();

        while (lZoneIdentifierCurrent != lZoneIdentifierEnd)
        {
            Server::Command::Groups::ZoneResponse  lZoneResponse;

            lRetval = lZoneResponse.Init(aGroupIdentifier, *lZoneIdentifierCurrent);
            nlREQUIRE_SUCCESS(lRetval, done);

            lBuffer = lZoneResponse.GetBuffer();
            lSize = lZoneResponse.GetSize();

            lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
            nlREQUIRE_SUCCESS(lRetval, done);

            lZoneIdentifierCurrent++;
        }
    }

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    adjust volume level request.
 *
 *  This handles and generates the server command response for a
 *  group adjust volume level request.
 *
 *  @param[in]      aInputBuffer   An pointer to an immutable buffer
 *                                 containing the adjust volume level
 *                                 content to put into the response
 *                                 buffer.
 *  @param[in]      aInputSize     The size, in bytes, of the content
 *                                 pointed to by @a aInputBuffer.
 *  @param[in,out]  aOutputBuffer  A mutable reference to the shared
 *                                 pointer into which the response is to
 *                                 be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
GroupsControllerBasis :: HandleAdjustVolumeResponse(const uint8_t *aInputBuffer, const size_t &aInputSize, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
{
    Server::Command::Groups::AdjustVolumeResponse  lAdjustVolumeResponse;
    const uint8_t *                                lBuffer;
    size_t                                         lSize;
    Status                                         lRetval;


    // Strip the incoming request delimiters '[' and ']' by adjusting
    // the buffer pointer by one (1) and size by two (2).

    lRetval = lAdjustVolumeResponse.Init(aInputBuffer + 1, aInputSize - 2);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lAdjustVolumeResponse.GetBuffer();
    lSize = lAdjustVolumeResponse.GetSize();

    lRetval = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    set volume mute state request.
 *
 *  This handles and generates the server command response for a
 *  group set volume mute state request.
 *
 *  @param[in]      aGroupIdentifier  An immutable reference for the
 *                                    group for which the volume mute
 *                                    state was set.
 *  @param[in]      aMute             An immutable reference to the
 *                                    volume mute state that was set.
 *  @param[in,out]  aBuffer           A mutable reference to the shared
 *                                    pointer into which the response
 *                                    is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
GroupsControllerBasis :: HandleSetMuteResponse(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                               const VolumeModel::MuteType &aMute,
                                               ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Groups::SetMuteResponse  lSetMuteResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lRetval;

    lRetval = lSetMuteResponse.Init(aGroupIdentifier, aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lSetMuteResponse.GetBuffer();
    lSize = lSetMuteResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    set volume level request.
 *
 *  This handles and generates the server command response for a
 *  group set volume level request.
 *
 *  @param[in]      aGroupIdentifier  An immutable reference for the
 *                                    group for which the volume mute
 *                                    state was set.
 *  @param[in]      aVolume           An immutable reference to the
 *                                    volume level that was set.
 *  @param[in,out]  aBuffer           A mutable reference to the shared
 *                                    pointer into which the response
 *                                    is to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
GroupsControllerBasis :: HandleSetVolumeResponse(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                                                 const VolumeModel::LevelType &aVolume,
                                                 ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Groups::SetVolumeResponse  lSetVolumeResponse;
    const uint8_t *                             lBuffer;
    size_t                                      lSize;
    Status                                      lRetval;


    lRetval = lSetVolumeResponse.Init(aGroupIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lSetVolumeResponse.GetBuffer();
    lSize = lSetVolumeResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Handle and generate the server command response for a group
 *    toggle volume mute state request.
 *
 *  This handles and generates the server command response for a
 *  group toggle volume mute state request.
 *
 *  @param[in]      aInputBuffer   An pointer to an immutable buffer
 *                                 containing the toggle volume mute
 *                                 state content to put into the
 *                                 response buffer.
 *  @param[in]      aInputSize     The size, in bytes, of the content
 *                                 pointed to by @a aInputBuffer.
 *  @param[in,out]  aOutputBuffer  A mutable reference to the shared
 *                                 pointer into which the response is to
 *                                 be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
GroupsControllerBasis :: HandleToggleMuteResponse(const uint8_t *aInputBuffer,
                                                  const size_t &aInputSize,
                                                  ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
{
    Server::Command::Groups::ToggleMuteResponse  lToggleMuteResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lRetval;


    // Strip the incoming request delimiters '[' and ']' by adjusting
    // the buffer pointer by one (1) and size by two (2).

    lRetval = lToggleMuteResponse.Init(aInputBuffer + 1, aInputSize - 2);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lToggleMuteResponse.GetBuffer();
    lSize = lToggleMuteResponse.GetSize();

    lRetval = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
