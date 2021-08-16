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
 *      This file implements a base object for realizing a HLX sources
 *      (inputs) controller, either in a client or server context.
 *
 */

#include "SourcesControllerBasis.hpp"

#include <errno.h>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

/**
 *  The maximum number of sources supported by the HLX server controller.
 *
 */
const SourcesControllerBasis::IdentifierType  SourcesControllerBasis::kSourcesMax = 8;

// MARK: Observer Methods

/**
 *  @brief
 *    Get the maximum number of supported HLX sources.
 *
 *  @param[out]  aSources  The maximum number of HLX sources on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
SourcesControllerBasis :: GetSourcesMax(size_t &aSources)
{
    Status  lRetval = kStatus_Success;

    aSources = GetSourcesMax();

    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX sources.
 *
 *  @returns
 *    The maximum number of supported HLX sources.
 *
 */
size_t
SourcesControllerBasis :: GetSourcesMax(void)
{
    return (kSourcesMax);
}

/**
 *  @brief
 *    Determine whether or not an source identifier is valid.
 *
 *  This determines whether or not the specified source
 *  identifier is valid.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source identifer to validate.
 *
 *  @returns
 *    True if the identifier is valid; otherwise, false.
 *
 */
bool
SourcesControllerBasis :: IsValidIdentifier(const IdentifierType &aSourceIdentifier)
{
    const bool lIsValid = (ValidateIdentifier(aSourceIdentifier) == kStatus_Success);


    return (lIsValid);
}

/**
 *  @brief
 *    Validate an source identifier.
 *
 *  This validates the specified source identifier.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the source identifier is
 *                            smaller or larger than supported.
 *
 */
Status
SourcesControllerBasis :: ValidateIdentifier(const IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSourceIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aSourceIdentifier <= kSourcesMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

Status
SourcesControllerBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mSources.Init(kSourcesMax);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
