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
 *      This file implements an object for managing a HLX zone.
 *
 */

#include "ZoneModel.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name and identifier.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *  @sa SetName
 *
 */
Status
ZoneModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = OutputModelBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mBalance.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSoundModel.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSourceIdentifier.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the zone name with.
 *  @param[in]  aIdentifier  An immutable reference for the zone
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
ZoneModel :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = OutputModelBasis::Init(aName, aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mBalance.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSoundModel.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSourceIdentifier.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent and
 *  identifier.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *  @param[in]  aIdentifier  An immutable reference for the zone
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
ZoneModel :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = OutputModelBasis::Init(aName, aNameLength, aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mBalance.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSoundModel.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSourceIdentifier.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           zone name with.
 *  @param[in]  aIdentifier  An immutable reference for the zone
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
ZoneModel :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = OutputModelBasis::Init(aName, aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mBalance.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSoundModel.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSourceIdentifier.Init();
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified zone model.
 *
 *  @param[in]  aZoneModel  An immutable reference to the
 *                              zone model to initialize
 *                              with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZoneModel :: Init(const ZoneModel &aZoneModel)
{
    Status lRetval = kStatus_Success;

    lRetval = OutputModelBasis::Init(aZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mBalance.Init(aZoneModel.mBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSoundModel.Init(aZoneModel.mSoundModel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    lRetval = mSourceIdentifier.Init(aZoneModel.mSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    if (lRetval == kStatus_ValueAlreadySet)
    {
        lRetval = kStatus_Success;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified zone model to this one.
 *
 *  @param[in]  aZoneModel  An immutable reference to the
 *                              zone model to assign (copy)
 *                              to this one.
 *
 *
 *  @returns
 *    A reference to this zone model after the assignment (copy)
 *    is complete.
 *
 */
ZoneModel &
ZoneModel :: operator =(const ZoneModel &aZoneModel)
{
    OutputModelBasis::operator =(aZoneModel);

    mBalance          = aZoneModel.mBalance;
    mSoundModel       = aZoneModel.mSoundModel;
    mSourceIdentifier = aZoneModel.mSourceIdentifier;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the zone stereophonic channel balance.
 *
 *  This attempts to get the zone stereophonic channel balance, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aBalance  A mutable reference to storage for the zone
 *                         stereophonic channel balance, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zone model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *
 *  @sa SetBalance
 *
 */
Status
ZoneModel :: GetBalance(BalanceType &aBalance) const
{
    Status lRetval = kStatus_Success;

    lRetval = mBalance.GetBalance(aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer bass level.
 *
 *  This attempts to get the model tone equalizer bass level, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aBass  A mutable reference to storage for the
 *                      model tone equalizer bass level, if
 *                      successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer bass
 *                                  level value has not been
 *                                  initialized with a known value.
 *
 */
Status
ZoneModel :: GetBass(ToneModel::LevelType &aBass) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetBass(aBass);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model channel mode.
 *
 *  This attempts to get the model channel mode, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aChannelMode  A mutable reference to storage
 *                             for the model channel mode, if
 *                             successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model channel mode value
 *                                  has not been initialized with a
 *                                  known value.
 *
 */
Status
ZoneModel :: GetChannelMode(ChannelMode &aChannelMode) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetChannelMode(aChannelMode);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer band model associated with the
 *    specified equalizer band identifier.
 *
 *  This attempts to get the equalizer band model associated with the
 *  specified equalizer band identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerBandIdentifier  An immutable reference to the
 *                                         equalizer band identifier
 *                                         corresponding to the
 *                                         equalizer band model to get.
 *  @param[out]  aEqualizerBandModel       A reference to a pointer for
 *                                         a mutable equalizer band
 *                                         model, set to the requested
 *                                         band model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer bands model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 */
Status
ZoneModel :: GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel)
{
    Status lRetval = kStatus_Success;


    lRetval = mSoundModel.GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer band model associated with the
 *    specified equalizer band identifier.
 *
 *  This attempts to get the equalizer band model associated with the
 *  specified equalizer band identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerBandIdentifier  An immutable reference to the
 *                                         equalizer band identifier
 *                                         corresponding to the
 *                                         equalizer band model to get.
 *  @param[out]  aEqualizerBandModel       A reference to a pointer for
 *                                         an immutable equalizer band
 *                                         model, set to the requested
 *                                         band model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer bands model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 */
Status
ZoneModel :: GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model equalizer preset value.
 *
 *  This attempts to get the model equalizer preset value, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aEqualizerPresetIdentifier    A mutable reference
 *                                             to storage for the
 *                                             model preset value, if
 *                                             successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model equalizer preset
 *                                  value has not been initialized
 *                                  with a known value.
 *
 */
Status
ZoneModel :: GetEqualizerPreset(EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetEqualizerPreset(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model equalizer highpass crossover filter
 *    model.
 *
 *  This attempts to get the model equalizer highpass crossover filter
 *  model.
 *
 *  @param[out]  aHighpassCrossoverModel   A reference to a pointer for
 *                                         a mutable equalizer
 *                                         highpass crossover filter
 *                                         model, if successful.
 *
 *  @retval  kStatus_Success        Unconditionally.
 *
 */
Status
ZoneModel :: GetHighpassCrossover(CrossoverModel *&aHighpassCrossoverModel)
{
    return (mSoundModel.GetHighpassCrossover(aHighpassCrossoverModel));
}

/**
 *  @brief
 *    Attempt to get the model equalizer highpass crossover filter
 *    model.
 *
 *  This attempts to get the model equalizer highpass crossover filter
 *  model.
 *
 *  @param[out]  aHighpassCrossoverModel  A reference to a pointer for
 *                                        an immutable equalizer
 *                                        highpass crossover filter
 *                                        model, if successful.
 *
 *  @retval  kStatus_Success        Unconditionally.
 *
 */
Status
ZoneModel :: GetHighpassCrossover(const CrossoverModel *&aHighpassCrossoverModel) const
{
    return (mSoundModel.GetHighpassCrossover(aHighpassCrossoverModel));
}

/**
 *  @brief
 *    Attempt to get the model equalizer highpass filter crossover
 *    frequency.
 *
 *  This attempts to get the model equalizer highpass filter crossover
 *  frequency, if it has been previously initialized or set.
 *
 *  @param[out]  aHighpassFrequency  A mutable reference to storage
 *                                   for the equalizer highpass filter
 *                                   crossover frequency, if
 *                                   successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer highpass filter
 *                                  crossover frequency value has not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
ZoneModel :: GetHighpassFrequency(CrossoverModel::FrequencyType &aHighpassFrequency) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetHighpassFrequency(aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model equalizer lowpass crossover filter
 *    model.
 *
 *  This attempts to get the model equalizer lowpass crossover filter
 *  model.
 *
 *  @param[out]  aLowpassCrossoverModel  A reference to a pointer for
 *                                       a mutable equalizer lowpass
 *                                       crossover filter model, if
 *                                       successful.
 *
 *  @retval  kStatus_Success        Unconditionally.
 *
 */
Status
ZoneModel :: GetLowpassCrossover(CrossoverModel *&aLowpassCrossoverModel)
{
    return (mSoundModel.GetLowpassCrossover(aLowpassCrossoverModel));
}

/**
 *  @brief
 *    Attempt to get the model equalizer lowpass crossover filter
 *    model.
 *
 *  This attempts to get the model equalizer lowpass crossover filter
 *  model.
 *
 *  @param[out]  aLowpassCrossoverModel  A reference to a pointer for
 *                                       an immutable equalizer
 *                                       lowpass crossover filter
 *                                       model, if successful.
 *
 *  @retval  kStatus_Success        Unconditionally.
 *
 */
Status
ZoneModel :: GetLowpassCrossover(const CrossoverModel *&aLowpassCrossoverModel) const
{
    return (mSoundModel.GetLowpassCrossover(aLowpassCrossoverModel));
}

/**
 *  @brief
 *    Attempt to get the model equalizer lowpass filter crossover
 *    frequency.
 *
 *  This attempts to get the model equalizer lowpass filter crossover
 *  frequency, if it has been previously initialized or set.
 *
 *  @param[out]  aLowpassFrequency  A mutable reference to storage
 *                                  for the equalizer highpass filter
 *                                  crossover frequency, if
 *                                  successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer lowpass filter
 *                                  crossover frequency value has not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
ZoneModel :: GetLowpassFrequency(CrossoverModel::FrequencyType &aLowpassFrequency) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetLowpassFrequency(aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model equalizer sound mode.
 *
 *  This attempts to get the model equalizer sound mode, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aSoundMode  A mutable reference to storage
 *                           for the model equalizer sound mode, if
 *                           successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model equalizer sound mode
 *                                  value has not been initialized
 *                                  with a known value.
 *
 */
Status
ZoneModel :: GetSoundMode(SoundMode &aSoundMode) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetSoundMode(aSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the source (input) identifier associated with this zone
 *    model.
 *
 *  @param[out]  aSourceIdentifier  A mutable reference to storage
 *                                  for the model source (input)
 *                                  identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zone source (input)
 *                                  has not been initialized with a
 *                                  known value(s).
 *
 */
Status
ZoneModel :: GetSource(SourceModel::IdentifierType &aSourceIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSourceIdentifier.GetIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer bass and treble levels.
 *
 *  This attempts to get the model tone equalizer bass and treble
 *  levels, if they have been previously initialized or set.
 *
 *  @param[out]  aBass    A mutable reference to storage for the
 *                        model tone equalizer bass level, if
 *                        successful.
 *  @param[out]  aTreble  A mutable reference to storage for the
 *                        model tone equalizer treble level, if
 *                        successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer bass
 *                                  or treble level values have not
 *                                  been initialized with a known
 *                                  value.
 *
 */
Status
ZoneModel :: GetTone(ToneModel::LevelType &aBass, ToneModel::LevelType &aTreble) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetTone(aBass, aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model tone equalizer treble level.
 *
 *  This attempts to get the model tone equalizer treble level, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aTreble  A mutable reference to storage for the
 *                        model tone equalizer treble level, if
 *                        successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model tone equalizer treble
 *                                  level value has not been
 *                                  initialized with a known value.
 *
 */
Status
ZoneModel :: GetTreble(ToneModel::LevelType &aTreble) const
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.GetTreble(aTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model volume fixed/locked state.
 *
 *  This attempts to get the model volume fixed/locked state, if it
 *  has been previously initialized or set.
 *
 *  @param[out]  aVolumeFixed  A mutable reference to storage for the
 *                             model volume fixed/locked state, if
 *                             successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the model volume fixed state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa SetVolumeFixed
 *
 *  @ingroup volume
 *
 */
Status
ZoneModel :: GetVolumeFixed(VolumeFixedType &aVolumeFixed) const
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.GetFixed(aVolumeFixed);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the model bass level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the model bass level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutBass  A mutable reference to storage for the
 *                         resulting tone equalizer bass level, if
 *                         successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum bass level.
 *
 */
Status
ZoneModel :: DecreaseBass(ToneModel::LevelType &aOutBass)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.DecreaseBass(aOutBass);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the model treble level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to decrease the model treble level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutTreble  A mutable reference to storage for the
 *                           resulting tone equalizer treble level, if
 *                           successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum treble level.
 *
 */
Status
ZoneModel :: DecreaseTreble(ToneModel::LevelType &aOutTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.DecreaseTreble(aOutTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the left channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the left channel.
 *
 *  @param[out]  aOutBalance   A mutable reference to storage for the
 *                             resulting stereophonic channel balance,
 *                             if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a balance that would exceed the
 *                                  maximum left channel bias.
 *
 *  @sa SetBalance
 *
 */
Status
ZoneModel :: IncreaseBalanceLeft(BalanceType &aOutBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = mBalance.IncreaseBalanceLeft(aOutBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the zone stereophonic channel balance by one (1)
 *    towards the right channel.
 *
 *  This attempts to increase the zone stereophonic channel balance by
 *  one (1) unit towards the right channel.
 *
 *  @param[out]  aOutBalance   A mutable reference to storage for the
 *                             resulting stereophonic channel balance,
 *                             if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the balance model balance value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a balance that would exceed the
 *                                  maximum right channel bias.
 *
 *  @sa SetBalance
 *
 */
Status
ZoneModel :: IncreaseBalanceRight(BalanceType &aOutBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = mBalance.IncreaseBalanceRight(aOutBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the model bass level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the model bass level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutBass  A mutable reference to storage for the
 *                         resulting tone equalizer bass level, if
 *                         successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum bass level.
 *
 */
Status
ZoneModel :: IncreaseBass(ToneModel::LevelType &aOutBass)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.IncreaseBass(aOutBass);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the model treble level of the tone equalizer by one (1)
 *    unit.
 *
 *  This attempts to increase the model treble level of the tone
 *  equalizer by one (1) unit.
 *
 *  @param[out]  aOutTreble  A mutable reference to storage for the
 *                           resulting tone equalizer treble level, if
 *                           successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          The attempted adjustment would result
 *                            in a level that would exceed the
 *                            maximum treble level.
 *
 */
Status
ZoneModel :: IncreaseTreble(ToneModel::LevelType &aOutTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.IncreaseTreble(aOutTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model stereophonic channel balance.
 *
 *  This initializes the model with the specified stereophonic channel
 *  balance.
 *
 *  @param[in]  aBalance  An immutable reference to the stereophonic
 *                        channel balance to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBalance value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aBalance value
 *                                    is out of range.
 *
 */
Status
ZoneModel :: SetBalance(const BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = mBalance.SetBalance(aBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer bass level.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  bass level.
 *
 *  @param[in]  aBass  An immutable reference to the tone
 *                     equalizer bass level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBass value has
 *                                    already been set.
 *  @retval  -ERANGE                  The specified @a aBass value is
 *                                    out of range.
 *
 */
Status
ZoneModel :: SetBass(const ToneModel::LevelType &aBass)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetBass(aBass);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model equalizer preset identifier value.
 *
 *  This attempts to set the model with the specified equalizer preset
 *  identifier value.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to
 *                                          the equalizer preset
 *                                          identifier value to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a
 *                                    aEqualizerPresetIdentifier value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aEqualizerPresetIdentifier value
 *                                    is out of range.
 *
 */
Status
ZoneModel :: SetEqualizerPreset(const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetEqualizerPreset(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the model crossover frequency of the equalizer highpass
 *    filter to the specified frequency.
 *
 *  This attempts to set the model crossover frequency of the
 *  equalizer highpass filter to the specified frequency.
 *
 *  @param[in]  aHighpassFrequency  An immutable reference to the
 *                                  highpass crossover frequency
 *                                  to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aHighpassFrequency
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aHighpassFrequency
 *                                    value is out of range.
 *
 */
Status
ZoneModel :: SetHighpassFrequency(const CrossoverModel::FrequencyType &aHighpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetHighpassFrequency(aHighpassFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Set the model crossover frequency of the equalizer lowpass
 *    filter to the specified frequency.
 *
 *  This attempts to set the model crossover frequency of the
 *  equalizer lowpass filter to the specified frequency.
 *
 *  @param[in]  aLowpassFrequency  An immutable reference to the
 *                                 lowpass crossover frequency
 *                                 to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLowpassFrequency
 *                                    value has already been set.
 *  @retval  -ERANGE                  The specified @a aLowpassFrequency
 *                                    value is out of range.
 *
 */
Status
ZoneModel :: SetLowpassFrequency(const CrossoverModel::FrequencyType &aLowpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetLowpassFrequency(aLowpassFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model equalizer sound mode.
 *
 *  This attempts to set the model with the specified equalizer sound
 *  mode.
 *
 *  @param[in]  aSoundMode  An immutable reference to the
 *                          equalizer sound mode to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aSoundMode value
 *                                    has already been set.
 *  @retval  -EINVAL                  The specified @a aSoundMode value
 *                                    is invalid.
 *
 */
Status
ZoneModel :: SetSoundMode(const SoundMode &aSoundMode)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetSoundMode(aSoundMode);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate a source (input) identifier with the
 *    zone model.
 *
 *  This attempts to associate the source (input)
 *  identifier with the zone model.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the source
 *                                 (input) identifier to associate
 *                                 with the zone model.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aSourceIdentifier
 *                                    value has already been set.
 *
 */
Status
ZoneModel :: SetSource(const SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mSourceIdentifier.SetIdentifier(aSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer bass and treble levels.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  bass and treble levels.
 *
 *  @param[in]  aBass    An immutable reference to the tone
 *                       equalizer bass level to set.
 *  @param[in]  aTreble  An immutable reference to the tone
 *                       equalizer treble level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aBass or @a
 *                                    aTreble value has already been
 *                                    set.
 *  @retval  -ERANGE                  The specified @a aBass or @a
 *                                    aTreble value is out of range.
 *
 */
Status
ZoneModel :: SetTone(const ToneModel::LevelType &aBass, const ToneModel::LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetTone(aBass, aTreble);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to set the model tone equalizer treble level.
 *
 *  This attempts to set the model with the specified tone equalizer
 *  treble level.
 *
 *  @param[in]  aTreble  An immutable reference to the tone
 *                       equalizer treble level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aTreble value has
 *                                    already been set.
 *  @retval  -ERANGE                  The specified @a aTreble value is
 *                                    out of range.
 *
 */
Status
ZoneModel :: SetTreble(const ToneModel::LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = mSoundModel.SetTreble(aTreble);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model volume fixed/locked state.
 *
 *  This attempts to set the model with the specified volume
 *  fixed/locked state.
 *
 *  @param[in]  aVolumeFixed  An immutable reference to the model
 *                            volume fixed/locked state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aVolumeFixed
 *                                    value has already been set.
 *
 *  @ingroup volume
 *
 */
Status
ZoneModel :: SetVolumeFixed(const VolumeFixedType &aVolumeFixed)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.SetFixed(aVolumeFixed);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided zone model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aZoneModel  An immutable reference to the zone
 *                          model to compare for equality.
 *
 *  @returns
 *    True if this zone model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool ZoneModel :: operator ==(const ZoneModel &aZoneModel) const
{
    return (OutputModelBasis::operator ==(aZoneModel)                   &&
            (mBalance                  == aZoneModel.mBalance         ) &&
            (mSoundModel               == aZoneModel.mSoundModel      ) &&
            (mSourceIdentifier         == aZoneModel.mSourceIdentifier));
}

}; // namespace Model

}; // namespace HLX
