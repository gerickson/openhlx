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
 *      This file defines a base, derivable object for managing the
 *      server-side observation and mutation of a collection of HLX
 *      objects such as equalizer presets, favorites, groups, or
 *      zones.
 *
 */

#ifndef OPENHLXSERVERCONTAINERCONTROLLERBASIS_HPP
#define OPENHLXSERVERCONTAINERCONTROLLERBASIS_HPP

#include <CoreFoundation/CoreFoundation.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>

namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A base, derivable object for managing the server-side
 *    observation and mutation of a collection of HLX objects.
 *
 *  A base, derivable object for managing the server-side observation
 *  and mutation of a collection of HLX objects such as equalizer
 *  presets, favorites, groups, or zones.
 *
 *  @ingroup server
 *
 */
class ContainerControllerBasis
{
protected:
    ContainerControllerBasis(void) = default;
    ~ContainerControllerBasis(void) = default;

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary,
                                               const Model::IdentifierModel::IdentifierType &aIdentifierMax,
                                               CFStringRef aContainerSchemaKey);
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary,
                                   const Model::IdentifierModel::IdentifierType &aIdentifierMax,
                                   CFStringRef aContainerSchemaKey) const;

    virtual Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aContainerBackupDictionary, const Model::IdentifierModel::IdentifierType &aElementIdentifier) = 0;
    virtual Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aContainerBackupDictionary, const Model::IdentifierModel::IdentifierType &aElementIdentifier) const = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONTAINERCONTROLLERBASIS_HPP
