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
 *      This file implements base objects for HLX client object data
 *      model volume properties state change notifications (SCNs).
 *
 */

#include "VolumeStateChangeNotificationBasis.hpp"


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
MuteNotificationBasis :: MuteNotificationBasis(void) :
    mMute(true)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the volume mute property state change notification
 *  with the specified volume mute state.
 *
 *  @param[in]  aMute                       An immutable reference to the
 *                                          volume mute state that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
MuteNotificationBasis :: Init(const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    mMute = aMute;

    return (lRetval);
}

/**
 *  @brief
 *    Return the state change volume mute state property.
 *
 *  @returns
 *    The volume mute state of the class object whose state changed.
 *
 */
MuteNotificationBasis :: MuteType
MuteNotificationBasis :: GetMute(void) const
{
    return (mMute);
}

/**
 *  @brief
 *    Return whether the state change volume mute state property was
 *    asserted/enabled.
 *
 *  @returns
 *    True if the volume mute state of the class object whose state
 *    changed was asserted/enabled; otherwise, false.
 *
 */
bool
MuteNotificationBasis :: IsMuted(void) const
{
    return (mMute == true);
}

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
VolumeNotificationBasis :: VolumeNotificationBasis(void) :
    mLevel(VolumeModel::kLevelMin)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the volume level property state change notification
 *  with the specified volume level.
 *
 *  @param[in]  aLevel                      An immutable reference to the
 *                                          volume level that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
VolumeNotificationBasis :: Init(const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    mLevel = aLevel;

    return (lRetval);
}

/**
 *  @brief
 *    Return the state change volume level property.
 *
 *  @returns
 *    The volume level of the class object whose state changed.
 *
 */
VolumeNotificationBasis :: LevelType
VolumeNotificationBasis :: GetVolume(void) const
{
    return (mLevel);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
