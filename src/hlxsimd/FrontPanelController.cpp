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
 *      observation and mutation of a HLX physical front panel.
 *
 */

#include "FrontPanelController.hpp"

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
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    A object for representing default data for a HLX physical front
 *    panel data model.
 *
 *  @private
 *
 */
struct FrontPanelModelDefaults
{
    FrontPanelModel::BrightnessType mBrightness;
    FrontPanelModel::LockedType     mLocked;
};

static const FrontPanelModel::BrightnessType kBrightnessDefault = 2;
static const FrontPanelModel::LockedType     kLockedDefault = false;

static const FrontPanelModelDefaults kFrontPanelModelDefaults =
{
    kBrightnessDefault,
    kLockedDefault
};

static CFStringRef               kFrontPanelSchemaKey = CFSTR("Front Panel");
static CFStringRef               kBrightnessSchemaKey = CFSTR("Brightness");
static CFStringRef               kLockedSchemaKey = CFSTR("Locked");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FrontPanelController :: FrontPanelController(void) :
    Common::FrontPanelControllerBasis(),
    Server::FrontPanelControllerBasis(Common::FrontPanelControllerBasis::mFrontPanelModel),
    Simulator::ObjectControllerBasis()
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
FrontPanelController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::FrontPanelControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::FrontPanelControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void FrontPanelController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;

    HandleQueryReceived(aBuffer);
}

void FrontPanelController :: ResetToDefaultConfiguration(void)
{
    Status lStatus;

    lStatus = GetModel().SetBrightness(kFrontPanelModelDefaults.mBrightness);
    nlCHECK_SUCCESS(lStatus);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = GetModel().SetLocked(kFrontPanelModelDefaults.mLocked);
    nlCHECK_SUCCESS(lStatus);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }
}

Status FrontPanelController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    CFDictionaryRef                  lFrontPanelDictionary = nullptr;
    CFNumberRef                      lBrightnessRef = nullptr;
    FrontPanelModel::BrightnessType  lBrightness;
    FrontPanelModel::LockedType      lLocked;
    bool                             lStatus;
    Status                           lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

    // Attempt to retrieve the front panel configuration subdictionary.

    lFrontPanelDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aBackupDictionary, kFrontPanelSchemaKey));
    nlREQUIRE_ACTION(lFrontPanelDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the brightness configuration.

    lBrightnessRef = static_cast<CFNumberRef>(CFDictionaryGetValue(lFrontPanelDictionary, kBrightnessSchemaKey));
    nlREQUIRE_ACTION(lBrightnessRef != nullptr, done, lRetval = kError_MissingConfiguration);

    lStatus = CFUNumberGetValue(lBrightnessRef, lBrightness);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_InvalidConfiguration);

    // Attempt to retrieve the locked configuration.

    lStatus = CFUDictionaryGetBoolean(lFrontPanelDictionary, kLockedSchemaKey, lLocked);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Attempt to set the brightness and locked configuration.

    lRetval = GetModel().SetBrightness(lBrightness);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = GetModel().SetLocked(lLocked);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

void FrontPanelController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    FrontPanelModel::BrightnessType  lBrightness;
    FrontPanelModel::LockedType      lLocked;
    CFMutableDictionaryRef           lFrontPanelDictionary = nullptr;
    Status                           lStatus;


    // Attempt to get the brightness value from the model.

    lStatus = GetModel().GetBrightness(lBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to get the locked value from the model.

    lStatus = GetModel().GetLocked(lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Create a mutable dictionary to store the values from the model
    // into.

    lFrontPanelDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                      0,
                                                      &kCFCopyStringDictionaryKeyCallBacks,
                                                      &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE(lFrontPanelDictionary != nullptr, done);

    // Add the model brightness and locked values into the
    // newly-created dictionary.

    CFUDictionarySetNumber(lFrontPanelDictionary, kBrightnessSchemaKey, lBrightness);
    CFUDictionarySetBoolean(lFrontPanelDictionary, kLockedSchemaKey, lLocked);

    // Add the newly-created dictionary into the backup configuration dictionary, keyed for this controller.

    CFDictionaryAddValue(aBackupDictionary, kFrontPanelSchemaKey, lFrontPanelDictionary);

 done:
    CFURelease(lFrontPanelDictionary);

    return;
}

// MARK: Command Request Completion Handlers

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
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void FrontPanelController :: SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::BrightnessType          lBrightness;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::SetBrightnessRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Brightness

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lBrightness);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetBrightness(lBrightness);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleBrightnessResponse(lBrightness, lResponseBuffer);
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

void FrontPanelController :: SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    FrontPanelModel::LockedType                  lLocked;
    Server::Command::FrontPanel::LockedResponse  lLockedResponse;
    ConnectionBuffer::MutableCountedPointer      lResponseBuffer;
    Status                                       lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::FrontPanel::SetLockedRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Locked

    lStatus = Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                               Common::Utilities::Distance(aMatches.at(1)),
                               lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = GetModel().SetLocked(lLocked);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleLockedResponse(lLocked, lResponseBuffer);
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

}; // namespace Simulator

}; // namespace HLX
