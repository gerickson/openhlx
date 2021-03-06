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
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <CommandManager.hpp>
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

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FavoritesController :: FavoritesController(void) :
    Common::FavoritesControllerBasis(),
    Server::FavoritesControllerBasis(Common::FavoritesControllerBasis::mFavorites,
                                     Common::FavoritesControllerBasis::kFavoritesMax),
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
FavoritesController :: ~FavoritesController(void)
{
    return;
}

Status
FavoritesController :: DoRequestHandlers(const bool &aRegister)
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
FavoritesController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::FavoritesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::FavoritesControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void FavoritesController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status          lStatus;


    (void)aConnection;

    // For each favorite, query the configuration.

    for (auto lFavoriteIdentifier = IdentifierModel::kIdentifierMin; lFavoriteIdentifier <= kFavoritesMax; lFavoriteIdentifier++)
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

    lFavoriteIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aFavoriteIdentifier);
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

    lFavoriteIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aFavoriteIdentifier);
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

// MARK: Command Request Handlers

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

void
FavoritesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
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
