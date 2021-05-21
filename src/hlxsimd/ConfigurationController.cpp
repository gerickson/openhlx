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
 *      This file...
 *
 */

#include "ConfigurationController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <CoreFoundation/CFDictionary.h>

#include <CFUtilities/CFUtilities.h>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <CommandManager.hpp>
#include <ConfigurationControllerDelegate.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Server
{

// Request data

Command::Configuration::LoadFromBackupRequest   ConfigurationController::kLoadFromBackupRequest;
Command::Configuration::QueryCurrentRequest     ConfigurationController::kQueryCurrentRequest;
Command::Configuration::ResetToDefaultsRequest  ConfigurationController::kResetToDefaultsRequest;
Command::Configuration::SaveToBackupRequest     ConfigurationController::kSaveToBackupRequest;

ConfigurationController :: ConfigurationController(void) :
    ControllerBasis(),
    mDelegate(nullptr)
{
    return;
}

ConfigurationController :: ~ConfigurationController(void)
{
    return;
}

Status ConfigurationController :: RequestInit(void)
{
    Status lRetval = kStatus_Success;

    lRetval = kLoadFromBackupRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kQueryCurrentRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kResetToDefaultsRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSaveToBackupRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ConfigurationController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kLoadFromBackupRequest,
            ConfigurationController::LoadFromBackupRequestReceivedHandler
        },

        {
            kQueryCurrentRequest,
            ConfigurationController::QueryCurrentRequestReceivedHandler
        },

        {
            kResetToDefaultsRequest,
            ConfigurationController::ResetToDefaultsRequestReceivedHandler
        },

        {
            kSaveToBackupRequest,
            ConfigurationController::SaveToBackupRequestReceivedHandler
        }
    };
    static const size_t               lRequestHandlerCount = sizeof (lRequestHandlers) / sizeof (lRequestHandlers[0]);
    Status                            lRetval = kStatus_Success;

    lRetval = ControllerBasis::DoRequestHandlers(&lRequestHandlers[0], &lRequestHandlers[lRequestHandlerCount], aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ConfigurationController :: Init(CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(lRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

ConfigurationControllerDelegate * ConfigurationController :: GetDelegate(void) const
{
    return (mDelegate);
}

Status ConfigurationController :: SetDelegate(ConfigurationControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

// MARK: Configuration Management

Status ConfigurationController :: LoadFromBackup(void)
{
    DeclareScopedFunctionTracer(lTracer);
    CFDictionaryRef lBackupDictionary = nullptr;
    Status          lRetval           = kStatus_Success;

    // First, invoke the delegate such that the serialized
    // configuration can be loaded in from storage. Due to first-time
    // start-up cases, this is expected and likely to fail.

    lRetval = OnLoadFromBackupConfigurationStorage(lBackupDictionary);
    nlEXPECT(lRetval >= kStatus_Success, done);

    // Next, invoke the delegate for fanout such that other
    // participants can extract their configuraton settings or state
    // from the dictionary.

    lRetval = OnLoadFromBackupConfiguration(lBackupDictionary);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    CFURelease(lBackupDictionary);

    return (lRetval);
}

Status ConfigurationController :: ResetToDefaults(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval = kStatus_Success;

    OnResetToDefaultConfiguration();

    return (lRetval);
}

Status ConfigurationController :: SaveToBackup(void)
{
    CFMutableDictionaryRef  lBackupDictionary = nullptr;
    Status                  lRetval = kStatus_Success;


    // Create a mutable dictionary to serialize backup configuration
    // into.

    lBackupDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                  0,
                                                  &kCFTypeDictionaryKeyCallBacks,
                                                  &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lBackupDictionary != nullptr, done, lRetval = -ENOMEM);

    // Next, invoke the delegate for fanout such that other
    // participants can inject their configuraton settings or state to
    // the dictionary.

    OnSaveToBackupConfiguration(lBackupDictionary);

    // Finally, invoke the delegate such that the serialized
    // configuration can be saved out to storage.

    lRetval = OnSaveToBackupConfigurationStorage(lBackupDictionary);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    CFURelease(lBackupDictionary);

    return (lRetval);
}

// MARK: Command Completion Handlers

void ConfigurationController :: LoadFromBackupRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    DeclareScopedFunctionTracer(lTracer);
    Command::Configuration::LoadFromBackupResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer         lResponseBuffer;
    const uint8_t *                                 lBuffer;
    size_t                                          lSize;
    Status                                          lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Configuration::LoadFromBackupRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lStatus = LoadFromBackup();
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Finally, either send the success or error confirmation back to
    // the client.

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

void ConfigurationController :: QueryCurrentRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Command::Configuration::QueryCurrentResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer       lResponseBuffer;
    Status                                        lStatus;
    const uint8_t *                               lBuffer;
    size_t                                        lSize;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Configuration::QueryCurrentRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // First, allocate and initialize the response buffer.

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Next, invoke the delegate for fanout such that other
    // participants can insert their settings or state to the
    // representation.

    OnQueryCurrentConfiguration(aConnection, lResponseBuffer);

    // Finally, either send the success or error confirmation back to
    // the client.

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

void ConfigurationController :: ResetToDefaultsRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Command::Configuration::ResetToDefaultsResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer          lResponseBuffer;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Configuration::ResetToDefaultsRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Invoke the delegate for fanout such that other participants can
    // reset their settings or state to their default values.

    OnResetToDefaultConfiguration();

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Finally, either send the success or error confirmation back to
    // the client.

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

void ConfigurationController :: SaveToBackupRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Command::Configuration::SavingToBackupResponse  lSavingToBackupNotification;
    Command::Configuration::SaveToBackupResponse    lSaveToBackupResponse;
    ConnectionBuffer::MutableCountedPointer         lResponseBuffer;
    const uint8_t *                                 lBuffer;
    size_t                                          lSize;
    Status                                          lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Configuration::SaveToBackupRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // There is a two-phase response for the save to back up command
    // request. First, the saving to backup "will save" notification
    // is sent before the serialization occurs. Then, after
    // serialization, the actual save to backup "did save" command
    // repsonse "bookend" is sent.

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, send the "will save" notification.

    lStatus = lSavingToBackupNotification.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSavingToBackupNotification.GetBuffer();
    lSize = lSavingToBackupNotification.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SendResponse(aConnection, lResponseBuffer);
    nlVERIFY_SUCCESS(lStatus);

    // First, invoke the delegate for fanout such that other
    // participants can insert their settings or state to the
    // representation.

    lStatus = SaveToBackup();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Finally, either send the "did save" command response success
    // "booked" back to the client, flushing the response buffer first
    // of the prior "will save" notification.

    lResponseBuffer->Flush();

    lStatus = lSaveToBackupResponse.Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSaveToBackupResponse.GetBuffer();
    lSize = lSaveToBackupResponse.GetSize();

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

// MARK: Delegate Fanout Methods

Status ConfigurationController :: OnLoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->LoadFromBackupConfiguration(*this, aBackupDictionary);
        nlREQUIRE(lRetval >= kStatus_Success, done);
    }

 done:
    return (lRetval);
}

Status ConfigurationController :: OnLoadFromBackupConfigurationStorage(CFDictionaryRef &aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        // Invoke the delegate to load the backup configuration. It is
        // entirely possible that the storage will be empty due to
        // first-time start-up, so expect that failure is likely to
        // occur here.

        lRetval = mDelegate->LoadFromBackupConfigurationStorage(*this, aBackupDictionary);
        nlEXPECT(lRetval >= kStatus_Success, done);
    }

 done:
    return (lRetval);
}

void ConfigurationController :: OnQueryCurrentConfiguration(ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    if (mDelegate != nullptr)
    {
        mDelegate->QueryCurrentConfiguration(*this, aConnection, aBuffer);
    }
}

void ConfigurationController :: OnResetToDefaultConfiguration(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ResetToDefaultConfiguration(*this);
    }
}

void ConfigurationController :: OnSaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    if (mDelegate != nullptr)
    {
        mDelegate->SaveToBackupConfiguration(*this, aBackupDictionary);
    }
}

Status ConfigurationController :: OnSaveToBackupConfigurationStorage(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    if (mDelegate != nullptr)
    {
        lRetval = mDelegate->SaveToBackupConfigurationStorage(*this, aBackupDictionary);
    }

    return (lRetval);
}

// MARK: Command Request Handler Trampolines

void ConfigurationController :: LoadFromBackupRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->LoadFromBackupRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: QueryCurrentRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryCurrentRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: ResetToDefaultsRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->ResetToDefaultsRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ConfigurationController :: SaveToBackupRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ConfigurationController *lController = static_cast<ConfigurationController *>(aContext);

    if (lController != nullptr)
    {
        lController->SaveToBackupRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Server

}; // namespace HLX
