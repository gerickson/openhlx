/*
 *    Copyright (c) 2021-2021 Grant Erickson
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
 *      This file implements a base, derivable object for managing the
 *      server-side observation and mutation of a collection of HLX
 *      objects such as equalizer presets, favorites, groups, or
 *      zones.
 *
 */

#include "ContainerControllerBasis.hpp"

#include <CFUtilities/CFUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;

namespace HLX
{

namespace Simulator
{

Common::Status
ContainerControllerBasis :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary,
                                                        const Model::IdentifierModel::IdentifierType &aIdentifierMax,
                                                        CFStringRef aContainerSchemaKey)
{
    CFDictionaryRef                  lContainerDictionary = nullptr;
    Status                           lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

    // Attempt to retrieve the zones configuration subdictionary.

    lContainerDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aBackupDictionary, aContainerSchemaKey));
    nlREQUIRE_ACTION(lContainerDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    for (auto lElementIdentifier = IdentifierModel::kIdentifierMin; lElementIdentifier <= aIdentifierMax; lElementIdentifier++)
    {
        lRetval = ElementLoadFromBackupConfiguration(lContainerDictionary, lElementIdentifier);
        nlREQUIRE(lRetval >= kStatus_Success, done);
    }

 done:
    return (lRetval);
}

void
ContainerControllerBasis :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary,
                                          const Model::IdentifierModel::IdentifierType &aIdentifierMax,
                                          CFStringRef aContainerSchemaKey) const
{
    CFMutableDictionaryRef  lContainerDictionary = nullptr;
    Status                  lStatus;


    lContainerDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                     0,
                                                     &kCFCopyStringDictionaryKeyCallBacks,
                                                     &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE(lContainerDictionary != nullptr, done);

    for (auto lElementIdentifier = IdentifierModel::kIdentifierMin; lElementIdentifier <= aIdentifierMax; lElementIdentifier++)
    {
        lStatus = ElementSaveToBackupConfiguration(lContainerDictionary, lElementIdentifier);
        nlREQUIRE_SUCCESS(lStatus, done);
    }

    // Add the newly-created and -populated dictionary into the backup
    // configuration dictionary, keyed for this controller.

    CFDictionaryAddValue(aBackupDictionary, aContainerSchemaKey, lContainerDictionary);

 done:
    CFURelease(lContainerDictionary);

    return;
}

}; // namespace Simulator

}; // namespace HLX
