/*
 *    Copyright (c) 2020-2022 Grant Erickson
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
 *      This file implements an object for managing the server-side
 *      observation and mutation of a collection of HLX zones.
 *
 */

#include "ZonesController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>
#include <OpenHLX/Model/ToneModel.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <EqualizerBandModelDefaults.hpp>
#include "EqualizerPresetsController.hpp"
#include <NameModelDefaults.hpp>
#include "SourcesController.hpp"
#include <Utilities.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

static const BalanceModel::BalanceType             kBalanceDefault           = BalanceModel::kBalanceCenter;
static const SoundModel::SoundMode                 kSoundModeDefault         = SoundModel::kSoundModeDisabled;
static const EqualizerBandModel::LevelType         kEqualizerBandDefault     = EqualizerBandModel::kLevelFlat;
static const EqualizerPresetModel::IdentifierType  kEqualizerPresetDefault   = IdentifierModel::kIdentifierMin;
static const ToneModel::LevelType                  kBassDefault              = ToneModel::kLevelFlat;
static const ToneModel::LevelType                  kTrebleDefault            = ToneModel::kLevelFlat;
static const CrossoverModel::FrequencyType         kLowpassFrequencyDefault  = 100;
static const CrossoverModel::FrequencyType         kHighpassFrequencyDefault = 100;
static const SourceModel::IdentifierType           kSourceDefault            = IdentifierModel::kIdentifierMin;
static const VolumeModel::MuteType                 kMuteDefault              = true;
static const VolumeModel::FixedType                kVolumeFixedDefault       = false;
static const VolumeModel::LevelType                kVolumeDefault            = VolumeModel::kLevelMin;

/**
 *  @brief
 *    A object for representing default data for a HLX DSP equalizer
 *    crossover filter data model.
 *
 *  @private
 *
 */
struct CrossoverModelDefaults
{
    CrossoverModel::FrequencyType  mFrequency;
};

/**
 *  @brief
 *    A object for representing default data for a HLX DSP equalizer
 *    tone data model.
 *
 *  @private
 *
 */
struct ToneModelDefaults
{
    ToneModel::LevelType                  mBass;
    ToneModel::LevelType                  mTreble;
};

/**
 *  @brief
 *    A object for representing default data for a HLX DSP sound data
 *    model.
 *
 *  @private
 *
 */
struct SoundModelDefaults
{
    SoundModel::SoundMode                 mSoundMode;
    Defaults::EqualizerBandModel          mZoneEqualizerBands[EqualizerBandsModel::kEqualizerBandsMax];
    EqualizerPresetModel::IdentifierType  mEqualizerPresetIdentifier;
    ToneModelDefaults                     mToneModel;
    CrossoverModelDefaults                mLowpassCrossover;
    CrossoverModelDefaults                mHighpassCrossover;
};

/**
 *  @brief
 *    A object for representing default data for a HLX zone data
 *    model.
 *
 *  @private
 *
 */
struct ZoneModelDefaults
{
    Defaults::NameModel         mName;
    BalanceModel::BalanceType   mBalance;
    SoundModelDefaults          mSoundModel;
    SourceModel::IdentifierType mSource;
    VolumeModel::MuteType       mMute;
    VolumeModel::LevelType      mVolume;
    VolumeModel::FixedType      mVolumeFixed;
};

static const ZoneModelDefaults kZoneModelDefaults[24] = {
    { { "Zone Name 1"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 2"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 3"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 4"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 5"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 6"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 7"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 8"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 9"  }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 10" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 11" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 12" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 13" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 14" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 15" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 16" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 17" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 18" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 19" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 20" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 21" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 22" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 23" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault },
    { { "Zone Name 24" }, kBalanceDefault, { kSoundModeDefault,
                                             {
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault },
                                                 { kEqualizerBandDefault }
                                             },
                                             kEqualizerPresetDefault,
                                             { kBassDefault, kTrebleDefault },
                                             { kLowpassFrequencyDefault },
                                             { kHighpassFrequencyDefault }, },
      kSourceDefault, kMuteDefault, kVolumeDefault, kVolumeFixedDefault }
};

static CFStringRef kBalanceSchemaKey               = CFSTR("Balance");
static CFStringRef kBassSchemaKey                  = CFSTR("Bass");
static CFStringRef kEqualizerLevelsPresetSchemaKey = CFSTR("Equalizer Levels");
static CFStringRef kEqualizerPresetSchemaKey       = CFSTR("Equalizer Preset");
static CFStringRef kHighpassFrequencySchemaKey     = CFSTR("Highpass Crossover Frequency");
static CFStringRef kLowpassFrequencySchemaKey      = CFSTR("Lowpass Crossover Frequency");
static CFStringRef kNameSchemaKey                  = CFSTR("Name");
static CFStringRef kSoundModeDictionarySchemaKey   = CFSTR("Sound Mode");
static CFStringRef kSoundModeValueSchemaKey        = CFSTR("Mode");
static CFStringRef kSourceSchemaKey                = CFSTR("Source");
static CFStringRef kTrebleSchemaKey                = CFSTR("Treble");
static CFStringRef kVolumeSchemaKey                = CFSTR("Volume");
static CFStringRef kVolumeLevelSchemaKey           = CFSTR("Level");
static CFStringRef kVolumeLockedSchemaKey          = CFSTR("Locked");
static CFStringRef kVolumeMutedSchemaKey           = CFSTR("Muted");
static CFStringRef kZonesSchemaKey                 = CFSTR("Zones");

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesController :: ZonesController(void) :
    Common::ZonesControllerBasis(),
    Server::ZonesControllerBasis(Common::ZonesControllerBasis::mZones,
                                 Common::ZonesControllerBasis::kZonesMax),
    Simulator::ContainerControllerBasis(),
    Simulator::ObjectControllerBasis()
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ZonesController :: ~ZonesController(void)
{
    return;
}

Status
ZonesController :: DoRequestHandlers(const bool &aRegister)
{
    static const RequestHandlerBasis  lRequestHandlers[] = {
        {
            kAdjustBalanceRequest,
            ZonesController::AdjustBalanceRequestReceivedHandler
        },

        {
            kDecreaseBassRequest,
            ZonesController::DecreaseBassRequestReceivedHandler
        },

        {
            kIncreaseBassRequest,
            ZonesController::IncreaseBassRequestReceivedHandler
        },

        {
            kDecreaseTrebleRequest,
            ZonesController::DecreaseTrebleRequestReceivedHandler
        },

        {
            kIncreaseTrebleRequest,
            ZonesController::IncreaseTrebleRequestReceivedHandler
        },

        {
            kDecreaseEqualizerBandRequest,
            ZonesController::DecreaseEqualizerBandRequestReceivedHandler
        },

        {
            kIncreaseEqualizerBandRequest,
            ZonesController::IncreaseEqualizerBandRequestReceivedHandler
        },

        {
            kDecreaseVolumeRequest,
            ZonesController::DecreaseVolumeRequestReceivedHandler
        },

        {
            kIncreaseVolumeRequest,
            ZonesController::IncreaseVolumeRequestReceivedHandler
        },

        {
            kMuteRequest,
            ZonesController::MuteRequestReceivedHandler
        },

        {
            kQueryRequest,
            ZonesController::QueryRequestReceivedHandler
        },

        {
            kQueryMuteRequest,
            ZonesController::QueryMuteRequestReceivedHandler
        },

        {
            kQuerySourceRequest,
            ZonesController::QuerySourceRequestReceivedHandler
        },

        {
            kQueryVolumeRequest,
            ZonesController::QueryVolumeRequestReceivedHandler
        },

        {
            kSetBalanceRequest,
            ZonesController::SetBalanceRequestReceivedHandler
        },

        {
            kSetEqualizerBandRequest,
            ZonesController::SetEqualizerBandRequestReceivedHandler
        },

        {
            kSetEqualizerPresetRequest,
            ZonesController::SetEqualizerPresetRequestReceivedHandler
        },

        {
            kSetHighpassCrossoverRequest,
            ZonesController::SetHighpassCrossoverRequestReceivedHandler
        },

        {
            kSetLowpassCrossoverRequest,
            ZonesController::SetLowpassCrossoverRequestReceivedHandler
        },

        {
            kSetNameRequest,
            ZonesController::SetNameRequestReceivedHandler
        },

        {
            kSetSoundModeRequest,
            ZonesController::SetSoundModeRequestReceivedHandler
        },

        {
            kSetSourceRequest,
            ZonesController::SetSourceRequestReceivedHandler
        },

        {
            kSetSourceAllRequest,
            ZonesController::SetSourceAllRequestReceivedHandler
        },

        {
            kSetToneRequest,
            ZonesController::SetToneRequestReceivedHandler
        },

        {
            kSetVolumeRequest,
            ZonesController::SetVolumeRequestReceivedHandler
        },

        {
            kSetVolumeAllRequest,
            ZonesController::SetVolumeAllRequestReceivedHandler
        },

        {
            kSetVolumeFixedRequest,
            ZonesController::SetVolumeFixedRequestReceivedHandler
        },

        {
            kToggleMuteRequest,
            ZonesController::ToggleMuteRequestReceivedHandler
        }
    };
    static constexpr size_t  lRequestHandlerCount = ElementsOf(lRequestHandlers);
    Status                   lRetval = kStatus_Success;

    lRetval = Server::ObjectControllerBasis::DoRequestHandlers(&lRequestHandlers[0],
                                                               &lRequestHandlers[lRequestHandlerCount],
                                                               this,
                                                               aRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ZonesController :: Init(Server::CommandManager &aCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::ZonesControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ZonesControllerBasis::Init(aCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoRequestHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Data Model Mutation State Change Methods

Status
ZonesController :: AdjustVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, VolumeModel::LevelType &aVolume)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    nlREQUIRE_ACTION(aAdjustment != 0, done, lRetval = -EINVAL);

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseVolume(aVolume);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseVolume(aVolume);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetMute(aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetVolume(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetVolume(aVolume);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: SetSource(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                             const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetSource(aSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ToggleMute(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                              Model::VolumeModel::MuteType &aMute)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->ToggleMute(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: GetEqualizerBand(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;


    // Attempt to get the zone model associated with the specified
    // identifier. Range checking on the identifier will occur during
    // this process.

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Attempt to get the equalizer band model associated with the
    // specified identifier. Range checking on the identifier will
    // occur during this process.

    lRetval = lZoneModel->GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustBalanceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const BalanceModel::ChannelType &aChannel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                              lZoneModel;
    BalanceModel::BalanceType                lBalance;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    nlREQUIRE_ACTION(((aChannel == BalanceModel::kChannelLeft) ||
                     (aChannel == BalanceModel::kChannelRight)),
                     done, lRetval = -EINVAL);

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aChannel == BalanceModel::kChannelLeft)
    {
        lRetval = lZoneModel->IncreaseBalanceLeft(lBalance);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aChannel == BalanceModel::kChannelRight)
    {
        lRetval = lZoneModel->IncreaseBalanceRight(lBalance);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = lBalanceResponse.Init(aZoneIdentifier, lBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBalanceResponse.GetBuffer();
    lSize = lBalanceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetBalanceReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const BalanceModel::BalanceType &aBalance, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                              lZoneModel;
    Server::Command::Zones::BalanceResponse  lBalanceResponse;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;
    Status                                   lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetBalance(aBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = lBalanceResponse.Init(aZoneIdentifier, aBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lBalanceResponse.GetBuffer();
    lSize = lBalanceResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustBassReceived(Server::ConnectionBasis &aConnection,
                                            const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                            const Model::ToneModel::LevelType &aAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the bass adjustment.

    lRetval = HandleAdjustBassReceived(aZoneIdentifier, aAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustBassReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                            const Model::ToneModel::LevelType &aAdjustment,
                                            Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    ToneModel::LevelType               lBass;
    ToneModel::LevelType               lTreble;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseBass(lBass);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseBass(lBass);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Assuming the adjustment was successful, get the treble so that
    // we can send a response (which must contain both bass and treble
    // in the response, even though only bass was changed).

    lRetval = lZoneModel->GetTreble(lTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleToneResponse(aZoneIdentifier, lBass, lTreble, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustTrebleReceived(Server::ConnectionBasis &aConnection, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::ToneModel::LevelType &aAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the bass adjustment.

    lRetval = HandleAdjustTrebleReceived(aZoneIdentifier, aAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustTrebleReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                              const Model::ToneModel::LevelType &aAdjustment,
                                              Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    ToneModel::LevelType               lBass;
    ToneModel::LevelType               lTreble;
    Status                             lRetval;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (aAdjustment > 0)
    {
        lRetval = lZoneModel->IncreaseTreble(lTreble);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aAdjustment < 0)
    {
        lRetval = lZoneModel->DecreaseTreble(lTreble);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Assuming the adjustment was successful, get the treble so that
    // we can send a response (which must contain both bass and treble
    // in the response, even though only treble was changed).

    lRetval = lZoneModel->GetBass(lBass);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleToneResponse(aZoneIdentifier, lBass, lTreble, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetMute(const bool &aConditionally, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status                             lRetval;


    lRetval = SetMute(aZoneIdentifier, aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // If the mute status was unchanged, SetMute will have returned
    // kStatus_ValueAlreadySet and there will be no need to generate
    // (and subsequently send) a response, unless the caller requested
    // an unconditional response. If we receive kStatus_Success, it is
    // the first time set or a change and response needs to be
    // unconditionally generated (and subsequently sent).

    if ((lRetval == kStatus_Success) || ((lRetval == kStatus_ValueAlreadySet) && !aConditionally))
    {
        lRetval = HandleMuteResponse(aZoneIdentifier, aMute, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetMuteConditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetMute(kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleSetMuteUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::MuteType &aMute, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetMute(!kConditionally, aZoneIdentifier, aMute, aBuffer));
}

Status
ZonesController :: HandleSetSoundMode(const bool &aConditionally, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    ZoneModel *                        lZoneModel;
    Status                             lRetval;

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lZoneModel->SetSoundMode(aSoundMode);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // If the sound mode was unchanged, SetSoundMode will have
    // returned kStatus_ValueAlreadySet and there will be no need to
    // generate (and subsequently send) a response, unless the caller
    // requested an unconditional response. If we receive
    // kStatus_Success, it is the first time set or a change and
    // response needs to be unconditionally generated (and
    // subsequently sent).

    if ((lRetval == kStatus_Success) || ((lRetval == kStatus_ValueAlreadySet) && !aConditionally))
    {
        lRetval = HandleSoundModeResponse(aZoneIdentifier, aSoundMode, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetSoundModeConditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetSoundMode(kConditionally, aZoneIdentifier, aSoundMode, aBuffer));
}

Status
ZonesController :: HandleSetSoundModeUnconditionally(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const SoundModel::SoundMode &aSoundMode, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    static constexpr bool kConditionally = true;

    return (HandleSetSoundMode(!kConditionally, aZoneIdentifier, aSoundMode, aBuffer));
}

Status
ZonesController :: HandleAdjustVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aAdjustment, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    VolumeModel::LevelType lVolume;
    Status                 lRetval;


    nlREQUIRE_ACTION(aAdjustment != 0, done, lRetval = -EINVAL);

    lRetval = AdjustVolume(aZoneIdentifier, aAdjustment, lVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, lVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetVolumeReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const VolumeModel::LevelType &aVolume, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status                             lRetval;


    lRetval = SetVolume(aZoneIdentifier, aVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = HandleVolumeResponse(aZoneIdentifier, aVolume, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleAdjustEqualizerBandReceived(Server::ConnectionBasis &aConnection, const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lRetval;


    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // First, ensure that the sound mode is set to zone equalizer mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lRetval = HandleSetSoundModeConditionally(aZoneIdentifier, SoundModel::kSoundModeZoneEqualizer, lResponseBuffer);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Next, go ahead and process the zone equalizer band adjustment.

    lRetval = HandleAdjustEqualizerBandReceived(aZoneIdentifier, aEqualizerBandIdentifier, aBandAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    if (lRetval >= kStatus_Success)
    {
        lRetval = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lRetval);
    }
    else
    {
        lRetval = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lRetval);
    }

    return (lRetval);
}

Status
ZonesController :: HandleAdjustEqualizerBandReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier,
                                                     const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                                     const Model::EqualizerBandModel::LevelType &aBandAdjustment,
                                                     Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    EqualizerBandModel::LevelType      lBandLevel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Increase or decrease, per the adjustment, the equalizer band
    // level.

    if (aBandAdjustment > 0)
    {
        lRetval = lEqualizerBandModel->IncreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else if (aBandAdjustment < 0)
    {
        lRetval = lEqualizerBandModel->DecreaseLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = HandleEqualizerBandResponse(aZoneIdentifier, aEqualizerBandIdentifier, lBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ZonesController :: HandleSetEqualizerBandReceived(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel::LevelType &aBandLevel, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    EqualizerBandModel *               lEqualizerBandModel;
    Status                             lRetval;


    // Attempt to get the equalizer band model associated with the
    // specified identifiers. Range checking on the identifiers will
    // occur during this process.

    lRetval = GetEqualizerBand(aZoneIdentifier, aEqualizerBandIdentifier, lEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lEqualizerBandModel->SetLevel(aBandLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lRetval = HandleEqualizerBandResponse(aZoneIdentifier, aEqualizerBandIdentifier, aBandLevel, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Configuration Management Methods

void ZonesController :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    static constexpr bool kIsConfiguration = true;
    Status                lStatus;


    (void)aConnection;

    lStatus = HandleQueryReceived(kIsConfiguration, aBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: ResetToDefaultConfiguration(void)
{
    ZoneModel *                         lZoneModel;
    EqualizerBandModel *                lEqualizerBandModel;
    EqualizerBandModel::IdentifierType  lEqualizerBandIdentifier;
    Status                              lStatus;


    for (auto lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        const ZoneModelDefaults &lZoneModelDefaults = kZoneModelDefaults[lZoneIdentifier - 1];

        lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
        nlREQUIRE_SUCCESS(lStatus, done);

        lStatus = lZoneModel->SetName(lZoneModelDefaults.mName.mName);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetBalance(lZoneModelDefaults.mBalance);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetSoundMode(lZoneModelDefaults.mSoundModel.mSoundMode);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
        {
            const Defaults::EqualizerBandModel &lEqualizerBandDefaults = lZoneModelDefaults.mSoundModel.mZoneEqualizerBands[lEqualizerBandIdentifier - 1];

            lStatus = lZoneModel->GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
            nlREQUIRE_SUCCESS(lStatus, done);

            lStatus = lEqualizerBandModel->SetLevel(lEqualizerBandDefaults.mLevel);
            nlCHECK_SUCCESS(lStatus);

            if (lStatus == kStatus_Success)
            {
                OnConfigurationIsDirty();
            }
        }

        lStatus = lZoneModel->SetEqualizerPreset(lZoneModelDefaults.mSoundModel.mEqualizerPresetIdentifier);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetTone(lZoneModelDefaults.mSoundModel.mToneModel.mBass,
                                      lZoneModelDefaults.mSoundModel.mToneModel.mTreble);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetLowpassFrequency(lZoneModelDefaults.mSoundModel.mLowpassCrossover.mFrequency);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetHighpassFrequency(lZoneModelDefaults.mSoundModel.mHighpassCrossover.mFrequency);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetSource(lZoneModelDefaults.mSource);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetMute(lZoneModelDefaults.mMute);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetVolume(lZoneModelDefaults.mVolume);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }

        lStatus = lZoneModel->SetVolumeFixed(lZoneModelDefaults.mVolumeFixed);
        nlCHECK_SUCCESS(lStatus);

        if (lStatus == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return;
}

Status
ZonesController :: ZoneEqualizerLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel)
{
    CFArrayRef                         lEqualizerLevelsArray = nullptr;
    CFIndex                            lEqualizerBandsCount;
    EqualizerBandModel::IdentifierType lEqualizerBandIdentifier;
    Status                             lRetval = kStatus_Success;

    lEqualizerLevelsArray = static_cast<CFArrayRef>(CFDictionaryGetValue(aSoundModeDictionary, kEqualizerLevelsPresetSchemaKey));
    nlREQUIRE_ACTION(lEqualizerLevelsArray != nullptr, done, lRetval = kError_MissingConfiguration);

    lEqualizerBandsCount = CFArrayGetCount(lEqualizerLevelsArray);
    nlREQUIRE_ACTION(lEqualizerBandsCount == EqualizerBandsModel::kEqualizerBandsMax, done, lRetval = kError_InvalidConfiguration);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        const CFIndex        lEqualizerBandIndex = (lEqualizerBandIdentifier - 1);
        EqualizerBandModel * lEqualizerBandModel;
        CFNumberRef          lBandLevelRef = nullptr;
        bool                 lStatus;
        EqualizerBandModel::LevelType         lBandLevel;

        lRetval = aZoneModel.GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBandLevelRef = static_cast<CFNumberRef>(CFArrayGetValueAtIndex(lEqualizerLevelsArray, lEqualizerBandIndex));
        nlREQUIRE_ACTION(lBandLevelRef != nullptr, done, lRetval = kError_InvalidConfiguration);

        lStatus = CFUNumberGetValue(lBandLevelRef, lBandLevel);
        nlREQUIRE_ACTION(lStatus, done, lRetval = kError_InvalidConfiguration);

        lRetval = lEqualizerBandModel->SetLevel(lBandLevel);
        nlREQUIRE(lRetval >= kStatus_Success, done);

        if (lRetval == kStatus_Success)
        {
            OnConfigurationIsDirty();
        }
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneEqualizerPresetLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel)
{
    bool                                  lStatus;
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lRetval = kStatus_Success;

    lStatus = CFUDictionaryGetNumber(aSoundModeDictionary, kEqualizerPresetSchemaKey, lEqualizerPresetIdentifier);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lRetval = aZoneModel.SetEqualizerPreset(lEqualizerPresetIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneHighpassCrossoverLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel)
{
    bool                           lStatus;
    CrossoverModel::FrequencyType  lHighpassFrequency;
    Status                         lRetval = kStatus_Success;

    lStatus = CFUDictionaryGetNumber(aSoundModeDictionary, kHighpassFrequencySchemaKey, lHighpassFrequency);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lRetval = aZoneModel.SetHighpassFrequency(lHighpassFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneLowpassCrossoverLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel)
{
    bool                           lStatus;
    CrossoverModel::FrequencyType  lLowpassFrequency;
    Status                         lRetval = kStatus_Success;

    lStatus = CFUDictionaryGetNumber(aSoundModeDictionary, kLowpassFrequencySchemaKey, lLowpassFrequency);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lRetval = aZoneModel.SetLowpassFrequency(lLowpassFrequency);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneToneLoadFromBackupConfiguration(CFDictionaryRef aSoundModeDictionary, Model::ZoneModel &aZoneModel)
{
    bool                  lStatus;
    ToneModel::LevelType  lBass;
    ToneModel::LevelType  lTreble;
    Status                lRetval = kStatus_Success;


    lStatus = CFUDictionaryGetNumber(aSoundModeDictionary, kBassSchemaKey, lBass);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lStatus = CFUDictionaryGetNumber(aSoundModeDictionary, kTrebleSchemaKey, lTreble);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lRetval = aZoneModel.SetTone(lBass, lTreble);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneSoundModeLoadFromBackupConfiguration(CFDictionaryRef aZoneDictionary, Model::ZoneModel &aZoneModel)
{
    CFDictionaryRef                       lSoundModeDictionary = nullptr;
    Boolean                               lStatus;
    SoundModel::SoundMode                 lSoundMode;
    Status                                lRetval = kStatus_Success;

    // Attempt to retrieve the sound mode configuration subdictionary.

    lSoundModeDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aZoneDictionary, kSoundModeDictionarySchemaKey));
    nlREQUIRE_ACTION(lSoundModeDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the sound mode configuration value.

    lStatus = CFUDictionaryGetNumber(lSoundModeDictionary, kSoundModeValueSchemaKey, lSoundMode);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Sound Mode

    lRetval = aZoneModel.SetSoundMode(lSoundMode);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    switch (lSoundMode)
    {

    case SoundModel::kSoundModeZoneEqualizer:
        lRetval = ZoneEqualizerLoadFromBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE(lRetval >= kStatus_Success, done);
        break;

    case SoundModel::kSoundModePresetEqualizer:
        lRetval = ZoneEqualizerPresetLoadFromBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE(lRetval >= kStatus_Success, done);
        break;

    case SoundModel::kSoundModeTone:
        lRetval = ZoneToneLoadFromBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE(lRetval >= kStatus_Success, done);
        break;

    case SoundModel::kSoundModeLowpass:
        lRetval = ZoneLowpassCrossoverLoadFromBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE(lRetval >= kStatus_Success, done);
        break;

    case SoundModel::kSoundModeHighpass:
        lRetval = ZoneHighpassCrossoverLoadFromBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE(lRetval >= kStatus_Success, done);
        break;

    default:
        break;

    }

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneVolumeLoadFromBackupConfiguration(CFDictionaryRef aZoneDictionary, Model::ZoneModel &aZoneModel)
{
    CFDictionaryRef          lVolumeDictionary = nullptr;
    bool                     lStatus;
    VolumeModel::FixedType   lLocked;
    VolumeModel::MuteType    lMuted;
    VolumeModel::LevelType  lVolumeLevel;
    Status                   lRetval = kStatus_Success;

    // Attempt to retrieve the volume configuration subdictionary.

    lVolumeDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aZoneDictionary, kVolumeSchemaKey));
    nlREQUIRE_ACTION(lVolumeDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the volume fixed/locked configuration

    lStatus = CFUDictionaryGetBoolean(lVolumeDictionary, kVolumeLockedSchemaKey, lLocked);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the volume mute configuration

    lStatus = CFUDictionaryGetBoolean(lVolumeDictionary, kVolumeMutedSchemaKey, lMuted);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Attempt to get the volume level configuration

    lStatus = CFUDictionaryGetNumber(lVolumeDictionary, kVolumeLevelSchemaKey, lVolumeLevel);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // (Volume) Fixed/Locked

    lRetval = aZoneModel.SetVolumeFixed(lLocked);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // (Volume) Mute

    lRetval = aZoneModel.SetMute(lMuted);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Volume (Level)

    lRetval = aZoneModel.SetVolume(lVolumeLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

 done:
    return (lRetval);
}

Status
ZonesController :: ElementLoadFromBackupConfiguration(CFDictionaryRef aZonesDictionary, const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    CFDictionaryRef                  lZoneDictionary = nullptr;
    CFStringRef                      lZoneIdentifierKey = nullptr;
    bool                             lStatus;
    CFStringRef                      lZoneName = nullptr;
    BalanceModel::BalanceType        lBalance;
    SourceModel::IdentifierType      lSourceIdentifier;
    ZoneModel *                      lZoneModel;
    Status                           lRetval = kStatus_Success;

    // Attempt to form the zone identifier key.

    lZoneIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aZoneIdentifier);
    nlREQUIRE_ACTION(lZoneIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    // Attempt to retrieve the zone dictionary.

    lZoneDictionary = static_cast<CFDictionaryRef>(CFDictionaryGetValue(aZonesDictionary, lZoneIdentifierKey));
    nlREQUIRE_ACTION(lZoneDictionary != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrive the zone balance configuration.

    lStatus = CFUDictionaryGetNumber(lZoneDictionary, kBalanceSchemaKey, lBalance);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the zone name configuration.

    lZoneName = static_cast<CFStringRef>(CFDictionaryGetValue(lZoneDictionary, kNameSchemaKey));
    nlREQUIRE_ACTION(lZoneName != nullptr, done, lRetval = kError_MissingConfiguration);

    // Attempt to retrieve the zone source configuration.

    lStatus = CFUDictionaryGetNumber(lZoneDictionary, kSourceSchemaKey, lSourceIdentifier);
    nlREQUIRE_ACTION(lStatus, done, lRetval = kError_MissingConfiguration);

    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Balance

    lRetval = lZoneModel->SetBalance(lBalance);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Name

    lRetval = lZoneModel->SetName(CFString(lZoneName).GetCString());
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Source

    lRetval = lZoneModel->SetSource(lSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    if (lRetval == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    // Sound Mode

    lRetval = ZoneSoundModeLoadFromBackupConfiguration(lZoneDictionary, *lZoneModel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

    // Volume

    lRetval = ZoneVolumeLoadFromBackupConfiguration(lZoneDictionary, *lZoneModel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    CFURelease(lZoneIdentifierKey);

    return (lRetval);
}

Status
ZonesController :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    ContainerControllerBasis::LoadFromBackupConfiguration(aBackupDictionary,
                                                          kZonesMax,
                                                          kZonesSchemaKey);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneEqualizerSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const ZoneModel &aZoneModel)
{
    CFMutableArrayRef                     lEqualizerLevelsArray = nullptr;
    EqualizerBandModel::IdentifierType    lEqualizerBandIdentifier;
    const EqualizerBandModel *            lEqualizerBandModel;
    EqualizerBandModel::LevelType         lBandLevel;
    Status                                lRetval = kStatus_Success;


    lEqualizerLevelsArray = CFArrayCreateMutable(kCFAllocatorDefault,
                                                 0,
                                                 &kCFTypeArrayCallBacks);
    nlREQUIRE_ACTION(lEqualizerLevelsArray != nullptr, done, lRetval = -ENOMEM);

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        CFNumberRef  lBandLevelNumber = nullptr;

        lRetval = aZoneModel.GetEqualizerBand(lEqualizerBandIdentifier, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = lEqualizerBandModel->GetLevel(lBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);

        lBandLevelNumber = CFUNumberCreate(kCFAllocatorDefault, lBandLevel);
        nlREQUIRE_ACTION(lBandLevelNumber != nullptr, done, lRetval = -ENOMEM);

        CFArrayAppendValue(lEqualizerLevelsArray, lBandLevelNumber);

        CFURelease(lBandLevelNumber);
    }

    CFDictionaryAddValue(aSoundModeDictionary, kEqualizerLevelsPresetSchemaKey, lEqualizerLevelsArray);

 done:
    CFURelease(lEqualizerLevelsArray);

    return (lRetval);
}

Status
ZonesController :: ZoneEqualizerPresetSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const ZoneModel &aZoneModel)
{
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lRetval = kStatus_Success;


    lRetval = aZoneModel.GetEqualizerPreset(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetNumber(aSoundModeDictionary, kEqualizerPresetSchemaKey, lEqualizerPresetIdentifier);

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneToneSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const ZoneModel &aZoneModel)
{
    ToneModel::LevelType  lBass;
    ToneModel::LevelType  lTreble;
    Status                lRetval = kStatus_Success;


    lRetval = aZoneModel.GetTone(lBass, lTreble);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetNumber(aSoundModeDictionary, kBassSchemaKey,   lBass);
    CFUDictionarySetNumber(aSoundModeDictionary, kTrebleSchemaKey, lTreble);

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneLowpassCrossoverSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const ZoneModel &aZoneModel)
{
    CrossoverModel::FrequencyType  lLowpassFrequency;
    Status                         lRetval = kStatus_Success;


    lRetval = aZoneModel.GetLowpassFrequency(lLowpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetNumber(aSoundModeDictionary, kLowpassFrequencySchemaKey, lLowpassFrequency);

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneHighpassCrossoverSaveToBackupConfiguration(CFMutableDictionaryRef aSoundModeDictionary, const ZoneModel &aZoneModel)
{
    CrossoverModel::FrequencyType  lHighpassFrequency;
    Status                         lRetval = kStatus_Success;


    lRetval = aZoneModel.GetHighpassFrequency(lHighpassFrequency);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetNumber(aSoundModeDictionary, kHighpassFrequencySchemaKey, lHighpassFrequency);

 done:
    return (lRetval);
}

Status
ZonesController :: ZoneSoundModeSaveToBackupConfiguration(CFMutableDictionaryRef aZoneDictionary, const ZoneModel &aZoneModel)
{
    CFMutableDictionaryRef                lSoundModeDictionary = nullptr;
    SoundModel::SoundMode                 lSoundMode;
    Status                                lRetval = kStatus_Success;


    lSoundModeDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                     0,
                                                     &kCFCopyStringDictionaryKeyCallBacks,
                                                     &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lSoundModeDictionary != nullptr, done, lRetval = -ENOMEM);

    // Sound Mode

    lRetval = aZoneModel.GetSoundMode(lSoundMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    switch (lSoundMode)
    {

    case SoundModel::kSoundModeZoneEqualizer:
        lRetval = ZoneEqualizerSaveToBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModePresetEqualizer:
        lRetval = ZoneEqualizerPresetSaveToBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeTone:
        lRetval = ZoneToneSaveToBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeLowpass:
        lRetval = ZoneLowpassCrossoverSaveToBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case SoundModel::kSoundModeHighpass:
        lRetval = ZoneHighpassCrossoverSaveToBackupConfiguration(lSoundModeDictionary, aZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    default:
        break;

    }

    // Add the actual sound mode value to the newly-created sound mode
    // dictionary.

    CFUDictionarySetNumber(lSoundModeDictionary, kSoundModeValueSchemaKey, lSoundMode);

    // Add the newly-created volume dictionary to the zone dictionary.

    CFDictionaryAddValue(aZoneDictionary, kSoundModeDictionarySchemaKey, lSoundModeDictionary);

 done:
    CFURelease(lSoundModeDictionary);

    return (lRetval);
}

Status
ZonesController :: ZoneVolumeSaveToBackupConfiguration(CFMutableDictionaryRef aZoneDictionary, const ZoneModel &aZoneModel)
{
    CFMutableDictionaryRef   lVolumeDictionary = nullptr;
    VolumeModel::FixedType   lLocked;
    VolumeModel::MuteType    lMuted;
    VolumeModel::LevelType  lVolume;
    Status                   lRetval = kStatus_Success;


    lVolumeDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                  0,
                                                  &kCFCopyStringDictionaryKeyCallBacks,
                                                  &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lVolumeDictionary != nullptr, done, lRetval = -ENOMEM);

    // (Volume) Fixed/Locked

    lRetval = aZoneModel.GetVolumeFixed(lLocked);
    nlREQUIRE_SUCCESS(lRetval, done);

    // (Volume) Mute

    lRetval = aZoneModel.GetMute(lMuted);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Volume (Level)

    lRetval = aZoneModel.GetVolume(lVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the volume level, muting, and locked state to the
    // newly-created volume dictionary.

    CFUDictionarySetNumber(lVolumeDictionary,  kVolumeLevelSchemaKey,  lVolume);
    CFUDictionarySetBoolean(lVolumeDictionary, kVolumeLockedSchemaKey, lLocked);
    CFUDictionarySetBoolean(lVolumeDictionary, kVolumeMutedSchemaKey,  lMuted);

    // Add the newly-created volume dictionary to the zone dictionary.

    CFDictionaryAddValue(aZoneDictionary, kVolumeSchemaKey, lVolumeDictionary);

 done:
    CFURelease(lVolumeDictionary);

    return (lRetval);
}

Status
ZonesController :: ElementSaveToBackupConfiguration(CFMutableDictionaryRef aZonesDictionary, const Model::ZoneModel::IdentifierType &aZoneIdentifier) const
{
    CFStringRef                           lZoneIdentifierKey = nullptr;
    CFMutableDictionaryRef                lZoneDictionary = nullptr;
    const ZoneModel *                     lZoneModel;
    const char *                          lName;
    Server::Command::Zones::NameResponse  lNameResponse;
    BalanceModel::BalanceType             lBalance;
    SourceModel::IdentifierType           lSourceIdentifier;
    Status                                lRetval = kStatus_Success;


    lRetval = mZones.GetZone(aZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lZoneIdentifierKey = Simulator::Utilities::Configuration::CreateCFString(aZoneIdentifier);
    nlREQUIRE_ACTION(lZoneIdentifierKey != nullptr, done, lRetval = -ENOMEM);

    lZoneDictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
                                                0,
                                                &kCFCopyStringDictionaryKeyCallBacks,
                                                &kCFTypeDictionaryValueCallBacks);
    nlREQUIRE_ACTION(lZoneDictionary != nullptr, done, lRetval = -ENOMEM);

    // Balance

    lRetval = lZoneModel->GetBalance(lBalance);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Name

    lRetval = lZoneModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Source

    lRetval = lZoneModel->GetSource(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    CFUDictionarySetNumber(lZoneDictionary,  kBalanceSchemaKey, lBalance);
    CFUDictionarySetCString(lZoneDictionary, kNameSchemaKey,    lName);
    CFUDictionarySetNumber(lZoneDictionary,  kSourceSchemaKey,  lSourceIdentifier);

    // Sound Mode

    lRetval = ZoneSoundModeSaveToBackupConfiguration(lZoneDictionary, *lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Volume

    lRetval = ZoneVolumeSaveToBackupConfiguration(lZoneDictionary, *lZoneModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the newly-created zone dictionary into the backup
    // configuration dictionary, keyed for this zone identifier.

    CFDictionaryAddValue(aZonesDictionary, lZoneIdentifierKey, lZoneDictionary);

 done:
    CFURelease(lZoneIdentifierKey);
    CFURelease(lZoneDictionary);

    return (lRetval);
}

void ZonesController :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    ContainerControllerBasis::SaveToBackupConfiguration(aBackupDictionary,
                                                        kZonesMax,
                                                        kZonesSchemaKey);
}

// MARK: Command Request Completion Handlers

void ZonesController :: AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    BalanceModel::ChannelType                lChannel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::AdjustBalanceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBalanceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Channel
    //
    // The validity of the channel will be range checked at
    // HandleAdjustBalanceReceived below.

    lChannel = *(reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBalanceReceived(lZoneIdentifier, lChannel, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const ToneModel::LevelType        kAdjustment = -1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBassReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBassReceived(aConnection, lZoneIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const ToneModel::LevelType        kAdjustment = 1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustBassReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustBassReceived(aConnection, lZoneIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const ToneModel::LevelType        kAdjustment = -1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustTrebleReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustTrebleReceived(aConnection, lZoneIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const ToneModel::LevelType        kAdjustment = 1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseBassRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustTrebleReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustTrebleReceived(aConnection, lZoneIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = -1;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustEqualizerBandReceived(aConnection, lZoneIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    static const EqualizerBandModel::LevelType  kAdjustment = 1;
    Model::ZoneModel::IdentifierType            lZoneIdentifier;
    EqualizerBandModel::IdentifierType          lEqualizerBandIdentifier;
    ConnectionBuffer::MutableCountedPointer     lResponseBuffer;
    Status                                      lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::IncreaseEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleAdjustEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleAdjustEqualizerBandReceived(aConnection, lZoneIdentifier, lEqualizerBandIdentifier, kAdjustment);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void ZonesController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = -1;
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::DecreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleAdjustVolumeReceived(lZoneIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    static const VolumeModel::LevelType      kAdjustment = 1;
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::IncreaseVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleAdjustVolumeReceived(lZoneIdentifier, kAdjustment, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    const char *                             lMutep;
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::MuteType                    lMute;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::MuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Muted/Unmuted

    lMutep = ((const char *)(aBuffer) + aMatches.at(1).rm_so);
    lMute = ((lMutep[0] == 'U') ? false : true);

    // Match 3/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteUnconditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetMuteUnconditionally(lZoneIdentifier, lMute, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static constexpr bool                    kIsConfiguration = true;
    ZoneModel::IdentifierType                lZoneIdentifier;
    Server::Command::Zones::QueryResponse    lResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lResponse.Init(lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion, indicating that
    // this is a zone-specific query, not a general configuration
    // query.

    lStatus = HandleQueryReceived(!kIsConfiguration, lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Second, put the response completion portion.

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryMuteReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryMuteReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQuerySourceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQuerySourceReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    ZoneModel::IdentifierType                lZoneIdentifier;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::QueryRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleQueryVolumeReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, put the solicited notifications portion.

    lStatus = HandleQueryVolumeReceived(lZoneIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    BalanceModel::ChannelType                lChannel;
    BalanceModel::BalanceType                lBalance;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetBalanceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetBalanceReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Channel
    //
    // The validity of the channel is enforced at the data model
    // normalization below.

    lChannel = *(reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);

    // Match 4/4: Level
    //
    // The validity of the balance will be range checked at
    // HandleSetBalanceReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBalance);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Adjust the balance from the HLX's L:{80, 0} to {0, 80}:R tagged
    // discontinuous model to a non-tagged, continuous L:{-80, 80}:R
    // model.

    if (lChannel == BalanceModel::kChannelLeft)
    {
        lBalance = -lBalance;
    }

    lStatus = HandleSetBalanceReceived(lZoneIdentifier, lBalance, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    EqualizerBandModel::IdentifierType       lEqualizerBandIdentifier;
    EqualizerBandModel::LevelType            lBandLevel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetEqualizerBandRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Equalizer Band Identifier
    //
    // The validity of the equalizer band identifier will be range
    // checked at HandleSetEqualizerBandReceived below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Equalizer Band Level
    //
    // The validity of the equalizer band level will be range checked
    // at HandleSetEqualizerBandReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lBandLevel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to zone equalizer mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeZoneEqualizer, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the zone equalizer band adjustment.

    lStatus = HandleSetEqualizerBandReceived(lZoneIdentifier, lEqualizerBandIdentifier, lBandLevel, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    EqualizerPresetModel::IdentifierType     lEqualizerPresetIdentifier;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetEqualizerPresetRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Equalizer Preset Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = EqualizerPresetsController::ValidateIdentifier(lEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to equalizer preset mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModePresetEqualizer, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the equalizer preset request.

    lStatus = lZoneModel->SetEqualizerPreset(lEqualizerPresetIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleEqualizerPresetResponse(lZoneIdentifier, lEqualizerPresetIdentifier, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    CrossoverModel::FrequencyType            lHighpassFrequency;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetHighpassCrossoverRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Highpass Frequency
    //
    // The validity of the highpass crossover frequency will be range
    // checked at SetHighpassFrequency below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lHighpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to highpass crossover mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeHighpass, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the highpass crossover frequency request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetHighpassFrequency(lHighpassFrequency);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleHighpassCrossoverResponse(lZoneIdentifier, lHighpassFrequency, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    CrossoverModel::FrequencyType            lLowpassFrequency;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetLowpassCrossoverRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Lowpass Frequency
    //
    // The validity of the lowpass crossover frequency will be range
    // checked at SetLowpassFrequency below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lLowpassFrequency);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to lowpass crossover mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeLowpass, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the lowpass crossover frequency request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetLowpassFrequency(lLowpassFrequency);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleLowpassCrossoverResponse(lZoneIdentifier, lLowpassFrequency, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    const char *                             lName;
    size_t                                   lNameSize;
    ZoneModel *                              lZoneModel;
    Server::Command::Zones::NameResponse     lNameResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetNameRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // GetZone below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Name

    lName = (reinterpret_cast<const char *>(aBuffer) + aMatches.at(2).rm_so);
    lNameSize = Common::Utilities::Distance(aMatches.at(2));

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // Get the zone model associated with the parsed zone
    // identifier. This will include a range check on the zone
    // identifier.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Attempt to set the parsed name. This will include range check
    // on the name length. If the set name is the same as the current
    // name, that should still be regarded as a success with a
    // success, rather than error, response sent.

    lStatus = lZoneModel->SetName(lName, lNameSize);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = lNameResponse.Init(lZoneIdentifier, lName, lNameSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lNameResponse.GetBuffer();
    lSize = lNameResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    SoundModel::SoundMode                    lSoundMode;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSoundModeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeUnconditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Sound Mode
    //
    // The validity of the sound mode will be range checked at
    // HandleSetSoundModeUnconditionally below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lSoundMode);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleSetSoundModeUnconditionally(lZoneIdentifier, lSoundMode, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    SourceModel::IdentifierType              lSourceIdentifier;
    Server::Command::Zones::SourceResponse   lSourceResponse;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;
    const uint8_t *                          lBuffer;
    size_t                                   lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSourceRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // SetSource below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Source Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(2).rm_so,
                                                Common::Utilities::Distance(aMatches.at(2)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SourcesController::ValidateIdentifier(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SetSource(lZoneIdentifier, lSourceIdentifier);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    lStatus = lSourceResponse.Init(lZoneIdentifier, lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSourceResponse.GetBuffer();
    lSize = lSourceResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType           lZoneIdentifier;
    SourceModel::IdentifierType                lSourceIdentifier;
    Server::Command::Zones::SourceAllResponse  lSourceAllResponse;
    ConnectionBuffer::MutableCountedPointer    lResponseBuffer;
    Status                                     lStatus;
    const uint8_t *                            lBuffer;
    size_t                                     lSize;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetSourceAllRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Source Identifier
    //
    // Parse and validate the identifier

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = SourcesController::ValidateIdentifier(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        lStatus = SetSource(lZoneIdentifier, lSourceIdentifier);
        nlREQUIRE(lStatus >= kStatus_Success, done);
    }

    lStatus = lSourceAllResponse.Init(lSourceIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lSourceAllResponse.GetBuffer();
    lSize = lSourceAllResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    ToneModel::LevelType                     lBass;
    ToneModel::LevelType                     lTreble;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetToneRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/4: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetSoundModeConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/4: Bass Level
    //
    // The validity of the bass level will be range checked at
    // SetTone below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lBass);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 4/4: Treble Level
    //
    // The validity of the treble level will be range checked at
    // SetTone below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(3).rm_so,
                                      Common::Utilities::Distance(aMatches.at(3)),
                                      lTreble);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the sound mode is set to tone mode
    //
    // A sound mode response will only be conditionally generated if
    // the sound mode changed as a result.

    lStatus = HandleSetSoundModeConditionally(lZoneIdentifier, SoundModel::kSoundModeTone, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the tone request.

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetTone(lBass, lTreble);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleToneResponse(lZoneIdentifier, lBass, lTreble, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType       kMuted = true;
    ZoneModel::IdentifierType                lZoneIdentifier;
    VolumeModel::LevelType                   lVolume;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // HandleSetMuteConditionally below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Level
    //
    // The validity of the volume level will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    // First, ensure that the zone is unmuted.
    //
    // A mute response will only be conditionally generated if the
    // mute status changed as a result.

    lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    // Next, go ahead and process the volume adjustment.

    lStatus = HandleSetVolumeReceived(lZoneIdentifier, lVolume, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    static const VolumeModel::MuteType        kMuted = true;
    Model::ZoneModel::IdentifierType          lZoneIdentifier;
    VolumeModel::LevelType                    lVolume;
    ConnectionBuffer::MutableCountedPointer   lResponseBuffer;
    Server::Command::Zones::VolumeAllResponse lVolumeAllResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeAllRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 1/2: Volume Level
    //
    // The validity of the volume level will be range checked at
    // HandleSetVolumeReceived below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(1).rm_so,
                                      Common::Utilities::Distance(aMatches.at(1)),
                                      lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    for (lZoneIdentifier = IdentifierModel::kIdentifierMin; lZoneIdentifier <= kZonesMax; lZoneIdentifier++)
    {
        // First, ensure that the zone is unmuted.
        //
        // A mute response will only be conditionally generated if the
        // mute status changed as a result.

        lStatus = HandleSetMuteConditionally(lZoneIdentifier, !kMuted, lResponseBuffer);
        nlREQUIRE(lStatus >= kStatus_Success, done);

        // Next, go ahead and process the volume adjustment.

        lStatus = SetVolume(lZoneIdentifier, lVolume);
        nlREQUIRE(lStatus >= kStatus_Success, done);
    }

    lStatus = lVolumeAllResponse.Init(lVolume);
    nlREQUIRE_SUCCESS(lStatus, done);

    lBuffer = lVolumeAllResponse.GetBuffer();
    lSize = lVolumeAllResponse.GetSize();

    lStatus = Common::Utilities::Put(*lResponseBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::FixedType                   lLocked;
    ZoneModel *                              lZoneModel;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::SetVolumeFixedRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/3: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // GetZone below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Match 3/3: Volume Fixed
    //
    // The validity of the volume lock will be range checked at
    // SetVolumeFixed below.

    lStatus = ::HLX::Utilities::Parse(aBuffer + aMatches.at(2).rm_so,
                                      Common::Utilities::Distance(aMatches.at(2)),
                                      lLocked);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = mZones.GetZone(lZoneIdentifier, lZoneModel);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = lZoneModel->SetVolumeFixed(lLocked);
    nlREQUIRE(lStatus >= kStatus_Success, done);

    if (lStatus == kStatus_Success)
    {
        OnConfigurationIsDirty();
    }

    lStatus = HandleVolumeFixedResponse(lZoneIdentifier, lLocked, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

void ZonesController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches)
{
    Model::ZoneModel::IdentifierType         lZoneIdentifier;
    VolumeModel::MuteType                    lMute;
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    Status                                   lStatus;


    (void)aSize;

    nlREQUIRE_ACTION(aMatches.size() == Server::Command::Zones::ToggleMuteRequest::kExpectedMatches, done, lStatus = kError_BadCommand);

    // Match 2/2: Zone Identifier
    //
    // The validity of the zone identifier will be range checked at
    // ToggleMute below.

    lStatus = Model::Utilities::ParseIdentifier(aBuffer + aMatches.at(1).rm_so,
                                                Common::Utilities::Distance(aMatches.at(1)),
                                                lZoneIdentifier);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lResponseBuffer->Init();
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = ToggleMute(lZoneIdentifier, lMute);
    nlREQUIRE_SUCCESS(lStatus, done);

    lStatus = HandleMuteResponse(lZoneIdentifier, lMute, lResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    if (lStatus >= kStatus_Success)
    {
        lStatus = SendResponse(aConnection, lResponseBuffer);
        nlVERIFY_SUCCESS(lStatus);
    }
    else
    {
        lStatus = SendErrorResponse(aConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Command Request Handler Trampolines

void ZonesController :: AdjustBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->AdjustBalanceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseBassRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseBassRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseBassRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseTrebleRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseTrebleRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseTrebleRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: DecreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->DecreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: IncreaseVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->IncreaseVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: MuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->MuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QuerySourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QuerySourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: QueryVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->QueryVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetBalanceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetBalanceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetEqualizerBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerBandRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetEqualizerPresetRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetEqualizerPresetRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetHighpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetHighpassCrossoverRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetLowpassCrossoverRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetLowpassCrossoverRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetNameRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSoundModeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSoundModeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSourceRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetSourceAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetSourceAllRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetToneRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetToneRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeAllRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeAllRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: SetVolumeFixedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->SetVolumeFixedRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

void ZonesController :: ToggleMuteRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const RegularExpression::Matches &aMatches, void *aContext)
{
    ZonesController *lController = static_cast<ZonesController *>(aContext);

    if (lController != nullptr)
    {
        lController->ToggleMuteRequestReceivedHandler(aConnection, aBuffer, aSize, aMatches);
    }
}

}; // namespace Simulator

}; // namespace HLX
