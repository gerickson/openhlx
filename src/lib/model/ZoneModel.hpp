/*
 *    Copyright (c) 2018-2024 Grant Erickson
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
 *      This file defines an object for managing a HLX zone data
 *      model.
 *
 */

#ifndef OPENHLXMMODELZONEMODEL_HPP
#define OPENHLXMMODELZONEMODEL_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/BalanceModel.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/OutputModelBasis.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX zone data model.
 *
 *  @ingroup model
 *  @ingroup zone
 *
 */
class ZoneModel :
    public OutputModelBasis
{
public:
    /**
     *  Convenience type redeclaring @a BalanceType from the
     *  balance model.
     *
     */
    typedef BalanceModel::BalanceType       BalanceType;

    /**
     *  Convenience type redeclaring @a ChannelMode from the
     *  sound model.
     *
     */
    typedef SoundModel::ChannelMode         ChannelMode;

    /**
     *  Convenience type redeclaring @a SoundMode from the
     *  sound model.
     *
     */
    typedef SoundModel::SoundMode           SoundMode;

    /**
     *  Convenience type redeclaring @a FixedType from the
     *  volume model.
     *
     */
    typedef VolumeModel::FixedType          VolumeFixedType;

public:
    ZoneModel(void) = default;
    virtual ~ZoneModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const char *aName, const IdentifierType &aIdentifier);
    Common::Status Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier);
    Common::Status Init(const std::string &aName, const IdentifierType &aIdentifier);
    Common::Status Init(const ZoneModel &aZoneModel);

    ZoneModel &operator =(const ZoneModel &aZoneModel);

    Common::Status GetBalance(BalanceType &aBalance) const;
    Common::Status GetBass(ToneModel::LevelType &aBass) const;
    Common::Status GetChannelMode(ChannelMode &aChannelMode) const;
    Common::Status GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel);
    Common::Status GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const;
    Common::Status GetEqualizerPreset(EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier) const;
    Common::Status GetHighpassCrossover(CrossoverModel *&aHighpassCrossoverModel);
    Common::Status GetHighpassCrossover(const CrossoverModel *&aHighpassCrossoverModel) const;
    Common::Status GetHighpassFrequency(CrossoverModel::FrequencyType &aHighpassFrequency) const;
    Common::Status GetLowpassCrossover(CrossoverModel *&aLowpassCrossoverModel);
    Common::Status GetLowpassCrossover(const CrossoverModel *&aLowpassCrossoverModel) const;
    Common::Status GetLowpassFrequency(CrossoverModel::FrequencyType &aLowpassFrequency) const;
    Common::Status GetSoundMode(SoundMode &aSoundMode) const;
    Common::Status GetSource(SourceModel::IdentifierType &aSourceIdentifier) const;
    Common::Status GetTone(ToneModel::LevelType &aBass, ToneModel::LevelType &aTreble) const;
    Common::Status GetTreble(ToneModel::LevelType &aTreble) const;
    Common::Status GetVolumeFixed(VolumeFixedType &aVolumeFixed) const;

    Common::Status DecreaseBass(ToneModel::LevelType &aOutBass);
    Common::Status DecreaseTreble(ToneModel::LevelType &aOutTreble);
    Common::Status IncreaseBalanceLeft(BalanceType &aOutBalance);
    Common::Status IncreaseBalanceRight(BalanceType &aOutBalance);
    Common::Status IncreaseBass(ToneModel::LevelType &aOutBass);
    Common::Status IncreaseTreble(ToneModel::LevelType &aOutTreble);
    Common::Status SetBalance(const BalanceType &aBalance);
    Common::Status SetBass(const ToneModel::LevelType &aBass);
    Common::Status SetEqualizerPreset(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);
    Common::Status SetHighpassFrequency(const CrossoverModel::FrequencyType &aHighpassFrequency);
    Common::Status SetLowpassFrequency(const CrossoverModel::FrequencyType &aLowpassFrequency);
    Common::Status SetSoundMode(const SoundMode &aSoundMode);
    Common::Status SetSource(const SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status SetTone(const ToneModel::LevelType &aBass, const ToneModel::LevelType &aTreble);
    Common::Status SetTreble(const ToneModel::LevelType &aTreble);
    Common::Status SetVolumeFixed(const VolumeFixedType &aVolumeFixed);

    bool operator ==(const ZoneModel &aZoneModel) const;

private:
    BalanceModel    mBalance;
    SoundModel      mSoundModel;
    IdentifierModel mSourceIdentifier;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELZONEMODEL_HPP
