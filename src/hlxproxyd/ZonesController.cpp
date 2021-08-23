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

#include "EqualizerPresetsController.hpp"
#include "SourcesController.hpp"


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


// Preprocessor Definitions

#define USE_PROXY 1

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
    Proxy::ControllerBasis()
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

Status
ZonesController :: DoRequestHandlers(const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
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

#if 1
    lRetval = Client::ZonesControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ZonesControllerBasis::Init(aServerCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);
#endif

    lRetval = Proxy::ControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // These MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
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

// MARK: Client-facing Server Command Request Completion Handlers

void ZonesController :: AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    BalanceModel::ChannelType                lChannel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::AdjustBalanceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBalanceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Channel
    //
    // The validity of the channel will be range checked at
    // HandleAdjustBalanceReceived below.

    lChannel = *(reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kBalanceResponse,
                                   ZonesController::SetBalanceCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBalanceReceived(lZoneIdentifier, lChannel, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const ToneModel::LevelType        kAdjustment = -1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBassReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   ZonesController::SetToneCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    static const ToneModel::LevelType        kAdjustment = 1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBassReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   ZonesController::SetToneCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    static const ToneModel::LevelType        kAdjustment = -1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustTrebleReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   ZonesController::SetToneCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    static const ToneModel::LevelType        kAdjustment = 1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustTrebleReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   ZonesController::SetToneCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    static const EqualizerBandModel::LevelType  kAdjustment = -1;
    Model::ZoneModel::IdentifierType            lZoneIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   ZonesController::SetEqualizerBandCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    static const EqualizerBandModel::LevelType  kAdjustment = 1;
    Model::ZoneModel::IdentifierType            lZoneIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::IncreaseEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   ZonesController::SetEqualizerBandCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);
#else // USE_PROXY
#endif // USE_PROXY

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
    DeclareScopedFunctionTracer(lTracer);
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = -1;
    ZoneModel::IdentifierType                lZoneIdentifier;
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

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   ZonesController::SetVolumeCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

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
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = 1;
    ZoneModel::IdentifierType                lZoneIdentifier;
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

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   ZonesController::SetVolumeCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

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
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    const char *                             lMutep;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::MuteType                    lMute;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::MuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteUnconditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kMuteResponse,
                                   ZonesController::SetMuteCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetMuteUnconditionally(lZoneIdentifier, lMute, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const bool                        kIsConfiguration = true;
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
                                          ZonesController::QueryCompleteHandler,
                                          ZonesController::CommandErrorHandler,
                                          ZonesController::QueryRequestReceivedHandler,
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
                                          ZonesController::SetMuteCompleteHandler,
                                          ZonesController::CommandErrorHandler,
                                          ZonesController::QueryMuteRequestReceivedHandler,
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
                                          ZonesController::SetSourceCompleteHandler,
                                          ZonesController::CommandErrorHandler,
                                          ZonesController::QuerySourceRequestReceivedHandler,
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

    // At this point, the inbound command appears valid. One of two
    // things will happen next. We'll attempt to satisfy the request
    // from our local data model cache. If we can satisfy it, we'll
    // send the response. If we cannot satisfy it due to a TBD error,
    // then we need to forward the request onto the proxied server. On
    // response, we will update the local cache and forward the
    // response back to the initiator. On server error, we will
    // forward the error back to the initiator. On server timeout, we
    // will send an error response back to the initiator.

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryVolumeReceived(lZoneIdentifier, lResponseBuffer);
    Log::Debug().Write("%s: %d: lStatus %d\n",
                       __func__, __LINE__, lStatus);
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
                                          ZonesController::SetVolumeCompleteHandler,
                                          ZonesController::CommandErrorHandler,
                                          ZonesController::QueryVolumeRequestReceivedHandler,
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
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    BalanceModel::ChannelType                lChannel;
    BalanceModel::BalanceType                lBalance;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetBalanceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetBalanceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Channel
    //
    // The validity of the channel is enforced at the data model
    // normalization below.

    lChannel = *(reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);

    // Match 4/4: Level
    //
    // The validity of the balance will be range checked at
    // HandleSetBalanceReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kBalanceResponse,
                                   ZonesController::SetBalanceCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Adjust the balance from the HLX's L:{80, 0} to {0, 80}:R tagged
    // discontinuous model to a non-tagged, continuous L:{-80, 80}:R
    // model.

    if (lChannel == BalanceModel::kChannelLeft)
    {
        lBalance = -lBalance;
    }

    lStatus = HandleSetBalanceReceived(lZoneIdentifier, lBalance, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    EqualizerBandModel::IdentifierType       lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType            lBandLevel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleSetEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level
    //
    // The validity of the equalizer band level will be range checked
    // at HandleSetEqualizerBandReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBandLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   ZonesController::SetEqualizerBandCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to zone equalizer mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeZoneEqualizer, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the zone equalizer band adjustment.

    lStatus = HandleSetEqualizerBandReceived(lZoneIdentifier, lEqualizerBandIdentifier, lBandLevel, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    EqualizerPresetModel::IdentifierType     lEqualizerPresetIdentifier;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetEqualizerPresetRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Equalizer Preset Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = EqualizerPresetsController::ValidateIdentifier(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerPresetResponse,
                                   ZonesController::SetEqualizerPresetCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to equalizer preset mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModePresetEqualizer, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the equalizer preset request.

    lStatus = lZoneModel->SetEqualizerPreset(lEqualizerPresetIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleEqualizerPresetResponse(lZoneIdentifier, lEqualizerPresetIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    CrossoverModel::FrequencyType            lHighpassFrequency;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetHighpassCrossoverRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Highpass Frequency
    //
    // The validity of the highpass crossover frequency will be range
    // checked at SetHighpassFrequency below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kHighpassCrossoverResponse,
                                   ZonesController::SetHighpassCrossoverCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to highpass crossover mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeHighpass, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the highpass crossover frequency request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetHighpassFrequency(lHighpassFrequency);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleHighpassCrossoverResponse(lZoneIdentifier, lHighpassFrequency, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    CrossoverModel::FrequencyType            lLowpassFrequency;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetLowpassCrossoverRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Lowpass Frequency
    //
    // The validity of the lowpass crossover frequency will be range
    // checked at SetLowpassFrequency below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kLowpassCrossoverResponse,
                                   ZonesController::SetLowpassCrossoverCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to lowpass crossover mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeLowpass, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the lowpass crossover frequency request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetLowpassFrequency(lLowpassFrequency);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleLowpassCrossoverResponse(lZoneIdentifier, lLowpassFrequency, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    const char *                             lName;
    size_t                                   lNameSize;
    ZoneModel *                              lZoneModel;
    Server::Command::Zones::NameResponse     lNameResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // GetZone below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kNameResponse,
                                   ZonesController::SetNameCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the zone model associated with the parsed zone
    // identifier. This will include a range check on the zone
    // identifier.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lZoneModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = lNameResponse.Init(lZoneIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    SoundModel::SoundMode                    lSoundMode;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSoundModeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeUnconditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Sound Mode
    //
    // The validity of the sound mode will be range checked at
    // HandleSetSoundModeUnconditionally below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSoundModeResponse,
                                   ZonesController::SetSoundModeCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetSoundModeUnconditionally(lZoneIdentifier, lSoundMode, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    SourceModel::IdentifierType              lSourceIdentifier;
    Server::Command::Zones::SourceResponse   lSourceResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSourceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // SetSource below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SourcesController::ValidateIdentifier(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSourceResponse,
                                   ZonesController::SetSourceCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SetSource(lZoneIdentifier, lSourceIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    lStatus = lSourceResponse.Init(lZoneIdentifier, lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSourceResponse.GetBuffer();
    lSize = lSourceResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType           lZoneIdentifier;
    SourceModel::IdentifierType                lSourceIdentifier;
    Server::Command::Zones::SourceAllResponse  lSourceAllResponse;
    ConnectionBuffer::MutableCountedPointer    lResponseBuffer;
    Status                                     lStatus;
    const uint8_t *                            lBuffer;
    size_t                                     lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSourceAllRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Source Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SourcesController::ValidateIdentifier(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if 0 // XXX USE_PROXY - Need SetSourceAllCompleteHandler implementation

    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSourceAllResponse,
                                   ZonesController::SetSourceAllCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        lStatus = SetSource(lZoneIdentifier, lSourceIdentifier);
        nlREQUIRE(lStatus >= kStatus_Success, done);
    }

    lStatus = lSourceAllResponse.Init(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSourceAllResponse.GetBuffer();
    lSize = lSourceAllResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    ToneModel::LevelType                     lBass;
    ToneModel::LevelType                     lTreble;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetToneRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Bass Level
    //
    // The validity of the bass level will be range checked at
    // SetTone below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lBass);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Treble Level
    //
    // The validity of the treble level will be range checked at
    // SetTone below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kToneResponse,
                                   ZonesController::SetToneCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the tone request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetTone(lBass, lTreble);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleToneResponse(lZoneIdentifier, lBass, lTreble, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    static const VolumeModel::MuteType       kMuted = true;
    ZoneModel::IdentifierType                lZoneIdentifier;
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

#if USE_PROXY
    // MARK: Proxy server handling path...
    //
    // Here, we a fully-formed and mostly-verified command mutation
    // request. We first send it to the proxied server. When the
    // response comes back, on success, it will be processed like a
    // normal volume changed notification with an attendant change to
    // the proxy data model. On error, it will be processed like a
    // normal error with the proxy error handler sending the error on
    // to the original client. On success, since the data model has
    // already been mutated, there is no further need to peform any
    // proxy server-side processing. The response is simply sent along
    // to the originally-initiating client.

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeResponse,
                                   ZonesController::SetVolumeCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

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
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType        kMuted = true;
    Model::ZoneModel::IdentifierType          lZoneIdentifier;
    VolumeModel::LevelType                    lVolume;
    ConnectionBuffer::MutableCountedPointer   lResponseBuffer;
    Server::Command::Zones::VolumeAllResponse lVolumeAllResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeAllRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 1/2: Volume Level
    //
    // The validity of the volume level will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                                      Common::Utilities::Distance(aMatches.at(1)),
                                      lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

#if 0 // XXX USE_PROXY - Need SetVolumeAllCompleteHandler implementation
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeAllResponse,
                                   ZonesController::SetVolumeAllCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        // First, ensure that the zone is unmuted.
        //
        // A mute response will only be conditionally generated if the
        // mute status changed as a result.

        lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
        nlREQUIRE(lStatus >= kStatus_Success, done);

        // Next, go ahead and process the volume adjustment.

        lStatus = SetVolume(lZoneIdentifier, lVolume);
        nlREQUIRE(lStatus >= kStatus_Success, done);
    }

    lStatus = lVolumeAllResponse.Init(lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lVolumeAllResponse.GetBuffer();
    lSize = lVolumeAllResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::FixedType                   lLocked;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeFixedRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // GetZone below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Fixed
    //
    // The validity of the volume lock will be range checked at
    // SetVolumeFixed below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kVolumeFixedResponse,
                                   ZonesController::SetVolumeFixedCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetVolumeFixed(lLocked);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        ;
    }

    lStatus = HandleVolumeFixedResponse(lZoneIdentifier, lLocked, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::MuteType                    lMute;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::ToggleMuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // ToggleMute below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

#if USE_PROXY
    // MARK: Proxy server handling path...

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kMuteResponse,
                                   ZonesController::SetMuteCompleteHandler,
                                   ZonesController::CommandErrorHandler,
                                   this);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
#else // USE_PROXY
    // MARK: Normal server handlng path...

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ToggleMute(lZoneIdentifier, lMute);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleMuteResponse(lZoneIdentifier, lMute, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif // USE_PROXY

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
    const bool kConditionally = true;

    return (HandleSetMute(kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleSetMuteUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    const bool kConditionally = true;

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
    const bool kConditionally = true;

    return (HandleSetSoundMode(kConditionally, aZoneIdentifier, aSoundMode, aBuffer));
}

Status
ZonesController :: HandleSetSoundModeUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    const bool kConditionally = true;

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
