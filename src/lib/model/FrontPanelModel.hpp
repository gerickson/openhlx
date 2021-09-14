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
 *      This file defines an object for managing the HLX physical
 *      front panel data model.
 *
 */

#ifndef OPENHLXMMODELFRONTPANELMODEL_HPP
#define OPENHLXMMODELFRONTPANELMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing the HLX physical front panel data model.
 *
 *  @ingroup model
 *  @ingroup front-panel
 *
 */
class FrontPanelModel
{
public:
    /**
     *  A type for the front panel display brightness level.
     *
     */
    typedef uint8_t BrightnessType;

    /**
     *  A type for the front panel display locked state.
     *
     */
    typedef bool    LockedType;

    static const BrightnessType kBrightnessMin;
    static const BrightnessType kBrightnessMax;

public:
    FrontPanelModel(void);
    virtual ~FrontPanelModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const BrightnessType &aBrightness, const LockedType &aLocked);
    Common::Status Init(const FrontPanelModel &aFrontPanelModel);

    FrontPanelModel &operator =(const FrontPanelModel &aFrontPanelModel);

    Common::Status GetBrightness(BrightnessType &aBrightness) const;
    Common::Status GetLocked(LockedType &aLocked) const;

    Common::Status SetBrightness(const BrightnessType &aBrightness);
    Common::Status SetLocked(const LockedType &aLocked);

    bool operator ==(const FrontPanelModel &aFrontPanelModel) const;

private:
    bool            mBrightnessIsNull;
    BrightnessType  mBrightness;
    bool            mLockedIsNull;
    LockedType      mLocked;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELFRONTPANELMODEL_HPP
