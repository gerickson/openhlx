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
 *      This file implements derived objects for a HLX client equalizer preset
 *      object data model state change notifications (SCNs).
 *
 */

#include "EqualizerPresetsStateChangeNotifications.hpp"

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the equalizer presets state change notification
 *  with the specified type and equalizer preset identifier.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of the
 *                                          equalizer preset object
 *                                          whose state changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsNotificationBasis :: Init(const Type &aType,
                                          const IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(aType);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = IdentifierNotificationBasis::Init(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the equalizer preset name property state change
 *  notification with the specified name extent and equalizer preset
 *  identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of the
 *                                          equalizer preset object
 *                                          whose name property state
 *                                          changed.
 *  @param[in]  aName                       A pointer to the start of
 *                                          the null- terminated C
 *                                          string name that changed.
 *  @param[in]  aNameLength                 An immutable reference to
 *                                          the length, in bytes, of
 *                                          @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsNameNotification :: Init(const IdentifierType &aEqualizerPresetIdentifier,
                                         const char *aName,
                                         const size_t &aNameLength)
{
    std::string lName;

    lName.assign(aName, aNameLength);

    return (Init(aEqualizerPresetIdentifier, lName));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the equalizer preset name property state change
 *  notification with the specified name and equalizer preset
 *  identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of the
 *                                          equalizer preset object
 *                                          whose name property state
 *                                          changed.
 *  @param[in]  aName                       An immutable reference to
 *                                          the name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsNameNotification :: Init(const IdentifierType &aEqualizerPresetIdentifier,
                                         const std::string &aName)
{
    Status lRetval = kStatus_Success;

    lRetval = EqualizerPresetsNotificationBasis::Init(kStateChangeType_EqualizerPresetName, aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = NameNotificationBasis::Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the equalizer preset band level property state
 *  change notification with the specified equalizer band identifer
 *  and level and equalizer preset identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the identifier of the
 *                                          equalizer preset object
 *                                          whose band level property
 *                                          state changed.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference to
 *                                          the specific equalizer
 *                                          band identifier whose band
 *                                          level property state
 *                                          changed.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to the
 *                                          equalizer band level that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsBandNotification :: Init(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                         const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                         const EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = EqualizerPresetsNotificationBasis::Init(kStateChangeType_EqualizerPresetBand,
                                                      aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandNotificationBasis::Init(aEqualizerBandIdentifier,
                                                   aEqualizerBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
