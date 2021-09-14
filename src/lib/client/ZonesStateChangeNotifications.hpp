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
 *      This file defines derived objects for a HLX client zone object
 *      data model state change notifications (SCNs).
 *
 */

#ifndef OPENHLXCLIENTZONESSTATECHANGENOTIFICATIONS_HPP
#define OPENHLXCLIENTZONESSTATECHANGENOTIFICATIONS_HPP

#include <memory>

#include <OpenHLX/Client/EqualizerBandStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/IdentifierStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/NameStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/SourceStateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationBasis.hpp>
#include <OpenHLX/Client/StateChangeNotificationTypes.hpp>
#include <OpenHLX/Client/VolumeStateChangeNotificationBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/BalanceModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A derivable object for a HLX client zone object data model state
 *    change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesNotificationBasis :
    public NotificationBasis,
    public IdentifierNotificationBasis
{
public:
    virtual ~ZonesNotificationBasis(void) = default;

protected:
    ZonesNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const IdentifierType &aZoneIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone object data model stereophonic
 *    channel balance property state change notification (SCN).
 *
 *  @ingroup balance
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesBalanceNotification :
    public ZonesNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a BalanceType from the
     *  balance model.
     *
     */
    typedef Model::BalanceModel::BalanceType BalanceType;

public:
    ZonesBalanceNotification(void) = default;
    virtual ~ZonesBalanceNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const BalanceType &aBalance);

    BalanceType GetBalance(void) const;

private:
    BalanceType mBalance;
};

/**
 *  @brief
 *    An object for a HLX client zone object sound data model
 *    equalizer band level property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesEqualizerBandNotification :
    public ZonesNotificationBasis,
    public EqualizerBandNotificationBasis
{
public:
    ZonesEqualizerBandNotification(void) = default;
    virtual ~ZonesEqualizerBandNotification(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                        const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                        const Model::EqualizerBandModel::LevelType &aEqualizerBandLevel);
};

/**
 *  @brief
 *    An object for a HLX client zone object sound data model
 *    equalizer preset property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesEqualizerPresetNotification :
    public ZonesNotificationBasis
{
public:
    ZonesEqualizerPresetNotification(void) = default;
    virtual ~ZonesEqualizerPresetNotification(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                        const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    Model::EqualizerPresetModel::IdentifierType GetEqualizerPreset(void) const;

private:
    Model::EqualizerPresetModel::IdentifierType  mEqualizerPresetIdentifier;
};

/**
 *  @brief
 *    A derivable object for a HLX client zone object sound data model
 *    crossover frequency property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesCrossoverNotificationBasis :
    public ZonesNotificationBasis
{
public:
    virtual ~ZonesCrossoverNotificationBasis(void) = default;

    Model::CrossoverModel::FrequencyType GetFrequency(void) const;

protected:
    ZonesCrossoverNotificationBasis(void) = default;

    Common::Status Init(const Type &aType, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aFrequency);

private:
    Model::CrossoverModel::FrequencyType  mFrequency;
};

/**
 *  @brief
 *    An object for a HLX client zone object sound data model highpass
 *    crossover frequency property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesHighpassCrossoverNotification :
    public ZonesCrossoverNotificationBasis
{
public:
    ZonesHighpassCrossoverNotification(void) = default;
    virtual ~ZonesHighpassCrossoverNotification(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aHighpassFrequency);
};

/**
 *  @brief
 *    An object for a HLX client zone object sound data model lowpass
 *    crossover frequency property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesLowpassCrossoverNotification :
    public ZonesCrossoverNotificationBasis
{
public:
    ZonesLowpassCrossoverNotification(void) = default;
    virtual ~ZonesLowpassCrossoverNotification(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::CrossoverModel::FrequencyType &aLowpassFrequency);
};

/**
 *  @brief
 *    An object for a HLX client zone object volume mute data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesMuteNotification :
    public ZonesNotificationBasis,
    public MuteNotificationBasis
{
public:
    ZonesMuteNotification(void) = default;
    virtual ~ZonesMuteNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const MuteType &aMute);
};

/**
 *  @brief
 *    An object for a HLX client zone object name data model property
 *    state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesNameNotification :
    public ZonesNotificationBasis,
    public NameNotificationBasis
{
public:
    ZonesNameNotification(void) = default;
    virtual ~ZonesNameNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const char *aName, const size_t &aNameLength);
    Common::Status Init(const IdentifierType &aZoneIdentifier, const std::string &aName);
};

/**
 *  @brief
 *    An object for a HLX client zone object sound mode data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesSoundModeNotification :
    public ZonesNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a SoundMode from the
     *  sound model.
     *
     */
    typedef Model::SoundModel::SoundMode SoundMode;

public:
    ZonesSoundModeNotification(void) = default;
    virtual ~ZonesSoundModeNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const SoundMode &aSoundMode);

    SoundMode GetSoundMode(void) const;

private:
    SoundMode mSoundMode;
};

/**
 *  @brief
 *    An object for a HLX client zone object source data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesSourceNotification :
    public ZonesNotificationBasis,
    public SourceNotificationBasis
{
public:
    ZonesSourceNotification(void) = default;
    virtual ~ZonesSourceNotification(void) = default;

    Common::Status Init(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
};

/**
 *  @brief
 *    An object for a HLX client zone object sound data model tone
 *    properties state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup zone
 *  @ingroup state-change
 *
 */
class ZonesToneNotification :
    public ZonesNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a LevelType from the
     *  tone model.
     *
     */
    typedef Model::ToneModel::LevelType LevelType;

public:
    ZonesToneNotification(void) = default;
    virtual ~ZonesToneNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const LevelType &aBass, const LevelType &aTreble);

    LevelType GetBass(void) const;
    LevelType GetTreble(void) const;

private:
    LevelType  mBass;
    LevelType  mTreble;
};

/**
 *  @brief
 *    An object for a HLX client zone object volume level data model
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class ZonesVolumeNotification :
    public ZonesNotificationBasis,
    public VolumeNotificationBasis
{
public:
    ZonesVolumeNotification(void) = default;
    virtual ~ZonesVolumeNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const LevelType &aVolume);
};

/**
 *  @brief
 *    An object for a HLX client zone object volume fixed/locked data
 *    model property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class ZonesVolumeLockedNotification :
    public ZonesNotificationBasis
{
public:
    ZonesVolumeLockedNotification(void) = default;
    virtual ~ZonesVolumeLockedNotification(void) = default;

    Common::Status Init(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::FixedType &aLocked);

    Model::VolumeModel::FixedType GetLocked(void) const;

private:
    Model::VolumeModel::FixedType  mLocked;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTZONESSTATECHANGENOTIFICATIONS_HPP
