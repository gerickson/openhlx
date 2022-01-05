/*
 *    Copyright (c) 2021-2022 Grant Erickson
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

#include "ZonesController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "EqualizerPresetsController.hpp"
#include "SourcesController.hpp"


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
ZonesController :: ZonesController(void) :
    Common::ZonesControllerBasis(),
    Client::ZonesControllerBasis(Common::ZonesControllerBasis::mZones,
                                 Common::ZonesControllerBasis::kZonesMax),
    Server::ZonesControllerBasis(Common::ZonesControllerBasis::mZones,
                                 Common::ZonesControllerBasis::kZonesMax),
    Proxy::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesController :: ~ZonesController(void)
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
ZonesController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::ZonesControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ZonesControllerBasis::Init(aServerCommandManager);
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
ZonesController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kBalanceResponse,
            ZonesController::BalanceNotificationReceivedHandler
        },

        {
            kEqualizerBandResponse,
            ZonesController::EqualizerBandNotificationReceivedHandler
        },

        {
            kEqualizerPresetResponse,
            ZonesController::EqualizerPresetNotificationReceivedHandler
        },

        {
            kHighpassCrossoverResponse,
            ZonesController::HighpassCrossoverNotificationReceivedHandler
        },

        {
            kLowpassCrossoverResponse,
            ZonesController::LowpassCrossoverNotificationReceivedHandler
        },

        {
            kMuteResponse,
            ZonesController::MuteNotificationReceivedHandler
        },

        {
            kNameResponse,
            ZonesController::NameNotificationReceivedHandler
        },

        {
            kSoundModeResponse,
            ZonesController::SoundModeNotificationReceivedHandler
        },

        {
            kSourceResponse,
            ZonesController::SourceNotificationReceivedHandler
        },

        {
            kSourceAllResponse,
            ZonesController::SourceAllNotificationReceivedHandler
        },

        {
            kToneResponse,
            ZonesController::ToneNotificationReceivedHandler
        },

        {
            kVolumeResponse,
            ZonesController::VolumeNotificationReceivedHandler
        },

        {
            kVolumeAllResponse,
            ZonesController::VolumeAllNotificationReceivedHandler
        },

        {
            kVolumeFixedResponse,
            ZonesController::VolumeFixedNotificationReceivedHandler
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

Status
ZonesController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kAdjustBalanceRequest,
            ZonesController::AdjustBalanceRequestReceivedHandler
        },

        {
            kDecreaseBassRequest,
            ZonesController::DecreaseBassRequestReceivedHandler
        },

        {
            kIncreaseBassRequest,
            ZonesController::IncreaseBassRequestReceivedHandler
        },

        {
            kDecreaseTrebleRequest,
            ZonesController::DecreaseTrebleRequestReceivedHandler
        },

        {
            kIncreaseTrebleRequest,
            ZonesController::IncreaseTrebleRequestReceivedHandler
        },

        {
            kDecreaseEqualizerBandRequest,
            ZonesController::DecreaseEqualizerBandRequestReceivedHandler
        },

        {
            kIncreaseEqualizerBandRequest,
            ZonesController::IncreaseEqualizerBandRequestReceivedHandler
        },

        {
            kDecreaseVolumeRequest,
            ZonesController::DecreaseVolumeRequestReceivedHandler
        },

        {
            kIncreaseVolumeRequest,
            ZonesController::IncreaseVolumeRequestReceivedHandler
        },

        {
            kMuteRequest,
            ZonesController::MuteRequestReceivedHandler
        },

        {
            kQueryRequest,
            ZonesController::QueryRequestReceivedHandler
        },

        {
            kQueryMuteRequest,
            ZonesController::QueryMuteRequestReceivedHandler
        },

        {
            kQuerySourceRequest,
            ZonesController::QuerySourceRequestReceivedHandler
        },

        {
            kQueryVolumeRequest,
            ZonesController::QueryVolumeRequestReceivedHandler
        },

        {
            kSetBalanceRequest,
            ZonesController::SetBalanceRequestReceivedHandler
        },

        {
            kSetEqualizerBandRequest,
            ZonesController::SetEqualizerBandRequestReceivedHandler
        },

        {
            kSetEqualizerPresetRequest,
            ZonesController::SetEqualizerPresetRequestReceivedHandler
        },

        {
            kSetHighpassCrossoverRequest,
            ZonesController::SetHighpassCrossoverRequestReceivedHandler
        },

        {
            kSetLowpassCrossoverRequest,
            ZonesController::SetLowpassCrossoverRequestReceivedHandler
        },

        {
            kSetNameRequest,
            ZonesController::SetNameRequestReceivedHandler
        },

        {
            kSetSoundModeRequest,
            ZonesController::SetSoundModeRequestReceivedHandler
        },

        {
            kSetSourceRequest,
            ZonesController::SetSourceRequestReceivedHandler
        },

        {
            kSetSourceAllRequest,
            ZonesController::SetSourceAllRequestReceivedHandler
        },

        {
            kSetToneRequest,
            ZonesController::SetToneRequestReceivedHandler
        },

        {
            kSetVolumeRequest,
            ZonesController::SetVolumeRequestReceivedHandler
        },

        {
            kSetVolumeAllRequest,
            ZonesController::SetVolumeAllRequestReceivedHandler
        },

        {
            kSetVolumeFixedRequest,
            ZonesController::SetVolumeFixedRequestReceivedHandler
        },

        {
            kToggleMuteRequest,
            ZonesController::ToggleMuteRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ObjectControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                               &lRequestHandlers[lRequestHandlerCount],
                                                               this,
                                                               aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
ZonesController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kIsConfiguration = true;
    Status                lRetval = kStatus_Success;


    (void)aConnection;

    lRetval = HandleQueryReceived(kIsConfiguration, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Zone stereophonic channel balance client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone stereophonic channel balance changed notification.
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
 *  @ingroup balance
 *
 */
void
ZonesController :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::BalanceNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone equalizer band level client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone equalizer band level changed notification.
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
ZonesController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer,
                                                            const size_t &aSize,
                                                            const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::EqualizerBandNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone equalizer preset client unsolicited notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone equalizer preset changed notification.
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
ZonesController :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::EqualizerPresetNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone tone equalizer state client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone tone equalizer state changed notification.
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
ZonesController :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::ToneNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone highpass filter crossover frequency client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone highpass filter crossover frequency changed notification.
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
ZonesController :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::HighpassCrossoverNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone lowpass filter crossover frequency client unsolicited
 *    notification handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  a zone lowpass filter crossover frequency changed notification.
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
ZonesController :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::LowpassCrossoverNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone volume mute state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume mute state changed notification.
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
ZonesController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::MuteNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone name changed client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone name changed notification.
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
ZonesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::NameNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

done:
    return;
}

/**
 *  @brief
 *    Zone equalizer sound mode changed client unsolicited
 *    notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone equalizer sound mode changed notification.
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
ZonesController :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::SoundModeNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone source (input) changed client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification
 *  for the zone source (input) changed notification.
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
ZonesController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::SourceNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    All zones source (input) changed client unsolicited notification
 *    handler.
 *
 *  This handles an unsolicited, asynchronous client notification for
 *  the all zones source (input) changed notification.
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
ZonesController :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::SourceAllNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone volume level state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume level state changed notification.
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
ZonesController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::VolumeNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    All zones volume level state changed client unsolicited notification
 *    handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the all zones volume level state changed notification.
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
ZonesController :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::VolumeAllNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Zone volume fixed/locked state changed client unsolicited
 *    notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the zone volume fixed/locked state changed notification.
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
ZonesController :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::ZonesControllerBasis *  lController = static_cast<Client::ZonesControllerBasis *>(this);
    Status                          lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::ZonesControllerBasis::VolumeFixedNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Zone stereophonic channel balance changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone stereophonic channel balance changed
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
ZonesController :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->BalanceNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer band level changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer band level changed
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
ZonesController :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->EqualizerBandNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer preset changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer preset changed notification.
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
ZonesController :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->EqualizerPresetNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone tone equalizer state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone tone equalizer state changed
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
ZonesController :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToneNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone highpass filter crossover frequency changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone highpass filter crossover frequency
 *  changed notification.
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
ZonesController :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->HighpassCrossoverNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone lowpass filter crossover frequency changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone lowpass filter crossover frequency
 *  changed notification.
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
ZonesController :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->LowpassCrossoverNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume mute state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume mute state changed notification.
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
ZonesController :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone name changed client unsolicited notification handler
 *    trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone name changed notification.
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
ZonesController :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->NameNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone equalizer sound mode changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone equalizer sound mode changed
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
ZonesController :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SoundModeNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone source (input) changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone source (input) changed notification.
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
ZonesController :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SourceNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    All zones source (input) changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the all zones source (input) changed
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
ZonesController :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SourceAllNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume level state changed client unsolicited notification
 *    handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume level state changed notification.
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
ZonesController :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);


    if (lController != nullptr)
    {
        lController->VolumeNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    All zones volume level state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the all zones volume level state changed
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
ZonesController :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeAllNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Zone volume fixed/locked state changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the zone volume fixed/locked state changed
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
ZonesController :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeFixedNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

void ZonesController :: AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kBalanceResponse,
                                   Client::ZonesControllerBasis::SetBalanceCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   Client::ZonesControllerBasis::SetToneCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   Client::ZonesControllerBasis::SetToneCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   Client::ZonesControllerBasis::SetToneCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   Client::ZonesControllerBasis::SetToneCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                      lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                      lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kMuteResponse,
                                   Client::ZonesControllerBasis::SetMuteCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static constexpr bool                    kIsConfiguration = true;
    ZoneModel::IdentifierType                lZoneIdentifier;
    Server::Command::Zones::QueryResponse    lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lResponse.Init(lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion, indicating that
    // this is a zone-specific query, not a general configuration
    // query.

    lStatus = HandleQueryReceived(!kIsConfiguration, lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

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
        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          kQueryResponse,
                                          Client::ZonesControllerBasis::QueryCompleteHandler,
                                          Client::ZonesControllerBasis::CommandErrorHandler,
                                          ZonesController::QueryRequestReceivedHandler,
                                          static_cast<Client::ZonesControllerBasis *>(this),
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

void ZonesController :: QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryMuteReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryMuteReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }
    else if (lStatus == kError_NotInitialized)
    {
        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          kMuteResponse,
                                          Client::ZonesControllerBasis::SetMuteCompleteHandler,
                                          Client::ZonesControllerBasis::CommandErrorHandler,
                                          ZonesController::QueryMuteRequestReceivedHandler,
                                          static_cast<Client::ZonesControllerBasis *>(this),
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

void ZonesController :: QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQuerySourceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQuerySourceReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }
    else if (lStatus == kError_NotInitialized)
    {
        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          kSourceResponse,
                                          Client::ZonesControllerBasis::SetSourceCompleteHandler,
                                          Client::ZonesControllerBasis::CommandErrorHandler,
                                          ZonesController::QuerySourceRequestReceivedHandler,
                                          static_cast<Client::ZonesControllerBasis *>(this),
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

void ZonesController :: QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryVolumeReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryVolumeReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlREQUIRE_SUCCESS(lStatus, exit);
    }
    else if (lStatus == kError_NotInitialized)
    {
        lStatus = ProxyObservationCommand(aConnection,
                                          aBuffer,
                                          aSize,
                                          aMatches,
                                          kVolumeResponse,
                                          Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                                          Client::ZonesControllerBasis::CommandErrorHandler,
                                          ZonesController::QueryVolumeRequestReceivedHandler,
                                          static_cast<Client::ZonesControllerBasis *>(this),
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

void ZonesController :: SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kBalanceResponse,
                                   Client::ZonesControllerBasis::SetBalanceCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::ZonesControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerPresetResponse,
                                   Client::ZonesControllerBasis::SetEqualizerPresetCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kHighpassCrossoverResponse,
                                   Client::ZonesControllerBasis::SetHighpassCrossoverCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kLowpassCrossoverResponse,
                                   Client::ZonesControllerBasis::SetLowpassCrossoverCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kNameResponse,
                                   Client::ZonesControllerBasis::SetNameCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSoundModeResponse,
                                   Client::ZonesControllerBasis::SetSoundModeCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSourceResponse,
                                   Client::ZonesControllerBasis::SetSourceCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSourceAllResponse,
                                   Client::ZonesControllerBasis::SetSourceAllCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   Client::ZonesControllerBasis::SetToneCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   Client::ZonesControllerBasis::SetVolumeCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeAllResponse,
                                   Client::ZonesControllerBasis::SetVolumeAllCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeFixedResponse,
                                   Client::ZonesControllerBasis::SetVolumeFixedCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status  lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kMuteResponse,
                                   Client::ZonesControllerBasis::SetMuteCompleteHandler,
                                   Client::ZonesControllerBasis::CommandErrorHandler,
                                   static_cast<Client::ZonesControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Client-facing Server Implementation

// MARK: Client-facing Server Data Model Mutation State Change Methods

Status
ZonesController :: AdjustVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, VolumeModel::LevelType &aVolume)
{
    DeclareScopedFunctionTracer(lTracer);
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    nlREQUIRE_ACTION(aAdjustment != 0, done, lRetval = -EINVAL);

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseVolume(aVolume);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseVolume(aVolume);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetMute(aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetVolume(aVolume);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetSource(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                             const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetSource(aSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ToggleMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              Model::VolumeModel::MuteType &aMute)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->ToggleMute(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: GetEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    // Attempt to get the zone model associated with the specified
    // identifier. Range checking on the identifier will occur during
    // this process.

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Attempt to get the equalizer band model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = lZoneModel->GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustBalanceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const BalanceModel::ChannelType &aChannel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                              lZoneModel;
    BalanceModel::BalanceType                lBalance;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    nlREQUIRE_ACTION(((aChannel == BalanceModel::kChannelLeft) ||
                     (aChannel == BalanceModel::kChannelRight)),
                     done, lRetval = -EINVAL);

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aChannel == BalanceModel::kChannelLeft)
    {
        lRetval = lZoneModel->IncreaseBalanceLeft(lBalance);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aChannel == BalanceModel::kChannelRight)
    {
        lRetval = lZoneModel->IncreaseBalanceRight(lBalance);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = lBalanceResponse.Init(aZoneIdentifier, lBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBalanceResponse.GetBuffer();
    lSize = lBalanceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetBalanceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const BalanceModel::BalanceType &aBalance, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                              lZoneModel;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetBalance(aBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = lBalanceResponse.Init(aZoneIdentifier, aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBalanceResponse.GetBuffer();
    lSize = lBalanceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustBassReceived(Server::ConnectionBasis &aConnection,
                                            const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                            const Model::ToneModel::LevelType &aAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the bass adjustment.

    lRetval = HandleAdjustBassReceived(aZoneIdentifier, aAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }

    if (lRetval < kStatus_Success)
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustBassReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                            const Model::ToneModel::LevelType &aAdjustment,
                                            Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    ToneModel::LevelType               lBass;
    ToneModel::LevelType               lTreble;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseBass(lBass);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseBass(lBass);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

    // Assuming the adjustment was successful, get the treble so that
    // we can send a response (which must contain both bass and treble
    // in the response, even though only bass was changed).

    lRetval = lZoneModel->GetTreble(lTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleToneResponse(aZoneIdentifier, lBass, lTreble, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustTrebleReceived(Server::ConnectionBasis &aConnection, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the bass adjustment.

    lRetval = HandleAdjustTrebleReceived(aZoneIdentifier, aAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }

    if (lRetval < kStatus_Success)
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustTrebleReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                              const Model::ToneModel::LevelType &aAdjustment,
                                              Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    ToneModel::LevelType               lBass;
    ToneModel::LevelType               lTreble;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseTreble(lTreble);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseTreble(lTreble);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

    // Assuming the adjustment was successful, get the treble so that
    // we can send a response (which must contain both bass and treble
    // in the response, even though only treble was changed).

    lRetval = lZoneModel->GetBass(lBass);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleToneResponse(aZoneIdentifier, lBass, lTreble, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetMute(const bool &aConditionally, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status                             lRetval;


    lRetval = SetMute(aZoneIdentifier, aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // If the mute status was unchanged, SetMute will have returned
    // kStatus_ValueAlreadySet and there will be no need to generate
    // (and subsequently send) a response, unless the caller requested
    // an unconditional response. If we receive kStatus_Success, it is
    // the first time set or a change and response needs to be
    // unconditionally generated (and subsequently sent).

    if ((lRetval == kStatus_Success) || ((lRetval == kStatus_ValueAlreadySet) && !aConditionally))
    {
        lRetval = HandleMuteResponse(aZoneIdentifier, aMute, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetMuteConditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetMute(kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleSetMuteUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetMute(!kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleSetSoundMode(const bool &aConditionally, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetSoundMode(aSoundMode);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

    // If the sound mode was unchanged, SetSoundMode will have
    // returned kStatus_ValueAlreadySet and there will be no need to
    // generate (and subsequently send) a response, unless the caller
    // requested an unconditional response. If we receive
    // kStatus_Success, it is the first time set or a change and
    // response needs to be unconditionally generated (and
    // subsequently sent).

    if ((lRetval == kStatus_Success) || ((lRetval == kStatus_ValueAlreadySet) && !aConditionally))
    {
        lRetval = HandleSoundModeResponse(aZoneIdentifier, aSoundMode, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetSoundModeConditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetSoundMode(kConditionally, aZoneIdentifier, aSoundMode, aBuffer));
}

Status
ZonesController :: HandleSetSoundModeUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetSoundMode(!kConditionally, aZoneIdentifier, aSoundMode, aBuffer));
}

Status
ZonesController :: HandleAdjustVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    VolumeModel::LevelType lVolume;
    Status                 lRetval;


    nlREQUIRE_ACTION(aAdjustment != 0, done, lRetval = -EINVAL);

    lRetval = AdjustVolume(aZoneIdentifier, aAdjustment, lVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, lVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status                             lRetval;


    lRetval = SetVolume(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, aVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustEqualizerBandReceived(Server::ConnectionBasis &aConnection, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to zone equalizer mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeZoneEqualizer, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the zone equalizer band adjustment.

    lRetval = HandleAdjustEqualizerBandReceived(aZoneIdentifier, aEqualizerBandIdentifier, aBandAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }

    if (lRetval < kStatus_Success)
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustEqualizerBandReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                     const Model::EqualizerBandModel::LevelType &aBandAdjustment,
                                                     Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    EqualizerBandModel::LevelType      lBandLevel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Increase or decrease, per the adjustment, the equalizer band
    // level.

    if (aBandAdjustment > 0)
    {
        lRetval = lEqualizerBandModel->IncreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aBandAdjustment < 0)
    {
        lRetval = lEqualizerBandModel->DecreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = HandleEqualizerBandResponse(aZoneIdentifier, aEqualizerBandIdentifier, lBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetEqualizerBandReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lEqualizerBandModel->SetLevel(aBandLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = HandleEqualizerBandResponse(aZoneIdentifier, aEqualizerBandIdentifier, aBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Client-facing Server Command Request Handler Trampolines

void ZonesController :: AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->AdjustBalanceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseBassRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseBassRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseTrebleRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseTrebleRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QuerySourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBalanceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerPresetRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetHighpassCrossoverRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLowpassCrossoverRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSoundModeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceAllRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetToneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeAllRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeFixedRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToggleMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Proxy

}; // namespace HLX
