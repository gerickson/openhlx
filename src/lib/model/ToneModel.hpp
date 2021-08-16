/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines an object for managing a HLX zone DSP sound
 *      model tone data model.
 *
 */

#ifndef OPENHLXMMODELTONEMODEL_HPP
#define OPENHLXMMODELTONEMODEL_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX zone DSP sound model tone data
 *    model.
 *
 *  @ingroup model
 *
 */
class ToneModel
{
public:
    /**
     *  A type for a tone equalizer bass or trebel boost or cut level.
     *
     */
    typedef int8_t                           LevelType;

public:
    static const LevelType                   kLevelMax;
    static const LevelType                   kLevelFlat;
    static const LevelType                   kLevelMin;

public:
    ToneModel(void);
    virtual ~ToneModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const LevelType &aBass, const LevelType &aTreble);
    Common::Status Init(const ToneModel &aToneModel);

    ToneModel &operator =(const ToneModel &aToneModel);

    Common::Status GetBass(LevelType &aBass) const;
    Common::Status GetTreble(LevelType &aTreble) const;
    Common::Status GetTone(LevelType &aBass, LevelType &aTreble) const;

    Common::Status DecreaseBass(LevelType &aOutBass);
    Common::Status IncreaseBass(LevelType &aOutBass);
    Common::Status DecreaseTreble(LevelType &aOutTreble);
    Common::Status IncreaseTreble(LevelType &aOutTreble);
    Common::Status SetBass(const LevelType &aBass);
    Common::Status SetTreble(const LevelType &aTreble);
    Common::Status SetTone(const LevelType &aBass, const LevelType &aTreble);

    bool operator ==(const ToneModel &aToneModel) const;

private:
    Common::Status AdjustBass(const LevelType &aAdjustment, LevelType &aOutBass);
    Common::Status AdjustTreble(const LevelType &aAdjustment, LevelType &aOutTreble);

private:
    bool            mBassIsNull;
    LevelType       mBass;
    bool            mTrebleIsNull;
    LevelType       mTreble;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELTONEMODEL_HPP
