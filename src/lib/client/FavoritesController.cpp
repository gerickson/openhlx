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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a collection of HLX favorites.
 *
 */

#include "FavoritesController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/FavoritesControllerCommands.hpp>
#include <OpenHLX/Client/FavoritesStateChangeNotifications.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>
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

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FavoritesController :: FavoritesController(void) :
    Client::ControllerBasis(),
    Common::FavoritesControllerBasis(),
    Client::FavoritesControllerBasis(),
    mFavoritesDidRefreshCount(0)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
FavoritesController :: ~FavoritesController(void)
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
FavoritesController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kNameResponse,
            FavoritesController::NameNotificationReceivedHandler
        }
    };
    static constexpr size_t                lNotificationHandlerCount = ElementsOf(lNotificationHandlers);
    Status                                 lRetval = kStatus_Success;

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
FavoritesController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = Client::FavoritesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mFavorites.Init(kFavoritesMax);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(lRegister);
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
 *  Presently, this controller does so by executing a "query favorite
 *  [QFn]" command with the peer server for each favorite.
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
FavoritesController :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mFavoritesDidRefreshCount = 0;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query favorite request for each favorite.

    lRetval = Query();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Query the current state of all favorites.
 *
 *  This queries the current state of all HLX server favorites.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FavoritesController :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (IdentifierType lFavoriteIdentifier = IdentifierModel::kIdentifierMin; lFavoriteIdentifier <= kFavoritesMax; lFavoriteIdentifier++)
    {
        lRetval = Query(lFavoriteIdentifier);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of a favorite.
 *
 *  This queries the current state of the specified HLX server
 *  favorite.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference to the identifier
 *                                   of the favorite to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the favorite identifier is
 *                                        smaller or larger than supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
FavoritesController :: Query(const IdentifierType &aFavoriteIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Favorites::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Favorites::Query>(lCommand)->Init(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          FavoritesController::QueryCompleteHandler,
                          FavoritesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX favorites.
 *
 *  @param[out]  aFavorites  The maximum number of HLX favorites on
 *                           success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
FavoritesController :: GetFavoritesMax(size_t &aFavorites) const
{
    Status  lRetval = kStatus_Success;

    aFavorites = kFavoritesMax;

    return (lRetval);
}

/**
 *  @brief
 *    Get the favorite model associated with specified favorite
 *    identifier.
 *
 *  @param[in]   aIdentifier  An immutable reference to the favorite
 *                            model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the favorite model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the favorite identifier is smaller
 *                            or larger than supported.
 *
 */
Status
FavoritesController :: GetFavorite(const IdentifierType &aIdentifier, const FavoriteModel *&aModel) const
{
    Status  lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mFavorites.GetFavorite(aIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the favorite identifier with the specified name.
 *
 *  This attempts to lookup the favorite identifier for the favorite
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    favorite to find an identifier
 *                                    for.
 *  @param[out]  aFavoriteIdentifier  A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No favorite could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
FavoritesController :: LookupIdentifier(const char *aName, IdentifierType &aFavoriteIdentifier) const
{
    const FavoriteModel * lFavoriteModel;
    Status                lRetval;


    lRetval = mFavorites.GetFavorite(aName, lFavoriteModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lFavoriteModel->GetIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Name Mutator Commands

/**
 *  @brief
 *    Set the favorite to the specified name.
 *
 *  This attempts to set the favorite with the provided identifier to the
 *  specified name on the peer HLX server controller.
 *
 *  @param[in]  aFavoriteIdentifier  An immutable reference for the
 *                                   favorite for which to set the
 *                                   name.
 *  @param[in]  aName                A pointer to the null-terminated
 *                                   C string to set the favorite name
 *                                   to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the favorite identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
FavoritesController :: SetName(const IdentifierType &aFavoriteIdentifier,
                               const char *aName)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = ValidateIdentifier(aFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Favorites::SetName());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Favorites::SetName>(lCommand)->Init(aFavoriteIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          FavoritesController::SetNameCompleteHandler,
                          FavoritesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query favorite client command response completion
 *    handler.
 *
 *  This handles an asynchronous client command response for the query
 *  favorite command request.
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
FavoritesController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    IdentifierType                 lFavoriteIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mFavoritesDidRefreshCount++;

    if (WasRefreshRequested())
    {
        const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mFavoritesDidRefreshCount),
                                                                static_cast<uint8_t>(kFavoritesMax));

        OnIsRefreshing(lPercentComplete);

        if (lPercentComplete == 100)
        {
            OnDidRefresh();
        }
    }

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous favorite set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  favorite set name command request.
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
FavoritesController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous favorites controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client favorites controller
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
FavoritesController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Favorite Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query favorite client command response completion
 *    handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query favorite command request.
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
FavoritesController :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous favorite set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the favorite set name command request.
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
FavoritesController :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous favorites controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client favorites
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
FavoritesController :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

/**
 *  @brief
 *    Favorite name changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the favorite name changed notification.
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
FavoritesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                          lFavoriteIdentifier;
    const char *                            lName;
    size_t                                  lNameSize;
    FavoriteModel *                         lFavoriteModel;
    StateChange::FavoritesNameNotification  lStateChangeNotification;
    Status                                  lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Favorites::NameResponse::kExpectedMatches, done);

    // Match 2/3: Favorite Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mFavorites.GetFavorite(lFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lFavoriteModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lFavoriteIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Favorite name changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the favorite name changed notification.
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
FavoritesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
