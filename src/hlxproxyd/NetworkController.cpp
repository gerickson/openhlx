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

#include "NetworkController.hpp"

#include <memory>

#include <errno.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Client/NetworkStateChangeNotifications.hpp>
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

// The query network response contains both state and
// configuration settings.

static const char * const kQueryResponseBuffer = \
"(IP192.168.1.48)\r\n"
"(NM255.255.255.0)\r\n"
"(GW192.168.1.1)\r\n";

// The query current configuration response contains ONLY
// configuration settings.

static const char * const kQueryCurrentResponseBuffer = kQueryResponseBuffer;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkController :: NetworkController(void) :
    Common::NetworkControllerBasis(),
    Client::NetworkControllerBasis(Common::NetworkControllerBasis::mNetworkModel),
    Server::NetworkControllerBasis(Common::NetworkControllerBasis::mNetworkModel),
    Proxy::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
NetworkController :: ~NetworkController(void)
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
NetworkController :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::NetworkControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::NetworkControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::NetworkControllerBasis::Init(aServerCommandManager);
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
NetworkController :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kDHCPv4EnabledResponse,
            NetworkController::DHCPv4EnabledNotificationReceivedHandler
        },

        {
            kEthernetEUI48Response,
            NetworkController::EthernetEUI48NotificationReceivedHandler
        },

        {
            kSDDPEnabledResponse,
            NetworkController::SDDPEnabledNotificationReceivedHandler
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
NetworkController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            NetworkController::QueryRequestReceivedHandler
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
NetworkController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kIsConfiguration = true;
    Status                lRetval = kStatus_Success;


    (void)aConnection;

    lRetval = HandleQueryReceived(kIsConfiguration, kQueryCurrentResponseBuffer, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Server-facing Client Unsolicited Notification Handlers

/**
 *  @brief
 *    Ethernet network interface DHCPv4 enabled changed client
 *    unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface DHCPv4 enabled changed
 *  notification.
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
NetworkController :: DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::NetworkControllerBasis *  lController = static_cast<Client::NetworkControllerBasis *>(this);
    Status                            lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::NetworkControllerBasis::DHCPv4EnabledNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Ethernet network interface EUI-48 address changed client
 *    unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface EUI-48 address changed
 *  notification.
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
NetworkController :: EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::NetworkControllerBasis *  lController = static_cast<Client::NetworkControllerBasis *>(this);
    Status                            lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::NetworkControllerBasis::EthernetEUI48NotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

/**
 *  @brief
 *    Ethernet network interface Control4 SDDP enabled changed client
 *    unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface Control4 SDDP enabled changed
 *  notification.
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
NetworkController :: SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Client::NetworkControllerBasis *  lController = static_cast<Client::NetworkControllerBasis *>(this);
    Status                            lStatus;


    lStatus = ProxyNotification(aBuffer,
                                aSize,
                                aMatches,
                                Client::NetworkControllerBasis::SDDPEnabledNotificationReceivedHandler,
                                lController);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

// MARK: Server-facing Client Unsolicited Notification Handler Trampolines

/**
 *  @brief
 *    Ethernet network interface DHCPv4 enabled state changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface DHCPv4 enabled
 *  state changed notification.
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
NetworkController :: DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->DHCPv4EnabledNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Ethernet network interface EUI-48 address changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface EUI-48 address
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
NetworkController :: EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->EthernetEUI48NotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Ethernet network interface Control4 SDDP enabled state changed
 *    client unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface Control4 SDDP
 *  enabled state changed notification.
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
NetworkController :: SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->SDDPEnabledNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

// MARK: Client-facing Server Command Request Completion Handlers

void NetworkController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static constexpr bool                    kIsConfiguration = true;
    Server::Command::Network::QueryResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Network::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(!kIsConfiguration, kQueryResponseBuffer, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lStatus = lResponse.Init();
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
                                          Client::NetworkControllerBasis::QueryCompleteHandler,
                                          Client::NetworkControllerBasis::CommandErrorHandler,
                                          NetworkController::QueryRequestReceivedHandler,
                                          static_cast<Client::NetworkControllerBasis *>(this),
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

// MARK: Client-facing Server Command Request Handler Trampolines

void NetworkController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Proxy

}; // namespace HLX
