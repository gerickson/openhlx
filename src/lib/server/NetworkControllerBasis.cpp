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
 *      Ethernet network interface controller, in a server.
 *
 */

#include "NetworkControllerBasis.hpp"

#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Class-scoped Command Request Regular Expression Data

/**
 *  Class-scoped server network interface query command request regular
 *  expression.
 *
 */
Server::Command::Network::QueryRequest             NetworkControllerBasis::kQueryRequest;

/**
 *  Class-scoped server Ethetner network interface DHCPv4 set enabled state command request
 *  regular expression.
 *
 */
Server::Command::Network::SetDHCPv4EnabledRequest  NetworkControllerBasis::kSetDHCPv4EnabledRequest;

/**
 *  Class-scoped server Ethetner network interface Control4 SDDP set enabled state command request
 *  regular expression.
 *
 */
Server::Command::Network::SetSDDPEnabledRequest    NetworkControllerBasis::kSetSDDPEnabledRequest;

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
    Server::ObjectControllerBasis(),
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
 *  This initializes the class with the specified command manager.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
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
NetworkControllerBasis :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ObjectControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Implementation

Status
NetworkControllerBasis :: RequestInit(void)
{
    Status lRetval = kStatus_Success;


    // Initialize static command request regular expression pattern data.

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetDHCPv4EnabledRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetSDDPEnabledRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

/**
 *  @brief
 *    Handle and generate the server command response for a network
 *    interface query request.
 *
 *  This handles and generates the server command response for an
 *  network interface query request.
 *
 *  @param[in]      aIsConfiguration  An immutable reference to a Boolean
 *                                    indicating whether the query
 *                                    request is coming from a query
 *                                    current configuration (true) or
 *                                    a network query (false) request.

 *  @param[in]      aInputBuffer      A pointer to a null-terminated
 *                                    C string containing the content to
 *                                    place into the response buffer.
 *  @param[in,out]  aOutputBuffer     A mutable reference to the shared
 *                                    pointer into which the response is
 *                                    to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the network interface model has
 *                                  not been completely and successfully
 *                                  initialized.
 *
 */
Status
NetworkControllerBasis :: HandleQueryReceived(const bool &aIsConfiguration,
                                              const char *aInputBuffer,
                                              Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer) const
{
    NetworkModel::EnabledType        lDHCPv4Enabled;
    NetworkModel::EthernetEUI48Type  lEthernetEUI48;
    NetworkModel::EnabledType        lSDDPEnabled;
    const uint8_t *                  lBuffer;
    size_t                           lSize;
    Status                           lRetval;


    // Handle any data model-sourced response content.

    lRetval = mNetworkModel.GetDHCPv4Enabled(lDHCPv4Enabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleDHCPv4EnabledResponse(lDHCPv4Enabled, aOutputBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (!aIsConfiguration)
    {
        lRetval = mNetworkModel.GetEthernetEUI48(lEthernetEUI48);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = HandleEthernetEUI48Response(lEthernetEUI48, aOutputBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    lRetval = mNetworkModel.GetSDDPEnabled(lSDDPEnabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleSDDPEnabledResponse(lSDDPEnabled, aOutputBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Handle any remaining precanned response content.

    lBuffer = reinterpret_cast<const uint8_t *>(aInputBuffer);
    lSize = strlen(aInputBuffer);

    lRetval = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface DHCPv4 enabled state request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface DHCPv4 enabled state request.
 *
 *  @param[in]      aEnabled   An immutable reference to the Ethernet
 *                             network interface DHCPv4 enabled state
 *                             for which the response is to be formed.
 *  @param[in,out]  aBuffer    A mutable reference to the shared
 *                             pointer into which the response is to
 *                             be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleDHCPv4EnabledResponse(const NetworkModel::EnabledType &aEnabled,
                                                      ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::DHCPv4EnabledResponse  lDHCPv4EnabledResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lDHCPv4EnabledResponse.Init(aEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lDHCPv4EnabledResponse.GetBuffer();
    lSize = lDHCPv4EnabledResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface default router IP address request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface default router IP address request.
 *
 *  @param[in]      aDefaultRouterAddress  An immutable reference to
 *                                         the Ethernet network
 *                                         interface default router IP
 *                                         address for which the
 *                                         response is to be formed.
 *  @param[in,out]  aBuffer                A mutable reference to the
 *                                         shared pointer into which
 *                                         the response is to be
 *                                         generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleDefaultRouterAddressResponse(const IPAddress &aDefaultRouterAddress,
                                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::IPDefaultRouterAddressResponse  lIPDefaultRouterAddressResponse;
    const uint8_t *                                           lBuffer;
    size_t                                                    lSize;
    Status                                                    lStatus;


    lStatus = lIPDefaultRouterAddressResponse.Init(aDefaultRouterAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lIPDefaultRouterAddressResponse.GetBuffer();
    lSize = lIPDefaultRouterAddressResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface EUI-48 request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface EUI-48 request.
 *
 *  @param[in]      aEthernetEUI48   An immutable reference to the
 *                                   Ethernet network interface EUI-48
 *                                   for which the response is to be
 *                                   formed.
 *  @param[in,out]  aBuffer          A mutable reference to the shared
 *                                   pointer into which the response is
 *                                   to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleEthernetEUI48Response(const NetworkModel::EthernetEUI48Type &aEthernetEUI48, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::EthernetEUI48Response  lEthernetEUI48Response;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lEthernetEUI48Response.Init(aEthernetEUI48);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lEthernetEUI48Response.GetBuffer();
    lSize = lEthernetEUI48Response.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface host IP address request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface host IP address request.
 *
 *  @param[in]      aHostAddress  An immutable reference to the
 *                                Ethernet network interface host IP
 *                                address for which the response is to
 *                                be formed.
 *  @param[in,out]  aBuffer       A mutable reference to the shared
 *                                pointer into which the response is
 *                                to be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleHostAddressResponse(const IPAddress &aHostAddress,
                                                    Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::IPHostAddressResponse  lIPHostAddressResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lIPHostAddressResponse.Init(aHostAddress);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lIPHostAddressResponse.GetBuffer();
    lSize = lIPHostAddressResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface IP netmask request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface IP netmask request.
 *
 *  @param[in]      aNetmask  An immutable reference to the Ethernet
 *                            network interface default router IP
 *                            address for which the response is to be
 *                            formed.
 *  @param[in,out]  aBuffer   A mutable reference to the shared
 *                            pointer into which the response is to be
 *                            generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleNetmaskResponse(const IPAddress &aNetmask,
                                                Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::IPNetmaskResponse  lIPNetmaskResponse;
    const uint8_t *                              lBuffer;
    size_t                                       lSize;
    Status                                       lStatus;


    lStatus = lIPNetmaskResponse.Init(aNetmask);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lIPNetmaskResponse.GetBuffer();
    lSize = lIPNetmaskResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

/**
 *  @brief
 *    Handle and generate the server command response for an Ethernet
 *    network interface Control4 SDDP enabled state request.
 *
 *  This handles and generates the server command response for an
 *  Ethernet network interface Control4 SDDP enabled state request.
 *
 *  @param[in]      aEnabled   An immutable reference to the Ethernet
 *                             network interface Control4 SDDP enabled
 *                             state for which the response is to be
 *                             formed.
 *  @param[in,out]  aBuffer    A mutable reference to the shared
 *                             pointer into which the response is to
 *                             be generated.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
/* static */ Status
NetworkControllerBasis :: HandleSDDPEnabledResponse(const NetworkModel::EnabledType &aEnabled, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::Network::SDDPEnabledResponse    lSDDPEnabledResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    lStatus = lSDDPEnabledResponse.Init(aEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSDDPEnabledResponse.GetBuffer();
    lSize = lSDDPEnabledResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

}; // namespace Server

}; // namespace HLX
