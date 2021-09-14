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
 *      This file defines derived objects for a HLX client physical
 *      front panel object data model state change notifications (SCNs).
 *
 */

#ifndef OPENHLXCLIENTFRONTPANELSTATECHANGENOTIFICATIONS_HPP
#define OPENHLXCLIENTFRONTPANELSTATECHANGENOTIFICATIONS_HPP

#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FrontPanelModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    An object for a HLX client physical front panel object
 *    display brightness data model property state change
 *    notification (SCN).
 *
 *  @ingroup client
 *  @ingroup front-panel
 *  @ingroup state-change
 *
 */
class FrontPanelBrightnessNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a BrightnessType from the
     *  front panel model.
     *
     */
    typedef Model::FrontPanelModel::BrightnessType BrightnessType;

public:
    FrontPanelBrightnessNotification(void);
    virtual ~FrontPanelBrightnessNotification(void) = default;

    Common::Status Init(const BrightnessType &aBrightness);

    BrightnessType GetBrightness(void) const;

private:
    BrightnessType mBrightness;
};

/**
 *  @brief
 *    An object for a HLX client physical front panel object
 *    locked data model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup front-panel
 *  @ingroup state-change
 *
 */
class FrontPanelLockedNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a LockedType from the
     *  front panel model.
     *
     */
    typedef Model::FrontPanelModel::LockedType LockedType;

public:
    FrontPanelLockedNotification(void);
    virtual ~FrontPanelLockedNotification(void) = default;

    Common::Status Init(const LockedType &aLocked);

    LockedType GetLocked(void) const;

private:
    LockedType mLocked;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFRONTPANELSTATECHANGENOTIFICATIONS_HPP
