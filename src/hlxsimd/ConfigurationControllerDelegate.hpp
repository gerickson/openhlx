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
 *      This file defines a delegate interface for the server
 *      simulator configuration controller object.
 *
 */

#ifndef OPENHLXSIMULATORCONFIGURATIONCONTROLLERDELEGATE_HPP
#define OPENHLXSIMULATORCONFIGURATIONCONTROLLERDELEGATE_HPP

#include <CoreFoundation/CFDictionary.h>

#include "ControllerBasisDelegate.hpp"


namespace HLX
{

namespace Server
{

class ConnectionBasis;

}; // namespace Server

namespace Simulator
{

class ConfigurationController;

/**
 *  @brief
 *    This file defines a delegate interface for the server
 *    configuration controller object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the management of HLX server
 *  configuration, including:
 *
 *    - Loading back up configuration from non-volatile storage.
 *    - Deserializing back up configuration from a backup representation.
 *    - Querying the current configuration.
 *    - Resetting to default configuration.
 *    - Serialzing back up configuration to a backup representation.
 *    - Storing backup configuration to non-volatile storage.
 *
 *  @ingroup server
 *  @ingroup configuration
 */
class ConfigurationControllerDelegate
{
public:
    ConfigurationControllerDelegate(void) = default;
    virtual ~ConfigurationControllerDelegate(void) = default;

    virtual Common::Status LoadFromBackupConfiguration(ConfigurationController &aController, CFDictionaryRef aBackupConfiguration) = 0;
    virtual Common::Status LoadFromBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef &aBackupDictionary) = 0;
    virtual void           QueryCurrentConfiguration(ConfigurationController &aController, Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) = 0;
    virtual void           ResetToDefaultConfiguration(ConfigurationController &aController) = 0;
    virtual void           SaveToBackupConfiguration(ConfigurationController &aController, CFMutableDictionaryRef aBackupDictionary) = 0;
    virtual Common::Status SaveToBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef aBackupDictionary) = 0;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORCONFIGURATIONCONTROLLERDELEGATE_HPP
