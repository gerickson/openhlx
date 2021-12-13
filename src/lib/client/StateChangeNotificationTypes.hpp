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
 *      This file enumerates HLX client state change notification
 *      (SCN) types.
 *
 */

#ifndef OPENHLXCLIENTSTATECHANGENOTIFICATIONTYPES_HPP
#define OPENHLXCLIENTSTATECHANGENOTIFICATIONTYPES_HPP


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    Enumeration of data model state change notification types.
 *
 *  @ingroup state-change
 *
 */
enum Type
{
    kStateChangeType_Min                              =   0,

    kStateChangeType_Unknown                          = kStateChangeType_Min,

    // Configuration

    kStateChangeType_ConfigurationLoadedFromBackup    = 100,
    kStateChangeType_ConfigurationResetToDefaults     = 101,
    kStateChangeType_ConfigurationSavingToBackup      = 102,
    kStateChangeType_ConfigurationSavedToBackup       = 103,

    // Equalizer Presets

    kStateChangeType_EqualizerPresetBand              = 201,
    kStateChangeType_EqualizerPresetName              = 202,

    // Favorites

    kStateChangeType_FavoriteName                     = 301,

    // Front Panel

    kStateChangeType_FrontPanelBrightness             = 401,
    kStateChangeType_FrontPanelLocked                 = 402,

    // Groups

    kStateChangeType_GroupMute                        = 500,
    kStateChangeType_GroupName                        = 501,
    kStateChangeType_GroupSource                      = 502,
    kStateChangeType_GroupVolume                      = 503,
    kStateChangeType_GroupIncreaseVolume              = 504,
    kStateChangeType_GroupDecreaseVolume              = 505,
    kStateChangeType_GroupZoneAdded                   = 506,
    kStateChangeType_GroupZoneRemoved                 = 507,

    // Infrared

    kStateChangeType_InfraredDisabled                 = 601,

    // Network

    kStateChangeType_NetworkDHCPv4Enabled             = 700,
    kStateChangeType_NetworkSDDPEnabled               = 701,

    // Sources

    kStateChangeType_SourceName                       = 800,

    // Zones

    kStateChangeType_ZoneBalance                      = 900,
    kStateChangeType_ZoneEqualizerBand                = 901,
    kStateChangeType_ZoneEqualizerPreset              = 902,
    kStateChangeType_ZoneHighpassCrossover            = 903,
    kStateChangeType_ZoneLowpassCrossover             = 904,
    kStateChangeType_ZoneMute                         = 905,
    kStateChangeType_ZoneName                         = 906,
    kStateChangeType_ZoneSoundMode                    = 907,
    kStateChangeType_ZoneSource                       = 908,
    kStateChangeType_ZoneTone                         = 909,
    kStateChangeType_ZoneVolume                       = 910,
    kStateChangeType_ZoneVolumeLocked                 = 911,

    kStateChangeType_Max
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTSTATECHANGENOTIFICATIONTYPES_HPP
