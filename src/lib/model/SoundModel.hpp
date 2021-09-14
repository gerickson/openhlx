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
 *      This file defines an object for managing an HLX zone DSP sound
 *      data model.
 *
 */

#ifndef OPENHLXMMODELSOUNDMODEL_HPP
#define OPENHLXMMODELSOUNDMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/EqualizerBandsModel.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/CrossoverModel.hpp>
#include <OpenHLX/Model/ToneModel.hpp>

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing an HLX zone DSP sound data model.
 *
 *  @ingroup model
 *
 */
class SoundModel
{
public:
    /**
     *  Enumerated type for the channel mode a zone (output) is
     *  operating in.
     *
     */
    enum ChannelMode
    {
        kChannelModeMono          = 1, //!< Monoaural, single channel mode.
        kChannelModeStereo        = 2  //!< Binaural, stereophonic channel mode.
    };

    /**
     *  Enumeration for the zone equalizer sound mode of operation.
     *
     *  @note
     *    These modes of operation implicitly alter and impact
     *    #ChannelMode.
     *
     */
    enum
    {
        /**
         *  The minimum or lowest zone equalizer sound mode.
         *
         */
        kSoundModeMin             = 0,

        /**
         *  The zone equalizer is disabled and all input signals are
         *  passed through to the output, unmodified.
         *
         */
        kSoundModeDisabled        = kSoundModeMin,

        /**
         *  The zone equalizer is operating in per zone mode, allowing
         *  band level settings unique to that zone and that zone
         *  only.
         *
         */
        kSoundModeZoneEqualizer   = 1,

        /**
         *  The zone equalizer is operating in preset mode, allowing
         *  from the selection of one of several equalizer presets.
         *
         */
        kSoundModePresetEqualizer = 2,

        /**
         *  The zone equalizer is operating in tone mode, allowing
         *  bass and treble settings unique to that zone and that zone
         *  only.
         *
         */
        kSoundModeTone            = 3,

        /**
         *  The zone equalizer is operating in lowpass crossover mode,
         *  allowing a lowpass crossover filter frequency to be set
         *  for the zone.
         *
         *  @note
         *    This mode implies @a ChannelMode::kChannelModeMono.
         *
         */
        kSoundModeLowpass         = 4,

        /**
         *  The zone equalizer is operating in highpass crossover mode,
         *  allowing a highpass crossover filter frequency to be set
         *  for the zone.
         *
         */
        kSoundModeHighpass        = 5,

        /**
         *  The maximum or highest zone equalizer sound mode.
         *
         */
        kSoundModeMax             = kSoundModeHighpass
    };

    /**
     *  Type for the zone equalizer mode of operation.
     *
     */
    typedef uint8_t SoundMode;

public:
    SoundModel(void);
    virtual ~SoundModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const SoundMode &aSoundMode);
    Common::Status Init(const SoundModel &aSoundModel);

    SoundModel &operator =(const SoundModel &aSoundModel);

    Common::Status GetChannelMode(ChannelMode &aChannelMode) const;
    Common::Status GetSoundMode(SoundMode &aSoundMode) const;

    Common::Status GetBass(ToneModel::LevelType &aBass) const;
    Common::Status GetTone(ToneModel::LevelType &aBass, ToneModel::LevelType &aTreble) const;
    Common::Status GetTreble(ToneModel::LevelType &aTreble) const;

    Common::Status GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel);
    Common::Status GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const;

    Common::Status GetEqualizerPreset(EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier) const;

    Common::Status GetHighpassCrossover(CrossoverModel *&aHighpassCrossoverModel);
    Common::Status GetHighpassCrossover(const CrossoverModel *&aHighpassCrossoverModel) const;
    Common::Status GetHighpassFrequency(CrossoverModel::FrequencyType &aHighpassFrequency) const;
    Common::Status GetLowpassCrossover(CrossoverModel *&aLowpassCrossoverModel);
    Common::Status GetLowpassCrossover(const CrossoverModel *&aLowpassCrossoverModel) const;
    Common::Status GetLowpassFrequency(CrossoverModel::FrequencyType &aHighpassFrequency) const;

    Common::Status SetSoundMode(const SoundMode &aSoundMode);

    Common::Status DecreaseBass(ToneModel::LevelType &aOutBass);
    Common::Status DecreaseTreble(ToneModel::LevelType &aOutTreble);
    Common::Status IncreaseBass(ToneModel::LevelType &aOutBass);
    Common::Status IncreaseTreble(ToneModel::LevelType &aOutTreble);
    Common::Status SetBass(const ToneModel::LevelType &aBass);
    Common::Status SetTone(const ToneModel::LevelType &aBass, const ToneModel::LevelType &aTreble);
    Common::Status SetTreble(const ToneModel::LevelType &aTreble);

    Common::Status SetEqualizerPreset(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    Common::Status SetHighpassFrequency(const CrossoverModel::FrequencyType &aHighpassFrequency);
    Common::Status SetLowpassFrequency(const CrossoverModel::FrequencyType &aHighpassFrequency);

    bool operator ==(const SoundModel &aSoundModel) const;

private:
    bool                 mSoundModeIsNull;
    SoundMode            mSoundMode;
    EqualizerBandsModel  mEqualizerBands;
    IdentifierModel      mEqualizerPresetIdentifier;
    ToneModel            mToneModel;
    CrossoverModel       mLowpassCrossover;
    CrossoverModel       mHighpassCrossover;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELSOUNDMODEL_HPP
