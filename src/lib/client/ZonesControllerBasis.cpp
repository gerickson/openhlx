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

#include "ZonesControllerBasis.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Common/ZonesControllerBasis.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
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

Command::Zones::BalanceResponse            ZonesControllerBasis::kBalanceResponse;
Command::Zones::EqualizerBandResponse      ZonesControllerBasis::kEqualizerBandResponse;
Command::Zones::EqualizerPresetResponse    ZonesControllerBasis::kEqualizerPresetResponse;
Command::Zones::HighpassCrossoverResponse  ZonesControllerBasis::kHighpassCrossoverResponse;
Command::Zones::LowpassCrossoverResponse   ZonesControllerBasis::kLowpassCrossoverResponse;
Command::Zones::MuteResponse               ZonesControllerBasis::kMuteResponse;
Command::Zones::NameResponse               ZonesControllerBasis::kNameResponse;
Command::Zones::QueryResponse              ZonesControllerBasis::kQueryResponse;
Command::Zones::SoundModeResponse          ZonesControllerBasis::kSoundModeResponse;
Command::Zones::SourceResponse             ZonesControllerBasis::kSourceResponse;
Command::Zones::SourceAllResponse          ZonesControllerBasis::kSourceAllResponse;
Command::Zones::ToneResponse               ZonesControllerBasis::kToneResponse;
Command::Zones::VolumeResponse             ZonesControllerBasis::kVolumeResponse;
Command::Zones::VolumeAllResponse          ZonesControllerBasis::kVolumeAllResponse;
Command::Zones::VolumeFixedResponse        ZonesControllerBasis::kVolumeFixedResponse;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesControllerBasis :: ZonesControllerBasis(Model::ZonesModel &aZonesModel,
                                             const Model::ZoneModel::IdentifierType &aZonesMax) :
    Client::ControllerBasis(),
    mZonesDidRefreshCount(0),
    mZonesModel(aZonesModel),
    mZonesMax(aZonesMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesControllerBasis :: ~ZonesControllerBasis(void)
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
ZonesControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
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
 *  Presently, this controller does so by executing a "query zone
 *  [QOn]" command with the peer server for each zone.
 *
 *  @param[in]  aTimeout  The timeout to use for the refresh operation
 *                        with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesControllerBasis :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mZonesDidRefreshCount = 0;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query zone request for each zone.

    lRetval = Query();
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
ZonesControllerBasis :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kBalanceResponse,
            ZonesControllerBasis::BalanceNotificationReceivedHandler
        },

        {
            kEqualizerBandResponse,
            ZonesControllerBasis::EqualizerBandNotificationReceivedHandler
        },

        {
            kEqualizerPresetResponse,
            ZonesControllerBasis::EqualizerPresetNotificationReceivedHandler
        },

        {
            kHighpassCrossoverResponse,
            ZonesControllerBasis::HighpassCrossoverNotificationReceivedHandler
        },

        {
            kLowpassCrossoverResponse,
            ZonesControllerBasis::LowpassCrossoverNotificationReceivedHandler
        },

        {
            kMuteResponse,
            ZonesControllerBasis::MuteNotificationReceivedHandler
        },

        {
            kNameResponse,
            ZonesControllerBasis::NameNotificationReceivedHandler
        },

        {
            kSoundModeResponse,
            ZonesControllerBasis::SoundModeNotificationReceivedHandler
        },

        {
            kSourceResponse,
            ZonesControllerBasis::SourceNotificationReceivedHandler
        },

        {
            kSourceAllResponse,
            ZonesControllerBasis::SourceAllNotificationReceivedHandler
        },

        {
            kToneResponse,
            ZonesControllerBasis::ToneNotificationReceivedHandler
        },

        {
            kVolumeResponse,
            ZonesControllerBasis::VolumeNotificationReceivedHandler
        },

        {
            kVolumeAllResponse,
            ZonesControllerBasis::VolumeAllNotificationReceivedHandler
        },

        {
            kVolumeFixedResponse,
            ZonesControllerBasis::VolumeFixedNotificationReceivedHandler
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
ZonesControllerBasis :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response data.

    lRetval = kBalanceResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kEqualizerBandResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kEqualizerPresetResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kHighpassCrossoverResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kLowpassCrossoverResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kMuteResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kNameResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSoundModeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSourceResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSourceAllResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kToneResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeAllResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kVolumeFixedResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mute / Source / Volume Mutation Command Completion Implementation

void
ZonesControllerBasis :: HandleMuteChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesMuteNotification     lStateChangeNotification;


    lStatus = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the mute status is unchanged, SetMute will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetMute(aMute);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aMute);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

void
ZonesControllerBasis :: HandleSourceChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SourceModel::IdentifierType &aSourceIdentifier)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesSourceNotification   lStateChangeNotification;


    lStatus = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the source is unchanged, SetSource will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetSource(aSourceIdentifier);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

void
ZonesControllerBasis :: HandleVolumeChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    StateChange::ZonesVolumeNotification   lStateChangeNotification;


    lStatus = mZonesModel.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the volume is unchanged, SetVolume will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetVolume(aVolume);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

// MARK: Observer Methods

/**
 *  @brief
 *    Query the current state of all zones.
 *
 *  This queries the current state of all HLX server zones.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesControllerBasis :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (auto lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= mZonesMax; lZoneIdentifier++)
    {
        lRetval = Query(lZoneIdentifier);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

/**
 *  @brief
 *    Query the current state of a zone.
 *
 *  This queries the current state of the specified HLX server
 *  zone.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the identifier
 *                               of the zone to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the zone identifier is
 *                                        smaller or larger than supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *
 */
Status
ZonesControllerBasis :: Query(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lRetval = Common::ZonesControllerBasis::ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::Zones::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Zones::Query>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesControllerBasis::QueryCompleteHandler,
                          ZonesControllerBasis::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query zone client command response completion
 *    handler.
 *
 *  This handles an asynchronous client command response for the query
 *  zone command request.
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
ZonesControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    ZoneModel::IdentifierType      lZoneIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mZonesDidRefreshCount++;

    MaybeUpdateRefreshIfRefreshWasRequested(static_cast<uint8_t>(mZonesDidRefreshCount),
                                            static_cast<uint8_t>(mZonesMax));

done:
    return;
}

/**
 *  @brief
 *    Asynchronous zone set stereophonic channel balance client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set stereophonic channel balance command request.
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
ZonesControllerBasis :: SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    BalanceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone equalizer set/decrease/increase equalizer
 *    band level client command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone equalizer set/decrease/increase equalizer band level
 *  command request.
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
ZonesControllerBasis :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    EqualizerBandNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set equalizer preset client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set equalizer preset command request.
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
ZonesControllerBasis :: SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    EqualizerPresetNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set highpass filter crossover frequency client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set highpass filter crossover frequency command request.
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
ZonesControllerBasis :: SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    HighpassCrossoverNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set lowpass filter crossover frequency client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set lowpass filter crossover frequency command request.
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
ZonesControllerBasis :: SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    LowpassCrossoverNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set volume mute client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume mute command request.
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
ZonesControllerBasis :: SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    MuteNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set name client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set name command request.
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
ZonesControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    NameNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set equalizer sound mode client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the zone
 *  set equalizer sound mode command request.
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
ZonesControllerBasis :: SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SoundModeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set source (input) client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the zone
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
ZonesControllerBasis :: SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    SourceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous all zones set source (input) client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the all
 *  zones set source (input) command request.
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
ZonesControllerBasis :: SetSourceAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    // XXX - TBD
}

/**
 *  @brief
 *    Asynchronous zone decrease/increase/set tone equalizer client
 *    command response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone decrease/increase/set tone equalizer command request.
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
ZonesControllerBasis :: SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    ToneNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zone set volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume level command request.
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
ZonesControllerBasis :: SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous all zones set volume level client command response
 *    completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  all zones set volume level command request.
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
ZonesControllerBasis :: SetVolumeAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    // XXX - TBD
}

/**
 *  @brief
 *    Asynchronous zone set volume fixed/locked client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the
 *  zone set volume fixed/locked command request.
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
ZonesControllerBasis :: SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeFixedNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
}

/**
 *  @brief
 *    Asynchronous zones controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client zones controller
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
ZonesControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Zone Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query zone client command response completion
 *    handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query zone command request.
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
ZonesControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set stereophonic channel balance client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set stereophonic channel balance command
 *  request.
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
ZonesControllerBasis :: SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                             const RegularExpression::Matches &aMatches,
                                             void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBalanceCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone equalizer set/decrease/increase equalizer
 *    band level client command response completion handler
 *    trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone equalizer set/decrease/increase equalizer
 *  band level command request.
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
ZonesControllerBasis :: SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerBandCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set equalizer preset client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set equalizer preset command request.
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
ZonesControllerBasis :: SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerPresetCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set highpass filter crossover frequency client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set highpass filter crossover frequency
 *  command request.
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
ZonesControllerBasis :: SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetHighpassCrossoverCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set lowpass filter crossover frequency client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set lowpass filter crossover frequency
 *  command request.
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
ZonesControllerBasis :: SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLowpassCrossoverCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume mute client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume mute command request.
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
ZonesControllerBasis :: SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetMuteCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set name client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set name command request.
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
ZonesControllerBasis :: SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set equalizer sound mode client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set equalizer sound mode command request.
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
ZonesControllerBasis :: SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSoundModeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set source (input) client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set source (input) command request.
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
ZonesControllerBasis :: SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous all zones set source (input) client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the all zones set source (input) command request.
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
ZonesControllerBasis :: SetSourceAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceAllCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone decrease/increase/set tone equalizer client
 *    command response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone decrease/increase/set tone equalizer command
 *  request.
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
ZonesControllerBasis :: SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetToneCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume level command request.
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
ZonesControllerBasis :: SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous all zones set volume level client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the all zones set volume level command request.
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
ZonesControllerBasis :: SetVolumeAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeAllCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zone set volume fixed/locked client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone set volume fixed/locked command request.
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
ZonesControllerBasis :: SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeFixedCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous zones controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client zones
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
ZonesControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

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
ZonesControllerBasis :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    BalanceModel::ChannelType              lChannel;
    BalanceModel::BalanceType              lBalance;
    ZoneModel *                            lZoneModel;
    StateChange::ZonesBalanceNotification  lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::BalanceResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Channel

    lChannel = *((const char *)(aBuffer) + aMatches.at(2).rm_so);

    // Match 4/4: Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Adjust the balance from the HLX's L:{80, 0} to {0, 80}:R tagged
    // discontinuous model to a non-tagged, continuous L:{-80, 80}:R
    // model.

    if (lChannel == BalanceModel::kChannelLeft)
    {
        lBalance = -lBalance;
    }

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the balance is unchanged, SetBalance will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetBalance(lBalance);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer,
                                                            const size_t &aSize,
                                                            const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    EqualizerBandModel::IdentifierType               lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType                    lLevel;
    ZoneModel *                                      lZoneModel;
    EqualizerBandModel *                             lEqualizerBandModel;
    StateChange::ZonesEqualizerBandNotification      lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::EqualizerBandResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the level is unchanged, SetLevel will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lEqualizerBandModel->SetLevel(lLevel);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lEqualizerBandIdentifier, lLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    EqualizerPresetModel::IdentifierType             lEqualizerPresetIdentifier;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesEqualizerPresetNotification    lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::EqualizerPresetResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Equalizer Preset Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the equalizer preset is unchanged, SetEqualizerPreset will
    // return kStatus_ValueAlreadySet and there will be no need to
    // send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = lZoneModel->SetEqualizerPreset(lEqualizerPresetIdentifier);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType           lZoneIdentifier;
    ToneModel::LevelType                lBass;
    ToneModel::LevelType                lTreble;
    ZoneModel *                         lZoneModel;
    StateChange::ZonesToneNotification  lStateChangeNotification;
    Status                              lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::ToneResponse::kExpectedMatches, done);

    // Match 2/4: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Bass Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lBass);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Treble Level

    lStatus = Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                               Common::Utilities::Distance(aMatches.at(3)),
                               lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the tone is unchanged, SetTone will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetTone(lBass, lTreble);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lBass, lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    CrossoverModel::FrequencyType                    lHighpassFrequency;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesHighpassCrossoverNotification  lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::HighpassCrossoverResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Highpass Frequency

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the highpass crossover frequency is unchanged,
    // SetHighpassFrequency will return kStatus_ValueAlreadySet and
    // there will be no need to send a state change notification. If
    // we receive kStatus_Success, it is the first time set or a
    // change and state change notification needs to be sent.

    lStatus = lZoneModel->SetHighpassFrequency(lHighpassFrequency);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                        lZoneIdentifier;
    CrossoverModel::FrequencyType                    lLowpassFrequency;
    ZoneModel *                                      lZoneModel;
    StateChange::ZonesLowpassCrossoverNotification   lStateChangeNotification;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::LowpassCrossoverResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Lowpass Frequency

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the lowpass crossover frequency is unchanged,
    // SetLowpassFrequency will return kStatus_ValueAlreadySet and
    // there will be no need to send a state change notification. If
    // we receive kStatus_Success, it is the first time set or a
    // change and state change notification needs to be sent.

    lStatus = lZoneModel->SetLowpassFrequency(lLowpassFrequency);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    const char *                           lMutep;
    ZoneModel::IdentifierType              lZoneIdentifier;
    VolumeModel::MuteType                  lMute;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::MuteResponse::kExpectedMatches, done);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleMuteChange(lZoneIdentifier, lMute);

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
ZonesControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    const char *                           lName;
    size_t                                 lNameSize;
    ZoneModel *                            lZoneModel;
    StateChange::ZonesNameNotification     lStateChangeNotification;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::NameResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the name is unchanged, SetName will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetName(lName, lNameSize);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    SoundModel::SoundMode                    lSoundMode;
    ZoneModel *                              lZoneModel;
    StateChange::ZonesSoundModeNotification  lStateChangeNotification;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SoundModeResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Sound Mode

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the sound mode is unchanged, SetSoundMode will return
    // kStatus_ValueAlreadySet and there will be no need to send a
    // state change notification. If we receive kStatus_Success, it is
    // the first time set or a change and state change notification
    // needs to be sent.

    lStatus = lZoneModel->SetSoundMode(lSoundMode);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
ZonesControllerBasis :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SourceResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleSourceChange(lZoneIdentifier, lSourceIdentifier);

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
ZonesControllerBasis :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::SourceAllResponse::kExpectedMatches, done);

    // Match 2/2: Source Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= mZonesMax; lZoneIdentifier++)
    {
        HandleSourceChange(lZoneIdentifier, lSourceIdentifier);
    }

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
ZonesControllerBasis :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    VolumeModel::LevelType                 lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleVolumeChange(lZoneIdentifier, lVolume);

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
ZonesControllerBasis :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType              lZoneIdentifier;
    VolumeModel::LevelType                 lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeAllResponse::kExpectedMatches, done);


    // Match 2/2: Volume

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= mZonesMax; lZoneIdentifier++)
    {
        HandleVolumeChange(lZoneIdentifier, lVolume);
    }

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
ZonesControllerBasis :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                   lZoneIdentifier;
    VolumeModel::FixedType                      lLocked;
    ZoneModel *                                 lZoneModel;
    StateChange::ZonesVolumeLockedNotification  lStateChangeNotification;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Zones::VolumeFixedResponse::kExpectedMatches, done);

    // Match 2/3: Zone Identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Fixed

    lStatus = Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                               Common::Utilities::Distance(aMatches.at(2)),
                               lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZonesModel.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the volume lock (fixed) is unchanged, SetVolumeFixed will
    // return kStatus_ValueAlreadySet and there will be no need to
    // send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = lZoneModel->SetVolumeFixed(lLocked);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lZoneIdentifier, lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

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
ZonesControllerBasis :: BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);


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
ZonesControllerBasis :: VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

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
ZonesControllerBasis :: VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesControllerBasis *lController = static_cast<ZonesControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->VolumeFixedNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
