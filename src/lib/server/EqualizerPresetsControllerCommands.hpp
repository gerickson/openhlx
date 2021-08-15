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
 *      This file defines objects for HLX server equalizer preset data
 *      model commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVEREQUALIZERPRESETSCONTROLLERCOMMANDS_HPP
#define HLXSERVEREQUALIZERPRESETSCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandEqualizerPresetsRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>

#include "CommandEqualizerBandResponseBasis.hpp"
#include "CommandNameSetResponseBasis.hpp"
#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace EqualizerPresets
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server equalizer preset data model query
 *    command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::EqualizerPresets::QueryRegularExpressionBasis
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
 *    An object for a HLX server equalizer preset data model query
 *    command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
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

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset);
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: Equalizer Band Level Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server equalizer preset band level decrease
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class DecreaseBandRequest :
    public RequestBasis
{
public:
    DecreaseBandRequest(void) = default;
    virtual ~DecreaseBandRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server equalizer preset band level increase
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class IncreaseBandRequest :
    public RequestBasis
{
public:
    IncreaseBandRequest(void) = default;
    virtual ~IncreaseBandRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server equalizer preset band level set data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetBandRequest :
    public RequestBasis,
    public Common::Command::EqualizerPresets::BandLevelRegularExpressionBasis
{
public:
    SetBandRequest(void) = default;
    virtual ~SetBandRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX server equalizer preset band level data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class BandResponse :
    public EqualizerBandResponseBasis
{
public:
    BandResponse(void) = default;
    virtual ~BandResponse(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aLevel);
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX server equalizer preset name data model
 *    property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetNameRequest :
    public RequestBasis,
    public Common::Command::EqualizerPresets::NameRegularExpressionBasis
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
 *    An object for a HLX server equalizer preset name data model
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class NameResponse :
    public NameSetResponseBasis
{
public:
    NameResponse(void) = default;
    virtual ~NameResponse(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset, const char * aName);
    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPreset, const char * aName, const size_t &aNameLength);
};

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVEREQUALIZERPRESETSCONTROLLERCOMMANDS_HPP
