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
 *      This file implements objects for HLX server configuration data
 *      model commands and their constituent requests and responses.
 *
 */

#include "ConfigurationControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


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
 *    This is the class default initializer.
 *
 *  This initializes the query current command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryCurrentRequest :: Init(void)
{
    return (QueryCurrentRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query current command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryCurrentResponse :: Init(void)
{
    static const char * const kConfigurationObject = "X";

    return (QueryResponseBasis::Init(kConfigurationObject));
}

// MARK: Configuration Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the load from backup command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LoadFromBackupRequest :: Init(void)
{
    return (LoadFromBackupRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the load from backup command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LoadFromBackupResponse :: Init(void)
{
    static const char * const kBuffer = "LOAD";

    return (ResponseBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the reset to defaults command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ResetToDefaultsRequest :: Init(void)
{
    return (ResetToDefaultsRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the reset to defaults command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ResetToDefaultsResponse :: Init(void)
{
    static const char * const kBuffer = "RESET";

    return (ResponseBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the save to backup command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SaveToBackupRequest :: Init(void)
{
    return (SaveToBackupRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the save to backup command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SaveToBackupResponse :: Init(void)
{
    static const char * const kBuffer = "SAVE";

    return (ResponseBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the saving to backup command response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SavingToBackupResponse :: Init(void)
{
    static const char * const kBuffer = "SAVING...";

    return (ResponseBasis::Init(kBuffer));
}

}; // namespace Configuration

}; // namespace Command

}; // namespace Server

}; // namespace HLX
