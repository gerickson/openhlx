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
 *      This file implements an object for a HLX server command error
 *      response buffer.
 *
 */

#include "CommandErrorResponse.hpp"


using namespace HLX::Common;


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a command error response buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ErrorResponse :: Init(void)
{
    static const char * const kErrorResponse = "ERROR";

    return (ResponseBasis::Init(kErrorResponse));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
