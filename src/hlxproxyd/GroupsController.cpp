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

#include "GroupsController.hpp"

#include <memory>
#include <vector>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "SourcesController.hpp"
#include "ZonesController.hpp"


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

namespace Detail
{

typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

};

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
GroupsController :: GroupsController(void) :
    Common::GroupsControllerBasis(),
    Client::GroupsControllerBasis(Common::GroupsControllerBasis::mGroups,
                                  Common::GroupsControllerBasis::kGroupsMax),
    Server::GroupsControllerBasis(Common::GroupsControllerBasis::mGroups,
                                  Common::GroupsControllerBasis::kGroupsMax),
    Proxy::ControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
GroupsController :: ~GroupsController(void)
{
    return;
}

Status
GroupsController :: DoRequestHandlers(const bool &aRegister)
{
    DeclareScopedFunctionTracer(lTracer);
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kAddZoneRequest,
            GroupsController::AddZoneRequestReceivedHandler
        },

        {
            kClearZonesRequest,
            GroupsController::ClearZonesRequestReceivedHandler
        },

        {
            kDecreaseVolumeRequest,
            GroupsController::DecreaseVolumeRequestReceivedHandler
        },

        {
            kIncreaseVolumeRequest,
            GroupsController::IncreaseVolumeRequestReceivedHandler
        },

        {
            kMuteRequest,
            GroupsController::MuteRequestReceivedHandler
        },

        {
            kQueryRequest,
            GroupsController::QueryRequestReceivedHandler
        },

        {
            kRemoveZoneRequest,
            GroupsController::RemoveZoneRequestReceivedHandler
        },

        {
            kSetNameRequest,
            GroupsController::SetNameRequestReceivedHandler
        },

        {
            kSetSourceRequest,
            GroupsController::SetSourceRequestReceivedHandler
        },

        {
            kSetVolumeRequest,
            GroupsController::SetVolumeRequestReceivedHandler
        },

        {
            kToggleMuteRequest,
            GroupsController::ToggleMuteRequestReceivedHandler
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
GroupsController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::GroupsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::GroupsControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::GroupsControllerBasis::Init(aServerCommandManager);
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
GroupsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
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

void GroupsController :: AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kZoneResponse,
                                   Client::GroupsControllerBasis::ChangeZoneCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
#if XXX // Need to create ClearZonesResponse
    // There is no static clear group zones response, so we instantiate
    // and initialize one on the stack.

    Client::Command::Groups::ClearZonesResponse  lClearZonesResponse;
    Status                                       lStatus;


    lStatus = lSetVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lClearZonesResponse,
                                   Client::GroupsControllerBasis::ClearZonesCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }
#endif

    return;
}

void GroupsController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static decrease group volume response, so we
    // instantiate and initialize one on the stack.

    Client::Command::Groups::DecreaseVolumeResponse  lDecreaseVolumeResponse;
    Status                                           lStatus;


    lStatus = lDecreaseVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lDecreaseVolumeResponse,
                                   Client::GroupsControllerBasis::DecreaseVolumeCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static increase group volume response, so we
    // instantiate and initialize one on the stack.

    Client::Command::Groups::IncreaseVolumeResponse  lIncreaseVolumeResponse;
    Status                                           lStatus;


    lStatus = lIncreaseVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lIncreaseVolumeResponse,
                                   Client::GroupsControllerBasis::IncreaseVolumeCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static set group mute response, so we instantiate
    // and initialize one on the stack.

    Client::Command::Groups::SetMuteResponse  lSetMuteResponse;
    Status                                    lStatus;


    lStatus = lSetMuteResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lSetMuteResponse,
                                   Client::GroupsControllerBasis::SetMuteCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lGroupIdentifier;
    Server::Command::Groups::QueryResponse   lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Groups::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Group Identifier
    //
    // The validity of the group identifier will be range checked at
    // HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lResponse.Init(lGroupIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lGroupIdentifier, lResponseBuffer);
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
                                          Client::GroupsControllerBasis::QueryCompleteHandler,
                                          Client::GroupsControllerBasis::CommandErrorHandler,
                                          GroupsController::QueryRequestReceivedHandler,
                                          static_cast<Client::GroupsControllerBasis *>(this),
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

void GroupsController :: RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kZoneResponse,
                                   Client::GroupsControllerBasis::ChangeZoneCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kNameResponse,
                                   Client::GroupsControllerBasis::SetNameCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Status                                   lStatus;


    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   kSourceResponse,
                                   Client::GroupsControllerBasis::SetSourceCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void GroupsController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static set group volume response, so we instantiate
    // and initialize one on the stack.

    Client::Command::Groups::SetVolumeResponse  lSetVolumeResponse;
    Status                                      lStatus;


    lStatus = lSetVolumeResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lSetVolumeResponse,
                                   Client::GroupsControllerBasis::SetVolumeCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus < kStatus_Success)
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

/*
 * As documented above, the toggle mute request functions quite
 * differently in the server group controller than it does in the
 * server zone controller.
 *
 * The group controller acts somewhat statelessly, since any member
 * zone may be independently mutated following a group operation that
 * includes such a zone. Consequently, group actions to attempt to
 * bring zone membership back into alignment with the prevailing
 * request, which may be a non-operation if zone state has not changed
 * since the last group operation.
 *
 * As a result, this handler (and, by extension, this controller) will
 * post a notification of state change to the server parent
 * controller. The server parent controller will receive notification
 * of the requested action and will carry it out by mutating the
 * relevant zones, based on membership.
 *
 * After the state change handling is complete, all this handler must
 * do is acknowledge the request by reflecting it back in the response
 * to the initiator.
 */
void GroupsController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    // There is no static toggle group mute response, so we instantiate
    // and initialize one on the stack.

    Client::Command::Groups::ToggleMuteResponse  lToggleMuteResponse;
    Status                                       lStatus;


    lStatus = lToggleMuteResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ProxyMutationCommand(aConnection,
                                   aBuffer,
                                   aSize,
                                   aMatches,
                                   lToggleMuteResponse,
                                   Client::GroupsControllerBasis::ToggleMuteCompleteHandler,
                                   Client::GroupsControllerBasis::CommandErrorHandler,
                                   static_cast<Client::GroupsControllerBasis *>(this));
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

void GroupsController :: AddZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->AddZoneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: ClearZonesRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ClearZonesRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: RemoveZoneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->RemoveZoneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void GroupsController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    GroupsController *lController = static_cast<GroupsController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToggleMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Proxy

}; // namespace HLX
