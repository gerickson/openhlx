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
 *      This file implements objects for HLX server infrared remote control
 *      interface data model commands and their constituent requests and
 *      responses.
 *
 */

#include "InfraredControllerCommands.hpp"

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

namespace Infrared
{

/**
 *  Infrared query command request regular expression pattern.
 *
 */
const char * const QueryRequest::kRequestRegexp   = "QIRL";
/**
 *  Expected number of infrared query command request regular
 *  expression pattern substring matches.
 *
 */
const size_t       QueryRequest::kExpectedMatches = 1;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the infrared query command request regular expression.
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

// MARK: Infrared Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set infrared disabled state command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetDisabledRequest :: Init(void)
{
    return (DisabledRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the infrared disabled state command response buffer.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled state
 *                         for which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DisabledResponse :: Init(const Model::InfraredModel::DisabledType &aDisabled)
{
    static const char * const  kObject = "IRL";
    OutputStringStream         lDisabledStream;
    std::string                lBuffer;

    // Set width to one (1) while upcasting the disable state to an
    // unsigned integer to ensure it is interpretted as something to
    // be converted rather than a character literal.

    lDisabledStream << std::setw(1) << static_cast<unsigned int>(aDisabled);

    // Compose the buffer with the command and disable state.

    lBuffer = kObject;
    lBuffer += lDisabledStream.str();

    return (ResponseBasis::Init(lBuffer.c_str()));
}

}; // namespace Infrared

}; // namespace Command

}; // namespace Server

}; // namespace HLX
