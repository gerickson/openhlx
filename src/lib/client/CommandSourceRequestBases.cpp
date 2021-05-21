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
 *      This file implements derivable objects for HLX client source
 *      (input) data model property mutation command request buffers.
 *
 */

#include "CommandSourceRequestBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer for a source (input) mutation
 *    request operation.
 *
 *  This initializes a source (input) property mutation request
 *  operation against a specific object identifier
 *
 *  @param[in]  aObject      A pointer to a null-terminated C string
 *                           representing the object for which the
 *                           source (input) mutation request is to be
 *                           made against. For example, "O" for a zone
 *                           object.
 *  @param[in]  aIdentifier  A reference to the specific object
 *                           identifier which the source (input)
 *                           mutation request is to be made against.
 *  @param[in]  aOperation   A pointer to a null-terminated C string
 *                           representing the operation to perform on
 *                           the mutation request.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceRequestBasis :: Init(const char *aObject,
                           const IdentifierModel::IdentifierType &aIdentifier,
                           const char *aOperation)
{
    return (SourceBufferBasis::Init(*this,
                                    aObject,
                                    aIdentifier,
                                    aOperation));
}

/**
 *  @brief
 *    This is the class initializer for a source (input) set request
 *    operation.
 *
 *  This initializes the source (input) property set request operation
 *  of a specified source against a specific object and identifier.
 *
 *  @param[in]      aObject            A pointer to a null-terminated
 *                                     C string representing the
 *                                     object for which the source
 *                                     (input) set request operation
 *                                     is to be made against. For
 *                                     example, "O" for a zone object.
 *  @param[in]      aIdentifier        A reference to the specific
 *                                     object dentifier which the
 *                                     source (input) set request
 *                                     operation is to be made
 *                                     against.
 *  @param[in]      aSourceIdentifier  An immutable reference to the
 *                                     source (input) to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceSetRequestBasis :: Init(const char *aObject,
                              const IdentifierModel::IdentifierType &aIdentifier,
                              const IdentifierModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetBufferBasis::Init(*this,
                                       aObject,
                                       aIdentifier,
                                       aSourceIdentifier));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
