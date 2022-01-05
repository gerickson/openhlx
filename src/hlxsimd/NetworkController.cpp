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

#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    A object for representing default data for a HLX Ethernet
 *    network interface data model.
 *
 *  @note
 *    For the simulator, the only two values that can be set to
 *    reasonable defaults are the DHCPv4 and Control4 SDDP enabled
 *    state settings. The Ethernet EUI-48 address, default router IP
 *    address, host IP address, and IP netmask are all dependent on
 *    the network interface over which a connection to the HLX server
 *    is established and, consequently, are dynamic in nature.
 *
 *  @private
 *
 */
struct NetworkModelDefaults
{
    NetworkModel::IPAddressType        mHostAddress;
    NetworkModel::IPAddressType        mNetmask;
    NetworkModel::IPAddressType        mDefaultRouterAddress;
    NetworkModel::EthernetEUI48Type    mEthernetEUI48;
    NetworkModel::EnabledType          mDHCPv4Enabled;
    NetworkModel::EnabledType          mSDDPEnabled;
};

static const NetworkModel::EnabledType     kSetDHCPv4EnabledDefault = true;
static const NetworkModel::EnabledType     kSetSDDPEnabledDefault   = false;

static const NetworkModelDefaults kNetworkModelDefaults =
{
    { },
    { },
    { },
    { },
    kSetDHCPv4EnabledDefault,
    kSetSDDPEnabledDefault
};

static CFStringRef           kDHCPSchemaKey    = CFSTR("DHCP");
static CFStringRef           kEnabledSchemaKey = CFSTR("Enabled");
static CFStringRef           kIPv4SchemaKey    = CFSTR("IPv4");
static CFStringRef           kNetworkSchemaKey = CFSTR("Network");
static CFStringRef           kSDDPSchemaKey    = CFSTR("SDDP");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
NetworkController :: NetworkController(void) :
    Common::NetworkControllerBasis(),
    Server::NetworkControllerBasis(Common::NetworkControllerBasis::mNetworkModel),
    Simulator::ObjectControllerBasis()
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

Status NetworkController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            NetworkController::QueryRequestReceivedHandler
        },

        {
            kSetDHCPv4EnabledRequest,
            NetworkController::SetDHCPv4EnabledRequestReceivedHandler
        },

        {
            kSetSDDPEnabledRequest,
            NetworkController::SetSDDPEnabledRequestReceivedHandler
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
NetworkController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::NetworkControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::NetworkControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void NetworkController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    static constexpr bool kIsConfiguration = true;

    HandleQueryReceived(kIsConfiguration, aConnection, aBuffer);
}

/**
 *  @brief
 *    Reset the network data model to defaults.
 *
 *  This is a configuration management delegate method that resets the
 *  network data model to default values.
 *
 *  @note
 *    For the simulator, the only two values that can be set to
 *    reasonable defaults are the DHCPv4 and Control4 SDDP enabled
 *    state settings. The Ethernet EUI-48 address, default router IP
 *    address, host IP address, and IP netmask are all dependent on
 *    the network interface over which a connection to the HLX server
 *    is established and, consequently, are dynamic in nature.
 *
 */
void
NetworkController :: ResetToDefaultConfiguration(void)
{
    Status lStatus;

    lStatus = GetModel().SetDHCPv4Enabled(kNetworkModelDefaults.mDHCPv4Enabled);
    nlCHECK_SUCCESS(lStatus);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = GetModel().SetSDDPEnabled(kNetworkModelDefaults.mSDDPEnabled);
    nlCHECK_SUCCESS(lStatus);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }
}

Status NetworkController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    CFDictionaryRef  lNetworkDictionary = nullptr;
    Status           lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

    // Attempt to retrieve the network configuration subdictionary.

    lNetworkDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aBackupDictionary, kNetworkSchemaKey));
    nlREQUIRE_ACTION(lNetworkDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the IPv4 configuration.

    lRetval = IPv4LoadFromBackupConfiguration(lNetworkDictionary, GetModel());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Attempt to retrieve the Control4 SDDP configuration.

    lRetval = SDDPLoadFromBackupConfiguration(lNetworkDictionary, GetModel());
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

void NetworkController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    CFMutableDictionaryRef  lNetworkDictionary = nullptr;
    Status                  lStatus;


    // Create a mutable dictionary to store the disabled value from
    // the model into.

    lNetworkDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                   0,
                                                   &kCFCopyStringDictionaryKeyCallBacks,
                                                   &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE(lNetworkDictionary != nullptr, done);

    // Add the IPv4 schema configuration into the newly-created
    // dictionary.

    lStatus = IPv4SaveToBackupConfiguration(lNetworkDictionary, GetModel());
    nlREQUIRE_SUCCESS(lStatus, done);

    // Add the Control4 SDDP schema configuration into the
    // newly-created dictionary.

    lStatus = SDDPSaveToBackupConfiguration(lNetworkDictionary, GetModel());
    nlREQUIRE_SUCCESS(lStatus, done);

    // Add the newly-created dictionary into the backup configuration
    // dictionary, keyed for this controller.

    CFDictionaryAddValue(aBackupDictionary, kNetworkSchemaKey, lNetworkDictionary);

 done:
    CFURelease(lNetworkDictionary);

    return;
}

// MARK: Backup Configuration Load / Save Handlers

Status
NetworkController :: IPv4LoadFromBackupConfiguration(CFDictionaryRef aNetworkDictionary, Model::NetworkModel &aNetworkModel)
{
    CFDictionaryRef            lIPv4Dictionary = nullptr;
    Status                     lRetval = kStatus_Success;

    // Attempt to retrieve the IPv4 configuration subdictionary.

    lIPv4Dictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aNetworkDictionary, kIPv4SchemaKey));
    nlREQUIRE_ACTION(lIPv4Dictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the DHCPv4 schema configuration

    lRetval = DHCPv4LoadFromBackupConfiguration(lIPv4Dictionary, aNetworkModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
NetworkController :: DHCPv4LoadFromBackupConfiguration(CFDictionaryRef aIPDictionary, Model::NetworkModel &aNetworkModel)
{
    CFDictionaryRef            lDHCPv4Dictionary = nullptr;
    bool                       lStatus;
    NetworkModel::EnabledType  lDHCPv4Enabled;
    Status                     lRetval = kStatus_Success;

    // Attempt to retrieve the DHCPv4 configuration subdictionary.

    lDHCPv4Dictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aIPDictionary, kDHCPSchemaKey));
    nlREQUIRE_ACTION(lDHCPv4Dictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the enabled property configuration

    lStatus = CFUDictionaryGetBoolean(lDHCPv4Dictionary, kEnabledSchemaKey, lDHCPv4Enabled);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // DHCPv4 Enabled

    lRetval = aNetworkModel.SetDHCPv4Enabled(lDHCPv4Enabled);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
NetworkController :: SDDPLoadFromBackupConfiguration(CFDictionaryRef aNetworkDictionary, Model::NetworkModel &aNetworkModel)
{
    CFDictionaryRef            lSDDPDictionary = nullptr;
    bool                       lStatus;
    NetworkModel::EnabledType  lSDDPEnabled;
    Status                     lRetval = kStatus_Success;

    // Attempt to retrieve the Control4 SDDP configuration subdictionary.

    lSDDPDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aNetworkDictionary, kSDDPSchemaKey));
    nlREQUIRE_ACTION(lSDDPDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the enabled property configuration

    lStatus = CFUDictionaryGetBoolean(lSDDPDictionary, kEnabledSchemaKey, lSDDPEnabled);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // SDDP Enabled

    lRetval = aNetworkModel.SetSDDPEnabled(lSDDPEnabled);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
NetworkController :: IPv4SaveToBackupConfiguration(CFMutableDictionaryRef aNetworkDictionary, const Model::NetworkModel &aNetworkModel)
{
    CFMutableDictionaryRef     lIPv4Dictionary;
    Status                     lRetval = kStatus_Success;


    lIPv4Dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                0,
                                                &kCFCopyStringDictionaryKeyCallBacks,
                                                &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lIPv4Dictionary != nullptr, done, lRetval = -ENOMEM);

    // Add the DHCPv4 schema configuration to the newly-created IPv4
    // dictionary.

    lRetval = DHCPv4SaveToBackupConfiguration(lIPv4Dictionary, aNetworkModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the newly-created IPv4 dictionary to the IP dictionary.

    CFDictionaryAddValue(aNetworkDictionary, kIPv4SchemaKey, lIPv4Dictionary);

 done:
    CFURelease(lIPv4Dictionary);

    return (lRetval);
}

Status
NetworkController :: DHCPv4SaveToBackupConfiguration(CFMutableDictionaryRef aIPDictionary, const Model::NetworkModel &aNetworkModel)
{
    CFMutableDictionaryRef     lDHCPv4Dictionary;
    NetworkModel::EnabledType  lDHCPv4Enabled;
    Status                     lRetval = kStatus_Success;


    lDHCPv4Dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                0,
                                                &kCFCopyStringDictionaryKeyCallBacks,
                                                &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lDHCPv4Dictionary != nullptr, done, lRetval = -ENOMEM);

    // Attempt to get the DHCPv4 enabled value from the model.

    lRetval = aNetworkModel.GetDHCPv4Enabled(lDHCPv4Enabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the enabled state to the newly-created DHCPv4 dictionary.

    CFUDictionarySetBoolean(lDHCPv4Dictionary, kEnabledSchemaKey, lDHCPv4Enabled);

    // Add the newly-created DHCPv4 dictionary to the IP dictionary.

    CFDictionaryAddValue(aIPDictionary, kDHCPSchemaKey, lDHCPv4Dictionary);

 done:
    CFURelease(lDHCPv4Dictionary);

    return (lRetval);
}

Status
NetworkController :: SDDPSaveToBackupConfiguration(CFMutableDictionaryRef aNetworkDictionary, const Model::NetworkModel &aNetworkModel)
{
    CFMutableDictionaryRef     lSDDPDictionary;
    NetworkModel::EnabledType  lSDDPEnabled;
    Status                     lRetval = kStatus_Success;


    lSDDPDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                0,
                                                &kCFCopyStringDictionaryKeyCallBacks,
                                                &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lSDDPDictionary != nullptr, done, lRetval = -ENOMEM);

    // Attempt to get the Control4 SDDP enabled value from the model.

    lRetval = aNetworkModel.GetSDDPEnabled(lSDDPEnabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the enabled state to the newly-created Control4 SDDP dictionary.

    CFUDictionarySetBoolean(lSDDPDictionary, kEnabledSchemaKey, lSDDPEnabled);

    // Add the newly-created Control4 SDDP dictionary to the network
    // dictionary.

    CFDictionaryAddValue(aNetworkDictionary, kSDDPSchemaKey, lSDDPDictionary);

 done:
    CFURelease(lSDDPDictionary);

    return (lRetval);
}

// MARK: Command Request Completion Handlers

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

    // First, put the solicited notifications portion, including both
    // the connection-dependent and -independent schema content.

    lStatus = HandleQueryReceived(!kIsConfiguration, aConnection, lResponseBuffer);
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
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void
NetworkController :: SetDHCPv4EnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    NetworkModel::EnabledType                        lEnabled;
    Server::Command::Network::DHCPv4EnabledResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer          lResponseBuffer;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Network::SetDHCPv4EnabledRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Enabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetDHCPv4Enabled(lEnabled);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleDHCPv4EnabledResponse(lEnabled, lResponseBuffer);
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

void
NetworkController :: SetSDDPEnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    NetworkModel::EnabledType                        lEnabled;
    Server::Command::Network::SDDPEnabledResponse    lResponse;
    ConnectionBuffer::MutableCountedPointer          lResponseBuffer;
    Status                                           lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Network::SetSDDPEnabledRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Enabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lEnabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetSDDPEnabled(lEnabled);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleSDDPEnabledResponse(lEnabled, lResponseBuffer);
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

void
NetworkController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void
NetworkController :: SetDHCPv4EnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetDHCPv4EnabledRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void
NetworkController :: SetSDDPEnabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    NetworkController *lController = static_cast<NetworkController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSDDPEnabledRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

Common::Status
NetworkController :: HandleQueryReceived(const bool &aIsConfiguration,
                                         Server::ConnectionBasis &aConnection,
                                         Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    NetworkModel::EthernetEUI48Type  lEthernetEUI48;
    IPAddress                        lDefaultRouterAddress;
    IPAddress                        lHostAddress;
    IPAddress                        lNetmask;
    Status                           lRetval = kStatus_Success;


    // Allow the server network controller basis to handle the common,
    // connection-independent query schema.

    lRetval = Server::NetworkControllerBasis::HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Handle the connection-dependent schema.
    //
    // Only get the Ethernet EUI-48 if the query is NOT in a configuration
    // retrieval context.

    if (!aIsConfiguration)
    {
        lRetval = aConnection.GetConfiguration(lEthernetEUI48,
                                               lHostAddress,
                                               lNetmask,
                                               lDefaultRouterAddress);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else
    {
        lRetval = aConnection.GetConfiguration(lHostAddress,
                                               lNetmask,
                                               lDefaultRouterAddress);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    lRetval = HandleHostAddressResponse(lHostAddress, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleNetmaskResponse(lNetmask, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleDefaultRouterAddressResponse(lDefaultRouterAddress, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (!aIsConfiguration)
    {
        lRetval = HandleEthernetEUI48Response(lEthernetEUI48, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

}; // namespace Simulator

}; // namespace HLX
