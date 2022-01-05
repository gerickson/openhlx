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
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 */

#include "InfraredController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

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
 *    A object for representing default data for a HLX infrared remote
 *    control data model.
 *
 *  @private
 *
 */
struct InfraredModelDefaults
{
    InfraredModel::DisabledType     mDisabled;
};

static const InfraredModel::DisabledType     kDisabledDefault = false;

static const InfraredModelDefaults kInfraredModelDefaults =
{
    kDisabledDefault
};

static CFStringRef           kInfraredSchemaKey = CFSTR("Infrared");
static CFStringRef           kDisabledSchemaKey = CFSTR("Disabled");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
InfraredController :: InfraredController(void) :
    Common::InfraredControllerBasis(),
    Server::InfraredControllerBasis(Common::InfraredControllerBasis::mInfraredModel),
    Simulator::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
InfraredController :: ~InfraredController(void)
{
    return;
}

Status InfraredController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            InfraredController::QueryRequestReceivedHandler
        },

        {
            kSetDisabledRequest,
            InfraredController::SetDisabledRequestReceivedHandler
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
InfraredController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::InfraredControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::InfraredControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void InfraredController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;

    HandleQueryReceived(aBuffer);
}

void InfraredController :: ResetToDefaultConfiguration(void)
{
    Status lStatus;

    lStatus = GetModel().SetDisabled(kInfraredModelDefaults.mDisabled);
    nlCHECK_SUCCESS(lStatus);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }
}

Status InfraredController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    CFDictionaryRef                  lInfraredDictionary = nullptr;
    InfraredModel::DisabledType      lDisabled;
    bool                             lStatus;
    Status                           lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

    // Attempt to retrieve the infrared configuration subdictionary.

    lInfraredDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aBackupDictionary, kInfraredSchemaKey));
    nlREQUIRE_ACTION(lInfraredDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the disabled configuration.

    lStatus = CFUDictionaryGetBoolean(lInfraredDictionary, kDisabledSchemaKey, lDisabled);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Attempt to set the disabled configuration.

    lRetval = GetModel().SetDisabled(lDisabled);
    nlCHECK_SUCCESS(lRetval);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

void InfraredController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    InfraredModel::DisabledType  lDisabled;
    CFMutableDictionaryRef       lInfraredDictionary = nullptr;
    Status                       lStatus;


    // Attempt to get the disabled value from the model.

    lStatus = GetModel().GetDisabled(lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Create a mutable dictionary to store the disabled value from
    // the model into.

    lInfraredDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                    0,
                                                    &kCFCopyStringDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE(lInfraredDictionary != nullptr, done);

    // Add the model disabled value into the newly-created dictionary.

    CFUDictionarySetBoolean(lInfraredDictionary, kDisabledSchemaKey, lDisabled);

    // Add the newly-created dictionary into the backup configuration dictionary, keyed for this controller.

    CFDictionaryAddValue(aBackupDictionary, kInfraredSchemaKey, lInfraredDictionary);

 done:
    CFURelease(lInfraredDictionary);

    return;
}

// MARK: Command Request Completion Handlers

void InfraredController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Infrared::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    HandleQueryReceived(lResponseBuffer);

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

void InfraredController :: SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    InfraredModel::DisabledType                  lDisabled;
    Server::Command::Infrared::DisabledResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Infrared::SetDisabledRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Disabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetDisabled(lDisabled);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleDisabledResponse(lDisabled, lResponseBuffer);
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

void InfraredController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void InfraredController :: SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetDisabledRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
