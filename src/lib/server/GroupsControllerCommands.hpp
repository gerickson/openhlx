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
 *      This file defines objects for HLX server group data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef OPENHLXSERVERGROUPSCONTROLLERCOMMANDS_HPP
#define OPENHLXSERVERGROUPSCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandGroupsRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>

#include "CommandNameSetResponseBasis.hpp"
#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"
#include "CommandSourceResponseBases.hpp"
#include "CommandVolumeResponseBases.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Groups
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group data model query command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Groups::QueryRegularExpressionBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group data model query command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class QueryResponse :
    public QueryResponseBasis
{
public:
    QueryResponse(void) = default;
    virtual ~QueryResponse(void) = default;

    // Allow both the base and derived class initializers

    using QueryResponseBasis::Init;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup);
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group volume mute data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class MuteRequest :
    public RequestBasis,
    public Common::Command::Groups::MuteRegularExpressionBasis
{
public:
    MuteRequest(void) = default;
    virtual ~MuteRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group volume mute toggle data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ToggleMuteRequest :
    public RequestBasis,
    public Common::Command::Groups::ToggleMuteRegularExpressionBasis
{
public:
    ToggleMuteRequest(void) = default;
    virtual ~ToggleMuteRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group volume mute data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetMuteResponse :
    public VolumeMuteResponseBasis
{
public:
    SetMuteResponse(void) = default;
    virtual ~SetMuteResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const Model::VolumeModel::MuteType &aMute);
};

/**
 *  @brief
 *    An object for a HLX server group volume mute toggle data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ToggleMuteResponse :
    public ResponseBasis
{
public:
    ToggleMuteResponse(void) = default;
    virtual ~ToggleMuteResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const uint8_t *aBuffer, const size_t &aSize);
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group name data model property mutation
 *    command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetNameRequest :
    public RequestBasis,
    public Common::Command::Groups::NameRegularExpressionBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group name data model property mutation
 *    command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class NameResponse :
    public NameSetResponseBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const char * aName);
    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const char * aName, const size_t &aNameLength);
};

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group source (input) data model property
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetSourceRequest :
    public RequestBasis,
    public Common::Command::Groups::SourceRegularExpressionBasis
{
public:
    SetSourceRequest(void) = default;
    virtual ~SetSourceRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group source (input) data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SourceResponse :
    public SourceSetResponseBasis
{
public:
    SourceResponse(void) = default;
    virtual ~SourceResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const Model::SourceModel::IdentifierType &aSource);
};

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group volume level decrease data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class DecreaseVolumeRequest :
    public RequestBasis,
    public Common::Command::Groups::DecreaseVolumeRegularExpressionBasis
{
public:
    DecreaseVolumeRequest(void) = default;
    virtual ~DecreaseVolumeRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group volume level increase data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class IncreaseVolumeRequest :
    public RequestBasis,
    public Common::Command::Groups::IncreaseVolumeRegularExpressionBasis
{
public:
    IncreaseVolumeRequest(void) = default;
    virtual ~IncreaseVolumeRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group volume level data model property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetVolumeRequest :
    public RequestBasis,
    public Common::Command::Groups::VolumeRegularExpressionBasis
{
public:
    SetVolumeRequest(void) = default;
    virtual ~SetVolumeRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group volume level adjustment data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class AdjustVolumeResponse :
    public ResponseBasis
{
public:
    AdjustVolumeResponse(void) = default;
    virtual ~AdjustVolumeResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const uint8_t *aBuffer, const size_t &aSize);
};

/**
 *  @brief
 *    An object for a HLX server group volume level data model set property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetVolumeResponse :
    public VolumeResponseBasis
{
public:
    SetVolumeResponse(void) = default;
    virtual ~SetVolumeResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const Model::VolumeModel::LevelType &aVolume);
};

// MARK: Zone (Membership) Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server group zone membership addition data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class AddZoneRequest :
    public RequestBasis
{
public:
    AddZoneRequest(void) = default;
    virtual ~AddZoneRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group zone membership deletion data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class RemoveZoneRequest :
    public RequestBasis
{
public:
    RemoveZoneRequest(void) = default;
    virtual ~RemoveZoneRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for HLX server group zone membership data model
 *    property mutation command response buffers.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ZoneResponseBasis :
    public ResponseBasis
{
public:
    ZoneResponseBasis(void) = default;
    virtual ~ZoneResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, char aOperation, const Model::ZoneModel::IdentifierType &aZone);
};

/**
 *  @brief
 *    An object for a HLX server group zone membership addition data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class AddZoneResponse :
    public ZoneResponseBasis
{
public:
    AddZoneResponse(void) = default;
    virtual ~AddZoneResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const Model::ZoneModel::IdentifierType &aZone);
};

/**
 *  @brief
 *    An object for a HLX server group zone membership deletion data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class RemoveZoneResponse :
    public ZoneResponseBasis
{
public:
    RemoveZoneResponse(void) = default;
    virtual ~RemoveZoneResponse(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroup, const Model::ZoneModel::IdentifierType &aZone);
};

/**
 *  @brief
 *    An object for a HLX server group zone membership clear data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ClearZonesRequest :
    public RequestBasis
{
public:
    ClearZonesRequest(void) = default;
    virtual ~ClearZonesRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server group zone membership clear data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ClearZonesResponse :
    public ResponseBasis
{
public:
    ClearZonesResponse(void) = default;
    virtual ~ClearZonesResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

/**
 *  @brief
 *    An object for a HLX server group zone membership data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup group
 *
 */
class ZoneResponse :
    public AddZoneResponse
{
public:
    ZoneResponse(void) = default;
    virtual ~ZoneResponse(void) = default;
};

}; // namespace Groups

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERGROUPSCONTROLLERCOMMANDS_HPP
