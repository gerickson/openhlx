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
 *      This file defines an object for managing the server-side
 *      observation and mutation of a HLX server configuration.
 *
 */

#ifndef OPENHLXSIMULATORCONFIGURATIONCONTROLLER_HPP
#define OPENHLXSIMULATORCONFIGURATIONCONTROLLER_HPP

#include <CoreFoundation/CFDictionary.h>

#include <OpenHLX/Server/ConfigurationControllerCommands.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

class ConfigurationControllerDelegate;

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a HLX server configuration.
 *
 *  @ingroup server
 *  @ingroup configuration
 *
 */
class ConfigurationController :
    public Simulator::ControllerBasis
{
public:
    ConfigurationController(void);
    ~ConfigurationController(void);

    Common::Status Init(Server::CommandManager &aCommandManager) final;

    // Delegate Management

    Common::Status SetDelegate(ConfigurationControllerDelegate *aDelegate);
    ConfigurationControllerDelegate *GetDelegate(void) const;

    // Configuration Management Methods

    Common::Status LoadFromBackup(void);
    Common::Status ResetToDefaults(void);
    Common::Status SaveToBackup(void);

    // Command Request Handler Trampolines

    static void LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    // Command Completion Handlers

    void LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

    // Delegate Fanout Methods

    Common::Status OnLoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary);
    Common::Status OnLoadFromBackupConfigurationStorage(CFDictionaryRef &aBackupDictionary);
    void OnQueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    void OnResetToDefaultConfiguration(void);
    void OnSaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary);
    Common::Status OnSaveToBackupConfigurationStorage(CFDictionaryRef aBackupDictionary);

private:
    ConfigurationControllerDelegate *                      mDelegate;

private:
    static Server::Command::Configuration::LoadFromBackupRequest   kLoadFromBackupRequest;
    static Server::Command::Configuration::QueryCurrentRequest     kQueryCurrentRequest;
    static Server::Command::Configuration::ResetToDefaultsRequest  kResetToDefaultsRequest;
    static Server::Command::Configuration::SaveToBackupRequest     kSaveToBackupRequest;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORCONFIGURATIONCONTROLLER_HPP
