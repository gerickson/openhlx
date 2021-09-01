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
 *      This file implements an object for....
 *
 */

#include "SourcesController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <NameModelDefaults.hpp>
#include <Utilities.hpp>


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
 *    A object for representing default data for a HLX source data
 *    model.
 *
 *  @private
 *
 */
struct SourceModelDefaults
{
    Defaults::NameModel mName;
};

static const SourceModelDefaults kSourceModelDefaults[8] = {
    { "Source Name 1" },
    { "Source Name 2" },
    { "Source Name 3" },
    { "Source Name 4" },
    { "Source Name 5" },
    { "Source Name 6" },
    { "Source Name 7" },
    { "Source Name 8" }
};

static CFStringRef      kSourcesSchemaKey = CFSTR("Sources");
static CFStringRef      kNameSchemaKey = CFSTR("Name");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
SourcesController :: SourcesController(void) :
    Common::SourcesControllerBasis(),
    Server::SourcesControllerBasis(Common::SourcesControllerBasis::mSources,
                                   Common::SourcesControllerBasis::kSourcesMax),
    Simulator::ContainerControllerBasis(),
    Simulator::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
SourcesController :: ~SourcesController(void)
{
    return;
}

Status
SourcesController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kSetNameRequest,
            SourcesController::SetNameRequestReceivedHandler
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
SourcesController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::SourcesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::SourcesControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void
SourcesController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lStatus = kStatus_Success;


    (void)aConnection;

    lStatus = HandleQueryReceived(aBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

done:
    return;
}

void SourcesController :: ResetToDefaultConfiguration(void)
{
    IdentifierType  lSourceIdentifier;
    SourceModel *   lSourceModel;
    Status          lStatus;


    for (lSourceIdentifier = IdentifierModel::kIdentifierMin; lSourceIdentifier <= kSourcesMax; lSourceIdentifier++)
    {
        lStatus = mSources.GetSource(lSourceIdentifier, lSourceModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lSourceModel->SetName(kSourceModelDefaults[lSourceIdentifier - 1].mName.mName);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return;
}

Status SourcesController :: ElementLoadFromBackupConfiguration(CFDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier)
{
    CFDictionaryRef                  lSourceDictionary = nullptr;
    CFStringRef                      lSourceIdentifierKey = nullptr;
    CFStringRef                      lSourceName = nullptr;
    SourceModel *                    lSourceModel;
    Status                           lRetval = kStatus_Success;

    // Attempt to form the source identifier key.

    lSourceIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aSourceIdentifier);
    nlREQUIRE_ACTION(lSourceIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    // Attempt to retrieve the source dictionary.

    lSourceDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aSourcesDictionary, lSourceIdentifierKey));
    nlREQUIRE_ACTION(lSourceDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the source name string.

    lSourceName = static_cast<CFStringRef>(CFDictionaryGetValue(lSourceDictionary, kNameSchemaKey));
    nlREQUIRE_ACTION(lSourceName != nullptr, done, lRetval = kError_MissingConfiguration);

    lRetval = mSources.GetSource(aSourceIdentifier, lSourceModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lSourceModel->SetName(CFString(lSourceName).GetCString());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    CFURelease(lSourceIdentifierKey);

    return (lRetval);
}

Status SourcesController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    ContainerControllerBasis::LoadFromBackupConfiguration(aBackupDictionary,
                                                          kSourcesMax,
                                                          kSourcesSchemaKey);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status SourcesController :: ElementSaveToBackupConfiguration(CFMutableDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier) const
{
    CFStringRef                   lSourceIdentifierKey = nullptr;
    CFMutableDictionaryRef        lSourceDictionary = nullptr;
    const SourceModel *           lSourceModel;
    const char *                  lName;
    Status                        lRetval = kStatus_Success;


    lRetval = mSources.GetSource(aSourceIdentifier, lSourceModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lSourceIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aSourceIdentifier);
    nlREQUIRE_ACTION(lSourceIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    lSourceDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                  0,
                                                  &kCFCopyStringDictionaryKeyCallBacks,
                                                  &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lSourceDictionary != nullptr, done, lRetval = -ENOMEM);

    // Name

    lRetval = lSourceModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetCString(lSourceDictionary, kNameSchemaKey, lName);

    // Add the newly-created source dictionary into the backup
    // configuration dictionary, keyed for this source identifier.

    CFDictionaryAddValue(aSourcesDictionary, lSourceIdentifierKey, lSourceDictionary);

 done:
    CFURelease(lSourceIdentifierKey);
    CFURelease(lSourceDictionary);

    return (lRetval);
}

void SourcesController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    ContainerControllerBasis::SaveToBackupConfiguration(aBackupDictionary,
                                                        kSourcesMax,
                                                        kSourcesSchemaKey);
}

// MARK: Command Completion Handlers

void SourcesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                           lSourceIdentifier;
    const char *                             lName;
    size_t                                   lNameSize;
    SourceModel *                            lSourceModel;
    Server::Command::Sources::NameResponse   lNameResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Sources::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Source Identifier
    //
    // The validity of the source identifier will be range checked at
    // GetSource below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the source model associated with the parsed source
    // identifier. This will include a range check on the source
    // identifier.

    lStatus = mSources.GetSource(lSourceIdentifier, lSourceModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lSourceModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lNameResponse.Init(lSourceIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

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

void SourcesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    SourcesController *lController = static_cast<SourcesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
