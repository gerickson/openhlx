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

#include "ConfigurationControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Request data

Server::Command::Configuration::LoadFromBackupRequest   ConfigurationControllerBasis::kLoadFromBackupRequest;
Server::Command::Configuration::QueryCurrentRequest     ConfigurationControllerBasis::kQueryCurrentRequest;
Server::Command::Configuration::ResetToDefaultsRequest  ConfigurationControllerBasis::kResetToDefaultsRequest;
Server::Command::Configuration::SaveToBackupRequest     ConfigurationControllerBasis::kSaveToBackupRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConfigurationControllerBasis :: ConfigurationControllerBasis(void) :
    Server::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConfigurationControllerBasis :: ~ConfigurationControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

Status
ConfigurationControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ConfigurationControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kLoadFromBackupRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryCurrentRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kResetToDefaultsRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSaveToBackupRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
