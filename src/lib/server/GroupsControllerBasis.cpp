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
 *      This file implements an object for...
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

typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

};

// Request data

Server::Command::Groups::AddZoneRequest         GroupsControllerBasis::kAddZoneRequest;
Server::Command::Groups::ClearZonesRequest      GroupsControllerBasis::kClearZonesRequest;
Server::Command::Groups::DecreaseVolumeRequest  GroupsControllerBasis::kDecreaseVolumeRequest;
Server::Command::Groups::IncreaseVolumeRequest  GroupsControllerBasis::kIncreaseVolumeRequest;
Server::Command::Groups::MuteRequest            GroupsControllerBasis::kMuteRequest;
Server::Command::Groups::QueryRequest           GroupsControllerBasis::kQueryRequest;
Server::Command::Groups::RemoveZoneRequest      GroupsControllerBasis::kRemoveZoneRequest;
Server::Command::Groups::SetNameRequest         GroupsControllerBasis::kSetNameRequest;
Server::Command::Groups::SetSourceRequest       GroupsControllerBasis::kSetSourceRequest;
Server::Command::Groups::SetVolumeRequest       GroupsControllerBasis::kSetVolumeRequest;
Server::Command::Groups::ToggleMuteRequest      GroupsControllerBasis::kToggleMuteRequest;

/**
 *  @brief
 *    This is the class default constructor.
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

Status
GroupsControllerBasis :: HandleQueryReceived(const Model::GroupModel::IdentifierType &aGroupIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
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

Status
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

Status
GroupsControllerBasis :: HandleSetMuteResponse(const Model::GroupModel::IdentifierType &aGroupIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
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

Status
GroupsControllerBasis :: HandleSetVolumeResponse(const Model::GroupModel::IdentifierType &aGroupIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
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

Status
GroupsControllerBasis :: HandleToggleMuteResponse(const uint8_t *aInputBuffer, const size_t &aInputSize, ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
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
