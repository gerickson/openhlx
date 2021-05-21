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
 *      This file implements a base, derivable object for an HLX client
 *      command request / response pair.
 *
 */

#include "CommandExchangeBasis.hpp"

#include <stddef.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ExchangeBasis :: ExchangeBasis(void) :
    mRequest(nullptr),
    mResponse(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the exchange with the specified request and
 *  response.
 *
 *  @param[in]  aRequest   A mutable reference to the command
 *                         request.
 *  @param[in]  aResponse  A mutable reference to the command
 *                         response.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ExchangeBasis :: Init(RequestBasis &aRequest, ResponseBasis &aResponse)
{
    Status lRetval = kStatus_Success;

    mRequest  = &aRequest;
    mResponse = &aResponse;

    return (lRetval);
}

/**
 *  @brief
 *    Return the request associated with the command.
 *
 *  @returns
 *    A mutable pointer to the command request.
 *
 */
RequestBasis *
ExchangeBasis :: GetRequest(void) const
{
    return (mRequest);
}

/**
 *  @brief
 *    Return the response associated with the command.
 *
 *  @returns
 *    A mutable pointer to the command response.
 *
 */
ResponseBasis *
ExchangeBasis :: GetResponse(void) const
{
    return (mResponse);
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
