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

#include "ZonesController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


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
    Proxy::ControllerBasis(),
    Common::ZonesControllerBasis(),
    Client::ZonesControllerBasis(),
    Server::ZonesControllerBasis(),
    mZonesDidRefreshCount(0)
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
            kMuteResponse,
            ZonesController::MuteNotificationReceivedHandler
        },

        {
            kSourceResponse,
            ZonesController::SourceNotificationReceivedHandler
        },

        {
            kVolumeResponse,
            ZonesController::VolumeNotificationReceivedHandler
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
ZonesController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kDecreaseVolumeRequest,
            ZonesController::DecreaseVolumeRequestReceivedHandler
        },

        {
            kIncreaseVolumeRequest,
            ZonesController::IncreaseVolumeRequestReceivedHandler
        },

        {
            kQueryRequest,
            ZonesController::QueryRequestReceivedHandler
        },

        {
            kQueryVolumeRequest,
            ZonesController::QueryVolumeRequestReceivedHandler
        },

        {
            kSetVolumeRequest,
            ZonesController::SetVolumeRequestReceivedHandler
        },
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
ZonesController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ZonesControllerBasis::Init();
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
ZonesController :: Refresh(const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
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

// MARK: Server-facing Client Observer Methods

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
ZonesController :: Query(void)
{
    Status lRetval = kStatus_Success;


    for (IdentifierType lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
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
ZonesController :: Query(const IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                                lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::Query>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ZonesController::QueryCompleteHandler,
                          ZonesController::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the maximum number of supported HLX zones.
 *
 *  @param[out]  aZones  The maximum number of HLX zones on success.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
ZonesController :: GetZonesMax(size_t &aZones) const
{
    Status  lRetval = kStatus_Success;

    aZones = kZonesMax;

    return (lRetval);
}

/**
 *  @brief
 *    Get the zone identifier with the specified name.
 *
 *  This attempts to lookup the zone identifier for the zone
 *  with the specified name.
 *
 *  @param[in]   aName                A pointer to a null-terminated
 *                                    C string of the name of the
 *                                    zone to find an identifier
 *                                    for.
 *  @param[out]  aZoneIdentifier      A reference to storage by which
 *                                    to return the identifier if
 *                                    successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No zone could be found with the
 *                            specified name.
 *
 *  @ingroup name
 *
 */
Status
ZonesController :: LookupIdentifier(const char *aName, IdentifierType &aZoneIdentifier) const
{
    const ZoneModel * lZoneModel;
    Status            lRetval;


    lRetval = mZones.GetZone(aName, lZoneModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lZoneModel->GetIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Mutator Methods

// MARK: Server-facing Client Volume Mutator Commands

/**
 *  @brief
 *    Set the zone volume level.
 *
 *  This attempts to set the zone volume level on the peer HLX server
 *  controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: SetVolume(const IdentifierType &aZoneIdentifier,
                             const Model::VolumeModel::LevelType &aLevel)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::SetVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::SetVolume>(lCommand)->Init(aZoneIdentifier, aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone volume level by one (1) unit.
 *
 *  This attempts to increase the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference for the
 *                                zone for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: IncreaseVolume(const IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::IncreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::IncreaseVolume>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the zone volume level by one (1) unit.
 *
 *  This attempts to decrease the zone volume by one (1) unit on the
 *  peer HLX server controller.
 *
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup volume
 *
 */
Status
ZonesController :: DecreaseVolume(const IdentifierType &aZoneIdentifier)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::DecreaseVolume());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::DecreaseVolume>(lCommand)->Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Command Completion Handlers

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
ZonesController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();
    const uint8_t *                lBuffer = lResponse->GetBuffer()->GetHead();
    IdentifierType                 lZoneIdentifier;
    Status                         lStatus;


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    mZonesDidRefreshCount++;

    if (WasRefreshRequested())
    {
        const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mZonesDidRefreshCount),
                                                                static_cast<uint8_t>(kZonesMax));

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
ZonesController :: SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                        lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                           lBufferSize = lResponse->GetBuffer()->GetSize();

    MuteNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
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
ZonesController :: SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                        lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                           lBufferSize = lResponse->GetBuffer()->GetSize();

    SourceNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
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
ZonesController :: SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches)
{
    const Client::Command::ResponseBasis * lResponse   = aExchange->GetResponse();
    const uint8_t *                lBuffer     = lResponse->GetBuffer()->GetHead();
    const size_t                   lBufferSize = lResponse->GetBuffer()->GetSize();

    VolumeNotificationReceivedHandler(lBuffer, lBufferSize, aMatches);
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
ZonesController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Client::Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                        lBuffer     = lRequest->GetBuffer();
    const size_t                           lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Zone Command", aError);
}

// MARK: Server-facing Client Command Completion Handler Trampolines

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
ZonesController :: QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
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
ZonesController :: SetMuteCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetMuteCompleteHandler(aExchange, aMatches);
    }
}


/**
 *  @brief
 *    Asynchronous zone set source (input) client command response
 *    completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the zone` set source (input) mute command request.
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
ZonesController :: SetSourceCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceCompleteHandler(aExchange, aMatches);
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
ZonesController :: SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeCompleteHandler(aExchange, aMatches);
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
ZonesController :: CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Server-facing Client Unsolicited Notification Handlers

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
    const char *                           lMutep;
    IdentifierType                         lZoneIdentifier;
    VolumeModel::MuteType                  lMute;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Zones::MuteResponse::kExpectedMatches, done);

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
    IdentifierType                         lZoneIdentifier;
    SourceModel::IdentifierType            lSourceIdentifier;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Zones::SourceResponse::kExpectedMatches, done);

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
    IdentifierType                         lZoneIdentifier;
    VolumeModel::LevelType                lVolume;
    Status                                 lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Client::Command::Zones::VolumeResponse::kExpectedMatches, done);

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

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

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

// MARK: Client-facing Server Command Request Completion Handlers

void ZonesController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = -1;
    IdentifierType                           lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleAdjustVolumeReceived(lZoneIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = 1;
    IdentifierType                           lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::IncreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleAdjustVolumeReceived(lZoneIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const bool                        kIsConfiguration = true;
    IdentifierType                           lZoneIdentifier;
    Server::Command::Zones::QueryResponse            lResponse;
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
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                           lZoneIdentifier;
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
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    IdentifierType                           lZoneIdentifier;
    VolumeModel::LevelType                   lVolume;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Level
    //
    // The validity of the volume level will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleSetVolumeReceived(lZoneIdentifier, lVolume, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Server-facing Client Implementation

void
ZonesController :: HandleMuteChange(const IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    Client::StateChange::ZonesMuteNotification     lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
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
ZonesController :: HandleSourceChange(const IdentifierType &aZoneIdentifier, const SourceModel::IdentifierType &aSourceIdentifier)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    Client::StateChange::ZonesSourceNotification   lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
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
ZonesController :: HandleVolumeChange(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume)
{
    ZoneModel *                            lZoneModel;
    Status                                 lStatus;
    Client::StateChange::ZonesVolumeNotification   lStateChangeNotification;


    lStatus = mZones.GetZone(aZoneIdentifier, lZoneModel);
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

done:
    return;
}

// MARK: Client-facing Server Implementation

// MARK: Client-facing Server Data Model Mutation State Change Methods

Status
ZonesController :: AdjustVolume(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, VolumeModel::LevelType &aVolume)
{
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
ZonesController :: SetMute(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute)
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
ZonesController :: HandleQueryReceived(const bool &aIsConfiguration, const IdentifierType &aZoneIdentifier, ConnectionBuffer::MutableCountedPointer &aOutputBuffer) const
{
    const ZoneModel *                        lZoneModel;
    const char *                             lName;
    Server::Command::Zones::NameResponse     lNameResponse;
    BalanceModel::BalanceType                lBalance;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Name Response

    lRetval = lZoneModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aZoneIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Source Response

    ;

    // Volume Response

    lRetval = HandleQueryVolumeReceived(aZoneIdentifier, *lZoneModel, aOutputBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Volume Fixed Response (include if for configuration)

    ;

    // Mute Response

    ;

    // Sound Mode Response

    ;

    // Balance Response

    ;

 done:
    return (lRetval);
}

Status
ZonesController :: HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const ZoneModel *                  lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleQueryVolumeReceived(aZoneIdentifier, *lZoneModel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleQueryVolumeReceived(const IdentifierType &aZoneIdentifier,
                                             const Model::ZoneModel &aZoneModel,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    VolumeModel::LevelType lVolume;
    Status                 lRetval;


    lRetval = aZoneModel.GetVolume(lVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, lVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetMute(const bool &aConditionally, const IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
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
ZonesController :: HandleSetMuteConditionally(const IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    const bool kConditionally = true;

    return (HandleSetMute(kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleAdjustVolumeReceived(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
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
ZonesController :: HandleSetVolumeReceived(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status                             lRetval;


    lRetval = SetVolume(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, aVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleMuteResponse(const IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::MuteResponse  lMuteResponse;
    const uint8_t *                       lBuffer;
    size_t                                lSize;
    Status                                lRetval;

    lRetval = lMuteResponse.Init(aZoneIdentifier, aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lMuteResponse.GetBuffer();
    lSize = lMuteResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleVolumeResponse(const IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Zones::VolumeResponse  lVolumeResponse;
    const uint8_t *                         lBuffer;
    size_t                                  lSize;
    Status                                  lRetval;


    lRetval = lVolumeResponse.Init(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lVolumeResponse.GetBuffer();
    lSize = lVolumeResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Client-facing Server Command Request Handler Trampolines

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

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
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

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Proxy

}; // namespace HLX
