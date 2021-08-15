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
 *      This file defines objects for HLX server physical front panel
 *      data model commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERFRONTPANELCONTROLLERCOMMANDS_HPP
#define HLXSERVERFRONTPANELCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandFrontPanelBufferBases.hpp>
#include <OpenHLX/Common/CommandFrontPanelRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FrontPanelModel.hpp>

#include "CommandRequestBasis.hpp"
#include "CommandResponseBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace FrontPanel
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server physical front panel data model
 *    query command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class QueryRequest :
    public RequestBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(void);

    static const char * const kRequestRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

// MARK: Front Panel Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server physical front panel display brightness
 *    data model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetBrightnessRequest :
    public RequestBasis,
    public Common::Command::FrontPanel::BrightnessRegularExpressionBasis
{
public:
    SetBrightnessRequest(void) = default;
    virtual ~SetBrightnessRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server physical front panel display brightness
 *    data model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class BrightnessResponse :
    public ResponseBasis,
    public Common::Command::FrontPanel::BrightnessBufferBasis
{
public:
    BrightnessResponse(void) = default;
    virtual ~BrightnessResponse(void) = default;

    Common::Status Init(const Model::FrontPanelModel::BrightnessType &aBrightness);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server physical front panel locked data
 *    model property mutation command request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class SetLockedRequest :
    public RequestBasis,
    public Common::Command::FrontPanel::LockedRegularExpressionBasis
{
public:
    SetLockedRequest(void) = default;
    virtual ~SetLockedRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server physical front panel locked data
 *    model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup front-panel
 *
 */
class LockedResponse :
    public ResponseBasis,
    public Common::Command::FrontPanel::LockedBufferBasis
{
public:
    LockedResponse(void) = default;
    virtual ~LockedResponse(void) = default;

    Common::Status Init(const Model::FrontPanelModel::LockedType &aLocked);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERFRONTPANELCONTROLLERCOMMANDS_HPP
