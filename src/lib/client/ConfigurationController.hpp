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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a HLX server configuration.
 *
 */

#ifndef OPENHLXCLIENTCONFIGURATIONCONTROLLER_HPP
#define OPENHLXCLIENTCONFIGURATIONCONTROLLER_HPP

#include <OpenHLX/Client/ConfigurationControllerBasis.hpp>
#include <OpenHLX/Client/ConfigurationControllerCommands.hpp>
#include <OpenHLX/Common/ConfigurationControllerBasis.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a HLX server configuration.
 *
 *  @ingroup client
 *  @ingroup configuration
 *
 */
class ConfigurationController :
    public Common::ConfigurationControllerBasis,
    public Client::ConfigurationControllerBasis
{
public:
    ConfigurationController(void);
    virtual ~ConfigurationController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Mutator Methods

    Common::Status LoadFromBackup(void);
    Common::Status SaveToBackup(void);
    Common::Status ResetToDefaults(void);
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONFIGURATIONCONTROLLER_HPP
