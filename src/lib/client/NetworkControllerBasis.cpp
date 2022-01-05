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
 *      This file implements a derivable object for realizing a HLX
 *      Ethernet network interface controller, in a client.
 *
 */

#include "NetworkControllerBasis.hpp"

#include <memory>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Client/NetworkStateChangeNotifications.hpp>
#include <OpenHLX/Model/NetworkModel.hpp>
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

// Class-scoped Notification Regular Expression Data

/**
 *  Class-scoped server network interface DHCPv4 enabled notification
 *  regular expression.
 *
 */
Command::Network::DHCPv4EnabledResponse           NetworkControllerBasis::kDHCPv4EnabledResponse;

/**
 *  Class-scoped server network interface EUI-48 address notification
 *  regular expression.
 *
 */
Command::Network::EthernetEUI48Response           NetworkControllerBasis::kEthernetEUI48Response;

/**
 *  Class-scoped server network interface default router IP address
 *  notification regular expression.
 *
 */
Command::Network::IPDefaultRouterAddressResponse  NetworkControllerBasis::kIPDefaultRouterAddressResponse;

/**
 *  Class-scoped server network interface host IP address notification
 *  regular expression.
 *
 */
Command::Network::IPHostAddressResponse           NetworkControllerBasis::kIPHostAddressResponse;

/**
 *  Class-scoped server network interface IP netmask notification
 *  regular expression.
 *
 */
Command::Network::IPNetmaskResponse               NetworkControllerBasis::kIPNetmaskResponse;

/**
 *  Class-scoped server network interface Control4 SDDP enabled
 *  notification regular expression.
 *
 */
Command::Network::SDDPEnabledResponse             NetworkControllerBasis::kSDDPEnabledResponse;

/**
 *  Class-scoped server network interface query notification regular
 *  expression.
 *
 */
Command::Network::QueryResponse                   NetworkControllerBasis::kQueryResponse;

/**
 *  @brief
 *    This is a class constructor.
 *
 *  This constructs the network interface controller with the
 *  specified network interface model.
 *
 *  @param[in]  aNetworkModel  A mutable reference to the network
 *                             interface model to construct the
 *                             controller with. This is retained by a
 *                             weak pointer reference and,
 *                             consequently, must remain in scope for
 *                             the lifetime of the controller.
 *
 */
NetworkControllerBasis :: NetworkControllerBasis(Model::NetworkModel &aNetworkModel) :
    Client::ObjectControllerBasis(),
    mNetworkModel(aNetworkModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
NetworkControllerBasis :: ~NetworkControllerBasis(void)
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
NetworkControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = ResponseInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager, aTimeout);
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
 *  Presently, this controller does so by executing a "query network
 *  [QE]" command with the peer server.
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
Status
NetworkControllerBasis :: Refresh(const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    // Notify the base controller that we have begun a refresh
    // operation.

    SetRefreshRequested(true);

    // Issue a query network request.

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
NetworkControllerBasis :: DoNotificationHandlers(const bool &aRegister)
{
    static const NotificationHandlerBasis  lNotificationHandlers[] = {
        {
            kDHCPv4EnabledResponse,
            NetworkControllerBasis::DHCPv4EnabledNotificationReceivedHandler
        },

        {
            kEthernetEUI48Response,
            NetworkControllerBasis::EthernetEUI48NotificationReceivedHandler
        },

        {
            kIPDefaultRouterAddressResponse,
            NetworkControllerBasis::IPDefaultRouterAddressNotificationReceivedHandler
        },

        {
            kIPHostAddressResponse,
            NetworkControllerBasis::IPHostAddressNotificationReceivedHandler
        },

        {
            kIPNetmaskResponse,
            NetworkControllerBasis::IPNetmaskNotificationReceivedHandler
        },

        {
            kSDDPEnabledResponse,
            NetworkControllerBasis::SDDPEnabledNotificationReceivedHandler
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
NetworkControllerBasis :: ResponseInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static notification response regular expression pattern data.

    lRetval = kDHCPv4EnabledResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kEthernetEUI48Response.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIPDefaultRouterAddressResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIPHostAddressResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kIPNetmaskResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSDDPEnabledResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Query the Ethernet network interface state.
 *
 *  This queries the current HLX server Ethernet network interface state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NetworkControllerBasis :: Query(void)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lCommand.reset(new Command::Network::Query());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Network::Query>(lCommand)->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          NetworkControllerBasis::QueryCompleteHandler,
                          NetworkControllerBasis::CommandErrorHandler,
                          this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Command Completion Handlers

/**
 *  @brief
 *    Asynchronous query Ethernet network interface client command
 *    response completion handler.
 *
 *  This handles an asynchronous client command response for the query
 *  Ethernet network interface command request.
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
NetworkControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                               const RegularExpression::Matches &aMatches)
{
    const Command::ResponseBasis * lResponse = aExchange->GetResponse();
    const size_t                   lExpectedMatchCount = lResponse->GetRegularExpression().GetExpectedMatchCount();


    nlREQUIRE(aMatches.size() == lExpectedMatchCount, done);

    MaybeUpdateRefreshIfRefreshWasRequested();

 done:
    return;
}

/**
 *  @brief
 *    Asynchronous network controller client command request
 *    error handler.
 *
 *  This handles any asynchronous client network controller
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
NetworkControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError)
{
    const Command::RequestBasis *  lRequest    = aExchange->GetRequest();
    const uint8_t *                lBuffer     = lRequest->GetBuffer();
    const size_t                   lBufferSize = lRequest->GetSize();

    OnCommandError(lBuffer, lBufferSize, "Network Command", aError);
}

// MARK: Command Completion Handler Trampolines

/**
 *  @brief
 *    Asynchronous query Ethernet network interface client command
 *    response completion handler trampoline.
 *
 *  This invokes the handler for an asynchronous client command
 *  response for the query Ethernet network interface command request.
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
NetworkControllerBasis :: QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCompleteHandler(aExchange, aMatches);
    }
}

/**
 *  @brief
 *    Asynchronous network controller client command request error
 *    handler trampoline.
 *
 *  This invokes the handler for any asynchronous client network
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
NetworkControllerBasis :: CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->CommandErrorHandler(aExchange, aError);
    }
}

// MARK: Unsolicited Notification Handlers

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
NetworkControllerBasis :: DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    NetworkModel::EnabledType                      lEnabled;
    StateChange::NetworkDHCPv4EnabledNotification  lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::DHCPv4EnabledResponse::kExpectedMatches, done);

    // Match 2/2: Enabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the DHCPv4 enabled state is unchanged, SetDHCPv4Enabled will
    // return kStatus_ValueAlreadySet and there will be no need to
    // send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetDHCPv4Enabled(lEnabled);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

static Common::Status
Parse(const uint8_t *aBuffer, const size_t &aBufferLength, NetworkModel::EthernetEUI48Type &aEthernetEUI48)
{
    int    lConversions;
    Status lRetval = kStatus_Success;


    (void)aBufferLength;

    lConversions = sscanf(reinterpret_cast<const char *>(aBuffer),
                          "%02hhx-%02hhx-%02hhx-%02hhx-%02hhx-%02hhx",
                          &aEthernetEUI48[0],
                          &aEthernetEUI48[1],
                          &aEthernetEUI48[2],
                          &aEthernetEUI48[3],
                          &aEthernetEUI48[4],
                          &aEthernetEUI48[5]);
    nlREQUIRE_ACTION(lConversions == sizeof(NetworkModel::EthernetEUI48Type), done, lRetval = -EINVAL);

 done:
    return (lRetval);
}

static Common::Status
Parse(const uint8_t *aBuffer, const size_t &aBufferLength, Common::IPAddress &aIPAddress)
{
    const size_t lLength = std::min(aBufferLength,
                                    static_cast<size_t>(INET6_ADDRSTRLEN));
    char         lBuffer[INET6_ADDRSTRLEN];
    Status       lRetval = kStatus_Success;


    memset(lBuffer, 0, INET6_ADDRSTRLEN);
    memcpy(lBuffer, aBuffer, lLength);

    lRetval = aIPAddress.FromString(lBuffer, lLength);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
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
NetworkControllerBasis :: EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    NetworkModel::EthernetEUI48Type                lEthernetEUI48;
    StateChange::NetworkEthernetEUI48Notification  lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::EthernetEUI48Response::kExpectedMatches, done);

    // Match 2/2: Ethernet Address

    lStatus = Parse(aBuffer + aMatches.at(1).rm_so,
                    Common::Utilities::Distance(aMatches.at(1)),
                    lEthernetEUI48);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the Ethernet EUI-48 address is unchanged, SetEthernetEUI48
    // will return kStatus_ValueAlreadySet and there will be no need
    // to send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetEthernetEUI48(lEthernetEUI48);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lEthernetEUI48);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Ethernet network interface default router IP address changed
 *    client unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface default router IP address changed
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
NetworkControllerBasis :: IPDefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Common::IPAddress                                       lDefaultRouterIPAddress;
    StateChange::NetworkIPDefaultRouterAddressNotification  lStateChangeNotification;
    Status                                                  lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::IPDefaultRouterAddressResponse::kExpectedMatches, done);

    // Match 2/2: IP Address

    lStatus = Parse(aBuffer + aMatches.at(1).rm_so,
                    Common::Utilities::Distance(aMatches.at(1)),
                    lDefaultRouterIPAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the IP address is unchanged, SetDefaultRouterAddress
    // will return kStatus_ValueAlreadySet and there will be no need
    // to send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetDefaultRouterAddress(lDefaultRouterIPAddress);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lDefaultRouterIPAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Ethernet network interface host IP address changed client
 *    unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface host IP address changed
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
NetworkControllerBasis :: IPHostAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Common::IPAddress                              lHostIPAddress;
    StateChange::NetworkIPHostAddressNotification  lStateChangeNotification;
    Status                                         lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::IPHostAddressResponse::kExpectedMatches, done);

    // Match 2/2: IP Address

    lStatus = Parse(aBuffer + aMatches.at(1).rm_so,
                    Common::Utilities::Distance(aMatches.at(1)),
                    lHostIPAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the IP address is unchanged, SetHostAddress
    // will return kStatus_ValueAlreadySet and there will be no need
    // to send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetHostAddress(lHostIPAddress);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lHostIPAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

/**
 *  @brief
 *    Ethernet network interface IP netmask changed client
 *    unsolicited notification handler.
 *
 *  This handles an asynchronous, unsolicited client notification for
 *  the Ethernet network interface IP netmask changed
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
NetworkControllerBasis :: IPNetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Common::IPAddress                          lIPNetmask;
    StateChange::NetworkIPNetmaskNotification  lStateChangeNotification;
    Status                                     lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::IPNetmaskResponse::kExpectedMatches, done);

    // Match 2/2: IP Address

    lStatus = Parse(aBuffer + aMatches.at(1).rm_so,
                    Common::Utilities::Distance(aMatches.at(1)),
                    lIPNetmask);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the IP address is unchanged, SetNetmask
    // will return kStatus_ValueAlreadySet and there will be no need
    // to send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetNetmask(lIPNetmask);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lIPNetmask);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

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
NetworkControllerBasis :: SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    NetworkModel::EnabledType                    lEnabled;
    StateChange::NetworkSDDPEnabledNotification  lStateChangeNotification;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE(aMatches.size() == Command::Network::SDDPEnabledResponse::kExpectedMatches, done);

    // Match 2/2: Enabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    // If the Control4 SDDP enabled state is unchanged, SetSDDPEnabled
    // will return kStatus_ValueAlreadySet and there will be no need
    // to send a state change notification. If we receive
    // kStatus_Success, it is the first time set or a change and state
    // change notification needs to be sent.

    lStatus = mNetworkModel.SetSDDPEnabled(lEnabled);
    nlEXPECT_SUCCESS(lStatus, done);

    lStatus = lStateChangeNotification.Init(lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    OnStateDidChange(lStateChangeNotification);

 done:
    return;
}

// MARK: Unsolicited Notification Handler Trampolines

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
NetworkControllerBasis :: DHCPv4EnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

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
NetworkControllerBasis :: EthernetEUI48NotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->EthernetEUI48NotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Ethernet network interface default router IP address changed
 *    client unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface default router IP
 *  address changed notification.
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
NetworkControllerBasis :: IPDefaultRouterAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->IPDefaultRouterAddressNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Ethernet network interface host IP address changed client
 *    unsolicited notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface host IP address
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
NetworkControllerBasis :: IPHostAddressNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->IPHostAddressNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

/**
 *  @brief
 *    Ethernet network interface IP netmask changed client unsolicited
 *    notification handler trampoline.
 *
 *  This invokes the handler for an unsolicited, asynchronous client
 *  notification for the Ethernet network interface IP netmask changed
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
NetworkControllerBasis :: IPNetmaskNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->IPNetmaskNotificationReceivedHandler(aBuffer, aSize, aMatches);
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
NetworkControllerBasis :: SDDPEnabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkControllerBasis *lController = static_cast<NetworkControllerBasis *>(aContext);

    if (lController != nullptr)
    {
        lController->SDDPEnabledNotificationReceivedHandler(aBuffer, aSize, aMatches);
    }
}

}; // namespace Client

}; // namespace HLX
