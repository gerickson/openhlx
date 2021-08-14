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
 *      This file defines objects for HLX server configuration data
 *      model commands and their constituent requests and responses.
 *
 */

#ifndef HLXSERVERCONFIGURATIONCONTROLLERCOMMANDS_HPP
#define HLXSERVERCONFIGURATIONCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandConfigurationRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>

#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Configuration
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server query current configuration command
 *    request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrentRequest :
    public RequestBasis,
    public Common::Command::Configuration::QueryCurrentRegularExpressionBasis
{
public:
    QueryCurrentRequest(void) = default;
    virtual ~QueryCurrentRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server query current configuration command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrentResponse :
    public QueryResponseBasis
{
public:
    QueryCurrentResponse(void) = default;
    virtual ~QueryCurrentResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

// MARK: Configuration Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server load from backup configuration command
 *    request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackupRequest :
    public RequestBasis,
    public Common::Command::Configuration::LoadFromBackupRegularExpressionBasis
{
public:
    LoadFromBackupRequest(void) = default;
    virtual ~LoadFromBackupRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server load from backup configuration command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackupResponse :
    public ResponseBasis
{
public:
    LoadFromBackupResponse(void) = default;
    virtual ~LoadFromBackupResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

/**
 *  @brief
 *    A object for a HLX server reset to default configuration command
 *    request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaultsRequest :
    public RequestBasis,
    public Common::Command::Configuration::ResetToDefaultsRegularExpressionBasis
{
public:
    ResetToDefaultsRequest(void) = default;
    virtual ~ResetToDefaultsRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server reset to default configuration command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaultsResponse :
    public ResponseBasis
{
public:
    ResetToDefaultsResponse(void) = default;
    virtual ~ResetToDefaultsResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

/**
 *  @brief
 *    A object for a HLX server save to backup configuration command
 *    request regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackupRequest :
    public RequestBasis,
    public Common::Command::Configuration::SaveToBackupRegularExpressionBasis
{
public:
    SaveToBackupRequest(void) = default;
    virtual ~SaveToBackupRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server save to backup configuration command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackupResponse :
    public ResponseBasis
{
public:
    SaveToBackupResponse(void) = default;
    virtual ~SaveToBackupResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

/**
 *  @brief
 *    A object for a HLX server saving to backup configuration command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SavingToBackupResponse :
    public ResponseBasis
{
public:
    SavingToBackupResponse(void) = default;
    virtual ~SavingToBackupResponse(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(void);
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONFIGURATIONCONTROLLERCOMMANDS_HPP
