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
 *      This file implements an object for....
 *
 */

#include "ConfigurationController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "ConfigurationControllerDelegate.hpp"


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConfigurationController :: ConfigurationController(void) :
    Common::ConfigurationControllerBasis(),
    Client::ConfigurationControllerBasis(),
    Server::ConfigurationControllerBasis(),
    Proxy::ControllerBasis(),
    mDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ConfigurationController :: ~ConfigurationController(void)
{
    return;
}

Status
ConfigurationController :: DoRequestHandlers(const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kLoadFromBackupRequest,
            ConfigurationController::LoadFromBackupRequestReceivedHandler
        },

        {
            kQueryCurrentRequest,
            ConfigurationController::QueryCurrentRequestReceivedHandler
        },

        {
            kResetToDefaultsRequest,
            ConfigurationController::ResetToDefaultsRequestReceivedHandler
        },

        {
            kSaveToBackupRequest,
            ConfigurationController::SaveToBackupRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                         &lRequestHandlers[lRequestHandlerCount],
                                                         this,
                                                         aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer client.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ConfigurationController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ConfigurationControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

#if 1
    lRetval = Client::ConfigurationControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ConfigurationControllerBasis::Init(aServerCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);
#endif

    lRetval = Proxy::ControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // These MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Delegate Management

ConfigurationControllerDelegate *
ConfigurationController :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
ConfigurationController :: SetDelegate(ConfigurationControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

// MARK: Client-facing Server Command Request Completion Handlers

void ConfigurationController :: LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    Server::Command::Configuration::LoadFromBackupResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer                 lResponseBuffer;
    const uint8_t *                                         lBuffer;
    size_t                                                  lSize;
    Status                                                  lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Configuration::LoadFromBackupRequest::kExpectedMatches, done, lStatus = kError_BadCommand);


done:
    return;
}

void ConfigurationController :: QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Server::Command::Configuration::QueryCurrentResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer               lResponseBuffer;
    Status                                                lStatus;
    const uint8_t *                                       lBuffer;
    size_t                                                lSize;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Configuration::QueryCurrentRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // First, allocate and initialize the response buffer.

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Next, invoke the delegate for fanout such that other
    // participants can insert their settings or state to the
    // representation.

    lStatus = OnQueryCurrentConfiguration(aConnection, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }
    else if (lStatus == kError_NotInitialized)
    {
        // There is no static query current configuration response, so
        // we instantiate and initialize one on the stack.

        Client::Command::Configuration::QueryCurrentResponse lQueryCurrentResponse;

        lStatus = lQueryCurrentResponse.Init();
        nlREQUIRE_SUCCESS(lStatus, exit);

        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          lQueryCurrentResponse,
                                          Client::ConfigurationControllerBasis::QueryCompleteHandler,
                                          Client::ConfigurationControllerBasis::CommandErrorHandler,
                                          ConfigurationController::QueryCurrentRequestReceivedHandler,
                                          static_cast<Client::ConfigurationControllerBasis *>(this),
                                          this);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }

 exit:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ConfigurationController :: ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Server::Command::Configuration::ResetToDefaultsResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer                  lResponseBuffer;
    const uint8_t *                                          lBuffer;
    size_t                                                   lSize;
    Status                                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Configuration::ResetToDefaultsRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

done:
    return;
}

void ConfigurationController :: SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Server::Command::Configuration::SavingToBackupResponse  lSavingToBackupNotification;
    Server::Command::Configuration::SaveToBackupResponse    lSaveToBackupResponse;
    ConnectionBuffer::MutableCountedPointer                 lResponseBuffer;
    const uint8_t *                                         lBuffer;
    size_t                                                  lSize;
    Status                                                  lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Configuration::SaveToBackupRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

done:
    return;
}

// MARK: Client-facing Server Command Request Handler Trampolines

void ConfigurationController :: LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->LoadFromBackupRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: QueryCurrentRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCurrentRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: ResetToDefaultsRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->ResetToDefaultsRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->SaveToBackupRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Configuration Delegation Fanout Methods

Status
ConfigurationController :: OnQueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->QueryCurrentConfiguration(*this, aConnection, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

// MARK: Server-facing Client Implementation

// MARK: Client-facing Server Implementation

}; // namespace Proxy

}; // namespace HLX
