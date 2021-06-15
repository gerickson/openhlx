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
 *      This file implements a base object for a HLX client object data
 *      model name property state change notification (SCN).
 *
 */

#include "NameStateChangeNotificationBasis.hpp"

#include <string>

#include <string.h>


using namespace HLX::Common;

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the name property state change notification
 *  with the specified name extent.
 *
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
NameNotificationBasis :: Init(const char *aName, const size_t &aNameLength)
{
    std::string lName;

    lName.assign(aName, aNameLength);

    return (Init(lName));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the name property state change notification
 *  with the specified name.
 *
 *  @param[in]  aName                       A pointer to the start of
 *                                          the null- terminated C
 *                                          string name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameNotificationBasis :: Init(const char *aName)
{
    const std::string lName = aName;

    return (Init(aName, strlen(aName)));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the name property state change notification
 *  with the specified name.
 *
 *  @param[in]  aName                       An immutable reference to
 *                                          the name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
NameNotificationBasis :: Init(const std::string &aName)
{
    Status lRetval = kStatus_Success;

    mName = aName;

    return (lRetval);
}

/**
 *  @brief
 *    Return the state change name property.
 *
 *  @returns
 *    The name of the class object whose name property state changed.
 *
 */
const char *
NameNotificationBasis :: GetName(void) const
{
    return (mName.c_str());
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
