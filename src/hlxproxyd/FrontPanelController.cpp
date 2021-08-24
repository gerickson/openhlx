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

#include "FrontPanelController.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/FrontPanelControllerCommands.hpp>
#include <OpenHLX/Client/FrontPanelStateChangeNotifications.hpp>
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
FrontPanelController :: FrontPanelController(void) :
    Common::FrontPanelControllerBasis(),
    Client::FrontPanelControllerBasis(Common::FrontPanelControllerBasis::mFrontPanelModel),
    Server::FrontPanelControllerBasis(Common::FrontPanelControllerBasis::mFrontPanelModel),
    Proxy::ControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
FrontPanelController :: ~FrontPanelController(void)
{
    return;
}

Status
FrontPanelController :: DoRequestHandlers(const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            FrontPanelController::QueryRequestReceivedHandler
        },

        {
            kSetBrightnessRequest,
            FrontPanelController::SetBrightnessRequestReceivedHandler
        },

        {
            kSetLockedRequest,
            FrontPanelController::SetLockedRequestReceivedHandler
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
FrontPanelController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::FrontPanelControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

#if 1
    lRetval = Client::FrontPanelControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::FrontPanelControllerBasis::Init(aServerCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);
#endif

    lRetval = Proxy::ControllerBasis::Init(aClientCommandManager, aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
FrontPanelController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
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

void FrontPanelController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::LockedType              lLocked;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleLockedResponse(lLocked, lResponseBuffer);
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
                                          Client::FrontPanelControllerBasis::QueryCompleteHandler,
                                          Client::FrontPanelControllerBasis::CommandErrorHandler,
                                          FrontPanelController::QueryRequestReceivedHandler,
                                          static_cast<Client::FrontPanelControllerBasis *>(this),
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

void FrontPanelController :: SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kBrightnessResponse,
                                   Client::FrontPanelControllerBasis::SetBrightnessCompleteHandler,
                                   Client::FrontPanelControllerBasis::CommandErrorHandler,
                                   static_cast<Client::FrontPanelControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void FrontPanelController :: SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status lStatus;

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kLockedResponse,
                                   Client::FrontPanelControllerBasis::SetLockedCompleteHandler,
                                   Client::FrontPanelControllerBasis::CommandErrorHandler,
                                   static_cast<Client::FrontPanelControllerBasis *>(this));
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

void FrontPanelController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void FrontPanelController :: SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBrightnessRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void FrontPanelController :: SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FrontPanelController *lController = static_cast<FrontPanelController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLockedRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Implementation

Status
FrontPanelController :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    FrontPanelModel::BrightnessType          lBrightness;
    FrontPanelModel::LockedType              lLocked;
    Status                                   lRetval;


    lRetval = GetModel().GetBrightness(lBrightness);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleBrightnessResponse(lBrightness, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = GetModel().GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleLockedResponse(lLocked, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status FrontPanelController :: HandleBrightnessResponse(const FrontPanelModel::BrightnessType &aBrightness, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::BrightnessResponse  lBrightnessResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lBrightnessResponse.Init(aBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lBrightnessResponse.GetBuffer();
    lSize = lBrightnessResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

Status FrontPanelController :: HandleLockedResponse(const FrontPanelModel::LockedType &aLocked, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::FrontPanel::LockedResponse  lLockedResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    lStatus = lLockedResponse.Init(aLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lLockedResponse.GetBuffer();
    lSize = lLockedResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

}; // namespace Proxy

}; // namespace HLX
