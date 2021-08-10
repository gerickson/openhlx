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
 *      observation and mutation of a collection of HLX equalizer
 *      presets.
 *
 */

#include "EqualizerPresetsController.hpp"

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

#include <EqualizerBandModelDefaults.hpp>
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

static const EqualizerBandModel::LevelType      kEqualizerBandDefault = EqualizerBandModel::kLevelFlat;

/**
 *  @brief
 *    A object for representing default data for a HLX equalizer
 *    preset data model.
 *
 *  @private
 *
 */
struct EqualizerPresetModelDefaults
{
    Defaults::NameModel          mName;
    Defaults::EqualizerBandModel mBands[EqualizerBandsModel::kEqualizerBandsMax];
};

static const EqualizerPresetModelDefaults kEqualizerPresetModelDefaults[10] = {
    {
        { "Preset Name 1"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 2"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 3"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 4"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 5"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 6"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 7"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 8"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 9"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    },
    {
        { "Preset Name 10"  } ,
        {
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault },
            { kEqualizerBandDefault }
        }
    }
};

static CFStringRef      kEqualizerPresetsSchemaKey = CFSTR("Equalizer Presets");
static CFStringRef      kNameSchemaKey = CFSTR("Name");
static CFStringRef      kEqualizerLevelsPresetSchemaKey = CFSTR("Equalizer Levels");

EqualizerPresetsController :: EqualizerPresetsController(void) :
    Simulator::ControllerBasis(),
    Server::ContainerControllerBasis(),
    Common::EqualizerPresetsControllerBasis(),
    Server::EqualizerPresetsControllerBasis()
{
    return;
}

EqualizerPresetsController :: ~EqualizerPresetsController(void)
{
    return;
}

Status EqualizerPresetsController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kDecreaseBandRequest,
            EqualizerPresetsController::DecreaseBandRequestReceivedHandler
        },

        {
            kIncreaseBandRequest,
            EqualizerPresetsController::IncreaseBandRequestReceivedHandler
        },

        {
            kQueryRequest,
            EqualizerPresetsController::QueryRequestReceivedHandler
        },

        {
            kSetBandRequest,
            EqualizerPresetsController::SetBandRequestReceivedHandler
        },

        {
            kSetNameRequest,
            EqualizerPresetsController::SetNameRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                         &lRequestHandlers[lRequestHandlerCount],
                                                         this,
                                                         aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status EqualizerPresetsController :: Init(Server::CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    const bool  lRegister = true;
    Status      lRetval = kStatus_Success;


    lRetval = Server::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerPresets.Init(kEqualizerPresetsMax);
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

Status EqualizerPresetsController :: GetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel)
{
    EqualizerPresetModel *             lEqualizerPresetModel;
    Status                             lRetval;


    // Attempt to get the equalizer preset model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Attempt to get the equalizer band model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = lEqualizerPresetModel->GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: HandleQueryReceived(const IdentifierType &aEqualizerPresetIdentifier, ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const EqualizerPresetModel *                     lEqualizerPresetModel;
    const char *                                     lName;
    Server::Command::EqualizerPresets::NameResponse  lNameResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    EqualizerBandModel::IdentifierType               lEqualizerBandIdentifier;
    const EqualizerBandModel *                       lEqualizerBandModel;
    EqualizerBandModel::LevelType                    lLevel;
    Server::Command::EqualizerPresets::BandResponse  lBandResponse;
    Status                                           lRetval;


    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lEqualizerPresetModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lNameResponse.Init(aEqualizerPresetIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        lRetval = lEqualizerPresetModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lEqualizerBandModel->GetLevel(lLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lBandResponse.Init(aEqualizerPresetIdentifier, lEqualizerBandIdentifier, lLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBuffer = lBandResponse.GetBuffer();
        lSize = lBandResponse.GetSize();

        lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(Server::ConnectionBasis &aConnection,
                                                       const IdentifierType &aEqualizerPresetIdentifier,
                                                       const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                       const Model::EqualizerBandModel::LevelType &aBandAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleAdjustBandReceived(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
EqualizerPresetsController :: HandleAdjustBandReceived(const IdentifierType &aEqualizerPresetIdentifier,
                                                       const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                       const Model::EqualizerBandModel::LevelType &aBandAdjustment,
                                                       Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    EqualizerBandModel::LevelType      lBandLevel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Increase or decrease, per the adjustment, the equalizer band
    // level.

    if (aBandAdjustment > 0)
    {
        lRetval = lEqualizerBandModel->IncreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aBandAdjustment < 0)
    {
        lRetval = lEqualizerBandModel->DecreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: HandleSetBandReceived(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lEqualizerBandModel->SetLevel(aBandLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = HandleBandResponse(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: HandleBandResponse(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Server::Command::EqualizerPresets::BandResponse  lBandResponse;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;
    Status                                           lRetval;

    lRetval = lBandResponse.Init(aEqualizerPresetIdentifier, aEqualizerBandIdentifier, aBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBandResponse.GetBuffer();
    lSize = lBandResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void EqualizerPresetsController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    EqualizerPresetModel::IdentifierType     lEqualizerPresetIdentifier;
    Status                                   lStatus;


    (void)aConnection;

    for (lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin; lEqualizerPresetIdentifier <= kEqualizerPresetsMax; lEqualizerPresetIdentifier++)
    {
        lStatus = HandleQueryReceived(lEqualizerPresetIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lStatus, done);
    }

 done:
    return;
}

void EqualizerPresetsController :: ResetToDefaultConfiguration(void)
{
    EqualizerPresetModel::IdentifierType     lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType       lEqualizerBandIdentifier;
    EqualizerPresetModel *                   lEqualizerPresetModel;
    EqualizerBandModel *                     lEqualizerBandModel;
    Status                                   lStatus;


    for (lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin; lEqualizerPresetIdentifier <= kEqualizerPresetsMax; lEqualizerPresetIdentifier++)
    {
        const EqualizerPresetModelDefaults &lEqualizerPresetModelDefaults = kEqualizerPresetModelDefaults[lEqualizerPresetIdentifier - 1];

        lStatus = mEqualizerPresets.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lEqualizerPresetModel->SetName(lEqualizerPresetModelDefaults.mName.mName);
        nlREQUIRE_SUCCESS(lStatus, done);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
        {
            lStatus = lEqualizerPresetModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lEqualizerBandModel->SetLevel(lEqualizerPresetModelDefaults.mBands[lEqualizerBandIdentifier - 1].mLevel);
            nlREQUIRE_SUCCESS(lStatus, done);

            if (lStatus == kStatus_Success)
            {
                OnConfigurationIsDirty();
            }
        }
    }

 done:
    return;
}

Status EqualizerPresetsController :: EqualizerPresetEqualizerLoadFromBackupConfiguration(CFDictionaryRef aEqualizerPresetDictionary, Model::EqualizerPresetModel &aEqualizerPresetModel)
{
    CFArrayRef                         lEqualizerLevelsArray = nullptr;
    CFIndex                            lEqualizerBandsCount;
    EqualizerBandModel::IdentifierType lEqualizerBandIdentifier;
    Status                             lRetval = kStatus_Success;

    lEqualizerLevelsArray = static_cast<CFArrayRef>(CFDictionaryGetValue(aEqualizerPresetDictionary, kEqualizerLevelsPresetSchemaKey));
    nlREQUIRE_ACTION(lEqualizerLevelsArray != nullptr, done, lRetval = kError_MissingConfiguration);

    lEqualizerBandsCount = CFArrayGetCount(lEqualizerLevelsArray);
    nlREQUIRE_ACTION(lEqualizerBandsCount == EqualizerBandsModel::kEqualizerBandsMax, done, lRetval = kError_InvalidConfiguration);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        const CFIndex        lEqualizerBandIndex = (lEqualizerBandIdentifier - 1);
        EqualizerBandModel * lEqualizerBandModel;
        CFNumberRef          lBandLevelRef = nullptr;
        bool                 lStatus;
        EqualizerBandModel::LevelType         lBandLevel;

        lRetval = aEqualizerPresetModel.GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBandLevelRef = static_cast<CFNumberRef>(CFArrayGetValueAtIndex(lEqualizerLevelsArray, lEqualizerBandIndex));
        nlREQUIRE_ACTION(lBandLevelRef != nullptr, done, lRetval = kError_InvalidConfiguration);

        lStatus = CFUNumberGetValue(lBandLevelRef, lBandLevel);
        nlREQUIRE_ACTION(lStatus, done, lRetval = kError_InvalidConfiguration);

        lRetval = lEqualizerBandModel->SetLevel(lBandLevel);
        nlREQUIRE(lRetval >= kStatus_Success, done);

        if (lRetval == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: ElementLoadFromBackupConfiguration(CFDictionaryRef aEqualizerPresetsDictionary, const IdentifierType &aEqualizerPresetIdentifier)
{
    CFDictionaryRef                  lEqualizerPresetDictionary = nullptr;
    CFStringRef                      lEqualizerPresetIdentifierKey = nullptr;
    CFStringRef                      lEqualizerPresetName = nullptr;
    EqualizerPresetModel *                    lEqualizerPresetModel;
    Status                           lRetval = kStatus_Success;

    // Attempt to form the source identifier key.

    lEqualizerPresetIdentifierKey = Server::Utilities::Configuration::CreateCFString(aEqualizerPresetIdentifier);
    nlREQUIRE_ACTION(lEqualizerPresetIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    // Attempt to retrieve the source dictionary.

    lEqualizerPresetDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aEqualizerPresetsDictionary, lEqualizerPresetIdentifierKey));
    nlREQUIRE_ACTION(lEqualizerPresetDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the source name string.

    lEqualizerPresetName = static_cast<CFStringRef>(CFDictionaryGetValue(lEqualizerPresetDictionary, kNameSchemaKey));
    nlREQUIRE_ACTION(lEqualizerPresetName != nullptr, done, lRetval = kError_MissingConfiguration);

    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Name

    lRetval = lEqualizerPresetModel->SetName(CFString(lEqualizerPresetName).GetCString());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Equalizer Band Levels

    lRetval = EqualizerPresetEqualizerLoadFromBackupConfiguration(lEqualizerPresetDictionary, *lEqualizerPresetModel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    CFURelease(lEqualizerPresetIdentifierKey);

    return (lRetval);
}

Status EqualizerPresetsController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    ContainerControllerBasis::LoadFromBackupConfiguration(aBackupDictionary,
                                                          kEqualizerPresetsMax,
                                                          kEqualizerPresetsSchemaKey);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status EqualizerPresetsController :: EqualizerPresetEqualizerSaveToBackupConfiguration(CFMutableDictionaryRef aEqualizerPresetDictionary, const EqualizerPresetModel &aEqualizerPresetModel)
{
    CFMutableArrayRef                     lEqualizerLevelsArray = nullptr;
    EqualizerBandModel::IdentifierType    lEqualizerBandIdentifier;
    const EqualizerBandModel *            lEqualizerBandModel;
    EqualizerBandModel::LevelType         lBandLevel;
    Status                                lRetval = kStatus_Success;


    lEqualizerLevelsArray = CFArrayCreateMutable(kCFAllocatorDefault,
                                                 0,
                                                 &kCFTypeArrayCallBacks);
    nlREQUIRE_ACTION(lEqualizerLevelsArray != nullptr, done, lRetval = -ENOMEM);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        CFNumberRef  lBandLevelRef = nullptr;

        lRetval = aEqualizerPresetModel.GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lEqualizerBandModel->GetLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBandLevelRef = CFUNumberCreate(kCFAllocatorDefault, lBandLevel);
        nlREQUIRE_ACTION(lBandLevelRef != nullptr, done, lRetval = -ENOMEM);

        CFArrayAppendValue(lEqualizerLevelsArray, lBandLevelRef);

        CFURelease(lBandLevelRef);
    }

    CFDictionaryAddValue(aEqualizerPresetDictionary, kEqualizerLevelsPresetSchemaKey, lEqualizerLevelsArray);

 done:
    CFURelease(lEqualizerLevelsArray);

    return (lRetval);
}

Status EqualizerPresetsController :: ElementSaveToBackupConfiguration(CFMutableDictionaryRef aEqualizerPresetsDictionary, const IdentifierType &aEqualizerPresetIdentifier) const
{
    CFStringRef                   lEqualizerPresetIdentifierKey = nullptr;
    CFMutableDictionaryRef        lEqualizerPresetDictionary = nullptr;
    const EqualizerPresetModel *  lEqualizerPresetModel;
    const char *                  lName;
    Status                        lRetval = kStatus_Success;


    lRetval = mEqualizerPresets.GetEqualizerPreset(aEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lEqualizerPresetIdentifierKey = Server::Utilities::Configuration::CreateCFString(aEqualizerPresetIdentifier);
    nlREQUIRE_ACTION(lEqualizerPresetIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    lEqualizerPresetDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                           0,
                                                           &kCFCopyStringDictionaryKeyCallBacks,
                                                           &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lEqualizerPresetDictionary != nullptr, done, lRetval = -ENOMEM);

    // Name

    lRetval = lEqualizerPresetModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetCString(lEqualizerPresetDictionary, kNameSchemaKey, lName);

    // Equalizer Band Levels

    lRetval = EqualizerPresetEqualizerSaveToBackupConfiguration(lEqualizerPresetDictionary, *lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the newly-created equalizer preset dictionary into the
    // backup configuration dictionary, keyed for this equalizer
    // preset identifier.

    CFDictionaryAddValue(aEqualizerPresetsDictionary, lEqualizerPresetIdentifierKey, lEqualizerPresetDictionary);

 done:
    CFURelease(lEqualizerPresetIdentifierKey);
    CFURelease(lEqualizerPresetDictionary);

    return (lRetval);
}

void EqualizerPresetsController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    ContainerControllerBasis::SaveToBackupConfiguration(aBackupDictionary,
                                                        kEqualizerPresetsMax,
                                                        kEqualizerPresetsSchemaKey);
}

// MARK: Command Completion Handlers

void EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = -1;
    IdentifierType                              lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::DecreaseBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBandReceived(aConnection, lEqualizerPresetIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = 1;
    IdentifierType                              lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::IncreaseBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBandReceived(aConnection, lEqualizerPresetIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                    lEqualizerPresetIdentifier;
    Server::Command::EqualizerPresets::QueryResponse  lResponse;
    ConnectionBuffer::MutableCountedPointer           lResponseBuffer;
    Status                                            lStatus;
    const uint8_t *                                   lBuffer;
    size_t                                            lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryReceived(lEqualizerPresetIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lStatus = lResponse.Init(lEqualizerPresetIdentifier);
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

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                           lEqualizerPresetIdentifier;
    EqualizerBandModel::IdentifierType       lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType            lBandLevel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::SetBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range
    // checked at HandleSetBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleSetBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level
    //
    // The validity of the equalizer band level will be range checked
    // at HandleSetBandReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBandLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetBandReceived(lEqualizerPresetIdentifier, lEqualizerBandIdentifier, lBandLevel, lResponseBuffer);
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

void EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    IdentifierType                                   lEqualizerPresetIdentifier;
    const char *                                     lName;
    size_t                                           lNameSize;
    EqualizerPresetModel *                           lEqualizerPresetModel;
    Server::Command::EqualizerPresets::NameResponse  lNameResponse;
    ConnectionBuffer::MutableCountedPointer          lResponseBuffer;
    Status                                           lStatus;
    const uint8_t *                                  lBuffer;
    size_t                                           lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::EqualizerPresets::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Equalizer Preset Identifier
    //
    // The validity of the equalizer preset identifier will be range checked at
    // GetEqualizerPreset below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the equalizer preset model associated with the parsed
    // equalizer preset identifier. This will include a range check on
    // the equalizer preset identifier.

    lStatus = mEqualizerPresets.GetEqualizerPreset(lEqualizerPresetIdentifier, lEqualizerPresetModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lEqualizerPresetModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lNameResponse.Init(lEqualizerPresetIdentifier, lName, lNameSize);
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

void EqualizerPresetsController :: DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void EqualizerPresetsController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    EqualizerPresetsController *lController = static_cast<EqualizerPresetsController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
