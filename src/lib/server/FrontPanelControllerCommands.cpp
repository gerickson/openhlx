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
 *      This file implements objects for HLX server physical front panel
 *      data model commands and their constituent requests and responses.
 *
 */

#include "FrontPanelControllerCommands.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace FrontPanel
{

/**
 *  Front panel query command request regular expression pattern.
 *
 */
const char * const QueryRequest::kRequestRegexp         = "QFPL";

/**
 *  Expected number of front panel query command request regular
 *  expression pattern matches.
 *
 */
const size_t       QueryRequest::kExpectedMatches       = 1;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the front panel query command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Front Panel Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set front panel brightness command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetBrightnessRequest :: Init(void)
{
    return (BrightnessRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel brightness state command response buffer.
 *
 *  @param[in]  aBrightness  An immutable reference to the brightness level
 *                           for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
BrightnessResponse :: Init(const Model::FrontPanelModel::BrightnessType &aBrightness)
{
    return (BrightnessBufferBasis::Init(*this, aBrightness));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set front panel locked state command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetLockedRequest :: Init(void)
{
    return (LockedRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel locked state command response buffer.
 *
 *  @param[in]  aLocked  An immutable reference to the locked state
 *                       for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
LockedResponse :: Init(const Model::FrontPanelModel::LockedType &aLocked)
{
    return (LockedBufferBasis::Init(*this, aLocked));
}

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Server

}; // namespace HLX
