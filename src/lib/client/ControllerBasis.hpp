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
 *      This file defines a base object for all client-side HLX
 *      controllers.
 *
 */

#ifndef OPENHLXCLIENTCONTROLLERBASIS_HPP
#define OPENHLXCLIENTCONTROLLERBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/CommandManagerDelegate.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>


namespace HLX
{

namespace Client
{

class ControllerBasisDelegate;

/**
 *  @brief
 *    A base object for all client-side HLX controllers.
 *
 *  @ingroup client
 *
 */
class ControllerBasis :
    public CommandManagerDelegate
{
public:
    virtual ~ControllerBasis(void);

    virtual Common::Status Init(CommandManager &aCommandManager);
    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    virtual Common::Status Refresh(void);

    /**
     *  @brief
     *    Refresh or obtain an up-to-date view of the server peer
     *    controller state.
     *
     *  This attempts to refresh or obtain an up-to-date view of the
     *  server peer controller state with the specified timeout.
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
    virtual Common::Status Refresh(const Common::Timeout &aTimeout) = 0;

    Common::Status SetDelegate(ControllerBasisDelegate *aDelegate);
    ControllerBasisDelegate *GetDelegate(void) const;

protected:
    ControllerBasis(void);

    /**
     *  @brief
     *    A base object for registering client-side handlers for
     *    solicited or unsolicited state change notifications.
     *
     *  A base object for registering client-side handlers for
     *  solicited (that is, in response to a command request) or
     *  unsolicited (that is, outside of a response to a command
     *  request) state change notifications.
     *
     *  @ingroup client
     *
     */
    struct NotificationHandlerBasis
    {
        /**
         *  A reference to the expected client command response
         *  regular expression pattern that, when matched, should
         *  trigger invocation of the associated handler.
         *
         */
        Command::ResponseBasis &                    mResponse;

        /**
         *  The handler to invoke when the response pattern is matched
         *  on receipt of a solicited or unsolicited state change
         *  notification.
         *
         */
        CommandManager::OnNotificationReceivedFunc  mOnNotificationReceivedHandler;
    };

    Common::Status DoNotificationHandlers(const NotificationHandlerBasis *aFirstNotificationHandler,
                                          const NotificationHandlerBasis *aLastNotificationHandler,
                                          void *aContext,
                                          const bool &aRegister);

    Common::Status SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                               CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                               CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                               void *aContext);
    Common::Status SendCommand(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                               const Common::Timeout &aTimeout,
                               CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                               CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                               void *aContext);

    // Refresh State Observation and Mutation Methods

    bool IsRefreshing(void) const;
    bool WasRefreshRequested(void) const;
    void SetRefreshRequested(const bool &aRefreshRequested);

    // Controller Delegation Action Methods

    void OnCommandError(const uint8_t *aCommandBuffer, const size_t &aCommandSize, const char *aCommandDescription, const Common::Error &aError);
    void OnIsRefreshing(const uint8_t &aPercentComplete);
    void OnDidRefresh(void);
    void OnStateDidChange(const StateChange::NotificationBasis &aStateChangeNotification);

    // Refresh State and Delegation Convenience Methods

    void MaybeUpdateRefreshIfRefreshWasRequested(const uint8_t &aNumerator, const uint8_t &aDenominator);
    void MaybeUpdateRefreshIfRefreshWasRequested(void);

private:
    ControllerBasisDelegate *  mDelegate;
    CommandManager *           mCommandManager;
    Common::Timeout            mTimeout;
    bool                       mRefreshRequested;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONTROLLERBASIS_HPP
