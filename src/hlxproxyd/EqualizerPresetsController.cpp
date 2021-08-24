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

#include "EqualizerPresetsController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


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
EqualizerPresetsController :: EqualizerPresetsController(void) :
    Common::EqualizerPresetsControllerBasis(),
    Client::EqualizerPresetsControllerBasis(Common::EqualizerPresetsControllerBasis::mEqualizerPresets,
                                            Common::EqualizerPresetsControllerBasis::kEqualizerPresetsMax),
    Server::EqualizerPresetsControllerBasis(Common::EqualizerPresetsControllerBasis::mEqualizerPresets,
                                            Common::EqualizerPresetsControllerBasis::kEqualizerPresetsMax),
    Proxy::ControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
EqualizerPresetsController :: ~EqualizerPresetsController(void)
{
    return;
}

Status
EqualizerPresetsController :: DoRequestHandlers(const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kDecreaseBandRequest,
            EqualizerPresetsController::DecreaseBandRequestReceivedHandler
        },

        {
            kIncreaseBandRequest,
            EqualizerPresetsController::IncreaseBandRequestReceivedHandler
        },

        {
            kQueryRequest,
            EqualizerPresetsController::QueryRequestReceivedHandler
        },

        {
            kSetBandRequest,
            EqualizerPresetsController::SetBandRequestReceivedHandler
        },

        {
            kSetNameRequest,
            EqualizerPresetsController::SetNameRequestReceivedHandler
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
EqualizerPresetsController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::EqualizerPresetsControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::EqualizerPresetsControllerBasis::Init(aServerCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

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
EqualizerPresetsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval = kStatus_Success;


    (void)aConnection;

    lRetval = HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Client-facing Server Command Request Completion Handlers

void
EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void
EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                    lEqualizerPresetIdentifier;
    Server::Command::EqualizerPresets::QueryResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer           lResponseBuffer;
    Status                                            lStatus;
    const uint8_t *                                   lBuffer;
    size_t                                            lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lEqualizerPresetIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lStatus = lResponse.Init(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

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
                                          Client::EqualizerPresetsControllerBasis::QueryCompleteHandler,
                                          Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                                          EqualizerPresetsController::QueryRequestReceivedHandler,
                                          static_cast<Client::EqualizerPresetsControllerBasis *>(this),
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

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kEqualizerBandResponse,
                                   Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                                   Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void
EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kNameResponse,
                                   Client::EqualizerPresetsControllerBasis::SetNameCompleteHandler,
                                   Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Client-facing Server Command Request Handler Trampolines

void EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Implementation

Status EqualizerPresetsController :: GetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel)
{
    EqualizerPresetModel *             lEqualizerPresetModel;
    Status                             lRetval;


    // Attempt to get the equalizer preset model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Attempt to get the equalizer band model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = lEqualizerPresetModel->GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(Server::ConnectionBasis &aConnection,
                                                       const IdentifierType &aEqualizerPresetIdentifier,
                                                       const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                       const Model::EqualizerBandModel::LevelType &aBandAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleAdjustBandReceived(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(const IdentifierType &aEqualizerPresetIdentifier,
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

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
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

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: HandleSetBandReceived(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lEqualizerBandModel->SetLevel(aBandLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        ;
    }

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Proxy

}; // namespace HLX
