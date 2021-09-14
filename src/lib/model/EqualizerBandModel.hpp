/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file defines an object for managing an HLX DSP equalizer
 *      band, whether used as part of an equalizer preset or zone
 *      equalizer.
 *
 */

#ifndef OPENHLXMMODELEQUALIZERBANDMODEL_HPP
#define OPENHLXMMODELEQUALIZERBANDMODEL_HPP

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
 *    An object for managing an HLX DSP equalizer band, whether used
 *    as part of an equalizer preset or zone equalizer.
 *
 *  @ingroup model
 *
 */
class EqualizerBandModel
{
public:
    /**
     *  A type for an equalizer band center frequency, in Hertz.
     *
     */
    typedef uint16_t                         FrequencyType;

    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  A type for an equalizer band boost or cut level.
     *
     */
    typedef int8_t                           LevelType;

public:
    static const LevelType                   kLevelMax;
    static const LevelType                   kLevelFlat;
    static const LevelType                   kLevelMin;

public:
    EqualizerBandModel(void);
    virtual ~EqualizerBandModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const FrequencyType &aFrequency, const LevelType &aLevel);
    Common::Status Init(const FrequencyType &aFrequency);
    Common::Status Init(const EqualizerBandModel &aEqualizerBandModel);

    EqualizerBandModel &operator =(const EqualizerBandModel &aEqualizerBandModel);

    Common::Status GetFrequency(FrequencyType &aFrequency) const;
    Common::Status GetLevel(LevelType &aLevel) const;

    Common::Status DecreaseLevel(LevelType &aOutLevel);
    Common::Status IncreaseLevel(LevelType &aOutLevel);
    Common::Status SetLevel(const LevelType &aLevel);

    bool operator ==(const EqualizerBandModel &aEqualizerBandModel) const;

private:
    Common::Status AdjustLevel(const LevelType &aAdjustment, LevelType &aOutLevel);
    Common::Status SetFrequency(const FrequencyType &aFrequency);

private:
    bool            mFrequencyIsNull;
    FrequencyType   mFrequency;
    bool            mLevelIsNull;
    LevelType       mLevel;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELEQUALIZERBANDMODEL_HPP
