/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file implements an object for managing the server-side
 *      observation and mutation of a HLX Ethernet network interface.
 *
 */

#include "NetworkController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

// Request data

Server::Command::Network::QueryRequest  NetworkController::kQueryRequest;

// The query network response contains both state and
// configuration settings.

static const char * const kQueryResponseBuffer = \
"(DHCP1)\r\n"
"(IP192.168.1.48)\r\n"
"(NM255.255.255.0)\r\n"
"(GW192.168.1.1)\r\n"
"(MAC00-50-C2-D8-24-71)\r\n"
"(SDDP0)\r\n";

// The query current configuration response contains ONLY
// configuration settings.

static const char * const kQueryCurrentResponseBuffer = \
"(DHCP1)\r\n"
"(IP192.168.1.48)\r\n"
"(NM255.255.255.0)\r\n"
"(GW192.168.1.1)\r\n"
"(SDDP0)\r\n";

/**
 *  @brief
 *    A object for representing default data for a HLX Ethernet
 *    network interface data model.
 *
 *  @private
 *
 */
struct NetworkModelDefaults
{
    NetworkModel::IPAddressType        mHLXAddress;
    NetworkModel::PrefixLengthType     mPrefixLength;
    NetworkModel::IPAddressType        mGatewayAddress;
    NetworkModel::EthernetAddressType  mEthernetAddress;
    NetworkModel::EnabledType          mDHCPv4Enabled;
    NetworkModel::EnabledType          mSDDPEnabled;
};

NetworkController :: NetworkController(void) :
    Simulator::ControllerBasis(),
    mNetworkModel()
{
    return;
}

NetworkController :: ~NetworkController(void)
{
    return;
}

Status NetworkController :: RequestInit(void)
{
    Status lRetval = kStatus_Success;

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status NetworkController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            NetworkController::QueryRequestReceivedHandler
        }
    };
    static const size_t               lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                            lRetval = kStatus_Success;

    lRetval = Server::ControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                         &lRequestHandlers[lRequestHandlerCount],
                                                         this,
                                                         aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status NetworkController :: Init(Server::CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mNetworkModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(lRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

void NetworkController :: QueryHandler(const char *aInputBuffer, Common::ConnectionBuffer::MutableCountedPointer &aOutputBuffer)
{
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lStatus;


    lBuffer = reinterpret_cast<const uint8_t *>(aInputBuffer);
    lSize = strlen(aInputBuffer);

    lStatus = Common::Utilities::Put(*aOutputBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

// MARK: Configuration Management Methods

void NetworkController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;

    QueryHandler(kQueryCurrentResponseBuffer, aBuffer);
}

void NetworkController :: ResetToDefaultConfiguration(void)
{
    return;
}

// MARK: Command Completion Handlers

void NetworkController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
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

    QueryHandler(kQueryResponseBuffer, lResponseBuffer);

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
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Command Request Handler Trampolines

void NetworkController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
