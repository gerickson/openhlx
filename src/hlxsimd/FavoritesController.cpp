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
 *      observation and mutation of a collection of HLX favorites.
 *
 */

#include "FavoritesController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <CommandManager.hpp>
#include <NameModelDefaults.hpp>
#include <Utilities.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

// Request data

Server::Command::Favorites::QueryRequest    FavoritesController::kQueryRequest;
Server::Command::Favorites::SetNameRequest  FavoritesController::kSetNameRequest;

/**
 *  @brief
 *    A object for representing default data for a HLX favorite data
 *    model.
 *
 *  @private
 *
 */
struct FavoriteModelDefaults
{
    Defaults::NameModel mName;
};

static const FavoriteModelDefaults kFavoriteModelDefaults[10] = {
    { "Favorite Name 1"  },
    { "Favorite Name 2"  },
    { "Favorite Name 3"  },
    { "Favorite Name 4"  },
    { "Favorite Name 5"  },
    { "Favorite Name 6"  },
    { "Favorite Name 7"  },
    { "Favorite Name 8"  },
    { "Favorite Name 9"  },
    { "Favorite Name 10" }
};

static CFStringRef      kFavoritesSchemaKey = CFSTR("Favorites");
static CFStringRef      kNameSchemaKey = CFSTR("Name");

FavoritesController :: FavoritesController(void) :
    Simulator::ControllerBasis(),
    Server::ContainerControllerBasis(),
    Common::FavoritesControllerBasis()
{
    return;
}

FavoritesController :: ~FavoritesController(void)
{
    return;
}

Status FavoritesController :: RequestInit(void)
{
    Status lRetval = kStatus_Success;

    lRetval = kQueryRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status FavoritesController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kQueryRequest,
            FavoritesController::QueryRequestReceivedHandler
        },

        {
            kSetNameRequest,
            FavoritesController::SetNameRequestReceivedHandler
        }
    };
    static const size_t               lRequestHandlerCount = sizeof (lRequestHandlers) / sizeof (lRequestHandlers[0]);
    Status                            lRetval = kStatus_Success;

    lRetval = ControllerBasis::DoRequestHandlers(&lRequestHandlers[0], &lRequestHandlers[lRequestHandlerCount], aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status FavoritesController :: Init(Server::CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mFavorites.Init(kFavoritesMax);
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

Status FavoritesController :: HandleQueryReceived(const IdentifierType &aFavoriteIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const FavoriteModel *                     lFavoriteModel;
    const char *                              lName;
    Server::Command::Favorites::NameResponse  lResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lRetval;


    lRetval = mFavorites.GetFavorite(aFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lFavoriteModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lResponse.Init(aFavoriteIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void FavoritesController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    IdentifierType  lFavoriteIdentifier;
    Status          lStatus;


    (void)aConnection;

    for (lFavoriteIdentifier = IdentifierModel::kIdentifierMin; lFavoriteIdentifier <= kFavoritesMax; lFavoriteIdentifier++)
    {
        lStatus = HandleQueryReceived(lFavoriteIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lStatus, done);
    }

 done:
    return;
}

void FavoritesController :: ResetToDefaultConfiguration(void)
{
    IdentifierType   lFavoriteIdentifier;
    FavoriteModel *  lFavoriteModel;
    Status           lStatus;


    for (lFavoriteIdentifier = IdentifierModel::kIdentifierMin; lFavoriteIdentifier <= kFavoritesMax; lFavoriteIdentifier++)
    {
        lStatus = mFavorites.GetFavorite(lFavoriteIdentifier, lFavoriteModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lFavoriteModel->SetName(kFavoriteModelDefaults[lFavoriteIdentifier - 1].mName.mName);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return;
}

Status FavoritesController :: ElementLoadFromBackupConfiguration(CFDictionaryRef aFavoritesDictionary, const IdentifierType &aFavoriteIdentifier)
{
    CFDictionaryRef                  lFavoriteDictionary = nullptr;
    CFStringRef                      lFavoriteIdentifierKey = nullptr;
    CFStringRef                      lFavoriteName = nullptr;
    FavoriteModel *                  lFavoriteModel;
    Status                           lRetval = kStatus_Success;

    // Attempt to form the favorite identifier key.

    lFavoriteIdentifierKey = Server::Utilities::Configuration::CreateCFString(aFavoriteIdentifier);
    nlREQUIRE_ACTION(lFavoriteIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    // Attempt to retrieve the favorite dictionary.

    lFavoriteDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aFavoritesDictionary, lFavoriteIdentifierKey));
    nlREQUIRE_ACTION(lFavoriteDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the favorite name string.

    lFavoriteName = static_cast<CFStringRef>(CFDictionaryGetValue(lFavoriteDictionary, kNameSchemaKey));
    nlREQUIRE_ACTION(lFavoriteName != nullptr, done, lRetval = kError_MissingConfiguration);

    lRetval = mFavorites.GetFavorite(aFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lFavoriteModel->SetName(CFString(lFavoriteName).GetCString());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    CFURelease(lFavoriteIdentifierKey);

    return (lRetval);
}

Status FavoritesController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    ContainerControllerBasis::LoadFromBackupConfiguration(aBackupDictionary,
                                                          kFavoritesMax,
                                                          kFavoritesSchemaKey);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status FavoritesController :: ElementSaveToBackupConfiguration(CFMutableDictionaryRef aFavoritesDictionary, const IdentifierType &aFavoriteIdentifier) const
{
    CFStringRef                   lFavoriteIdentifierKey = nullptr;
    CFMutableDictionaryRef        lFavoriteDictionary = nullptr;
    const FavoriteModel *         lFavoriteModel;
    const char *                  lName;
    Status                        lRetval = kStatus_Success;


    lRetval = mFavorites.GetFavorite(aFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lFavoriteIdentifierKey = Server::Utilities::Configuration::CreateCFString(aFavoriteIdentifier);
    nlREQUIRE_ACTION(lFavoriteIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    lFavoriteDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                    0,
                                                    &kCFCopyStringDictionaryKeyCallBacks,
                                                    &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lFavoriteDictionary != nullptr, done, lRetval = -ENOMEM);

    // Name

    lRetval = lFavoriteModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetCString(lFavoriteDictionary, kNameSchemaKey, lName);

    // Add the newly-created favorite dictionary into the backup
    // configuration dictionary, keyed for this favorite identifier.

    CFDictionaryAddValue(aFavoritesDictionary, lFavoriteIdentifierKey, lFavoriteDictionary);

 done:
    CFURelease(lFavoriteIdentifierKey);
    CFURelease(lFavoriteDictionary);

    return (lRetval);
}

void FavoritesController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    ContainerControllerBasis::SaveToBackupConfiguration(aBackupDictionary,
                                                        kFavoritesMax,
                                                        kFavoritesSchemaKey);
}

// MARK: Command Completion Handlers

void FavoritesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                             lFavoriteIdentifier;
    Server::Command::Favorites::QueryResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer    lResponseBuffer;
    Status                                     lStatus;
    const uint8_t *                            lBuffer;
    size_t                                     lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Favorites::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Favorite Identifier
    //
    // The validity of the favorite identifier will be range checked at
    // HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lFavoriteIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lStatus = lResponse.Init(lFavoriteIdentifier);
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

void FavoritesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    IdentifierType                            lFavoriteIdentifier;
    const char *                              lName;
    size_t                                    lNameSize;
    FavoriteModel *                           lFavoriteModel;
    Server::Command::Favorites::NameResponse  lNameResponse;
    ConnectionBuffer::MutableCountedPointer   lResponseBuffer;
    Status                                    lStatus;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Favorites::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Favorite Identifier
    //
    // The validity of the favorite identifier will be range checked at
    // GetFavorite below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lFavoriteIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the favorite model associated with the parsed favorite
    // identifier. This will include a range check on the favorite
    // identifier.

    lStatus = mFavorites.GetFavorite(lFavoriteIdentifier, lFavoriteModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lFavoriteModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lNameResponse.Init(lFavoriteIdentifier, lName, lNameSize);
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

void FavoritesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void FavoritesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    FavoritesController *lController = static_cast<FavoritesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
