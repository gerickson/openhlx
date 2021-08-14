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
#include "ProxyCommand.hpp"


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

namespace Detail
{
    // XXX - Need to figure out how to make the lifetime of this
    // persist across multiple, potentially-failed proxy requests and
    // how to limit the number of proxy requests since infinite loops
    // may be introduced.

    struct ProxyContext
    {
        Server::ConnectionBasis *  mConnection;
        const uint8_t *            mBuffer;
        size_t                     mSize;
        RegularExpression::Matches mMatches;
        Client::CommandManager::OnCommandCompleteFunc mOnCommandCompleteHandler;
        Client::CommandManager::OnCommandErrorFunc    mOnCommandErrorHandler;
        Server::CommandManager::OnRequestReceivedFunc mOnRequestReceivedHandler;
        void *                                        mTheirContext;
        void *                                        mOurContext;
    };
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConfigurationController :: ConfigurationController(void) :
    Proxy::ControllerBasis(),
    Common::ConfigurationControllerBasis(),
    Client::ConfigurationControllerBasis(),
    Server::ConfigurationControllerBasis(),
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
ConfigurationController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ConfigurationControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::ConfigurationControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ConfigurationControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Proxy::ControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // These MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

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

Status
ConfigurationController :: ProxyCommand(Server::ConnectionBasis &aConnection,
                                const uint8_t *aBuffer,
                                const size_t &aSize,
                                const Common::RegularExpression::Matches &aMatches,
                                const Client::Command::ResponseBasis &aResponse,
                                Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    std::unique_ptr<Detail::ProxyContext>                 lProxyContext;
    Status                                                lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBuffer != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aSize > 0, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandCompleteHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandErrorHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnRequestReceivedHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aContext != nullptr, done, lRetval = -EINVAL);

    lProxyContext.reset(new Detail::ProxyContext());
    nlREQUIRE_ACTION(lProxyContext, done, lRetval = -ENOMEM);

    lProxyContext->mConnection               = &aConnection;
    lProxyContext->mBuffer                   = aBuffer;
    lProxyContext->mSize                     = aSize;
    lProxyContext->mMatches                  = aMatches;
    lProxyContext->mOnCommandCompleteHandler = aOnCommandCompleteHandler;
    lProxyContext->mOnCommandErrorHandler    = aOnCommandErrorHandler;
    lProxyContext->mOnRequestReceivedHandler = aOnRequestReceivedHandler;
    lProxyContext->mTheirContext             = aContext;
    lProxyContext->mOurContext               = this;

    lCommand.reset(new Proxy::Command::Proxy());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Proxy::Command::Proxy>(lCommand)->Init(aBuffer, aSize, aResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ConfigurationController::ProxyCompleteHandler,
                          ConfigurationController::ProxyErrorHandler,
                          lProxyContext.release());

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
ConfigurationController :: Refresh(const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
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

// MARK: Server-facing Client Observer Methods

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
ConfigurationController :: QueryCurrent(void)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;

    lCommand.reset(new Client::Command::Configuration::QueryCurrent());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Configuration::QueryCurrent>(lCommand)->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand, ConfigurationController::QueryCompleteHandler, ConfigurationController::CommandErrorHandler, this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

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
ConfigurationController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                           lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();


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
ConfigurationController :: LoadFromBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);


    (void)aExchange;

    nlREQUIRE(aMatches.size() == Client::Command::Configuration::LoadFromBackupResponse::kExpectedMatches, done);


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
ConfigurationController :: SaveToBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                        lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                           lBufferSize = lResponse->GetBuffer()->GetSize();

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
ConfigurationController :: ResetToDefaultsCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);


    (void)aExchange;

    nlREQUIRE(aMatches.size() == Client::Command::Configuration::ResetToDefaultsResponse::kExpectedMatches, done);

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
ConfigurationController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                        lBuffer     = lRequest->GetBuffer();
    const size_t                           lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Configuration Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

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
ConfigurationController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

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
ConfigurationController :: LoadFromBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

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
ConfigurationController :: SaveToBackupCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

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
ConfigurationController :: ResetToDefaultsCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

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
ConfigurationController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
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
    DeclareScopedFunctionTracer(lTracer);


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Configuration::SaveToBackupResponse::kExpectedMatches, done);

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
    DeclareScopedFunctionTracer(lTracer);


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Configuration::SavingToBackupResponse::kExpectedMatches, done);

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
        Client::Command::Configuration::QueryCurrentResponse lQueryCurrentResponse;

        lStatus = lQueryCurrentResponse.Init();
        nlREQUIRE_SUCCESS(lStatus, exit);

        lStatus = ProxyCommand(aConnection,
                               aBuffer,
                               aSize,
                               aMatches,
                               lQueryCurrentResponse,
                               ConfigurationController::QueryCompleteHandler,
                               ConfigurationController::CommandErrorHandler,
                               ConfigurationController::QueryCurrentRequestReceivedHandler,
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

// MARK: Proxy Handlers

void
ConfigurationController :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                     const Common::Error &aError,
                                     Server::ConnectionBasis &aConnection,
                                     Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                     void * aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lStatus;

    aOnCommandErrorHandler(aExchange, aError, aContext);

    lStatus = SendErrorResponse(aConnection);
    nlVERIFY_SUCCESS(lStatus);
}

void
ConfigurationController :: ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                        const Common::RegularExpression::Matches &aClientMatches,
                                        Server::ConnectionBasis &aConnection,
                                        const uint8_t *aBuffer,
                                        const size_t &aSize,
                                        const Common::RegularExpression::Matches &aServerMatches,
                                        Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                        Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                        void * aContext)
{
    DeclareScopedFunctionTracer(lTracer);

    aOnCommandCompleteHandler(aExchange, aClientMatches, aContext);

    aOnRequestReceivedHandler(aConnection, aBuffer, aSize, aServerMatches, aContext);
}

// MARK: Proxy Handler Trampolines

/* static */ void
ConfigurationController :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ConfigurationController *lController = static_cast<ConfigurationController *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyErrorHandler(aExchange,
                                           aError,
                                           *lContext->mConnection,
                                           lContext->mOnCommandErrorHandler,
                                           lContext->mTheirContext);
        }

        delete lContext;
    }
}

/* static */ void
ConfigurationController :: ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);    

    if (lContext != nullptr)
    {
        ConfigurationController *lController = static_cast<ConfigurationController *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyCompleteHandler(aExchange,
                                              aMatches,
                                              *lContext->mConnection,
                                              lContext->mBuffer,
                                              lContext->mSize,
                                              lContext->mMatches,
                                              lContext->mOnCommandCompleteHandler,
                                              lContext->mOnRequestReceivedHandler,
                                              lContext->mTheirContext);

        }

        delete lContext;
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
