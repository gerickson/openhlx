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
 *      This file implements derived objects for a HLX client infrared
 *      remote control object data model state change notifications
 *      (SCNs).
 *
 */

#include "InfraredStateChangeNotifications.hpp"

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
InfraredDisabledNotification :: InfraredDisabledNotification(void) :
    NotificationBasis(),
    mDisabled(true)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the infrared remote control disabled property
 *  state change notification with the specified disabled state.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled
 *                         state that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
InfraredDisabledNotification :: Init(const DisabledType &aDisabled)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(kStateChangeType_InfraredDisabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    mDisabled = aDisabled;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the infrared remote control state change disabled state property.
 *
 *  @returns
 *    The disabled state of the infrared remote control whose state changed.
 *
 */
InfraredDisabledNotification :: DisabledType
InfraredDisabledNotification :: GetDisabled(void) const
{
    return (mDisabled);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
