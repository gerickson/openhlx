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
 *      This file implements an object for a HLX client command error
 *      response regular expression.
 *
 */

#include "CommandErrorResponse.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  The underlying command response regular expression pattern string.
 *
 */
const char * const ErrorResponse::kResponseRegexp  = "ERROR";

/**
 *  The number of expected substring matches for the command response
 *  regular expression pattern string.
 *
 */
const size_t ErrorResponse::kExpectedMatches       = 1;


/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a command error response regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ErrorResponse :: Init(void)
{
    return (ResponseBasis::Init(kResponseRegexp, kExpectedMatches));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
