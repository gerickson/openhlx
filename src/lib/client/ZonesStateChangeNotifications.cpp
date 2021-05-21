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
 *      This file implements derived objects for a HLX client zone object
 *      data model state change notifications (SCNs).
 *
 */

#include "ZonesStateChangeNotifications.hpp"

#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


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
 *    This is the class initializer.
 *
 *  This initializes the zone state change notification with the
 *  specified type and zone identifier.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose state
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesNotificationBasis :: Init(const Type &aType,
                               const IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = NotificationBasis::Init(aType);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = IdentifierNotificationBasis::Init(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone stereophonic channel balance property
 *  state change notification with the specified stereophonic channel
 *  balance state and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose
 *                                          stereophonic channel
 *                                          balance property state
 *                                          changed.
 *  @param[in]  aBalance                    An immutable reference to
 *                                          the stereophonic channel
 *                                          balance state that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesBalanceNotification :: Init(const IdentifierType &aZoneIdentifier,
                                 const BalanceType &aBalance)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneBalance,
                                           aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mBalance = aBalance;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change stereophonic channel balance property.
 *
 *  @returns
 *    The stereophonic channel balance of the zone whose state
 *    changed.
 *
 */
ZonesBalanceNotification::BalanceType
ZonesBalanceNotification :: GetBalance(void) const
{
    return (mBalance);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone equalizer band level property state
 *  change notification with the specified equalizer band identifer
 *  and level and zone equalizer identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone equalizer object
 *                                          whose band level property
 *                                          state changed.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference to
 *                                          the specific equalizer
 *                                          band identifier whose band
 *                                          level property state
 *                                          changed.
 *  @param[in]  aEqualizerBandLevel         An immutable reference to the
 *                                          equalizer band level that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesEqualizerBandNotification :: Init(const ZoneModel::IdentifierType &aZoneIdentifier,
                                       const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                       const EqualizerBandModel::LevelType &aEqualizerBandLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneEqualizerBand,
                                           aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandNotificationBasis::Init(aEqualizerBandIdentifier,
                                                   aEqualizerBandLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone equalizer preset property state change
 *  notification with the specified equalizer preset and zone
 *  identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose
 *                                          equalizer preset property
 *                                          state changed.
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to
 *                                          the equalizer preset that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesEqualizerPresetNotification :: Init(const ZoneModel::IdentifierType &aZoneIdentifier,
                                         const EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneEqualizerPreset, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mEqualizerPresetIdentifier = aEqualizerPresetIdentifier;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change equalizer preset property.
 *
 *  @returns
 *    The equalizer preset property of the zone whose state changed.
 *
 */
EqualizerPresetModel::IdentifierType
ZonesEqualizerPresetNotification :: GetEqualizerPreset(void) const
{
    return (mEqualizerPresetIdentifier);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes a zone filter crossover frequency property state
 *  change notification with the specified filter crossover frequency
 *  and zone identifier.
 *
 *  @param[in]  aType                       An immutable reference to
 *                                          the state change
 *                                          notification type to
 *                                          initialize with. This
 *                                          indicates what object
 *                                          class and what property
 *                                          within that object class
 *                                          changed.
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose filter
 *                                          crossover frequency
 *                                          property state changed.
 *  @param[in]  aFrequency                  An immutable reference to
 *                                          the filter crossover
 *                                          frequency that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesCrossoverNotificationBasis :: Init(const Type &aType,
                                        const ZoneModel::IdentifierType &aZoneIdentifier,
                                        const CrossoverModel::FrequencyType &aFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(aType, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mFrequency = aFrequency;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change crossover filter frequency property.
 *
 *  @returns
 *    The crossover filter frequency of the zone whose state changed.
 *
 */
CrossoverModel::FrequencyType
ZonesCrossoverNotificationBasis :: GetFrequency(void) const
{
    return (mFrequency);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone highpass filter crossover frequency
 *  property state change notification with the specified highpass
 *  filter crossover frequency and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose highpass
 *                                          filter crossover frequency
 *                                          property state changed.
 *  @param[in]  aHighpassFrequency          An immutable reference to
 *                                          the highpass filter
 *                                          crossover frequency that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesHighpassCrossoverNotification :: Init(const ZoneModel::IdentifierType &aZoneIdentifier,
                                           const CrossoverModel::FrequencyType &aHighpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesCrossoverNotificationBasis::Init(kStateChangeType_ZoneHighpassCrossover,
                                                    aZoneIdentifier,
                                                    aHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone lowpass filter crossover frequency
 *  property state change notification with the specified lowpass
 *  filter crossover frequency and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose lowpass
 *                                          filter crossover frequency
 *                                          property state changed.
 *  @param[in]  aLowpassFrequency           An immutable reference to
 *                                          the lowpass filter
 *                                          crossover frequency that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesLowpassCrossoverNotification :: Init(const ZoneModel::IdentifierType &aZoneIdentifier,
                                          const CrossoverModel::FrequencyType &aLowpassFrequency)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesCrossoverNotificationBasis::Init(kStateChangeType_ZoneLowpassCrossover,
                                                    aZoneIdentifier,
                                                    aLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone volume mute property state change
 *  notification with the specified volume mute state and zone
 *  identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose volume
 *                                          mute property state
 *                                          changed.
 *  @param[in]  aMute                       An immutable reference to
 *                                          the volume mute state that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesMuteNotification :: Init(const IdentifierType &aZoneIdentifier, const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneMute, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = MuteNotificationBasis::Init(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone name property state change notification
 *  with the specified name extent and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose name
 *                                          property state changed.
 *  @param[in]  aName                       A pointer to the start of
 *                                          the null- terminated C
 *                                          string name that changed.
 *  @param[in]  aNameLength                 An immutable reference to
 *                                          the length, in bytes, of
 *                                          @a aName.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesNameNotification :: Init(const IdentifierType &aZoneIdentifier,
                              const char *aName,
                              const size_t &aNameLength)
{
    std::string lName;

    lName.assign(aName, aNameLength);

    return (Init(aZoneIdentifier, lName));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone name property state change notification
 *  with the specified name and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose name
 *                                          property state changed.
 *  @param[in]  aName                       An immutable reference to
 *                                          the name that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesNameNotification :: Init(const IdentifierType &aZoneIdentifier,
                              const std::string &aName)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneName, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = NameNotificationBasis::Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone equalizer sound mode property state
 *  change notification with the specified sound mode and zone
 *  identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose
 *                                          equalizer sound mode
 *                                          property state changed.
 *  @param[in]  aSoundMode                  An immutable reference to
 *                                          the equalizer sound mode
 *                                          that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesSoundModeNotification :: Init(const IdentifierType &aZoneIdentifier,
                                   const SoundMode &aSoundMode)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneSoundMode,
                                           aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mSoundMode = aSoundMode;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change equalizer sound mode property.
 *
 *  @returns
 *    The equalizer sound mode of the zone whose state changed.
 *
 */
ZonesSoundModeNotification::SoundMode
ZonesSoundModeNotification :: GetSoundMode(void) const
{
    return (mSoundMode);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone source (input) property state change
 *  notification with the specified source identifier and zone
 *  identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose source
 *                                          property state changed.
 *  @param[in]  aSourceIdentifier           An immutable reference to
 *                                          the source identifier that
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesSourceNotification :: Init(const ZoneModel::IdentifierType &aZoneIdentifier,
                                const SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneSource, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SourceNotificationBasis::Init(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the zone tone equalizer bass and treble levels
 *  property state change notification with the specified bass and
 *  treble levels and zone identifier.
 *
 *  @note
 *    The HLX server batches the bass and treble properties
 *    together. On receipt of this state change, at least one, but
 *    possibly both, levels have changed.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose tone
 *                                          equalizer bass and/or
 *                                          treble levels property
 *                                          state(s) changed.
 *  @param[in]  aBass                       An immutable reference to
 *                                          the bass level that
 *                                          changed (if at all).
 *  @param[in]  aTreble                     An immutable reference to the
 *                                          treble level that changed
 *                                          (if at all).
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesToneNotification :: Init(const IdentifierType &aZoneIdentifier, const LevelType &aBass, const LevelType &aTreble)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneTone, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mBass   = aBass;
    mTreble = aTreble;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change tone equalizer bass level property.
 *
 *  @returns
 *    The tone equalizer bass level of the zone whose state changed.
 *
 */
ZonesToneNotification :: LevelType
ZonesToneNotification :: GetBass(void) const
{
    return (mBass);
}

/**
 *  @brief
 *    Return the state change tone equalizer treble level property.
 *
 *  @returns
 *    The tone equalizer treble level of the zone whose state changed.
 *
 */
ZonesToneNotification :: LevelType
ZonesToneNotification :: GetTreble(void) const
{
    return (mTreble);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone volume level property state change
 *  notification with the specified volume level state and zone
 *  identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose volume
 *                                          level property state
 *                                          changed.
 *  @param[in]  aLevel                      An immutable reference to
 *                                          the volume level state
 *                                          that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesVolumeNotification :: Init(const IdentifierType &aZoneIdentifier,
                                const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneVolume, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = VolumeNotificationBasis::Init(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the zone volume fixed/locked state property state
 *  change notification with the specified volume fixed/locked state
 *  and zone identifier.
 *
 *  @param[in]  aZoneIdentifier             An immutable reference
 *                                          to the identifier of the
 *                                          zone object whose volume
 *                                          fixed/locked property
 *                                          state changed.
 *  @param[in]  aLocked                     An immutable reference to
 *                                          the volume fixed/locked
 *                                          state that changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesVolumeLockedNotification :: Init(const IdentifierType &aZoneIdentifier,
                                      const VolumeModel::FixedType &aLocked)
{
    Status lRetval = kStatus_Success;

    lRetval = ZonesNotificationBasis::Init(kStateChangeType_ZoneVolumeLocked,
                                           aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    mLocked = aLocked;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the state change volume fixed/locked state property.
 *
 *  @returns
 *    The volume fixed/locked state of the class object whose state
 *    changed.
 *
 */
VolumeModel::FixedType
ZonesVolumeLockedNotification :: GetLocked(void) const
{
    return (mLocked);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
