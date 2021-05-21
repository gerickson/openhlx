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
 *      This file implements objects for HLX client physical front panel
 *      data model commands and their constituent requests and responses.
 *
 */

#include "FrontPanelControllerCommands.hpp"

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

namespace FrontPanel
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel query command request buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryLockedBufferBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel query command response regular
 *  expression.
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
     * implementation of the HLX such that the response to [QFPL] is
     * not "(FPL#)(QFPL)" but rather just "(FPL#)", the same response
     * as to a lock command.
     */

    return (LockedRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel query command.
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
 *  This initializes the front panel set brightness command request
 *  buffer.
 *
 *  @param[in]  aBrightness  An immutable reference to the
 *                           brightness level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetBrightnessRequest :: Init(const FrontPanelModel::BrightnessType &aBrightness)
{
    return (BrightnessBufferBasis::Init(*this, aBrightness));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel brightness state command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
BrightnessResponse :: Init(void)
{
    return (BrightnessRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel set brightness state command.
 *
 *  @param[in]  aBrightness  An immutable reference to the
 *                           brightness level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetBrightness :: Init(const FrontPanelModel::BrightnessType &aBrightness)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aBrightness);
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
 *  This initializes the front panel set locked state command request
 *  buffer.
 *
 *  @param[in]  aLocked  An immutable reference to the
 *                       locked state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetLockedRequest :: Init(const FrontPanelModel::LockedType &aLocked)
{
    return (LockedBufferBasis::Init(*this, aLocked));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel locked state command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LockedResponse :: Init(void)
{
    return (LockedRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel set locked state command.
 *
 *  @param[in]  aLocked  An immutable reference to the
 *                       locked state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetLocked :: Init(const FrontPanelModel::LockedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Client

}; // namespace HLX
