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

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ConfigurationControllerCommands.hpp>
#include <OpenHLX/Client/ConfigurationStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

// Notification response data

Command::Configuration::SaveToBackupResponse     ConfigurationControllerBasis::kSaveToBackupResponse;
Command::Configuration::SavingToBackupResponse   ConfigurationControllerBasis::kSavingToBackupResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConfigurationControllerBasis :: ConfigurationControllerBasis(void) :
    Client::ControllerBasis()
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
 *                               future operations with the peer server.
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
ConfigurationControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status          lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Refresh or obtain an up-to-date view of the server peer state.
 *
 *  This attempts to refresh or obtain an up-to-date view of the
 *  server peer state with the specified timeout.
 *
 *  Presently, this controller does so by executing a "query current
 *  configuration [QX]" command with the peer server.
 *
 *  @param[in]  aTimeout  The timeout to use for the refresh operation
 *                        with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ConfigurationControllerBasis :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query current configuration request.

    lRetval = QueryCurrent();
    nlREQUIRE_SUCCESS(lRetval, done);

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
ConfigurationControllerBasis :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kSaveToBackupResponse,
            ConfigurationControllerBasis::SaveToBackupNotificationReceivedHandler
        },

        {
            kSavingToBackupResponse,
            ConfigurationControllerBasis::SavingToBackupNotificationReceivedHandler
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

/**
 *  @brief
 *    Initialize client command response regular expression patterns.
 *
 *  This initializes solicited and unsolicited client command
 *  responses that this controller would like to register to handle.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ConfigurationControllerBasis :: ResponseInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    // Initialize static notification response regular expression pattern data.

    lRetval = kSaveToBackupResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSavingToBackupResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Query the current configuration.
 *
 *  This queries the current and active HLX server configuration.
 *
 *  @note
 *    Because this is involves a lot of data and the
 *    character-at-a-time nature of the physical HLX server, there is
 *    a substantial amount of latency associated with the response to
 *    this command.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ConfigurationControllerBasis :: QueryCurrent(void)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;

    lCommand.reset(new Command::Configuration::QueryCurrent());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Configuration::QueryCurrent>(lCommand)->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ConfigurationControllerBasis::QueryCompleteHandler,
                          ConfigurationControllerBasis::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query current configuration client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the query
 *  current configuration command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
ConfigurationControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    MaybeUpdateRefreshIfRefreshWasRequested();

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous load from backup configuration client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the load from
 *  backup configuration command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
ConfigurationControllerBasis :: LoadFromBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    StateChange::ConfigurationLoadedFromBackupNotification  lStateChangeNotification;
    Status lStatus;


    (void)aExchange;

    nlREQUIRE(aMatches.size() == Command::Configuration::LoadFromBackupResponse::kExpectedMatches, done);

    lStatus = lStateChangeNotification.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous save to backup configuration client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the save
 *  to backup configuration command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
ConfigurationControllerBasis :: SaveToBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SaveToBackupNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous reset to defaults configuration client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the reset
 *  to defaults configuration command request.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command response
 *                         and its original request.
 *  @param[in]  aMatches   An immutable reference to the regular
 *                         expression substring matches associated
 *                         with the client command response that
 *                         triggered this handler.
 *
 */
void
ConfigurationControllerBasis :: ResetToDefaultsCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    StateChange::ConfigurationResetToDefaultsNotification  lStateChangeNotification;
    Status lStatus;


    (void)aExchange;

    nlREQUIRE(aMatches.size() == Command::Configuration::ResetToDefaultsResponse::kExpectedMatches, done);

    lStatus = lStateChangeNotification.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous configuration controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client configuration controller
 *  command request that results in an error response from the HLX
 *  peer server.
 *
 *  @param[in]  aExchange  A mutable shared pointer to the exchange
 *                         associated with the client command error
 *                         and its original request.
 *  @param[in]  aError     An immutable reference to the error
 *                         associated with the failed client command
 *                         request.
 *
 */
void
ConfigurationControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Configuration Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query current configuration client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query current configuration command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
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
ConfigurationControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous load from backup configuration client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the load from backup configuration command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
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
ConfigurationControllerBasis :: LoadFromBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->LoadFromBackupCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous save to backup configuration client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the save to backup configuration command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
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
ConfigurationControllerBasis :: SaveToBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SaveToBackupCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous reset to defaults configuration client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the reset to defaults configuration command request.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             response and its original request.
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
ConfigurationControllerBasis :: ResetToDefaultsCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->ResetToDefaultsCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous configuration controller client command request
 *    error handler trampoline.
 *
 *  This invokes the handler for any asynchronous client configuration
 *  controller command request that results in an error response from
 *  the HLX peer server.
 *
 *  @param[in]      aExchange  A mutable shared pointer to the exchange
 *                             associated with the client command
 *                             error response and its original
 *                             request.
 *  @param[in]      aError     An immutable reference to the error
 *                             associated with the failed command
 *                             request.
 *  @param[in,out]  aContext   A pointer to the controller class
 *                             instance that registered this
 *                             trampoline to call back into from
 *                             the trampoline.
 *
 */
void
ConfigurationControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

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
ConfigurationControllerBasis :: SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    StateChange::ConfigurationSavedToBackupNotification lStateChangeNotification;
    Status                                              lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Configuration::SaveToBackupResponse::kExpectedMatches, done);

    lStatus = lStateChangeNotification.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ConfigurationControllerBasis :: SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    StateChange::ConfigurationSavingToBackupNotification  lStateChangeNotification;
    Status lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Configuration::SavingToBackupResponse::kExpectedMatches, done);

    lStatus = lStateChangeNotification.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

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
ConfigurationControllerBasis :: SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

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
ConfigurationControllerBasis :: SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationControllerBasis *lController = static_cast<ConfigurationControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SavingToBackupNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
