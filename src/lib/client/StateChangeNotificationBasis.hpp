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
 *      This file defines a base object for a HLX client state change
 *      notification (SCN).
 *
 */

#ifndef OPENHLXCLIENTSTATECHANGENOTIFICATIONBASIS_HPP
#define OPENHLXCLIENTSTATECHANGENOTIFICATIONBASIS_HPP

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A base object for a HLX client state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *
 */
class NotificationBasis
{
public:
    virtual ~NotificationBasis(void) = default;

    Type GetType(void) const;

protected:
    NotificationBasis(void);

    Common::Status Init(const Type &aType);

private:
    Type mType;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTSTATECHANGENOTIFICATIONBASIS_HPP
