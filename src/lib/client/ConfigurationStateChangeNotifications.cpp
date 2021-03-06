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
 *      This file implements derived objects for a HLX client configuration
 *      object data model state change notifications (SCNs).
 *
 */

#include "ConfigurationStateChangeNotifications.hpp"

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConfigurationLoadedFromBackupNotification :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_ConfigurationLoadedFromBackup);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConfigurationResetToDefaultsNotification :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_ConfigurationResetToDefaults);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConfigurationSavingToBackupNotification :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_ConfigurationSavingToBackup);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConfigurationSavedToBackupNotification :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_ConfigurationSavedToBackup);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
