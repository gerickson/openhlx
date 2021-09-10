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
 *      This file implements objects for HLX server source data
 *      model commands and their constituent requests and responses.
 *
 */

#include "SourcesControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Sources
{

static const char * const kSourceObject = "I";

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set name command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the source name command response buffer.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference
 *                                 for the source identifier
 *                                 for which to form the name
 *                                 response.
 *  @param[in]  aName              A pointer to the null-
 *                                 terminated C string of the
 *                                 source name for
 *                                 which to form the
 *                                 response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier,
                     const char * aName)
{
    return (NameSetResponseBasis::Init(kSourceObject,
                                       aSourceIdentifier,
                                       aName));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the source name command response buffer.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference
 *                                 for the source
 *                                 identifier for which to
 *                                 form the name response.
 *  @param[in]  aName              A pointer to the null-
 *                                 terminated C string of the
 *                                 source name for
 *                                 which to form the
 *                                 response.
 *  @param[in]  aNameLength        An immutable reference to
 *                                 the length, in bytes, of
 *                                 @a aName.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::SourceModel::IdentifierType &aSourceIdentifier,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kSourceObject,
                                       aSourceIdentifier,
                                       aName,
                                       aNameLength));
}

}; // namespace Sources

}; // namespace Command

}; // namespace Server

}; // namespace HLX
