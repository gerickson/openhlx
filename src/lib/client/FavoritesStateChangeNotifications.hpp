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
 *      This file defines derived objects for a HLX client favorite object
 *      data model state change notifications (SCNs).
 *
 */

#ifndef HLXCLIENTFAVORITESSTATECHANGENOTIFICATIONS_HPP
#define HLXCLIENTFAVORITESSTATECHANGENOTIFICATIONS_HPP

#include <OpenHLX/Client/IdentifierStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/NameStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/SourceStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FavoriteModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A derivable object for a HLX client favorite object data model state
 *    change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup favorite
 *  @ingroup state-change
 *
 */
class FavoritesNotificationBasis :
    public NotificationBasis,
    public IdentifierNotificationBasis
{
public:
    virtual ~FavoritesNotificationBasis(void) = default;

protected:
    FavoritesNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const IdentifierType &aFavoriteIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client favorite object name data model property
 *    state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup favorite
 *  @ingroup state-change
 *
 */
class FavoritesNameNotification :
    public FavoritesNotificationBasis,
    public NameNotificationBasis
{
public:
    FavoritesNameNotification(void) = default;
    virtual ~FavoritesNameNotification(void) = default;

    Common::Status Init(const IdentifierType &aFavoriteIdentifier, const char *aName, const size_t &aNameLength);
    Common::Status Init(const IdentifierType &aFavoriteIdentifier, const std::string &aName);
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTFAVORITESSTATECHANGENOTIFICATIONS_HPP
