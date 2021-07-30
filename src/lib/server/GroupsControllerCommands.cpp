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
 *      This file implements objects for HLX server group data model
 *      commands and their constituent requests and responses.
 *
 */

#include "GroupsControllerCommands.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Groups
{

static const char * const kGroupObject = "G";

const char * const AddZoneRequest::kRequestRegexp        = "G([[:digit:]]+)AO([[:digit:]]+)";
const char * const ClearZonesRequest::kRequestRegexp     = "GAR";
const char * const RemoveZoneRequest::kRequestRegexp     = "G([[:digit:]]+)RO([[:digit:]]+)";

const size_t AddZoneRequest::kExpectedMatches            = 3;
const size_t ClearZonesRequest::kExpectedMatches         = 1;
const size_t RemoveZoneRequest::kExpectedMatches         = 3;

// MARK: Observer Requests, Responses, and Commands

Status QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

Status QueryResponse :: Init(const Model::GroupModel::IdentifierType &aGroup)
{
    return (QueryResponseBasis::Init(kGroupObject, aGroup));
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

Status MuteRequest :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

Status ToggleMuteRequest :: Init(void)
{
    return (ToggleMuteRegularExpressionBasis::Init(*this));
}

Status SetMuteResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const Model::VolumeModel::MuteType &aMute)
{
    return (VolumeMuteResponseBasis::Init(kGroupObject, aGroup, aMute));
}

Status ToggleMuteResponse :: Init(const uint8_t *aBuffer, const size_t &aSize)
{
    return (ResponseBasis::Init(reinterpret_cast<const char *>(aBuffer), aSize));
}

// MARK: Name Mutator Requests, Responses, and Commands

Status SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

Status
NameResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const char * aName)
{
    return (NameSetResponseBasis::Init(kGroupObject, aGroup, aName));
}

Status
NameResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const char * aName, const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kGroupObject, aGroup, aName, aNameLength));
}

// MARK: Source Mutator Requests, Responses, and Commands

Status SetSourceRequest :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

Status SourceResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const Model::SourceModel::IdentifierType &aSource)
{
    return (SourceSetResponseBasis::Init(kGroupObject, aGroup, aSource));
}

// MARK: Volume Mutator Requests, Responses, and Commands

Status DecreaseVolumeRequest :: Init(void)
{
    return (DecreaseVolumeRegularExpressionBasis::Init(*this));
}

Status IncreaseVolumeRequest :: Init(void)
{
    return (IncreaseVolumeRegularExpressionBasis::Init(*this));
}

Status SetVolumeRequest :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

Status AdjustVolumeResponse :: Init(const uint8_t *aBuffer, const size_t &aSize)
{
    return (ResponseBasis::Init(reinterpret_cast<const char *>(aBuffer), aSize));
}

Status
SetVolumeResponse :: Init(const Model::GroupModel::IdentifierType &aGroup,
                          const Model::VolumeModel::LevelType &aVolume)
{
    return (VolumeResponseBasis::Init(kGroupObject, aGroup, aVolume));
}

// MARK: Zone (Membership) Mutator Requests, Responses, and Commands

Status AddZoneRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status RemoveZoneRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ZoneResponseBasis :: Init(const Model::GroupModel::IdentifierType &aGroup, char aOperation, const Model::ZoneModel::IdentifierType &aZone)
{
    static const char   kZoneObject = 'O';
    OutputStringStream  lGroupIdentifierStream;
    OutputStringStream  lZoneIdentifierStream;
    std::string         lBuffer;


    lGroupIdentifierStream << aGroup;
    lZoneIdentifierStream << aZone;

    // Compose the buffer.

    lBuffer = kGroupObject;
    lBuffer += lGroupIdentifierStream.str();
    lBuffer += aOperation;
    lBuffer += kZoneObject;
    lBuffer += lZoneIdentifierStream.str();

    return (ResponseBasis::Init(lBuffer.c_str(), lBuffer.size()));
}

Status AddZoneResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const Model::ZoneModel::IdentifierType &aZone)
{
    static const char kAddOperation = 'A';

    return (ZoneResponseBasis::Init(aGroup, kAddOperation, aZone));
}

Status RemoveZoneResponse :: Init(const Model::GroupModel::IdentifierType &aGroup, const Model::ZoneModel::IdentifierType &aZone)
{
    static const char kRemoveOperation = 'R';

    return (ZoneResponseBasis::Init(aGroup, kRemoveOperation, aZone));
}

Status ClearZonesRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ClearZonesResponse :: Init(void)
{
    const std::string lBuffer = std::string(kGroupObject) + "AR";

    return (ResponseBasis::Init(lBuffer.c_str()));
}

}; // namespace Groups

}; // namespace Command

}; // namespace Server

}; // namespace HLX
