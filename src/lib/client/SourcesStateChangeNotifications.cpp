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
 *      This file implements derived objects for a HLX client source object
 *      data model state change notifications (SCNs).
 *
 */

#include "SourcesStateChangeNotifications.hpp"

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


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
 *  This initializes the source state change notification with the
 *  specified type and source identifier.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aSourceIdentifier           An immutable reference
 *                                          to the identifier of the
 *                                          source object whose state
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourcesNotificationBasis :: Init(const Type &aType,
                                 const IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(aType);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = IdentifierNotificationBasis::Init(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the source name property state change
 *  notification with the specified name extent and source identifier.
 *
 *  @param[in]  aSourceIdentifier           An immutable reference
 *                                          to the identifier of the
 *                                          source object whose name
 *                                          property state changed.
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
SourcesNameNotification :: Init(const IdentifierType &aSourceIdentifier,
                                const char *aName,
                                const size_t &aNameLength)
{
    std::string lName;

    lName.assign(aName, aNameLength);

    return (Init(aSourceIdentifier, lName));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the sources name property state change
 *  notification with the specified name and source identifier.
 *
 *  @param[in]  aSourceIdentifier           An immutable reference
 *                                          to the identifier of the
 *                                          source object whose name
 *                                          property state changed.
 *  @param[in]  aName                       An immutable reference to
 *                                          the name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourcesNameNotification :: Init(const IdentifierType &aSourceIdentifier,
                                const std::string &aName)
{
    Status lRetval = kStatus_Success;

    lRetval = SourcesNotificationBasis::Init(kStateChangeType_SourceName, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = NameNotificationBasis::Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
