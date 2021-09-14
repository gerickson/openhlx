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
 *      This file implements a derivable object for composing HLX server
 *      response buffers for commands that query a data model object or
 *      property thereof.
 *
 */

#include "CommandQueryResponseBasis.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an object observation (that is, query) response
 *  against a specific object.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the observation response
 *                               is to formed. For example, "O" for
 *                               a zone object.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryResponseBasis :: Init(const char *aObject)
{
    return (QueryBufferBasis::Init(*this, aObject));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an object observation (that is, query) response
 *  operation against a specific object and identfier.
 *
 *  @param[in]      aObject      A pointer to a null-terminated C
 *                               string representing the object for
 *                               which the observation response
 *                               is to formed. For example, "O" for
 *                               a zone object.
 *  @param[in]  aIdentifier      A reference to the specific object
 *                               identifier for which the query
 *                               response is to be formed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryResponseBasis :: Init(const char *aObject,
                           const IdentifierType &aIdentifier)
{
    return (QueryBufferBasis::Init(*this, aObject, aIdentifier));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
