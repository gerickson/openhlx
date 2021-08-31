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

#include "SourcesControllerBasis.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/SourcesControllerCommands.hpp>
#include <OpenHLX/Client/SourcesStateChangeNotifications.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
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

Command::Sources::NameResponse      SourcesControllerBasis::kNameResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
SourcesControllerBasis :: SourcesControllerBasis(Model::SourcesModel &aSourcesModel,
                                                 const Model::SourceModel::IdentifierType &aSourcesMax) :
    Client::ObjectControllerBasis(),
    mSourcesModel(aSourcesModel),
    mSourcesMax(aSourcesMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
SourcesControllerBasis :: ~SourcesControllerBasis(void)
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
SourcesControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status          lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager, aTimeout);
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
 *  The peer server sources controller supports no such commands, so
 *  this is effectively a non-operation.
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
SourcesControllerBasis :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Unlike other collection controllers (that is, favorites,
    // groups, etc.)  the peer server source controller supports no
    // "query source [QIn]" command.  Source state can only be
    // obtained via the configuration controller.

    MaybeUpdateRefreshIfRefreshWasRequested();

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
SourcesControllerBasis :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kNameResponse,
            SourcesControllerBasis::NameNotificationReceivedHandler
        }
    };
    static constexpr size_t  lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Client::ObjectControllerBasis::DoNotificationHandlers(&lNotificationHandlers[0],
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
SourcesControllerBasis :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response regular expression pattern data.

    lRetval = kNameResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous source set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  source set name command request.
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
SourcesControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous sources controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client sources controller
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
SourcesControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Source Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous source set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the source set name command request.
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
SourcesControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    SourcesControllerBasis *lController = static_cast<SourcesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous sources controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client sources
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
SourcesControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    SourcesControllerBasis *lController = static_cast<SourcesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

/**
 *  @brief
 *    Source name changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the source name changed notification.
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
SourcesControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    SourceModel::IdentifierType            lSourceIdentifier;
    const char *                           lName;
    size_t                                 lNameSize;
    SourceModel *                          lSourceModel;
    StateChange::SourcesNameNotification   lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Sources::NameResponse::kExpectedMatches, done);

    // Match 2/3: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mSourcesModel.GetSource(lSourceIdentifier, lSourceModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lSourceModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lSourceIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Source name changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the source name changed notification.
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
SourcesControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    SourcesControllerBasis *lController = static_cast<SourcesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
