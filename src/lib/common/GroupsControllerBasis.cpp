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
 *      This file implements a base object for realizing a HLX groups
 *      controller, either in a client or server context.
 *
 */

#include "GroupsControllerBasis.hpp"

#include <errno.h>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

/**
 *  The maximum number of groups supported by the HLX server
 *  controller.
 *
 */
const GroupsControllerBasis::IdentifierType  GroupsControllerBasis::kGroupsMax = 10;

// MARK: Observer Methods

/**
 *  @brief
 *    Get the maximum number of supported HLX groups.
 *
 *  @param[out]  aGroups  The maximum number of HLX groups on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
GroupsControllerBasis :: GetGroupsMax(IdentifierType &aGroups)
{
    Status  lRetval = kStatus_Success;

    aGroups = GetGroupsMax();

    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX groups.
 *
 *  @returns
 *    The maximum number of supported HLX groups.
 *
 */
GroupsControllerBasis::IdentifierType
GroupsControllerBasis :: GetGroupsMax(void)
{
    return (kGroupsMax);
}

/**
 *  @brief
 *    Determine whether or not an group identifier is valid.
 *
 *  This determines whether or not the specified group
 *  identifier is valid.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the
 *                                group identifer to validate.
 *
 *  @returns
 *    True if the identifier is valid; otherwise, false.
 *
 */
bool
GroupsControllerBasis :: IsValidIdentifier(const IdentifierType &aGroupIdentifier)
{
    const bool lIsValid = (ValidateIdentifier(aGroupIdentifier) == kStatus_Success);


    return (lIsValid);
}

/**
 *  @brief
 *    Validate an group identifier.
 *
 *  This validates the specified group identifier.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the
 *                                group identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *
 */
Status
GroupsControllerBasis :: ValidateIdentifier(const IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aGroupIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aGroupIdentifier <= kGroupsMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

Status
GroupsControllerBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mGroups.Init(kGroupsMax);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Common

}; // namespace HLX
