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
    Proxy::ObjectControllerBasis(),
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

// MARK: Initializer(s)

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

    lRetval = Client::ConfigurationControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ConfigurationControllerBasis::Init(aServerCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Proxy::ObjectControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
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

// MARK: Implementation

/**
 *  @brief
 *    Register or unregister notification handlers.
 *
 *  This registers or unregisters the solicited and unsolicited client
 *  command response notification handlers that this controller is
 *  interested in and will handle on behalf of the client.
 *
 *  @param[in]  aRegister  Indicates whether to register (true) or
 *                         unregister (false) the handlers.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If either of the handler iterators
 *                                        was null.
 *  @retval  -EEXIST                      If a registration already exists.
 *  @retval  -ENOENT                      If there was no such handler
 *                                        registration to unregister.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ConfigurationController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kSaveToBackupResponse,
            ConfigurationController::SaveToBackupNotificationReceivedHandler
        },

        {
            kSavingToBackupResponse,
            ConfigurationController::SavingToBackupNotificationReceivedHandler
        },
    };
    static constexpr size_t  lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Client::ControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
                                                              &lNotificationHandlers[lNotificationHandlerCount],
                                                              this,
                                                              aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ConfigurationController :: DoRequestHandlers(const bool &aRegister)
{
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

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Save to backup configuration client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  the save to backup configuration state change notification.
 *
 *  @note
 *    The HLX server saves any dirty configuration to non-volatile
 *    storage every 30 seconds. Consequently, a notification such as
 *    this may be received in response to such a save, absent any
 *    client-initiated save to backup command request.
 *
 *  @param[in]  aBuffer   An immutable pointer to the start of the
 *                        buffer extent containing the notification.
 *  @param[in]  aSize     An immutable reference to the size of the
 *                        buffer extent containing the notification.
 *  @param[in]  aMatches  An immutable reference to the regular
 *                        expression substring matches associated
 *                        with the client command response that
 *                        triggered this handler.
 *
 */
void
ConfigurationController :: SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ConfigurationControllerBasis *  lController = static_cast<Client::ConfigurationControllerBasis *>(this);
    Status                                  lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ConfigurationControllerBasis::SaveToBackupNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Saving to backup configuration client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  the saving to backup configuration notification.
 *
 *  @note
 *    The HLX server saves any dirty configuration to non-volatile
 *    storage every 30 seconds. Consequently, a notification such as
 *    this may be received in response to such a save, absent any
 *    client-initiated save to backup command request.
 *
 *  @param[in]  aBuffer   An immutable pointer to the start of the
 *                        buffer extent containing the notification.
 *  @param[in]  aSize     An immutable reference to the size of the
 *                        buffer extent containing the notification.
 *  @param[in]  aMatches  An immutable reference to the regular
 *                        expression substring matches associated
 *                        with the client command response that
 *                        triggered this handler.
 *
 */
void
ConfigurationController :: SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ConfigurationControllerBasis *  lController = static_cast<Client::ConfigurationControllerBasis *>(this);
    Status                                  lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ConfigurationControllerBasis::SavingToBackupNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Save to backup configuration client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the save to backup configuration notification.
 *
 *  @param[in]      aBuffer    An immutable pointer to the start of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aSize      An immutable reference to the size of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
ConfigurationController :: SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->SaveToBackupNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Saving to backup configuration client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the saving to backup configuration notification.
 *
 *  @param[in]      aBuffer    An immutable pointer to the start of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aSize      An immutable reference to the size of the
 *                             buffer extent containing the
 *                             notification.
 *  @param[in]      aMatches   An immutable reference to the regular
 *                             expression substring matches associated
 *                             with the client command response that
 *                             triggered this handler.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
ConfigurationController :: SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->SavingToBackupNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

void ConfigurationController :: LoadFromBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static load from backup configuration response, so
    // we instantiate and initialize one on the stack.

    Client::Command::Configuration::LoadFromBackupResponse lLoadFromBackupResponse;
    Status lStatus;


    lStatus = lLoadFromBackupResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lLoadFromBackupResponse,
                                   Client::ConfigurationControllerBasis::LoadFromBackupCompleteHandler,
                                   Client::ConfigurationControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ConfigurationControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

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
    // There is no static reset to defaults configuration response, so
    // we instantiate and initialize one on the stack.

    Client::Command::Configuration::ResetToDefaultsResponse lResetToDefaultsResponse;
    Status lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lResetToDefaultsResponse,
                                   Client::ConfigurationControllerBasis::ResetToDefaultsCompleteHandler,
                                   Client::ConfigurationControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ConfigurationControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ConfigurationController :: SaveToBackupRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSaveToBackupResponse,
                                   Client::ConfigurationControllerBasis::SaveToBackupCompleteHandler,
                                   Client::ConfigurationControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ConfigurationControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

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

}; // namespace Proxy

}; // namespace HLX
