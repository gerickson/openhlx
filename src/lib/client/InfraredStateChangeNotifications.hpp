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
 *      This file defines derived objects for a HLX client infrared
 *      remote control object data model state change notifications
 *      (SCNs).
 *
 */

#ifndef HLXCLIENTINFRAREDSTATECHANGENOTIFICATIONS_HPP
#define HLXCLIENTINFRAREDSTATECHANGENOTIFICATIONS_HPP

#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    An object for a HLX client infrared remote control object
 *    disabled data model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup infrared
 *  @ingroup state-change
 *
 */
class InfraredDisabledNotification :
    public NotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a DisabledType from the
     *  infrared remote control model.
     *
     */
    typedef Model::InfraredModel::DisabledType DisabledType;

public:
    InfraredDisabledNotification(void);
    virtual ~InfraredDisabledNotification(void) = default;

    Common::Status Init(const DisabledType &aDisabled);

    DisabledType GetDisabled(void) const;

private:
    DisabledType mDisabled;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTINFRAREDSTATECHANGENOTIFICATIONS_HPP
