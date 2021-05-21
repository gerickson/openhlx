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
 *      This file implements objects for HLX client infrared remote control
 *      data model commands and their constituent requests and responses.
 *
 */

#include "InfraredControllerCommands.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandQueryRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Infrared
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the infrared remote control query command request
 *  buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryDisabledBufferBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the infrared remote control query command
 *  response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    /*
     * There is a bug in either the documentation or in the
     * implementation of the HLX such that the response to [QIRL] is
     * not "(IRL#)(QIRL)" but rather just "(IRL#)", the same response
     * as to a disable command.
     */

    return (DisabledRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the infrared remote control query command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
Query :: Init(void)
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
 *    This is the class initializer.
 *
 *  This initializes the infrared remote control set locked state
 *  command request buffer.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled
 *                         state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetDisabledRequest :: Init(const InfraredModel::DisabledType &aDisabled)
{
    return (DisabledBufferBasis::Init(*this, aDisabled));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the infrared remote control locked state command
 *  response regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DisabledResponse :: Init(void)
{
    return (DisabledRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the infrared remote control set locked state
 *  command.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled
 *                         state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetDisabled :: Init(const InfraredModel::DisabledType &aDisabled)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aDisabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Infrared

}; // namespace Command

}; // namespace Client

}; // namespace HLX
