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
 *      This file defines derived objects for a HLX client
 *      configuration object data model state change notifications
 *      (SCNs).
 *
 */

#ifndef HLXCLIENTCONFIGURATIONSTATECHANGENOTIFICATIONS_HPP
#define HLXCLIENTCONFIGURATIONSTATECHANGENOTIFICATIONS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    An object for a HLX client configuration object data model
 *    configuration-loaded-from-backup state change notification
 *    (SCN).
 *
 *  @ingroup client
 *  @ingroup configuration
 *  @ingroup state-change
 *
 */
class ConfigurationLoadedFromBackupNotification :
    public NotificationBasis
{
public:
    ConfigurationLoadedFromBackupNotification(void) = default;
    virtual ~ConfigurationLoadedFromBackupNotification(void) = default;

    Common::Status Init(void);
};

/**
 *  @brief
 *    An object for a HLX client configuration object data model
 *    configuration-reset-to-defaults state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup configuration
 *  @ingroup state-change
 *
 */
class ConfigurationResetToDefaultsNotification :
    public NotificationBasis
{
public:
    ConfigurationResetToDefaultsNotification(void) = default;
    virtual ~ConfigurationResetToDefaultsNotification(void) = default;

    Common::Status Init(void);
};

/**
 *  @brief
 *    An object for a HLX client configuration object data model
 *    configuration-saving-to-backup state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup configuration
 *  @ingroup state-change
 *
 */
class ConfigurationSavingToBackupNotification :
    public NotificationBasis
{
public:
    ConfigurationSavingToBackupNotification(void) = default;
    virtual ~ConfigurationSavingToBackupNotification(void) = default;

    Common::Status Init(void);
};

/**
 *  @brief
 *    An object for a HLX client configuration object data model
 *    configuration-saved-to-backup state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup configuration
 *  @ingroup state-change
 *
 */
class ConfigurationSavedToBackupNotification :
    public NotificationBasis
{
public:
    ConfigurationSavedToBackupNotification(void) = default;
    virtual ~ConfigurationSavedToBackupNotification(void) = default;

    Common::Status Init(void);
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCONFIGURATIONSTATECHANGENOTIFICATIONS_HPP
