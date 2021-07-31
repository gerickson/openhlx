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

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include "CommandManager.hpp"


using namespace HLX::Common;
using namespace HLX::Model;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Request data

Command::Infrared::QueryRequest        InfraredController::kQueryRequest;
Command::Infrared::SetDisabledRequest  InfraredController::kSetDisabledRequest;

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

InfraredController :: InfraredController(void) :
    Simulator::ControllerBasis(),
    mInfraredModel()
{
    return;
}

InfraredController :: ~InfraredController(void)
{
    return;
}

Status InfraredController :: RequestInit(void)
{
    Status lRetval = kStatus_Success;

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetDisabledRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
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
    static const size_t               lRequestHandlerCount = sizeof (lRequestHandlers) / sizeof (lRequestHandlers[0]);
    Status                            lRetval = kStatus_Success;

    lRetval = ControllerBasis::DoRequestHandlers(&lRequestHandlers[0], &lRequestHandlers[lRequestHandlerCount], aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status InfraredController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mInfraredModel.Init();
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

void InfraredController :: QueryHandler(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    InfraredModel::DisabledType              lDisabled;
    Status                                   lStatus;


    lStatus = mInfraredModel.GetDisabled(lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleDisabledResponse(lDisabled, aBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

Status InfraredController :: HandleDisabledResponse(const InfraredModel::DisabledType &aDisabled, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Command::Infrared::DisabledResponse      lDisabledResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lStatus;


    lStatus = lDisabledResponse.Init(aDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lDisabledResponse.GetBuffer();
    lSize = lDisabledResponse.GetSize();

    lStatus = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return (lStatus);
}

// MARK: Configuration Management Methods

void InfraredController :: QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;

    QueryHandler(aBuffer);
}

void InfraredController :: ResetToDefaultConfiguration(void)
{
    Status lStatus;

    lStatus = mInfraredModel.SetDisabled(kInfraredModelDefaults.mDisabled);
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

    lRetval = mInfraredModel.SetDisabled(lDisabled);
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

    lStatus = mInfraredModel.GetDisabled(lDisabled);
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

// MARK: Command Completion Handlers

void InfraredController :: QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aBuffer;
    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Infrared::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    QueryHandler(lResponseBuffer);

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

void InfraredController :: SetDisabledRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    InfraredModel::DisabledType              lDisabled;
    Command::Infrared::DisabledResponse      lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Command::Infrared::SetDisabledRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Disabled

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lDisabled);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mInfraredModel.SetDisabled(lDisabled);
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

void InfraredController :: QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void InfraredController :: SetDisabledRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    InfraredController *lController = static_cast<InfraredController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetDisabledRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Server

}; // namespace HLX
