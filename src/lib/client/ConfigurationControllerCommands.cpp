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
 *      This file implements objects for HLX client configuration data
 *      model commands and their constituent requests and responses.
 *
 */

#include "ConfigurationControllerCommands.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <CommandQueryRequestBasis.hpp>
#include <CommandResponseBasis.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Client
{

namespace Command
{

namespace Configuration
{

/**
 *  The configuration saving to backup command regular expression
 *  pattern string.
 *
 */
const char * const SavingToBackupResponse::kResponseRegexp  = "SAVING\\.\\.\\.";

/**
 *  The configuration saving to backup command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t SavingToBackupResponse::kExpectedMatches       = 1;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query current command request buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryCurrentRequest :: Init(void)
{
    const char *lObject = "X";

    return (QueryRequestBasis::Init(lObject));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query current command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryCurrentResponse :: Init(void)
{
    return (QueryCurrentRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query current command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryCurrent :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the load from backup command request buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
LoadFromBackupRequest :: Init(void)
{
    static const char * const kBuffer = "LOAD";

    return (RequestBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the load from backup command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LoadFromBackupResponse :: Init(void)
{
    return (LoadFromBackupRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the load from backup command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LoadFromBackup :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the save to backup command request buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SaveToBackupRequest :: Init(void)
{
    static const char * const kBuffer = "SAVE";

    return (RequestBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the saving to backup command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SaveToBackupResponse :: Init(void)
{
    return (SaveToBackupRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the save to backup command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SaveToBackup :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the saving to backup notification/command
 *  response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SavingToBackupResponse :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = ResponseBasis::Init(kResponseRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the reset to defaults command request buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ResetToDefaultsRequest :: Init(void)
{
    static const char * const kBuffer = "RESET";

    return (RequestBasis::Init(kBuffer));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the reset to defaults command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ResetToDefaultsResponse :: Init(void)
{
    return (ResetToDefaultsRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the reset to defaults command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ResetToDefaults :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Configuration

}; // namespace Command

}; // namespace Client

}; // namespace HLX
