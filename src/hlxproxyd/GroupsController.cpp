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

#include "GroupsController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "ProxyCommand.hpp"


using namespace HLX::Client;
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
GroupsController :: GroupsController(void) :
    Proxy::ControllerBasis(),
    Common::GroupsControllerBasis(),
    Client::GroupsControllerBasis(),
    Server::GroupsControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
GroupsController :: ~GroupsController(void)
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
GroupsController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kSetMuteResponse,
            GroupsController::MuteNotificationReceivedHandler
        },

        {
            kNameResponse,
            GroupsController::NameNotificationReceivedHandler
        },

        {
            kSourceResponse,
            GroupsController::SourceNotificationReceivedHandler
        },

        {
            kSetVolumeResponse,
            GroupsController::VolumeNotificationReceivedHandler
        },

        {
            kZoneResponse,
            GroupsController::ZoneNotificationReceivedHandler
        }
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
GroupsController :: DoRequestHandlers(const bool &aRegister)
{
    Status                   lRetval = kStatus_Success;

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
GroupsController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::GroupsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::GroupsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::GroupsControllerBasis::Init();
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

/**
 *  @brief
 *    Refresh or obtain an up-to-date view of the server peer state.
 *
 *  This attempts to refresh or obtain an up-to-date view of the
 *  server peer state with the specified timeout.
 *
 *  Presently, this controller does so by executing a "query
 *  group [QGn]" command with the peer server.
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
GroupsController :: Refresh(const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mGroupsDidRefreshCount = 0;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query group request for each group.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
GroupsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status                                lRetval = kStatus_Success;


    (void)aConnection;

    // For each group, query the configuration.

done:
    return (lRetval);
}

// MARK: Server-facing Client Observer Methods


/**
 *  @brief
 *    Query the current state of all groups.
 *
 *  This queries the current state of all HLX server groups.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
GroupsController :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (IdentifierType lGroupIdentifier = IdentifierModel::kIdentifierMin; lGroupIdentifier <= kGroupsMax; lGroupIdentifier++)
    {
        lRetval = Query(lGroupIdentifier);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of a group.
 *
 *  This queries the current state of the specified HLX server
 *  group.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the identifier
 *                                of the group to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the group identifier is
 *                                        smaller or larger than supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
GroupsController :: Query(const IdentifierType &aGroupIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Groups::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Groups::Query>(lCommand)->Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          GroupsController::QueryCompleteHandler,
                          GroupsController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query group client command response completion
 *    handler.
 *
 *  This handles an asynchronous client command response for the query
 *  group command request.
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
GroupsController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    IdentifierType                 lGroupIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mGroupsDidRefreshCount++;

    MaybeUpdateRefreshIfRefreshWasRequested(static_cast<uint8_t>(mGroupsDidRefreshCount),
                                            static_cast<uint8_t>(kGroupsMax));

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous group set volume mute client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  group set volume mute command request.
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
GroupsController :: SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    MuteNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous group toggle (flip) volume mute client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the group
 *  toggle (flip) volume mute command request.
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
GroupsController :: ToggleMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis *         lResponse   = aExchange->GetResponse();
    const uint8_t *                        lBuffer     = lResponse->GetBuffer()->GetHead();
    IdentifierType                         lGroupIdentifier;
    GroupModel *                           lGroupModel;
    VolumeModel::MuteType                  lMute;
    Status                                 lStatus;


    nlREQUIRE(aMatches.size() == Client::Command::Groups::ToggleMuteResponse::kExpectedMatches, done);

    // Match 2/2: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // As with many aspects of group state, the state of a group mute
    // toggle is handled differently since the command completion does
    // NOT contain the ending state. The command completion only
    // contains a confirmation that the toggle did
    // occur. Consequently, we have to manually derive the group mute
    // state.

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lGroupModel->GetMute(lMute);
    nlEXPECT_SUCCESS(lStatus, done);

    // On the assumption that the toggle was successfully, flip the
    // current mute state and invoke the change handler.

    HandleMuteChange(lGroupIdentifier, *lGroupModel, !lMute);

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous group set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  group set name command request.
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
GroupsController :: SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous group set source (input) client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the group
 *  set source (input) command request.
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
GroupsController :: SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SourceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous group set volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  group set volume level command request.
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
GroupsController :: SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous group increase volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  group increase volume level command request.
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
GroupsController :: IncreaseVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis *                 lResponse   = aExchange->GetResponse();
    const uint8_t *                                lBuffer     = lResponse->GetBuffer()->GetHead();
    IdentifierType                                 lGroupIdentifier;
    Status                                         lStatus;
    StateChange::GroupsIncreaseVolumeNotification  lStateChangeNotification;


    nlREQUIRE(aMatches.size() == Client::Command::Groups::IncreaseVolumeResponse::kExpectedMatches, done);

    // Match 2/2: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // As with all things in the group controller, the publisher
    // (server) does not send an unmute notification along with the
    // volume notification if the group volume was changed while muted,
    // as is the case with the zone controller. Consequently, when
    // requested, an unmute state change notification must be
    // synthesized and sent along before the volume state change
    // notification.

    HandleUnmuteChange(lGroupIdentifier);

    // As with many aspects of group state, the state of a group
    // volume increase toggle is handled differently since the command
    // completion does NOT contain the ending state.
    //
    // The command completion only contains a confirmation that the
    // volume increase did successfully occur. This is because volume
    // increases are saturating across all zones in the group. Each
    // zone will increment by one from its current value; however,
    // those that have already reached the maximum value will remain
    // at the maximum. As such, even a simple average of all the zones
    // could not be represented with fidelity in the typical integral
    // volume response format.
    //
    // Consequently, we generate an internal-only event to indicate
    // that successful volume increase that will get trapped by the
    // parent controller. The parent controller will then handle
    // state: both fan out of constituent zones state but also the
    // group volume state itself.

    lStatus = lStateChangeNotification.Init(lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous group decrease volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  group decrease volume level command request.
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
GroupsController :: DecreaseVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis *                 lResponse   = aExchange->GetResponse();
    const uint8_t *                                lBuffer     = lResponse->GetBuffer()->GetHead();
    IdentifierType                                 lGroupIdentifier;
    Status                                         lStatus;
    StateChange::GroupsDecreaseVolumeNotification  lStateChangeNotification;


    nlREQUIRE(aMatches.size() == Client::Command::Groups::DecreaseVolumeResponse::kExpectedMatches, done);

    // Match 2/2: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // As with all things in the group controller, the publisher
    // (server) does not send an unmute notification along with the
    // volume notification if the group volume was changed while muted,
    // as is the case with the zone controller. Consequently, when
    // requested, an unmute state change notification must be
    // synthesized and sent along before the volume state change
    // notification.

    HandleUnmuteChange(lGroupIdentifier);

    // As with many aspects of group state, the state of a group
    // volume increase toggle is handled differently since the command
    // completion does NOT contain the ending state.
    //
    // The command completion only contains a confirmation that the
    // volume decrease did successfully occur. This is because volume
    // decreases are saturating across all zones in the group. Each
    // zone will decrement by one from its current value; however,
    // those that have already reached the minimum value will remain
    // at the minimum. As such, even a simple average of all the zones
    // could not be represented with fidelity in the typical integral
    // volume response format.
    //
    // Consequently, we generate an internal-only event to indicate
    // that successful volume decrease that will get trapped by the
    // parent controller. The parent controller will then handle
    // state: both fan out of constituent zones state but also the
    // group volume state itself.

    lStatus = lStateChangeNotification.Init(lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous group change (add/remove) zone client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the group
 *  change (add/remove) zone command request.
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
GroupsController :: ChangeZoneCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    ZoneNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous groups controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client groups controller
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
GroupsController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Group Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query group client command response completion
 *    handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query group command request.
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
GroupsController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group set volume mute client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group set volume mute command request.
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
GroupsController :: SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetMuteCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group toggle (flip) volume mute client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group toggle (flip) volume mute command request.
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
GroupsController :: ToggleMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToggleMuteCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group set name command request.
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
GroupsController :: SetNameCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group set source (input) client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group set source (input) mute command request.
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
GroupsController :: SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group change (add/remove) zone client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group change (add/remove) zone command request.
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
GroupsController :: ChangeZoneCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ChangeZoneCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group set volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group set volume level command request.
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
GroupsController :: SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group increase volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group increase volume level command request.
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
GroupsController :: IncreaseVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseVolumeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous group decrease volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the group decrease volume level command request.
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
GroupsController :: DecreaseVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseVolumeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous groups controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client groups
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
GroupsController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Group volume mute state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the group volume mute state changed notification.
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
 *  @ingroup volume
 *
 */
void
GroupsController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    const char *                           lMutep;
    IdentifierType                         lGroupIdentifier;
    VolumeModel::MuteType                  lMute;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Groups::SetMuteResponse::kExpectedMatches, done);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleMuteChange(lGroupIdentifier, lMute);

 done:
    return;
}

/**
 *  @brief
 *    Group name changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the group name changed notification.
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
GroupsController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lGroupIdentifier;
    const char *                           lName;
    size_t                                 lNameSize;
    GroupModel *                           lGroupModel;
    StateChange::GroupsNameNotification    lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Groups::NameResponse::kExpectedMatches, done);

    // Match 2/3: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lGroupModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lGroupIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Group source (input) changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the group source (input) changed notification.
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
GroupsController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lGroupIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Groups::SourceResponse::kExpectedMatches, done);

    // Match 2/3: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleSourceChange(lGroupIdentifier, lSourceIdentifier);

 done:
    return;
}

/**
 *  @brief
 *    Group volume level state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the group volume level state changed notification.
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
 *  @ingroup volume
 *
 */
void
GroupsController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lGroupIdentifier;
    VolumeModel::LevelType                 lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Groups::SetVolumeResponse::kExpectedMatches, done);

    // Match 2/3: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    // As with all things in the group controller, the publisher
    // (server) does not send an unmute notification along with the
    // volume notification if the group volume was changed while muted,
    // as is the case with the zone controller. Consequently, when
    // requested, an unmute state change notification must be
    // requested and sent along with the volume state change.

    {
        const bool  lHandleUnmute = true;

        HandleVolumeChange(lGroupIdentifier, lVolume, lHandleUnmute);
    }

 done:
    return;
}

/**
 *  @brief
 *    Group zone membership state changed client unsolicited
 *    notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the group zone membership state changed notification.
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
GroupsController :: ZoneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                         lGroupIdentifier;
    char                                   lZoneOperation;
    bool                                   lIsZoneAdded;
    IdentifierType                         lZoneIdentifier;
    GroupModel *                           lGroupModel;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Groups::ZoneResponse::kExpectedMatches, done);

    // Match 2/4: Group Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Zone Operation

    lZoneOperation = *((const char *)(aBuffer) + aMatches.at(2).rm_so);
    lIsZoneAdded = (lZoneOperation == 'A');

    // Match 3/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(3).rm_so,
                                                Common::Utilities::Distance(aMatches.at(3)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mGroups.GetGroup(lGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    if (lIsZoneAdded)
    {
        StateChange::GroupsZoneAddedNotification lStateChangeNotification;

        // If the zone has already been added, AddZone will return
        // kStatus_ValueAlreadySet and there will be no need to send a
        // state change notification. If we receive kStatus_Success,
        // it is a first time addition and a state change notification
        // needs to be sent.

        lStatus = lGroupModel->AddZone(lZoneIdentifier);
        nlEXPECT_SUCCESS(lStatus, done);

        lStatus = lStateChangeNotification.Init(lGroupIdentifier, lZoneIdentifier);
        nlREQUIRE_SUCCESS(lStatus, done);

        OnStateDidChange(lStateChangeNotification);
    }
    else
    {
        StateChange::GroupsZoneRemovedNotification lStateChangeNotification;

        // If the zone has already been removed, RemoveZone will
        // return -ENOENT and there will be no need to send a state
        // change notification. If we receive kStatus_Success, it is
        // the first time removal and a state change notification
        // needs to be sent.

        lStatus = lGroupModel->RemoveZone(lZoneIdentifier);
        nlEXPECT_SUCCESS(lStatus, done);

        lStatus = lStateChangeNotification.Init(lGroupIdentifier, lZoneIdentifier);
        nlREQUIRE_SUCCESS(lStatus, done);

        OnStateDidChange(lStateChangeNotification);
    }

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Group volume mute state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the group volume mute state changed notification.
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
GroupsController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Group name changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the group name changed notification.
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
GroupsController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Group source (input) changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the group source (input) changed notification.
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
GroupsController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SourceNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Group volume level state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the group volume level state changed notification.
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
GroupsController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Group zone membership state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the group zone membership state changed
 *  notification.
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
GroupsController :: ZoneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ZoneNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

// MARK: Client-facing Server Command Request Handler Trampolines

// MARK: Proxy Handlers

// MARK: Proxy Handler Trampolines

// MARK: Server-facing Client Implementation

void
GroupsController :: HandleMuteChange(const IdentifierType &aGroupIdentifier,
                                     const Model::VolumeModel::MuteType &aMute)
{
    GroupModel *                           lGroupModel;
    StateChange::GroupsMuteNotification    lStateChangeNotification;
    Status                                 lStatus;


    lStatus = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleMuteChange(aGroupIdentifier, *lGroupModel, aMute);

 done:
    return;
}

void
GroupsController :: HandleUnmuteChange(const IdentifierType &aGroupIdentifier)
{
    const VolumeModel::MuteType  lMute = true;

    HandleMuteChange(aGroupIdentifier, !lMute);
}

void
GroupsController :: HandleVolumeChange(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume)
{
    GroupModel *                           lGroupModel;
    StateChange::GroupsVolumeNotification  lStateChangeNotification;
    Status                                 lStatus;


    lStatus = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleVolumeChange(aGroupIdentifier, *lGroupModel, aVolume);

 done:
    return;
}

void
GroupsController :: HandleVolumeChange(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute)
{
    GroupModel *                           lGroupModel;
    StateChange::GroupsVolumeNotification  lStateChangeNotification;
    Status                                 lStatus;


    lStatus = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleVolumeChange(aGroupIdentifier, *lGroupModel, aVolume, aHandleUnmute);

 done:
    return;
}

void
GroupsController :: HandleSourceChange(const IdentifierType &aGroupIdentifier,
                                            const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    GroupModel *                           lGroupModel;
    StateChange::GroupsSourceNotification  lStateChangeNotification;
    Status                                 lStatus;


    lStatus = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleSourceChange(aGroupIdentifier, *lGroupModel, aSourceIdentifier);

 done:
    return;
}

void
GroupsController :: HandleSourcesChange(const IdentifierType &aGroupIdentifier,
                                        const Model::GroupModel::Sources &aSourceIdentifierCollection)
{
    GroupModel *                           lGroupModel;
    StateChange::GroupsSourceNotification  lStateChangeNotification;
    Status                                 lStatus;


    lStatus = mGroups.GetGroup(aGroupIdentifier, lGroupModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleSourcesChange(aGroupIdentifier, *lGroupModel, aSourceIdentifierCollection);

 done:
    return;
}

void
GroupsController :: HandleMuteChange(const IdentifierType &aGroupIdentifier,
                                     Model::GroupModel &aGroupModel,
                                     const Model::VolumeModel::MuteType &aMute)
{
    StateChange::GroupsMuteNotification    lStateChangeNotification;
    Status                                 lStatus;


    // If the mute status is unchanged, SetMute will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = aGroupModel.SetMute(aMute);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aGroupIdentifier, aMute);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

void
GroupsController :: HandleUnmuteChange(const IdentifierType &aGroupIdentifier,
                                       Model::GroupModel &aGroupModel)
{
    const VolumeModel::MuteType  lMute = true;

    HandleMuteChange(aGroupIdentifier, aGroupModel, !lMute);
}

void
GroupsController :: HandleVolumeChange(const IdentifierType &aGroupIdentifier,
                                       Model::GroupModel &aGroupModel,
                                       const Model::VolumeModel::LevelType &aVolume)
{
    const bool  lHandleUnmute = false;

    HandleVolumeChange(aGroupIdentifier, aGroupModel, aVolume, lHandleUnmute);
}

void
GroupsController :: HandleVolumeChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume, const bool &aHandleUnmute)
{
    StateChange::GroupsVolumeNotification  lStateChangeNotification;
    Status                                 lStatus;


    // If the volume is unchanged, SetVolume will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = aGroupModel.SetVolume(aVolume);
    nlEXPECT_SUCCESS(lStatus, done);

    // As with all things in the group controller, the publisher
    // (server) does not send an unmute notification along with the
    // volume notification if the group volume was changed while muted,
    // as is the case with the zone controller. Consequently, when
    // requested, an unmute state change notification must be
    // synthesized and sent along with the volume state change
    // notification.

    if (aHandleUnmute)
    {
        HandleUnmuteChange(aGroupIdentifier, aGroupModel);
    }

    lStatus = lStateChangeNotification.Init(aGroupIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

void
GroupsController :: HandleSourceChange(const IdentifierType &aGroupIdentifier, Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    StateChange::GroupsSourceNotification  lStateChangeNotification;
    Status                                 lStatus;


    // If the source is unchanged, SetSource will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = aGroupModel.SetSource(aSourceIdentifier);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aGroupIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

void
GroupsController :: HandleSourcesChange(const IdentifierType & aGroupIdentifier,
                                        Model::GroupModel & aGroupModel,
                                        const Model::GroupModel::Sources & aSourceIdentifierCollection)
{
    StateChange::GroupsSourceNotification  lStateChangeNotification;
    Status                                 lStatus;


    // If the source is unchanged, SetSource will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = aGroupModel.SetSources(aSourceIdentifierCollection);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aGroupIdentifier, aSourceIdentifierCollection);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}


// MARK: Client-facing Server Implementation

}; // namespace Proxy

}; // namespace HLX