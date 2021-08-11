/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements an object for...
 *
 */

#include "GroupsControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

// Notification response data

Command::Groups::SetMuteResponse         GroupsControllerBasis::kSetMuteResponse;
Command::Groups::NameResponse            GroupsControllerBasis::kNameResponse;
Command::Groups::QueryResponse           GroupsControllerBasis::kQueryResponse;
Command::Groups::SourceResponse          GroupsControllerBasis::kSourceResponse;
Command::Groups::SetVolumeResponse       GroupsControllerBasis::kSetVolumeResponse;
Command::Groups::ZoneResponse            GroupsControllerBasis::kZoneResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
GroupsControllerBasis :: GroupsControllerBasis(void) :
    mGroupsDidRefreshCount(0)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
GroupsControllerBasis :: ~GroupsControllerBasis(void)
{
    return;
}

Status
GroupsControllerBasis :: Init(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Initialize client command response regular expression patterns.
 *
 *  This initializes solicited and unsolicited client command
 *  responses that this controller would like to register to handle.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
GroupsControllerBasis :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response data.

    lRetval = kSetMuteResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kNameResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSourceResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kZoneResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
