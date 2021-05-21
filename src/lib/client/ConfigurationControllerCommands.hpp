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
 *      This file defines objects for HLX client configuration data
 *      model commands and their constituent requests and responses.
 *
 */

#ifndef HLXCLIENTCONFIGURATIONCONTROLLERCOMMANDS_HPP
#define HLXCLIENTCONFIGURATIONCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/Errors.hpp>

#include <CommandConfigurationRegularExpressionBases.hpp>
#include <CommandExchangeBasis.hpp>
#include <CommandQueryRequestBasis.hpp>
#include <CommandResponseBasis.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Configuration
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client query current configuration command
 *    request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrentRequest :
    public QueryRequestBasis
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
 *    A object for a HLX client query current configuration command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrentResponse :
    public ResponseBasis,
    public Common::Command::Configuration::QueryCurrentRegularExpressionBasis
{
public:
    QueryCurrentResponse(void) = default;
    virtual ~QueryCurrentResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client query current configuration command
 *    request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class QueryCurrent :
    public ExchangeBasis
{
public:
    QueryCurrent(void) = default;
    virtual ~QueryCurrent(void) = default;

    Common::Status Init(void);

private:
    QueryCurrentRequest  mRequest;
    QueryCurrentResponse mResponse;
};

// MARK: Configuration Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX client load from backup configuration command
 *    request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackupRequest :
    public RequestBasis
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
 *    A object for a HLX client load from backup configuration command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackupResponse :
    public ResponseBasis,
    public Common::Command::Configuration::LoadFromBackupRegularExpressionBasis
{
public:
    LoadFromBackupResponse(void) = default;
    virtual ~LoadFromBackupResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client load from backup configuration command
 *    request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class LoadFromBackup :
    public ExchangeBasis
{
public:
    LoadFromBackup(void) = default;
    virtual ~LoadFromBackup(void) = default;

    Common::Status Init(void);

private:
    LoadFromBackupRequest  mRequest;
    LoadFromBackupResponse mResponse;
};

/**
 *  @brief
 *    A object for a HLX client save to backup configuration command
 *    request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackupRequest :
    public RequestBasis
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
 *    A object for a HLX client save to backup configuration command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackupResponse :
    public ResponseBasis,
    public Common::Command::Configuration::SaveToBackupRegularExpressionBasis
{
public:
    SaveToBackupResponse(void) = default;
    virtual ~SaveToBackupResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client save to backup configuration command
 *    request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class SaveToBackup :
    public ExchangeBasis
{
public:
    SaveToBackup(void) = default;
    virtual ~SaveToBackup(void) = default;

    Common::Status Init(void);

private:
    SaveToBackupRequest  mRequest;
    SaveToBackupResponse mResponse;
};

/**
 *  @brief
 *    A object for a HLX client saving to backup configuration command
 *    response regular expression.
 *
 *  @ingroup client
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

    Common::Status Init(void);

    static const char * const kResponseRegexp;
    static const size_t       kExpectedMatches;

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client reset to default configuration command
 *    request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaultsRequest :
    public RequestBasis
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
 *    A object for a HLX client reset to default configuration command
 *    response regular expression.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaultsResponse :
    public ResponseBasis,
    public Common::Command::Configuration::ResetToDefaultsRegularExpressionBasis
{
public:
    ResetToDefaultsResponse(void) = default;
    virtual ~ResetToDefaultsResponse(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX client reset to default configuration command
 *    request / response pair.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup configuration
 *
 */
class ResetToDefaults :
    public ExchangeBasis
{
public:
    ResetToDefaults(void) = default;
    virtual ~ResetToDefaults(void) = default;

    Common::Status Init(void);

private:
    ResetToDefaultsRequest  mRequest;
    ResetToDefaultsResponse mResponse;
};

}; // namespace Configuration

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCONFIGURATIONCONTROLLERCOMMANDS_HPP
