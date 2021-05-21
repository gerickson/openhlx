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
 *      This file implements a base object for a HLX client state change
 *      notification (SCN).
 *
 */

#include "StateChangeNotificationBasis.hpp"


using namespace HLX::Common;

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
NotificationBasis :: NotificationBasis(void) :
    mType(kStateChangeType_Unknown)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  @param[in]  aType  An immutable reference to the state change
 *                     notification type to initialize with. This
 *                     indicates what object class and what property
 *                     within that object class changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NotificationBasis :: Init(const Type &aType)
{
    Status lRetval = kStatus_Success;

    mType = aType;

    return (lRetval);
}

/**
 *  @brief
 *    Return the state change notification type.
 *
 *  @returns
 *    The state change notification type.
 *
 */
Type
NotificationBasis :: GetType(void) const
{
    return (mType);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
