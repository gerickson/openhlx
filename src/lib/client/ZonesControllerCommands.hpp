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
 *      This file defines objects for HLX client zone data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTZONESCONTROLLERCOMMANDS_HPP
#define HLXCLIENTZONESCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandBalanceRequestBases.hpp>
#include <OpenHLX/Client/CommandEqualizerBandRequestBases.hpp>
#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandNameSetRequestBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Client/CommandSourceRequestBases.hpp>
#include <OpenHLX/Client/CommandVolumeRequestBases.hpp>
#include <OpenHLX/Common/CommandToneBufferBasis.hpp>
#include <OpenHLX/Common/CommandZonesRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Zones
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone data model query command request
 *    buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryRequest :
    virtual public Client::Command::RequestBasis,
    public Client::Command::QueryRequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone data model query command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::Zones::QueryRegularExpressionBasis
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
 *    An object for a HLX client zone data model query command request
 *    / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class MuteResponse :
    public ResponseBasis,
    public Common::Command::Zones::MuteRegularExpressionBasis
{
public:
    MuteResponse(void) = default;
    virtual ~MuteResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone source data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceResponse :
    public ResponseBasis,
    public Common::Command::Zones::SourceRegularExpressionBasis
{
public:
    SourceResponse(void) = default;
    virtual ~SourceResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client all-zones source data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceAllResponse :
    public ResponseBasis,
    public Common::Command::Zones::SourceAllRegularExpressionBasis
{
public:
    SourceAllResponse(void) = default;
    virtual ~SourceAllResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone volume level data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeResponse :
    public ResponseBasis,
    public Common::Command::Zones::VolumeRegularExpressionBasis
{
public:
    VolumeResponse(void) = default;
    virtual ~VolumeResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client all-zones volume level data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeAllResponse :
    public ResponseBasis,
    public Common::Command::Zones::VolumeAllRegularExpressionBasis
{
public:
    VolumeAllResponse(void) = default;
    virtual ~VolumeAllResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: Balance Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance data
 *    model property mutation command request buffer.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetBalanceRequest :
    public BalanceSetRequestBasis
{
public:
    SetBalanceRequest(void) = default;
    virtual ~SetBalanceRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance);
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance data
 *    model property mutation command response regular expression.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class BalanceResponse :
    public ResponseBasis,
    public Common::Command::Zones::BalanceRegularExpressionBasis
{
public:
    BalanceResponse(void) = default;
    virtual ~BalanceResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance data
 *    model property mutation command request / response pair.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetBalance :
    public Command::ExchangeBasis
{
public:
    SetBalance(void) = default;
    virtual ~SetBalance(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance);

private:
    SetBalanceRequest  mRequest;
    BalanceResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance
 *    adjust-towards-the-left-channel data model property mutation
 *    command request buffer.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBalanceLeftRequest :
    public BalanceAdjustRequestBasis
{
public:
    IncreaseBalanceLeftRequest(void) = default;
    virtual ~IncreaseBalanceLeftRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance
 *    adjust-towards-the-left-channel data model property mutation
 *    command request / response pair.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBalanceLeft :
    public ExchangeBasis
{
public:
    IncreaseBalanceLeft(void) = default;
    virtual ~IncreaseBalanceLeft(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    IncreaseBalanceLeftRequest  mRequest;
    BalanceResponse             mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance
 *    adjust-towards-the-right-channel data model property mutation
 *    command request buffer.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBalanceRightRequest :
    public BalanceAdjustRequestBasis
{
public:
    IncreaseBalanceRightRequest(void) = default;
    virtual ~IncreaseBalanceRightRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone stereophonic channel balance
 *    adjust-towards-the-right-channel data model property mutation
 *    command request / response pair.
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBalanceRight :
    public ExchangeBasis
{
public:
    IncreaseBalanceRight(void) = default;
    virtual ~IncreaseBalanceRight(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    IncreaseBalanceRightRequest  mRequest;
    BalanceResponse              mResponse;
};

// MARK: Equalizer Band Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level data
 *    model property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerBandResponse :
    public ResponseBasis,
    public Common::Command::Zones::EqualizerBandLevelRegularExpressionBasis
{
public:
    EqualizerBandResponse(void) = default;
    virtual ~EqualizerBandResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerBandRequest :
    public EqualizerBandSetRequestBasis
{
public:
    SetEqualizerBandRequest(void) = default;
    virtual ~SetEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerBand :
    public ExchangeBasis
{
public:
    SetEqualizerBand(void) = default;
    virtual ~SetEqualizerBand(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);

private:
    SetEqualizerBandRequest  mRequest;
    EqualizerBandResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level
 *    increase data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseEqualizerBandRequest :
    public EqualizerBandIncreaseRequestBasis
{
public:
    IncreaseEqualizerBandRequest(void) = default;
    virtual ~IncreaseEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level
 *    increase data model property mutation command request / response
 *    pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseEqualizerBand :
    public ExchangeBasis
{
public:
    IncreaseEqualizerBand(void) = default;
    virtual ~IncreaseEqualizerBand(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

private:
    IncreaseEqualizerBandRequest  mRequest;
    EqualizerBandResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level
 *    decrease data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseEqualizerBandRequest :
    public EqualizerBandDecreaseRequestBasis
{
public:
    DecreaseEqualizerBandRequest(void) = default;
    virtual ~DecreaseEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer band level
 *    decrease data model property mutation command request / response
 *    pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseEqualizerBand :
    public ExchangeBasis
{
public:
    DecreaseEqualizerBand(void) = default;
    virtual ~DecreaseEqualizerBand(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

private:
    DecreaseEqualizerBandRequest  mRequest;
    EqualizerBandResponse         mResponse;
};

// MARK: (Sound Mode) Equalizer Preset Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer preset data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerPresetResponse :
    public ResponseBasis,
    public Common::Command::Zones::EqualizerPresetRegularExpressionBasis
{
public:
    EqualizerPresetResponse(void) = default;
    virtual ~EqualizerPresetResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer preset data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerPresetRequest :
    public RequestBasis,
    public Common::Command::EqualizerPresetBufferBasis
{
public:
    SetEqualizerPresetRequest(void) = default;
    virtual ~SetEqualizerPresetRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                        const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound equalizer preset data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerPreset :
    public ExchangeBasis
{
public:
    SetEqualizerPreset(void) = default;
    virtual ~SetEqualizerPreset(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                        const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

private:
    SetEqualizerPresetRequest  mRequest;
    EqualizerPresetResponse    mResponse;
};

// MARK: (Sound Mode) Highpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound highpass crossover
 *    frequency data model property mutation command response regular
 *    expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class HighpassCrossoverResponse :
    public ResponseBasis,
    public Common::Command::Zones::HighpassCrossoverRegularExpressionBasis
{
public:
    HighpassCrossoverResponse(void) = default;
    virtual ~HighpassCrossoverResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound highpass crossover frequency
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetHighpassCrossoverRequest :
    public RequestBasis,
    public Common::Command::EqualizerHighpassCrossoverBufferBasis
{
public:
    SetHighpassCrossoverRequest(void) = default;
    virtual ~SetHighpassCrossoverRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound highpass crossover
 *    frequency data model property mutation command request /
 *    response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetHighpassCrossover :
    public ExchangeBasis
{
public:
    SetHighpassCrossover(void) = default;
    virtual ~SetHighpassCrossover(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);

private:
    SetHighpassCrossoverRequest  mRequest;
    HighpassCrossoverResponse    mResponse;
};

// MARK: (Sound Mode) Lowpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound lowpass crossover frequency
 *    data model property mutation command response regular
 *    expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class LowpassCrossoverResponse :
    public ResponseBasis,
    public Common::Command::Zones::LowpassCrossoverRegularExpressionBasis
{
public:
    LowpassCrossoverResponse(void) = default;
    virtual ~LowpassCrossoverResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound lowpass crossover frequency
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetLowpassCrossoverRequest :
    public RequestBasis,
    public Common::Command::EqualizerLowpassCrossoverBufferBasis
{
public:
    SetLowpassCrossoverRequest(void) = default;
    virtual ~SetLowpassCrossoverRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound lowpass crossover
 *    frequency data model property mutation command request /
 *    response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetLowpassCrossover :
    public ExchangeBasis
{
public:
    SetLowpassCrossover(void) = default;
    virtual ~SetLowpassCrossover(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);

private:
    SetLowpassCrossoverRequest  mRequest;
    LowpassCrossoverResponse    mResponse;
};

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone volume mute set/assert data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class SetMuteRequest :
    public VolumeMuteSetRequestBasis
{
public:
    SetMuteRequest(void) = default;
    virtual ~SetMuteRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute set/assert data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetMute :
    public ExchangeBasis
{
public:
    SetMute(void) = default;
    virtual ~SetMute(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    SetMuteRequest  mRequest;
    MuteResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute clear/deassert data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class ClearMuteRequest :
    public VolumeMuteClearRequestBasis
{
public:
    ClearMuteRequest(void) = default;
    virtual ~ClearMuteRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute clear/deassert data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ClearMute :
    public ExchangeBasis
{
public:
    ClearMute(void) = default;
    virtual ~ClearMute(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    ClearMuteRequest  mRequest;
    MuteResponse      mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute toggle data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class ToggleMuteRequest :
    public VolumeMuteToggleRequestBasis
{
public:
    ToggleMuteRequest(void) = default;
    virtual ~ToggleMuteRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone volume mute toggle data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ToggleMute :
    public ExchangeBasis
{
public:
    ToggleMute(void) = default;
    virtual ~ToggleMute(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    ToggleMuteRequest  mRequest;
    MuteResponse       mResponse;
};

// MARK: Sound Mode Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound mode data model property
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SoundModeResponse :
    public ResponseBasis,
    public Common::Command::Zones::SoundModeRegularExpressionBasis
{
public:
    SoundModeResponse(void) = default;
    virtual ~SoundModeResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound mode data model set property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSoundModeRequest :
    public RequestBasis,
    public Common::Command::EqualizerSoundModeBufferBasis
{
public:
    SetSoundModeRequest(void) = default;
    virtual ~SetSoundModeRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound mode data model property
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSoundMode :
    public ExchangeBasis
{
public:
    SetSoundMode(void) = default;
    virtual ~SetSoundMode(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aSoundMode);

private:
    SetSoundModeRequest  mRequest;
    SoundModeResponse    mResponse;
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone name data model property mutation
 *    command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class NameResponse :
    public ResponseBasis,
    public Common::Command::Zones::NameRegularExpressionBasis
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
 *    An object for a HLX client zone name data model property mutation
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetNameRequest :
    public NameSetRequestBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char *aName);
};

/**
 *  @brief
 *    An object for a HLX client zone name data model property mutation
 *    command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetName :
    public ExchangeBasis
{
public:
    SetName(void) = default;
    virtual ~SetName(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char *aName);

private:
    SetNameRequest mRequest;
    NameResponse   mResponse;
};

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone source data model set property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSourceRequest :
    public SourceSetRequestBasis
{
public:
    SetSourceRequest(void) = default;
    virtual ~SetSourceRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone source data model property
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSource :
    public ExchangeBasis
{
public:
    SetSource(void) = default;
    virtual ~SetSource(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

private:
    SetSourceRequest  mRequest;
    SourceResponse    mResponse;
};

// MARK: (Sound Mode) Tone Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone sound tone bass decrease data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseBassRequest :
    public RequestBasis,
    public Common::Command::ToneBufferBasis
{
public:
    DecreaseBassRequest(void) = default;
    virtual ~DecreaseBassRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone treble decrease data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseTrebleRequest :
    public RequestBasis,
    public Common::Command::ToneBufferBasis
{
public:
    DecreaseTrebleRequest(void) = default;
    virtual ~DecreaseTrebleRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone bass increase data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBassRequest :
    public RequestBasis,
    public Common::Command::ToneBufferBasis
{
public:
    IncreaseBassRequest(void) = default;
    virtual ~IncreaseBassRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone treble increase data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseTrebleRequest :
    public RequestBasis,
    public Common::Command::ToneBufferBasis
{
public:
    IncreaseTrebleRequest(void) = default;
    virtual ~IncreaseTrebleRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone data model property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetToneRequest :
    public RequestBasis,
    public Common::Command::ToneBufferBasis
{
public:
    SetToneRequest(void) = default;
    virtual ~SetToneRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, const Model::ToneModel::LevelType &aTreble);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone data model properties
 *    mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ToneResponse :
    public ResponseBasis,
    public Common::Command::Zones::ToneRegularExpressionBasis
{
public:
    ToneResponse(void) = default;
    virtual ~ToneResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone bass decrease data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseBass :
    public ExchangeBasis
{
public:
    DecreaseBass(void) = default;
    virtual ~DecreaseBass(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    DecreaseBassRequest  mRequest;
    ToneResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone treble decrease data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseTreble :
    public ExchangeBasis
{
public:
    DecreaseTreble(void) = default;
    virtual ~DecreaseTreble(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    DecreaseTrebleRequest  mRequest;
    ToneResponse           mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone bass increase data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBass :
    public ExchangeBasis
{
public:
    IncreaseBass(void) = default;
    virtual ~IncreaseBass(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    IncreaseBassRequest  mRequest;
    ToneResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone treble increase data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseTreble :
    public ExchangeBasis
{
public:
    IncreaseTreble(void) = default;
    virtual ~IncreaseTreble(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    IncreaseTrebleRequest  mRequest;
    ToneResponse           mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone sound tone data model properties
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetTone :
    public ExchangeBasis
{
public:
    SetTone(void) = default;
    virtual ~SetTone(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, const Model::ToneModel::LevelType &aTreble);

private:
    SetToneRequest  mRequest;
    ToneResponse    mResponse;
};

// MARK: Volme Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client zone volume level data model property
 *    mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class SetVolumeRequest :
    public VolumeSetRequestBasis
{
public:
    SetVolumeRequest(void) = default;
    virtual ~SetVolumeRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
};

/**
 *  @brief
 *    An object for a HLX client zone volume level data model property
 *    mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class SetVolume :
    public Command::ExchangeBasis
{
public:
    SetVolume(void) = default;
    virtual ~SetVolume(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);

private:
    SetVolumeRequest  mRequest;
    VolumeResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume level increase data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class IncreaseVolumeRequest :
    public VolumeIncreaseRequestBasis
{
public:
    IncreaseVolumeRequest(void) = default;
    virtual ~IncreaseVolumeRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone volume level increase data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class IncreaseVolume :
    public ExchangeBasis
{
public:
    IncreaseVolume(void) = default;
    virtual ~IncreaseVolume(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    IncreaseVolumeRequest  mRequest;
    VolumeResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume level decrease data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class DecreaseVolumeRequest :
    public VolumeDecreaseRequestBasis
{
public:
    DecreaseVolumeRequest(void) = default;
    virtual ~DecreaseVolumeRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone volume level decrease data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class DecreaseVolume :
    public ExchangeBasis
{
public:
    DecreaseVolume(void) = default;
    virtual ~DecreaseVolume(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

private:
    DecreaseVolumeRequest  mRequest;
    VolumeResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client zone volume fixed/locked data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class SetVolumeFixedRequest :
    public VolumeFixedRequestBasis
{
public:
    SetVolumeFixedRequest(void) = default;
    virtual ~SetVolumeFixedRequest(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);
};

/**
 *  @brief
 *    An object for a HLX client zone volume fixed/locked data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeFixedResponse :
    public ResponseBasis,
    public Common::Command::Zones::VolumeFixedRegularExpressionBasis
{
public:
    VolumeFixedResponse(void) = default;
    virtual ~VolumeFixedResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client zone volume fixed/locked data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class SetVolumeFixed :
    public Command::ExchangeBasis
{
public:
    SetVolumeFixed(void) = default;
    virtual ~SetVolumeFixed(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);

private:
    SetVolumeFixedRequest  mRequest;
    VolumeFixedResponse    mResponse;
};

}; // namespace Zones

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTZONESCONTROLLERCOMMANDS_HPP
