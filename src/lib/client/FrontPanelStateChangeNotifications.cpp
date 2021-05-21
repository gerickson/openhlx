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
 *      This file implements derived objects for a HLX client physical
 *      front panel object data model state change notifications (SCNs).
 *
 */

#include "FrontPanelStateChangeNotifications.hpp"

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>


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
 *    This is the class default constructor.
 *
 */
FrontPanelBrightnessNotification :: FrontPanelBrightnessNotification(void) :
    NotificationBasis(),
    mBrightness(FrontPanelModel::kBrightnessMin)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel brightness property state change
 *  notification with the specified brightness level.
 *
 *  @param[in]  aBrightness  An immutable reference to the brightness
 *                           level that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
FrontPanelBrightnessNotification :: Init(const BrightnessType &aBrightness)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_FrontPanelBrightness);
    nlREQUIRE_SUCCESS(lRetval, done);

    mBrightness = aBrightness;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the front panel state change brightness level property.
 *
 *  @returns
 *    The brightness level of the front panel whose state changed.
 *
 */
FrontPanelBrightnessNotification :: BrightnessType
FrontPanelBrightnessNotification :: GetBrightness(void) const
{
    return (mBrightness);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
FrontPanelLockedNotification :: FrontPanelLockedNotification(void) :
    NotificationBasis(),
    mLocked(true)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the front panel locked property state change
 *  notification with the specified locked state.
 *
 *  @param[in]  aLocked  An immutable reference to the locked state
 *                       that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
FrontPanelLockedNotification :: Init(const LockedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_FrontPanelLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    mLocked = aLocked;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the front panel state change locked state property.
 *
 *  @returns
 *    The locked state of the front panel whose state changed.
 *
 */
FrontPanelLockedNotification :: LockedType
FrontPanelLockedNotification :: GetLocked(void) const
{
    return (mLocked);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
