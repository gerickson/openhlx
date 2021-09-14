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
 *      This file defines a derivable object for realizing a HLX
 *      configuration controller, in a server.
 *
 */

#ifndef OPENHLXSERVERCONFIGURATIONCONTROLLERBASIS_HPP
#define OPENHLXSERVERCONFIGURATIONCONTROLLERBASIS_HPP

#include <OpenHLX/Server/ConfigurationControllerCommands.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A derivable object for realizing a HLX configuration controller,
 *    in a server.
 *
 *  @ingroup server
 *  @ingroup configuration
 *
 */
class ConfigurationControllerBasis :
    public Server::ObjectControllerBasis
{
public:
    virtual ~ConfigurationControllerBasis(void);

protected:
    ConfigurationControllerBasis(void);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

protected:
    static Server::Command::Configuration::LoadFromBackupRequest   kLoadFromBackupRequest;
    static Server::Command::Configuration::QueryCurrentRequest     kQueryCurrentRequest;
    static Server::Command::Configuration::ResetToDefaultsRequest  kResetToDefaultsRequest;
    static Server::Command::Configuration::SaveToBackupRequest     kSaveToBackupRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONFIGURATIONCONTROLLERBASIS_HPP
