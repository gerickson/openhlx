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
 *      This file defines objects for HLX client equalizer preset data
 *      model commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTEQUALIZERPRESETSCONTROLLERCOMMANDS_HPP
#define HLXCLIENTEQUALIZERPRESETSCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Client/CommandEqualizerBandRequestBases.hpp>
#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandNameSetRequestBasis.hpp>
#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/CommandEqualizerPresetsRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace EqualizerPresets
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client equalizer preset data model query
 *    command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class QueryRequest :
    virtual public Client::Command::RequestBasis,
    public Client::Command::QueryRequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset data model query
 *    command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::EqualizerPresets::QueryRegularExpressionBasis
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
 *    An object for a HLX client equalizer preset data model query
 *    command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

// MARK: Mutator Requests, Responses, and Commands

// MARK: Equalizer Band Level Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class EqualizerBandResponse :
    public ResponseBasis,
    public Common::Command::EqualizerPresets::BandLevelRegularExpressionBasis
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
 *    An object for a HLX client equalizer preset band level set data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetEqualizerBandRequest :
    public EqualizerBandSetRequestBasis
{
public:
    SetEqualizerBandRequest(void) = default;
    virtual ~SetEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetEqualizerBand :
    public ExchangeBasis
{
public:
    SetEqualizerBand(void) = default;
    virtual ~SetEqualizerBand(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);

private:
    SetEqualizerBandRequest  mRequest;
    EqualizerBandResponse    mResponse;
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level increase
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class IncreaseEqualizerBandRequest :
    public EqualizerBandIncreaseRequestBasis
{
public:
    IncreaseEqualizerBandRequest(void) = default;
    virtual ~IncreaseEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level increase
 *    data model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class IncreaseEqualizerBand :
    public ExchangeBasis
{
public:
    IncreaseEqualizerBand(void) = default;
    virtual ~IncreaseEqualizerBand(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

private:
    IncreaseEqualizerBandRequest  mRequest;
    EqualizerBandResponse         mResponse;
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level decrease
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class DecreaseEqualizerBandRequest :
    public EqualizerBandDecreaseRequestBasis
{
public:
    DecreaseEqualizerBandRequest(void) = default;
    virtual ~DecreaseEqualizerBandRequest(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset band level decrease
 *    data model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class DecreaseEqualizerBand :
    public ExchangeBasis
{
public:
    DecreaseEqualizerBand(void) = default;
    virtual ~DecreaseEqualizerBand(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

private:
    DecreaseEqualizerBandRequest  mRequest;
    EqualizerBandResponse         mResponse;
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    An object for a HLX client equalizer preset name data model
 *    property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetNameRequest :
    public NameSetRequestBasis
{
public:
    SetNameRequest(void) = default;
    virtual ~SetNameRequest(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const char *aName);
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset name data model
 *    property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class NameResponse :
    public ResponseBasis,
    public Common::Command::EqualizerPresets::NameRegularExpressionBasis
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
 *    An object for a HLX client equalizer preset name data model
 *    property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-preset
 *
 */
class SetName :
    public ExchangeBasis
{
public:
    SetName(void) = default;
    virtual ~SetName(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const char *aName);

private:
    SetNameRequest mRequest;
    NameResponse   mResponse;
};

}; // namespace EqualizerPresets

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTEQUALIZERPRESETSCONTROLLERCOMMANDS_HPP
