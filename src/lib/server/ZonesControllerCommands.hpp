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
 *      This file defines objects for HLX server zone data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERZONESCONTROLLERCOMMANDS_HPP
#define HLXSERVERZONESCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandEqualizerBufferBases.hpp>
#include <OpenHLX/Common/CommandToneBufferBasis.hpp>
#include <OpenHLX/Common/CommandZonesRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Server/CommandBalanceResponseBasis.hpp>
#include <OpenHLX/Server/CommandEqualizerBandResponseBasis.hpp>
#include <OpenHLX/Server/CommandNameSetResponseBasis.hpp>
#include <OpenHLX/Server/CommandPropertyResponseBases.hpp>
#include <OpenHLX/Server/CommandQueryResponseBasis.hpp>
#include <OpenHLX/Server/CommandRequestBasis.hpp>
#include <OpenHLX/Server/CommandSourceResponseBases.hpp>
#include <OpenHLX/Server/CommandVolumeResponseBases.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Zones
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone data model query command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Zones::QueryRegularExpressionBasis
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
 *    An object for a HLX server zone data model query command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
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

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZone);
};

/**
 *  @brief
 *    An object for a HLX server zone volume mute data model property
 *    query command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryMuteRequest :
    public RequestBasis
{
public:
    QueryMuteRequest(void) = default;
    virtual ~QueryMuteRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone source (input) data model property
 *    query command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QuerySourceRequest :
    public RequestBasis
{
public:
    QuerySourceRequest(void) = default;
    virtual ~QuerySourceRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone volume level data model property
 *    query command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryVolumeRequest :
    public RequestBasis
{
public:
    QueryVolumeRequest(void) = default;
    virtual ~QueryVolumeRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: Balance Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone stereophonic channel balance
 *    adjust data model property mutation command request regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class AdjustBalanceRequest :
    public RequestBasis
{
public:
    AdjustBalanceRequest(void) = default;
    virtual ~AdjustBalanceRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone stereophonic channel balance set
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetBalanceRequest :
    public RequestBasis,
    public Common::Command::Zones::BalanceRegularExpressionBasis
{
public:
    SetBalanceRequest(void) = default;
    virtual ~SetBalanceRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone stereophonic channel balance data
 *    model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class BalanceResponse :
    public BalanceResponseBasis
{
public:
    BalanceResponse(void) = default;
    virtual ~BalanceResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::BalanceModel::BalanceType &aBalance);
};

// MARK: (Sound Mode) Equalizer Band Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer band level
 *    decrease data model property mutation command request regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseEqualizerBandRequest :
   public RequestBasis
{
public:
    DecreaseEqualizerBandRequest(void) = default;
    virtual ~DecreaseEqualizerBandRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer band level
 *    increase data model property mutation command request regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseEqualizerBandRequest :
   public RequestBasis
{
public:
    IncreaseEqualizerBandRequest(void) = default;
    virtual ~IncreaseEqualizerBandRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer band level data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerBandRequest :
    public RequestBasis,
    public Common::Command::Zones::EqualizerBandLevelRegularExpressionBasis
{
public:
    SetEqualizerBandRequest(void) = default;
    virtual ~SetEqualizerBandRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer band level data
 *    model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerBandResponse :
    public EqualizerBandResponseBasis
{
public:
    EqualizerBandResponse(void) = default;
    virtual ~EqualizerBandResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel);
};

// MARK: (Sound Mode) Equalizer Preset Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer preset data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetEqualizerPresetRequest :
    public RequestBasis,
    public Common::Command::Zones::EqualizerPresetRegularExpressionBasis
{
public:
    SetEqualizerPresetRequest(void) = default;
    virtual ~SetEqualizerPresetRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound equalizer preset data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerPresetResponse :
    public ResponseBasis,
    public Common::Command::EqualizerPresetBufferBasis
{
public:
    EqualizerPresetResponse(void) = default;
    virtual ~EqualizerPresetResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    // Explicitly hide base class initializers

private:
    using ResponseBasis::Init;
};

// MARK: (Sound Mode) Highpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound highpass crossover frequency
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetHighpassCrossoverRequest :
    public RequestBasis,
    public Common::Command::Zones::HighpassCrossoverRegularExpressionBasis
{
public:
    SetHighpassCrossoverRequest(void) = default;
    virtual ~SetHighpassCrossoverRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound highpass crossover
 *    frequency data model property mutation command response regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class HighpassCrossoverResponse :
    public ResponseBasis,
    public Common::Command::EqualizerHighpassCrossoverBufferBasis
{
public:
    HighpassCrossoverResponse(void) = default;
    virtual ~HighpassCrossoverResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);

    // Explicitly hide base class initializers

private:
    using ResponseBasis::Init;
};

// MARK: (Sound Mode) Lowpass Crossover Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound lowpass crossover frequency
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetLowpassCrossoverRequest :
    public RequestBasis,
    public Common::Command::Zones::LowpassCrossoverRegularExpressionBasis
{
public:
    SetLowpassCrossoverRequest(void) = default;
    virtual ~SetLowpassCrossoverRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound lowpass crossover frequency
 *    data model property mutation command response regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class LowpassCrossoverResponse :
    public ResponseBasis,
    public Common::Command::EqualizerLowpassCrossoverBufferBasis
{
public:
    LowpassCrossoverResponse(void) = default;
    virtual ~LowpassCrossoverResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);

    // Explicitly hide base class initializers

private:
    using ResponseBasis::Init;
};


// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone volume mute data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class MuteRequest :
    public RequestBasis,
    public Common::Command::Zones::MuteRegularExpressionBasis
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
 *    An object for a HLX server zone volume mute toggle data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ToggleMuteRequest :
    public RequestBasis,
    public Common::Command::Zones::ToggleMuteRegularExpressionBasis
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
 *    An object for a HLX server zone volume mute data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class MuteResponse :
    public VolumeMuteResponseBasis
{
public:
    MuteResponse(void) = default;
    virtual ~MuteResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone name data model property mutation
 *    command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetNameRequest :
    public RequestBasis,
    public Common::Command::Zones::NameRegularExpressionBasis
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
 *    An object for a HLX server zone name data model property mutation
 *    command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class NameResponse :
    public NameSetResponseBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char * aName);
    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const char * aName, const size_t &aNameLength);
};

// MARK: Sound Mode Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound mode data model set property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSoundModeRequest :
    public RequestBasis,
    public Common::Command::Zones::SoundModeRegularExpressionBasis
{
public:
    SetSoundModeRequest(void) = default;
    virtual ~SetSoundModeRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound mode data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SoundModeResponse :
    public ResponseBasis,
    public Common::Command::EqualizerSoundModeBufferBasis
{
public:
    SoundModeResponse(void) = default;
    virtual ~SoundModeResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SoundModel::SoundMode &aMode);

    // Explicitly hide base class initializers

private:
    using ResponseBasis::Init;
};

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone source data model set property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSourceRequest :
    public RequestBasis,
    public Common::Command::Zones::SourceRegularExpressionBasis
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
 *    An object for a HLX server zone source data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceResponse :
    public SourceSetResponseBasis
{
public:
    SourceResponse(void) = default;
    virtual ~SourceResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    An object for a HLX server all-zones source data model set property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetSourceAllRequest :
    public RequestBasis,
    public Common::Command::Zones::SourceAllRegularExpressionBasis
{
public:
    SetSourceAllRequest(void) = default;
    virtual ~SetSourceAllRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server all-zones source data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceAllResponse :
    public SourceAllResponseBasis
{
public:
    SourceAllResponse(void) = default;
    virtual ~SourceAllResponse(void) = default;

    Common::Status Init(const Model::SourceModel::IdentifierType &aSourceIdentifier);
};

// MARK: (Sound Mode) Tone Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone sound tone bass decrease data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseBassRequest :
    public RequestBasis
{
public:
    DecreaseBassRequest(void) = default;
    virtual ~DecreaseBassRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound tone bass increase data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseBassRequest :
    public RequestBasis
{
public:
    IncreaseBassRequest(void) = default;
    virtual ~IncreaseBassRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound tone treble decrease data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseTrebleRequest :
    public RequestBasis
{
public:
    DecreaseTrebleRequest(void) = default;
    virtual ~DecreaseTrebleRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound tone treble increase data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseTrebleRequest :
    public RequestBasis
{
public:
    IncreaseTrebleRequest(void) = default;
    virtual ~IncreaseTrebleRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound tone data model property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetToneRequest :
    public RequestBasis,
    public Common::Command::Zones::ToneRegularExpressionBasis
{
public:
    SetToneRequest(void) = default;
    virtual ~SetToneRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone sound tone data model properties
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ToneResponse :
    public ResponseBasis,
    public Common::Command::ToneBufferBasis
{
public:
    ToneResponse(void) = default;
    virtual ~ToneResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aBass, const Model::ToneModel::LevelType &aTreble);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server zone volume level decrease data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class DecreaseVolumeRequest :
    public RequestBasis
{
public:
    DecreaseVolumeRequest(void) = default;
    virtual ~DecreaseVolumeRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone volume level increase data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class IncreaseVolumeRequest :
    public RequestBasis
{
public:
    IncreaseVolumeRequest(void) = default;
    virtual ~IncreaseVolumeRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone volume level data model property
 *    mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetVolumeRequest :
    public RequestBasis,
    public Common::Command::Zones::VolumeRegularExpressionBasis
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
 *    An object for a HLX server zone volume level data model property
 *    mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class VolumeResponse :
    public VolumeResponseBasis
{
public:
    VolumeResponse(void) = default;
    virtual ~VolumeResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);
};

/**
 *  @brief
 *    An object for a HLX server all-zones volume level data model
 *    set property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetVolumeAllRequest :
    public RequestBasis,
    public Common::Command::Zones::VolumeAllRegularExpressionBasis
{
public:
    SetVolumeAllRequest(void) = default;
    virtual ~SetVolumeAllRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server all-zones volume level data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class VolumeAllResponse :
    public VolumeAllResponseBasis
{
public:
    VolumeAllResponse(void) = default;
    virtual ~VolumeAllResponse(void) = default;

    Common::Status Init(const Model::VolumeModel::LevelType &aVolume);

    // Explicitly hide base class initializers

private:
    using ResponseBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone volume fixed/locked data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SetVolumeFixedRequest :
    public RequestBasis,
    public Common::Command::Zones::VolumeFixedRegularExpressionBasis
{
public:
    SetVolumeFixedRequest(void) = default;
    virtual ~SetVolumeFixedRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server zone volume fixed/locked data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup zone
 *
 */
class VolumeFixedResponse :
    public VolumeFixedResponseBasis
{
public:
    VolumeFixedResponse(void) = default;
    virtual ~VolumeFixedResponse(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aVolumeFixed);
};

}; // namespace Zones

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERZONESCONTROLLERCOMMANDS_HPP
