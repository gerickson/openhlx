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
 *      This file defines a delegate interface for the proxy
 *      configuration controller object.
 *
 */

#ifndef OPENHLXPROXYCONFIGURATIONCONTROLLERDELEGATE_HPP
#define OPENHLXPROXYCONFIGURATIONCONTROLLERDELEGATE_HPP

#include <CoreFoundation/CFDictionary.h>


namespace HLX
{

namespace Server
{

class ConnectionBasis;

}; // namespace Server

namespace Proxy
{

class ConfigurationController;

/**
 *  @brief
 *    This file defines a delegate interface for the proxy
 *    configuration controller object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the management of HLX server
 *  configuration, including:
 *
 *    - Querying the current configuration.
 *
 *  @ingroup proxy
 *  @ingroup configuration
 */
class ConfigurationControllerDelegate
{
public:
    ConfigurationControllerDelegate(void) = default;
    virtual ~ConfigurationControllerDelegate(void) = default;

    virtual Common::Status QueryCurrentConfiguration(ConfigurationController &aController, Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) = 0;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXPROXYCONFIGURATIONCONTROLLERDELEGATE_HPP
