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
 *      This file defines an object for managing a HLX output volume
 *      data model.
 *
 */

#ifndef OPENHLXMMODELVOLUMEMODEL_HPP
#define OPENHLXMMODELVOLUMEMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX output volume data model.
 *
 *  @ingroup model
 *  @ingroup volume
 *
 */
class VolumeModel
{
public:
    /**
     *  A type for the volume fixed/locked state.
     *
     */
    typedef bool   FixedType;

    /**
     *  A type for the volume mute state.
     *
     */
    typedef bool   MuteType;

    /**
     *  A type for the volume level.
     *
     */
    typedef int8_t LevelType;

    static const LevelType kLevelMin;
    static const LevelType kLevelMax;

public:
    VolumeModel(void);
    virtual ~VolumeModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const LevelType &aVolume, const MuteType &aMute);
    Common::Status Init(const VolumeModel &aVolumeModel);

    VolumeModel &operator =(const VolumeModel &aVolumeModel);

    Common::Status GetVolume(LevelType &aVolume) const;
    Common::Status GetFixed(FixedType &aFixed) const;
    Common::Status GetMute(MuteType &aMute) const;

    Common::Status DecreaseVolume(LevelType &aOutLevel);
    Common::Status IncreaseVolume(LevelType &aOutLevel);
    Common::Status SetVolume(const LevelType &aVolume);
    Common::Status SetFixed(const FixedType &aFixed);
    Common::Status SetMute(const MuteType &aMute);
    Common::Status ToggleMute(MuteType &aOutMute);

    bool operator ==(const VolumeModel &aVolumeModel) const;

private:
    Common::Status AdjustVolume(const LevelType &aAdjustment, LevelType &aOutLevel);

private:
    bool       mLevelIsNull;
    LevelType  mLevel;
    bool       mFixedIsNull;
    FixedType  mFixed;
    bool       mMuteIsNull;
    MuteType   mMute;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELVOLUMEMODEL_HPP
