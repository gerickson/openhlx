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

#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

// Server-facing Client notification response data

Client::Command::Zones::VolumeResponse             ZonesController::kVolumeResponse;
/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesController :: ZonesController(void) :
    ControllerBasis(),
    Common::ZonesControllerBasis(),
    mZonesDidRefreshCount(0),
    mZones()
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
ZonesController :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response data.

    lRetval = kVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
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
    Status lRetval = kStatus_Success;


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
ZonesController :: Init(Client::CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mZones.Init(kZonesMax);
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
ZonesController :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mZonesDidRefreshCount = 0;

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
    Client::Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                                  lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Client::Command::Zones::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Client::Command::Zones::Query>(lCommand)->Init(aZoneIdentifier);
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

done:
    return;
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
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();
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

}; // namespace Proxy

}; // namespace HLX
