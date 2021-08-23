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
 *      This file implements a base object for all client-side HLX
 *      controllers.
 *
 */

#include "ControllerBasis.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Percentage.hpp>

#include "CommandManager.hpp"
#include "ConnectionManager.hpp"
#include "ControllerBasisDelegate.hpp"


using namespace HLX::Common;
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
ControllerBasis :: ControllerBasis(void) :
    mErrorDelegate(nullptr),
    mRefreshDelegate(nullptr),
    mStateChangeDelegate(nullptr),
    mCommandManager(nullptr),
    mTimeout(),
    mRefreshRequested(false)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the controller basis with a default timeout.
 *
 *  @param[in]  aCommandManager  A mutable reference to the command
 *                               manager to initialize this controller
 *                               basis with as the controller
 *                               basis is to be a delegate of the
 *                               command manager.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = Init(aCommandManager, kTimeoutDefault);

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the controller basis with a specified timeout.
 *
 *  @param[in]  aCommandManager  A mutable reference to the command
 *                               manager to initialize this controller
 *                               basis with as the controller
 *                               basis is to be a delegate of the
 *                               command manager.
 *  @param[in]  aTimeout         An immutable reference to the default
 *                               timeout to use with invocations of
 *                               #SendCommand.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    mCommandManager = &aCommandManager;
    mTimeout        = aTimeout;

    return (lRetval);
}

/**
 *  @brief
 *    Register or unregister notification handlers.
 *
 *  This registers or unregisters the solicited and unsolicited client
 *  command response notification handlers that this controller is
 *  interested in and will handle on behalf of the client.
 *
 *  @param[in]  aFirstNotificationHandler  An iterator to the first
 *                                         notification handler to
 *                                         register or unregister.
 *  @param[in]  aLastNotificationHandler   An iterator to the last
 *                                         notification handler to
 *                                         register or unregister.
 *  @param[in]  aRegister                  Indicates whether to
 *                                         register (true) or unregister
 *                                         (false) the handlers.
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
ControllerBasis :: DoNotificationHandlers(const NotificationHandlerBasis *aFirstNotificationHandler,
                                          const NotificationHandlerBasis *aLastNotificationHandler,
                                          void *aContext,
                                          const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr,      done, lRetval = kError_NotInitialized);
    nlREQUIRE_ACTION(aFirstNotificationHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLastNotificationHandler  != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aContext != nullptr, done, lRetval = -EINVAL);

    while (aFirstNotificationHandler != aLastNotificationHandler)
    {
        if (aRegister)
        {
            lRetval = mCommandManager->RegisterNotificationHandler(aFirstNotificationHandler->mResponse, aContext, aFirstNotificationHandler->mOnNotificationReceivedHandler);
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        else
        {
            lRetval = mCommandManager->UnregisterNotificationHandler(aFirstNotificationHandler->mResponse, aContext);
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        ++aFirstNotificationHandler;
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    Refresh or obtain an up-to-date view of the server peer
 *    controller state.
 *
 *  This attempts to refresh or obtain an up-to-date view of the
 *  server peer controller state with the default timeout.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ControllerBasis :: Refresh(void)
{
    Status lRetval = kStatus_Success;

    lRetval = Refresh(mTimeout);

    return (lRetval);
}

/**
 *  @brief
 *    Return the error delegate for the controller basis.
 *
 *  @returns
 *    A pointer to the error delegate for the controller basis.
 *
 */
ControllerBasisErrorDelegate *
ControllerBasis :: GetErrorDelegate(void) const
{
    return (mErrorDelegate);
}

/**
 *  @brief
 *    Return the refresh delegate for the controller basis.
 *
 *  @returns
 *    A pointer to the refresh delegate for the controller basis.
 *
 */
ControllerBasisRefreshDelegate *
ControllerBasis :: GetRefreshDelegate(void) const
{
    return (mRefreshDelegate);
}

/**
 *  @brief
 *    Return the state change delegate for the controller basis.
 *
 *  @returns
 *    A pointer to the state change delegate for the controller basis.
 *
 */
ControllerBasisStateChangeDelegate *
ControllerBasis :: GetStateChangeDelegate(void) const
{
    return (mStateChangeDelegate);
}

/**
 *  @brief
 *    Set the error delegate for the controller basis.
 *
 *  This attempts to set a error delegate for the controller basis.
 *
 *  @param[in]  aErrorDelegate  A pointer to the error delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the error delegate was already
 *                                    set to the specified value.
 *
 */
Status
ControllerBasis :: SetErrorDelegate(ControllerBasisErrorDelegate *aErrorDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aErrorDelegate != mErrorDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mErrorDelegate = aErrorDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the refresh delegate for the controller basis.
 *
 *  This attempts to set a refresh delegate for the controller basis.
 *
 *  @param[in]  aRefreshDelegate  A pointer to the refresh delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the refresh delegate was already
 *                                    set to the specified value.
 *
 */
Status
ControllerBasis :: SetRefreshDelegate(ControllerBasisRefreshDelegate *aRefreshDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aRefreshDelegate != mRefreshDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mRefreshDelegate = aRefreshDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the state change delegate for the controller basis.
 *
 *  This attempts to set a state change delegate for the controller
 *  basis.
 *
 *  @param[in]  aStateChangeDelegate  A pointer to the state change
 *                                    delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the state change delegate was
 *                                    already set to the specified value.
 *
 */
Status
ControllerBasis :: SetStateChangeDelegate(ControllerBasisStateChangeDelegate *aStateChangeDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aStateChangeDelegate != mStateChangeDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mStateChangeDelegate = aStateChangeDelegate;

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a client command request to the peer connected server with
 *    a default timeout.
 *
 *  This attempts to send a client command exchange request and
 *  expected response pair to the peer connected server with a default
 *  timeout.
 *
 *  @param[in]  aExchange                  A mutable shared pointer
 *                                         to the exchange on which
 *                                         the client command request
 *                                         should be sent and on which
 *                                         the response should be
 *                                         received.
 *  @param[in]  aOnCommandCompleteHandler  The handler trampoline to
 *                                         invoke when a successful
 *                                         command response is
 *                                         received.
 *  @param[in]  aOnCommandErrorHandler     The handler trampoline to
 *                                         invoke when a command error
 *                                         response is received.
 *  @param[in]  aContext                   A pointer to the caller-
 *                                         specific context that sent
 *                                         the command and that will
 *                                         be provided to the
 *                                         completion or error
 *                                         handler, when invoked.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If there were insufficient resources
 *                                  to allocate memory for the state
 *                                  associated with the exchange.
 *  @retval  kError_NotInitialized  The controller basis was not properly
 *                                  initialized.
 *
 */
Status
ControllerBasis :: SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                               CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                               CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                               void *aContext)
{
    Status lRetval = kStatus_Success;

    lRetval = SendCommand(aExchange,
                          mTimeout,
                          aOnCommandCompleteHandler,
                          aOnCommandErrorHandler,
                          aContext);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Send a client command request to the peer connected server.
 *
 *  This attempts to send a client command exchange request and
 *  expected response pair to the peer connected server.
 *
 *  @param[in]  aExchange                  A mutable shared pointer
 *                                         to the exchange on which
 *                                         the client command request
 *                                         should be sent and on which
 *                                         the response should be
 *                                         received.
 *  @param[in]  aTimeout                   An immutable reference to
 *                                         to timeout by which the
 *                                         response should be
 *                                         received.
 *  @param[in]  aOnCommandCompleteHandler  The handler trampoline to
 *                                         invoke when a successful
 *                                         command response is
 *                                         received.
 *  @param[in]  aOnCommandErrorHandler     The handler trampoline to
 *                                         invoke when a command error
 *                                         response is received.
 *  @param[in]  aContext                   A pointer to the caller-
 *                                         specific context that sent
 *                                         the command and that will
 *                                         be provided to the
 *                                         completion or error
 *                                         handler, when invoked.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If there were insufficient resources
 *                                  to allocate memory for the state
 *                                  associated with the exchange.
 *  @retval  kError_NotInitialized  The controller basis was not properly
 *                                  initialized.
 *
 */
Status
ControllerBasis :: SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                               const Timeout &aTimeout,
                               CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                               CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                               void *aContext)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendCommand(aExchange,
                                           aTimeout,
                                           aOnCommandCompleteHandler,
                                           aOnCommandErrorHandler,
                                           aContext);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the controller is in the middle of a refresh.
 *
 *  This returns a Boolean indicating whether (true) or not (false)
 *  the controller is in the middle of a refresh operation with the
 *  peer server controller for up-to-date state.
 *
 *  @returns
 *    True if the controller is refreshing; otherwise, false.
 *
 */
bool
ControllerBasis :: IsRefreshing(void) const
{
    return (WasRefreshRequested());
}

/**
 *  @brief
 *    Returns whether or not a controller refresh was requested or is
 *    in flight.
 *
 *  This returns a Boolean indicating whether (true) or not (false) a
 *  controller refresh operation with the peer server controller for
 *  up-to-date state was requested or is in flight.
 *
 *  @returns
 *    True if a controller refresh was requested or is in flight;
 *    otherwise, false.
 *
 */
bool
ControllerBasis :: WasRefreshRequested(void) const
{
    return (mRefreshRequested);
}

/**
 *  @brief
 *    Sets the controller refresh request state.
 *
 *  This sets the controller peer server state refresh request state
 *  and should be asserted whenever a controller refresh has been
 *  requested or is in flight and should be deasserted whenever such a
 *  refresh completes.
 *
 *  @param[in]  aRefreshRequested  An immutable reference to the refresh
 *                                 state to set.
 *
 *  @sa IsRefreshRequested
 *  @sa WasRefreshRequested
 */
void
ControllerBasis :: SetRefreshRequested(const bool &aRefreshRequested)
{
    mRefreshRequested = aRefreshRequested;
}

// MARK: Controller Delegation Actions

/**
 *  @brief
 *    Signals to controller delegates that a command error occurred.
 *
 *  This is invoked by a controller in response to a command error to
 *  signal delegates that a command error has occurred.
 *
 *  @param[in]  aCommandBuffer       An immutable pointer to the start
 *                                   of the command buffer extent
 *                                   containing the original client
 *                                   command request that caused the
 *                                   error.
 *  @param[in]  aCommandSize         An immutable reference to the size
 *                                   of the command buffer extent
 *                                   containing the original client
 *                                   command request that caused the
 *                                   error.
 *  @param[in]  aCommandDescription  An optional, null-terminated C
 *                                   string further describing the
 *                                   command that caused the error.
 *  @param[in]  aError               An immutable reference to the
 *                                   error associated with the
 *                                   failed client command request.
 *
 */
void
ControllerBasis :: OnCommandError(const uint8_t *       aCommandBuffer,
                                  const size_t &        aCommandSize,
                                  const char *          aCommandDescription,
                                  const Common::Error & aError)
{
    Log::Error().Write("Error: %s: %.*s: %d\n",
                       aCommandDescription,
                       static_cast<int>(aCommandSize),
                       aCommandBuffer,
                       aError);

    if (mErrorDelegate != nullptr)
    {
        mErrorDelegate->ControllerError(*this, aError);
    }
}

/**
 *  @brief
 *    Signals to controller delegates the progress of a refresh.
 *
 *  This is invoked by a controller in response to a refresh to signal
 *  delegates the progress of that refresh.
 *
 *  @param[in]  aPercentComplete     A reference to the percentage
 *                                   (0-100) of the refresh operation
 *                                   that has completed.
 *
 */
void
ControllerBasis :: OnIsRefreshing(const uint8_t &aPercentComplete)
{
    if (mRefreshDelegate != nullptr)
    {
        mRefreshDelegate->ControllerIsRefreshing(*this, aPercentComplete);
    }
}

/**
 *  @brief
 *    Signals to controller delegates that a refresh has completed.
 *
 *  This is invoked by a controller in response to a refresh
 *  completion to signal delegates that the refresh has completed.
 *
 */
void
ControllerBasis :: OnDidRefresh(void)
{
    if (mRefreshDelegate != nullptr)
    {
        mRefreshDelegate->ControllerDidRefresh(*this);
    }

    SetRefreshRequested(false);
}

/**
 *  @brief
 *    Signals to controller delegates that a state change has
 *    occurred from the peer server controller.
 *
 *  This is invoked by a controller in response to a peer server
 *  controller state change notification to signal delegates of that
 *  state change.
 *
 *  @param[in]  aStateChangeNotification  An immutable reference to
 *                                        a notification describing
 *                                        the state change.
 *
 */
void
ControllerBasis :: OnStateDidChange(const StateChange::NotificationBasis &aStateChangeNotification)
{
    if (mStateChangeDelegate != nullptr)
    {
        mStateChangeDelegate->ControllerStateDidChange(*this, aStateChangeNotification);
    }
}

// MARK: Refresh State and Delegation Convenience Methods

void
ControllerBasis :: MaybeUpdateRefreshIfRefreshWasRequested(const uint8_t &aNumerator, const uint8_t &aDenominator)
{
    if (WasRefreshRequested())
    {
        const Percentage lPercentComplete = CalculatePercentage(aNumerator, aDenominator);

        OnIsRefreshing(lPercentComplete);

        if (lPercentComplete == 100)
        {
            OnDidRefresh();
        }
    }
}

void
ControllerBasis :: MaybeUpdateRefreshIfRefreshWasRequested(void)
{
    if (WasRefreshRequested())
    {
        OnDidRefresh();
    }
}

}; // namespace Client

}; // namespace HLX
