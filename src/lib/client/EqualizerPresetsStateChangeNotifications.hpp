/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines derived objects for a HLX client equalizer preset
 *      object data model state change notifications (SCNs).
 *
 */

#ifndef OPENHLXCLIENTEQUALIZERPRESETSSTATECHANGENOTIFICATIONS_HPP
#define OPENHLXCLIENTEQUALIZERPRESETSSTATECHANGENOTIFICATIONS_HPP

#include <memory>

#include <OpenHLX/Client/EqualizerBandStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/IdentifierStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/NameStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A derivable object for a HLX client equalizer preset object data
 *    model state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup equalizer-preset
 *  @ingroup state-change
 *
 */
class EqualizerPresetsNotificationBasis :
    public NotificationBasis,
    public IdentifierNotificationBasis
{
public:
    virtual ~EqualizerPresetsNotificationBasis(void) = default;

protected:
    EqualizerPresetsNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const IdentifierType &aEqualizerPreset);
};

/**
 *  @brief
 *    An object for a HLX client equalizer-preset object name data
 *     model propertystate change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup equalizer-preset
 *  @ingroup state-change
 *
 */
class EqualizerPresetsNameNotification :
    public EqualizerPresetsNotificationBasis,
    public NameNotificationBasis
{
public:
    EqualizerPresetsNameNotification(void) = default;
    virtual ~EqualizerPresetsNameNotification(void) = default;

    Common::Status Init(const IdentifierType &aEqualizerPresetIdentifier, const char *aName, const size_t &aNameLength);
    Common::Status Init(const IdentifierType &aEqualizerPresetIdentifier, const std::string &aName);
};

/**
 *  @brief
 *    An object for a HLX client equalizer preset object sound
 *    data model equalizer band level property state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup equalizer-preset
 *  @ingroup state-change
 *
 */
class EqualizerPresetsBandNotification :
    public EqualizerPresetsNotificationBasis,
    public EqualizerBandNotificationBasis
{
public:
    EqualizerPresetsBandNotification(void) = default;
    virtual ~EqualizerPresetsBandNotification(void) = default;

    Common::Status Init(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTEQUALIZERPRESETSSTATECHANGENOTIFICATIONS_HPP
