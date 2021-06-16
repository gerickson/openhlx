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
 *      This file defines objects for HLX client group data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTGROUPSCONTROLLERCOMMANDS_HPP
#define HLXCLIENTGROUPSCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandNameSetRequestBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Client/CommandSourceRequestBases.hpp>
#include <OpenHLX/Client/CommandVolumeRequestBases.hpp>
#include <OpenHLX/Common/CommandGroupsRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Groups
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client group data model query command request
 *    buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class QueryRequest :
    virtual public Client::Command::RequestBasis,
    public Client::Command::QueryRequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group data model query command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::Groups::QueryRegularExpressionBasis
{
public:
    QueryResponse(void) = default;
    virtual ~QueryResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group data model query command request
 *    / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group source data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SourceResponse :
    public ResponseBasis,
    public Common::Command::Groups::SourceRegularExpressionBasis
{
public:
    SourceResponse(void) = default;
    virtual ~SourceResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client group volume mute set/assert data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class SetMuteRequest :
    public VolumeMuteSetRequestBasis
{
public:
    SetMuteRequest(void) = default;
    virtual ~SetMuteRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group volume mute data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetMuteResponse :
    public ResponseBasis,
    public Common::Command::Groups::MuteRegularExpressionBasis
{
public:
    SetMuteResponse(void) = default;
    virtual ~SetMuteResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group volume mute set/assert data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetMute :
    public ExchangeBasis
{
public:
    SetMute(void) = default;
    virtual ~SetMute(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    SetMuteRequest  mRequest;
    SetMuteResponse mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group volume mute clear/deassert data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class ClearMuteRequest :
    public VolumeMuteClearRequestBasis
{
public:
    ClearMuteRequest(void) = default;
    virtual ~ClearMuteRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group volume mute clear/deassert data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class ClearMute :
    public ExchangeBasis
{
public:
    ClearMute(void) = default;
    virtual ~ClearMute(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    ClearMuteRequest  mRequest;
    SetMuteResponse   mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group volume mute toggle data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class ToggleMuteResponse :
    public ResponseBasis,
    public Common::Command::Groups::ToggleMuteRegularExpressionBasis
{
public:
    ToggleMuteResponse(void) = default;
    virtual ~ToggleMuteResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group volume mute toggle data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class ToggleMuteRequest :
    public VolumeMuteToggleRequestBasis
{
public:
    ToggleMuteRequest(void) = default;
    virtual ~ToggleMuteRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group volume mute toggle data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class ToggleMute :
    public ExchangeBasis
{
public:
    ToggleMute(void) = default;
    virtual ~ToggleMute(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    ToggleMuteRequest  mRequest;
    ToggleMuteResponse mResponse;
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client group name data model property mutation
 *    command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class NameResponse :
    public ResponseBasis,
    public Common::Command::Groups::NameRegularExpressionBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group name data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetNameRequest :
    public NameSetRequestBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const char *aName);
};

/**
 *  @brief
 *    An object for a HLX client group name data model property mutation
 *    command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetName :
    public ExchangeBasis
{
public:
    SetName(void) = default;
    virtual ~SetName(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const char *aName);

private:
    SetNameRequest mRequest;
    NameResponse   mResponse;
};

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client group source (input) data model property
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetSourceRequest :
    public SourceSetRequestBasis
{
public:
    SetSourceRequest(void) = default;
    virtual ~SetSourceRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group source data model property
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class SetSource :
    public ExchangeBasis
{
public:
    SetSource(void) = default;
    virtual ~SetSource(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

private:
    SetSourceRequest  mRequest;
    SourceResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group volume level data model set property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class SetVolumeResponse :
    public ResponseBasis,
    public Common::Command::Groups::VolumeRegularExpressionBasis
{
public:
    SetVolumeResponse(void) = default;
    virtual ~SetVolumeResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group volume level data model property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class SetVolumeRequest :
    public VolumeSetRequestBasis
{
public:
    SetVolumeRequest(void) = default;
    virtual ~SetVolumeRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                        const Model::VolumeModel::LevelType &aLevel);
};

/**
 *  @brief
 *    An object for a HLX client group volume level data model property
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class SetVolume :
    public ExchangeBasis
{
public:
    SetVolume(void) = default;
    virtual ~SetVolume(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                        const Model::VolumeModel::LevelType &aLevel);

private:
    SetVolumeRequest   mRequest;
    SetVolumeResponse  mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group volume level increase data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class IncreaseVolumeResponse :
    public ResponseBasis,
    public Common::Command::Groups::IncreaseVolumeRegularExpressionBasis
{
public:
    IncreaseVolumeResponse(void) = default;
    virtual ~IncreaseVolumeResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group volume level increase data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class IncreaseVolumeRequest :
    public VolumeIncreaseRequestBasis
{
public:
    IncreaseVolumeRequest(void) = default;
    virtual ~IncreaseVolumeRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group volume level increase data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class IncreaseVolume :
    public ExchangeBasis
{
public:
    IncreaseVolume(void) = default;
    virtual ~IncreaseVolume(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    IncreaseVolumeRequest   mRequest;
    IncreaseVolumeResponse  mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group volume level decrease data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class DecreaseVolumeResponse :
    public ResponseBasis,
    public Common::Command::Groups::DecreaseVolumeRegularExpressionBasis
{
public:
    DecreaseVolumeResponse(void) = default;
    virtual ~DecreaseVolumeResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group volume level decrease data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class DecreaseVolumeRequest :
    public VolumeDecreaseRequestBasis
{
public:
    DecreaseVolumeRequest(void) = default;
    virtual ~DecreaseVolumeRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group volume level decrease data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *  @ingroup volume
 *
 */
class DecreaseVolume :
    public ExchangeBasis
{
public:
    DecreaseVolume(void) = default;
    virtual ~DecreaseVolume(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier);

private:
    DecreaseVolumeRequest   mRequest;
    DecreaseVolumeResponse  mResponse;
};

// MARK: Zone (Membership) Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A derivable object for a HLX client group zone membership data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class ZoneRequestBasis :
    public RequestBasis
{
protected:
    ZoneRequestBasis(void) = default;
    virtual ~ZoneRequestBasis(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, char aOperation, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group zone membership addition data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class AddZoneRequest :
    public ZoneRequestBasis
{
public:
    AddZoneRequest(void) = default;
    virtual ~AddZoneRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group zone membership deletion data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class RemoveZoneRequest :
    public ZoneRequestBasis
{
public:
    RemoveZoneRequest(void) = default;
    virtual ~RemoveZoneRequest(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client group zone membership data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class ZoneResponse :
    public ResponseBasis
{
public:
    ZoneResponse(void) = default;
    virtual ~ZoneResponse(void) = default;

    Common::Status Init(void);

    static const char * const kResponseRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client group zone membership addition data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class AddZone :
    public ExchangeBasis
{
public:
    AddZone(void) = default;
    virtual ~AddZone(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    AddZoneRequest mRequest;
    ZoneResponse   mResponse;
};

/**
 *  @brief
 *    An object for a HLX client group zone membership deletion data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup group
 *
 */
class RemoveZone :
    public ExchangeBasis
{
public:
    RemoveZone(void) = default;
    virtual ~RemoveZone(void) = default;

    Common::Status Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    RemoveZoneRequest mRequest;
    ZoneResponse      mResponse;
};

}; // namespace Groups

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTGROUPSCONTROLLERCOMMANDS_HPP
