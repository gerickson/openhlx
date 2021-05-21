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
 *      This file defines base objects for HLX client object data
 *      model volume properties state change notifications (SCNs).
 *
 */

#ifndef HLXCLIENTVOLUMESTATECHANGENOTIFICATIONBASIS_HPP
#define HLXCLIENTVOLUMESTATECHANGENOTIFICATIONBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A derivable object for a HLX client object volume mute data
 *    model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class MuteNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a MuteType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::MuteType MuteType;

public:
    virtual ~MuteNotificationBasis(void) = default;

    MuteType GetMute(void) const;
    bool IsMuted(void) const;

protected:
    MuteNotificationBasis(void);

    Common::Status Init(const MuteType &aMute);

private:
    MuteType mMute;
};

/**
 *  @brief
 *    A derivable object for a HLX client object volume level data
 *    model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *  @ingroup volume
 *
 */
class VolumeNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a LevelType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::LevelType LevelType;

public:
    virtual ~VolumeNotificationBasis(void) = default;

    LevelType GetVolume(void) const;

protected:
    VolumeNotificationBasis(void);

    Common::Status Init(const LevelType &aLevel);

private:
    LevelType mLevel;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTVOLUMESTATECHANGENOTIFICATIONBASIS_HPP
