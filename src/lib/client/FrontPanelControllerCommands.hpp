/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file defines objects for HLX client physical front panel
 *      data model commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTFRONTPANELCONTROLLERCOMMANDS_HPP
#define HLXCLIENTFRONTPANELCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/Errors.hpp>

#include <CommandExchangeBasis.hpp>
#include <CommandFrontPanelBufferBases.hpp>
#include <CommandFrontPanelRegularExpressionBases.hpp>
#include <CommandQueryRequestBasis.hpp>
#include <CommandResponseBasis.hpp>
#include <OpenHLX/Model/FrontPanelModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace FrontPanel
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client physical front panel data model
 *    query command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::FrontPanel::QueryLockedBufferBasis
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
 *    A object for a HLX client physical front panel data model
 *    query command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class QueryResponse :
    public ResponseBasis,
    public Common::Command::FrontPanel::LockedRegularExpressionBasis
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
 *    A object for a HLX client physical front panel data model
 *    query command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class Query :
    public ExchangeBasis
{
public:
    Query(void) = default;
    virtual ~Query(void) = default;

    Common::Status Init(void);

private:
    QueryRequest  mRequest;
    QueryResponse mResponse;
};

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client physical front panel display brightness
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetBrightnessRequest :
    public RequestBasis,
    public Common::Command::FrontPanel::BrightnessBufferBasis
{
public:
    SetBrightnessRequest(void) = default;
    virtual ~SetBrightnessRequest(void) = default;

    Common::Status Init(const Model::FrontPanelModel::BrightnessType &aBrightness);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client physical front panel display brightness
 *    data model property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class BrightnessResponse :
    public ResponseBasis,
    public Common::Command::FrontPanel::BrightnessRegularExpressionBasis
{
public:
    BrightnessResponse(void) = default;
    virtual ~BrightnessResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client physical front panel display brightness
 *    data model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetBrightness :
    public ExchangeBasis
{
public:
    SetBrightness(void) = default;
    virtual ~SetBrightness(void) = default;

    Common::Status Init(const Model::FrontPanelModel::BrightnessType &aBrightness);

private:
    SetBrightnessRequest  mRequest;
    BrightnessResponse    mResponse;
};

/**
 *  @brief
 *    A object for a HLX client physical front panel locked data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetLockedRequest :
    public RequestBasis,
    public Common::Command::FrontPanel::LockedBufferBasis
{
public:
    SetLockedRequest(void) = default;
    virtual ~SetLockedRequest(void) = default;

    Common::Status Init(const Model::FrontPanelModel::LockedType &aLocked);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client physical front panel locked data
 *    model property mutation command response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class LockedResponse :
    public ResponseBasis,
    public Common::Command::FrontPanel::LockedRegularExpressionBasis
{
public:
    LockedResponse(void) = default;
    virtual ~LockedResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client physical front panel locked data
 *    model property mutation command request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetLocked :
    public ExchangeBasis
{
public:
    SetLocked(void) = default;
    virtual ~SetLocked(void) = default;

    Common::Status Init(const Model::FrontPanelModel::LockedType &aLocked);

private:
    SetLockedRequest  mRequest;
    LockedResponse    mResponse;
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTFRONTPANELCONTROLLERCOMMANDS_HPP
