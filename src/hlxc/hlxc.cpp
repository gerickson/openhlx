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
 *      This file implements a HLX command line interface (CLI)
 *      control client program executable.
 *
 */

#include <string>
#include <vector>

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>
#include <NuovationsUtilities/GenerateShortOptions.hpp>

#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
#include <OpenHLX/Client/FavoritesStateChangeNotifications.hpp>
#include <OpenHLX/Client/FrontPanelStateChangeNotifications.hpp>
#include <OpenHLX/Client/GroupsStateChangeNotifications.hpp>
#include <OpenHLX/Client/HLXController.hpp>
#include <OpenHLX/Client/InfraredStateChangeNotifications.hpp>
#include <OpenHLX/Client/SourcesStateChangeNotifications.hpp>
#include <OpenHLX/Client/ZonesStateChangeNotifications.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/Version.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>


using namespace HLX::Client;
using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;
using namespace boost::filesystem;
using namespace std;


// Preprocessor Definitions

#define OPT_BASE                     0x00001000

#define OPT_DEBUG                    'd'
#define OPT_HELP                     'h'
#define OPT_IPV4_ONLY                '4'
#define OPT_IPV6_ONLY                '6'
#define OPT_QUIET                    'q'
#define OPT_SYSLOG                   's'
#define OPT_TIMEOUT                  't'
#define OPT_VERBOSE                  'v'
#define OPT_VERSION                  'V'

// Command Object Options

#define OPT_EQUALIZER_PRESET         (OPT_BASE +  1)
#define OPT_GROUP                    (OPT_BASE +  2)
#define OPT_SOURCE                   (OPT_BASE +  3)
#define OPT_ZONE                     (OPT_BASE +  4)

// Command Subobject Options

#define OPT_EQUALIZER_BAND           (OPT_BASE +  5)

// Command Operation Options

#define OPT_GET_BALANCE              (OPT_BASE +  6)
#define OPT_GET_BASS                 (OPT_BASE +  7)
#define OPT_GET_EQUALIZER_BAND       (OPT_BASE +  8)
#define OPT_GET_MUTE                 (OPT_BASE +  9)
#define OPT_GET_NAME                 (OPT_BASE + 10)
#define OPT_GET_SOUND_MODE           (OPT_BASE + 11)
#define OPT_GET_SOURCE               (OPT_BASE + 12)
#define OPT_GET_TREBLE               (OPT_BASE + 13)
#define OPT_GET_VOLUME               (OPT_BASE + 14)

#define OPT_SET_BALANCE              (OPT_BASE + 15)
#define OPT_SET_BASS                 (OPT_BASE + 16)
#define OPT_SET_EQUALIZER_BAND       (OPT_BASE + 17)
#define OPT_SET_EQUALIZER_PRESET     (OPT_BASE + 18)
#define OPT_SET_HIGHPASS_CROSSOVER   (OPT_BASE + 19)
#define OPT_SET_LOWPASS_CROSSOVER    (OPT_BASE + 20)
#define OPT_SET_MUTE                 (OPT_BASE + 21)
#define OPT_SET_NAME                 (OPT_BASE + 22)
#define OPT_SET_SOUND_MODE           (OPT_BASE + 23)
#define OPT_SET_SOURCE               (OPT_BASE + 24)
#define OPT_SET_TREBLE               (OPT_BASE + 25)
#define OPT_SET_VOLUME               (OPT_BASE + 26)
#define OPT_SET_VOLUME_LOCKED        (OPT_BASE + 27)

#define OPT_DECREASE_BASS            (OPT_BASE + 28)
#define OPT_DECREASE_EQUALIZER_BAND  (OPT_BASE + 29)
#define OPT_DECREASE_TREBLE          (OPT_BASE + 30)
#define OPT_DECREASE_VOLUME          (OPT_BASE + 31)

#define OPT_INCREASE_BALANCE_LEFT    (OPT_BASE + 32)
#define OPT_INCREASE_BALANCE_RIGHT   (OPT_BASE + 33)
#define OPT_INCREASE_BASS            (OPT_BASE + 34)
#define OPT_INCREASE_EQUALIZER_BAND  (OPT_BASE + 35)
#define OPT_INCREASE_TREBLE          (OPT_BASE + 36)
#define OPT_INCREASE_VOLUME          (OPT_BASE + 37)

#define OPT_TOGGLE_MUTE              (OPT_BASE + 38)

#define OPT_ADD_ZONE                 (OPT_BASE + 39)
#define OPT_REMOVE_ZONE              (OPT_BASE + 40)


// Type Declarations

enum OptFlags {
    kOptNone            = 0x00000000,
    kOptIPv4Only        = 0x00000001,
    kOptIPv6Only        = 0x00000002,
    kOptPriority        = 0x00000004,
    kOptQuiet           = 0x00000008,
    kOptSyslog          = 0x00000010,

    kOptTimeout         = 0x00000080,

    kOptHasObjectArg    = 0x00000100,
    kOptHasSubobjectArg = 0x00000200,
    kOptHasOperationArg = 0x00000400
};

/**
 *  @brief
 *    An object that forms the value portion of an option + value
 *    command line option argument pair.
 *
 *  The object consists of a null-terminated C string pointer which
 *  points to the literal value parsed from the command line and a
 *  union of possible parsed value representations of that string,
 *  based on the option that was associated with the argument value.
 *
 */
struct Argument {
    const char *                              mString;
    union {
        BalanceModel::BalanceType             mBalance;
        ToneModel::LevelType                  mBass;
        EqualizerBandModel::IdentifierType    mEqualizerBand;
        EqualizerBandModel::LevelType         mEqualizerBandLevel;
        EqualizerPresetModel::IdentifierType  mEqualizerPreset;
        CrossoverModel::FrequencyType         mFrequency;
        GroupModel::IdentifierType            mGroup;
        SoundModel::SoundMode                 mSoundMode;
        SourceModel::IdentifierType           mSource;
        ToneModel::LevelType                  mTreble;
        VolumeModel::MuteType                 mMute;
        const char *                          mName;
        VolumeModel::LevelType               mVolume;
        VolumeModel::FixedType                mVolumeLocked;
        ZoneModel::IdentifierType             mZone;
    } mUnion;
};

typedef uint16_t Option;

/**
 *  @brief
 *    An object that represents the option + value command line option
 *    argument pair.
 *
 *  The object consists of a mapped enumeration representing the
 *  encountered command line option and a compound value representing
 *  both the string and parsed value of the string, based on the
 *  enumerated option.
 *
 *  @sa Argument
 */
struct OptionArgumentPair {
    Option              mOption;
    Argument            mArgument;
};

/**
 *  @brief
 *    An object that represents a HLX client command request, its
 *    dispatch state, and its expected completion event, if any.
 *
 */
struct ClientArgument {
    OptionArgumentPair  mObjectOptionArgument;
    OptionArgumentPair  mSubobjectOptionArgument;
    OptionArgumentPair  mOperationOptionArgument;
    bool                mDidDispatch;
    StateChange::Type   mExpectedClientArgumentCompletionEvent;
};

class Client;

// Function Prototypes

static Status DispatchCommand(Controller &aController, ClientArgument &aClientArgument, const uint32_t &aOptFlags, const Timeout &aTimeout);

// Global Variables

static uint32_t             sOptFlags            = 0;
static Log::Level           sDebug               = 0;
static Log::Level           sError               = 0;
static Log::Level           sVerbose             = 0;

static const char *         sProgram             = nullptr;

static Timeout              sTimeout;

static ClientArgument       sClientArgument;

static Client *             sHLXClient           = nullptr;

static const struct option  sOptions[] = {
    { "debug",                   optional_argument,  nullptr,   OPT_DEBUG                   },
    { "help",                    no_argument,        nullptr,   OPT_HELP                    },
    { "ipv4-only",               no_argument,        nullptr,   OPT_IPV4_ONLY               },
    { "ipv6-only",               no_argument,        nullptr,   OPT_IPV6_ONLY               },
    { "quiet",                   no_argument,        nullptr,   OPT_QUIET                   },
    { "timeout",                 required_argument,  nullptr,   OPT_TIMEOUT                 },
    { "verbose",                 optional_argument,  nullptr,   OPT_VERBOSE                 },
    { "version",                 no_argument,        nullptr,   OPT_VERSION                 },

    { "equalizer-band",          required_argument,  nullptr,   OPT_EQUALIZER_BAND          },
    { "equalizer-preset",        required_argument,  nullptr,   OPT_EQUALIZER_PRESET        },
    { "group",                   required_argument,  nullptr,   OPT_GROUP                   },
    { "source",                  required_argument,  nullptr,   OPT_SOURCE                  },
    { "zone",                    required_argument,  nullptr,   OPT_ZONE                    },

    { "get-balance",             no_argument,        nullptr,   OPT_GET_BALANCE             },
    { "get-bass",                no_argument,        nullptr,   OPT_GET_BASS                },
    { "get-equalizer-band",      no_argument,        nullptr,   OPT_GET_EQUALIZER_BAND      },
    { "get-mute",                no_argument,        nullptr,   OPT_GET_MUTE                },
    { "get-name",                no_argument,        nullptr,   OPT_GET_NAME                },
    { "get-sound-mode",          no_argument,        nullptr,   OPT_GET_SOUND_MODE          },
    { "get-source",              no_argument,        nullptr,   OPT_GET_SOURCE              },
    { "get-treble",              no_argument,        nullptr,   OPT_GET_TREBLE              },
    { "get-volume",              no_argument,        nullptr,   OPT_GET_VOLUME              },

    { "set-balance",             required_argument,  nullptr,   OPT_SET_BALANCE             },
    { "set-bass",                required_argument,  nullptr,   OPT_SET_BASS                },
    { "set-equalizer-band",      required_argument,  nullptr,   OPT_SET_EQUALIZER_BAND      },
    { "set-equalizer-preset",    required_argument,  nullptr,   OPT_SET_EQUALIZER_PRESET    },
    { "set-highpass-crossover",  required_argument,  nullptr,   OPT_SET_HIGHPASS_CROSSOVER  },
    { "set-lowpass-crossover",   required_argument,  nullptr,   OPT_SET_LOWPASS_CROSSOVER   },
    { "set-mute",                required_argument,  nullptr,   OPT_SET_MUTE                },
    { "set-name",                required_argument,  nullptr,   OPT_SET_NAME                },
    { "set-sound-mode",          required_argument,  nullptr,   OPT_SET_SOUND_MODE          },
    { "set-source",              required_argument,  nullptr,   OPT_SET_SOURCE              },
    { "set-treble",              required_argument,  nullptr,   OPT_SET_TREBLE              },
    { "set-volume",              required_argument,  nullptr,   OPT_SET_VOLUME              },
    { "set-volume-locked",       required_argument,  nullptr,   OPT_SET_VOLUME_LOCKED       },

    { "decrease-bass",           no_argument,        nullptr,   OPT_DECREASE_BASS           },
    { "decrease-equalizer-band", no_argument,        nullptr,   OPT_DECREASE_EQUALIZER_BAND },
    { "decrease-treble",         no_argument,        nullptr,   OPT_DECREASE_TREBLE         },
    { "decrease-volume",         no_argument,        nullptr,   OPT_DECREASE_VOLUME         },
    { "increase-balance-left",   no_argument,        nullptr,   OPT_INCREASE_BALANCE_LEFT   },
    { "increase-balance-right",  no_argument,        nullptr,   OPT_INCREASE_BALANCE_RIGHT  },
    { "increase-bass",           no_argument,        nullptr,   OPT_INCREASE_BASS           },
    { "increase-equalizer-band", no_argument,        nullptr,   OPT_INCREASE_EQUALIZER_BAND },
    { "increase-treble",         no_argument,        nullptr,   OPT_INCREASE_TREBLE         },
    { "increase-volume",         no_argument,        nullptr,   OPT_INCREASE_VOLUME         },

    { "toggle-mute",             no_argument,        nullptr,   OPT_TOGGLE_MUTE             },

    { "add-zone",                required_argument,  nullptr,   OPT_ADD_ZONE                },
    { "remove-zone",             required_argument,  nullptr,   OPT_REMOVE_ZONE             },

    { nullptr,                   0,                  nullptr,   0                           }
};

static const char * const   sShortUsageString =
"Usage: %s [ options ] { <URL> | <host[:port]> | <file> } [ <identifier option> [ { equalizer preset | group | source | zone } <option> ]]\n";

static const char * const   sLongUsageString =
"\n"
" General options:\n"
"\n"
"  -d, --debug[=LEVEL]                 Enable diagnostic output, optionally at\n"
"                                      level LEVEL.\n"
"  -h, --help                          Print this help, then exit.\n"
"  -q, --quiet                         Run silently, suppressing all diagnostic\n"
"                                      and informative output.\n"
"  -s, --syslog                        Write all error, diagnostic and informative\n"
"                                      output only to the system log, rather than\n"
"                                      to both the system log as well as standard\n"
"                                      error and standard output.\n"
"  -v, --verbose[=LEVEL]               Enable verbose output, optionally at level\n"
"                                      LEVEL.\n"
"  -V, --version                       Print version and copyright information, \n"
"                                      then exit.\n"
"\n"
" Client Options:\n"
"\n"
"  -4, --ipv4-only                     Force hlxc to use IPv4 addresses only.\n"
"  -6, --ipv6-only                     Force hlxc to use IPv6 addresses only.\n"
"  -t, --timeout=MILLISECONDS          Set a connection timeout of MILLISECONDS \n"
"                                      milliseconds.\n"
"\n"
" Identifier Options:\n"
"\n"
"  --equalizer-preset=PRESET           Perform operation on the equalizer preset\n"
"                                      name or identifier PRESET.\n"
"  --group=GROUP                       Perform operation on group name or\n"
"                                      identifier GROUP.\n"
"  --source=SOURCE                     Perform operation on source name or\n"
"                                      identifier SOURCE.\n"
"  --zone=ZONE                         Perform operation on zone name or\n"
"                                      identifier ZONE.\n"
"\n"
" Balance Options:\n"
"\n"
"  --increase-balance-left             Increase the balance from the current\n"
"                                      position towards the left channel by one\n"
"                                      (1) for the specified zone.\n"
"  --increase-balance-right            Increase the balance from the current\n"
"                                      position towards the right channel by\n"
"                                      one (1) for the specified zone.\n"
"  --set-balance=POSITION              Set the balance to position POSITION for\n"
"                                      the specified zone.\n"
"\n"
" Equalization Options:\n"
"\n"
"  --equalizer-band=BAND               Perform operation on equalizer band BAND.\n"
"  --decrease-equalizer-band           Decrease the level from the current\n"
"                                      position by one (1) for the specified\n"
"                                      equalizer band.\n"
"  --increase-equalizer-band           Increase the level from the current\n"
"                                      position by one (1) for the specified\n"
"                                      equalizer band.\n"
"  --set-equalizer-band=LEVEL          Set the level to level LEVEL for the\n"
"                                      specified equalizer band.\n"
"\n"
" Group Options:\n"
"\n"
"  --add-zone=ZONE                     Add the zone name or identifier ZONE to\n"
"                                      the specified group.\n"
"  --remove-zone=ZONE                  Remove the zone name or identifier ZONE\n"
"                                      to the specified group.\n"
"\n"
" Name Options:\n"
"\n"
"  --set-name=NAME                     Set the specified band, group, preset,\n"
"                                      source, or zone to the name NAME.\n"
"\n"
" Source Options:\n"
"\n"
"  --set-source=SOURCE                 Set the specified group or zone to the\n"
"                                      source name or identifier SOURCE.\n"
"\n"
" Volume Options:\n"
"\n"
"  --decrease-volume                   Decrease the volume from the current position\n"
"                                      by one (1) for the specified group or zone.\n"
"  --increase-volume                   Increase the volume from the current position\n"
"                                      by one (1) for the specified group or zone.\n"
"  --set-mute=MUTE                     Set the muting to MUTE (zero (0) or one (1))\n"
"                                      for the specified group or zone.\n"
"  --set-volume=VOLUME                 Set the volume to volume VOLUME for the\n"
"                                      specified group or zone.\n"
"  --toggle-mute                       Toggle (flip) the muting from the current\n"
"                                      value the specified group or zone.\n"
"\n"
" Zone Options:\n"
"\n"
"  --decrease-bass                     Decrease the bass level from the current\n"
"                                      position by one (1) for the specified zone.\n"
"  --decrease-treble                   Decrease the treble level from the current\n"
"                                      position by one (1) for the specified zone.\n"
"  --increase-bass                     Increase the bass level from the current\n"
"                                      position by one (1) for the specified zone.\n"
"  --increase-treble                   Increase the treble level from the current\n"
"                                      position by one (1) for the specified zone.\n"
"  --set-bass=LEVEL                    Set the bass level to level LEVEL for the\n"
"                                      specified zone.\n"
"  --set-equalizer-preset=PRESET       Set the equalizer preset to preset PRESET\n"
"                                      for the specified zone.\n"
"  --set-highpass-crossover=FREQUENCY  Set the highpass crossover frequency to\n"
"                                      frequency FREQUENCY for the specified zone.\n"
"  --set-lowpass-crossover=FREQUENCY   Set the lowpass crossover frequency to\n"
"                                      frequency FREQUENCY for the specified zone.\n"
"  --set-sound-mode=MODE               Set the zone sound mode to mode MODE.\n"
"  --set-treble=LEVEL                  Set the treble level to level LEVEL for\n"
"                                      the specified zone.\n"
"  --set-volume-locked=LOCK            Set the zone volume locking to LOCK (zero\n"
"                                      (0) or one (1)) for the specified zone.\n"
"                                      When locked the zone volume is locked at\n"
"                                      the current volume level. When unlocked,\n"
"                                      to zone volume may be changed.\n"
"\n";

namespace Detail
{
    typedef std::vector<SourceModel::IdentifierType> SourceIdentifiers;
};

/**
 *  @brief
 *    An object that effects the desired HLX command line control
 *    client.
 *
 *  This effects the core of the command line control client by
 *  instantiating an instance of the HLX client controller and that
 *  implements the required client delegations for that controller.
 *
 */
class Client :
    public ControllerDelegate
{
public:
    Client(void);
    ~Client(void);

    Status Init(void);

    Status Start(const char *aMaybeURL, const bool &aUseIPv6, const bool &aUseIPv4, const Timeout &aTimeout);
    Status Stop(void);
    Status Stop(const Status &aStatus);

    const HLX::Client::Controller &GetController(void) const;
    HLX::Client::Controller &GetController(void);
    Status GetStatus(void) const;
    void SetStatus(const Status &aStatus);

private:
    // Resolve

    void ControllerWillResolve(Controller &aController, const char *aHost) final;
    void ControllerIsResolving(Controller &aController, const char *aHost) final;
    void ControllerDidResolve(Controller &aController, const char *aHost, const IPAddress &aIPAddress) final;
    void ControllerDidNotResolve(Controller &aController, const char *aHost, const Error &aError) final;

    // Connect

    void ControllerWillConnect(Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout) final;
    void ControllerIsConnecting(Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout) final;
    void ControllerDidConnect(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotConnect(Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Disconnect

    void ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError) final;
    void ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Refresh / Reload

    void ControllerWillRefresh(Controller &aController) final;
    void ControllerIsRefreshing(Controller &aController, const uint8_t &aPercentComplete) final;
    void ControllerDidRefresh(Controller &aController) final;
    void ControllerDidNotRefresh(Controller &aController, const Error &aError) final;

    // State Change

    void ControllerStateDidChange(Controller &aController, const StateChange::NotificationBasis &aStateChangeNotification) final;

    // Error

    void ControllerError(Controller &aController, const Error &aError) final;

private:
    static void OnSignal(int aSignal);

private:
    RunLoopParameters                mRunLoopParameters;
    HLX::Client::Controller          mHLXClientController;
    Status                           mStatus;
};

Client :: Client(void) :
    ControllerDelegate(),
    mRunLoopParameters(),
    mHLXClientController(),
    mStatus(kStatus_Success)
{
    return;
}

Client :: ~Client(void)
{
    return;
}

Status Client :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRunLoopParameters.Init(CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXClientController.Init(mRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXClientController.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

static ConnectionManager::Versions
GetVersions(const bool &aUseIPv6, const bool &aUseIPv4)
{
    using Version  = ConnectionManagerBasis::Version;
    using Versions = ConnectionManagerBasis::Versions;

    const Versions kVersions =
        (((aUseIPv6) ? Version::kIPv6 : 0) |
         ((aUseIPv4) ? Version::kIPv4 : 0));

    return (kVersions);
}

Status Client :: Start(const char *aMaybeURL, const bool &aUseIPv6, const bool &aUseIPv4, const Timeout &aTimeout)
{
    Status lStatus = kStatus_Success;

    lStatus = mHLXClientController.Connect(aMaybeURL, GetVersions(aUseIPv6, aUseIPv4), aTimeout);

    return (lStatus);
}

Status Client :: Stop(void)
{
    return (Stop(kStatus_Success));
}

Status Client :: Stop(const Status &aStatus)
{
    Status lStatus = kStatus_Success;

    SetStatus(aStatus);

    if (mHLXClientController.IsConnected())
    {
        lStatus = mHLXClientController.Disconnect();
        nlEXPECT_SUCCESS(lStatus, done);
    }

 done:
    CFRunLoopStop(mRunLoopParameters.GetRunLoop());

    return (lStatus);
}

const HLX::Client::Controller &Client :: GetController(void) const
{
    return (mHLXClientController);
}

HLX::Client::Controller &Client :: GetController(void)
{
    return (mHLXClientController);
}

Status Client :: GetStatus(void) const
{
    return (mStatus);
}

void Client :: SetStatus(const Status &aStatus)
{
    mStatus = aStatus;
}

// Controller Delegate Methods

// Resolve

void Client :: ControllerWillResolve(Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Will resolve \"%s\".\n", aHost);
}

void Client :: ControllerIsResolving(Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Is resolving \"%s\".\n", aHost);
}

void Client :: ControllerDidResolve(Controller &aController, const char *aHost, const IPAddress &aIPAddress)
{
    char   lBuffer[INET6_ADDRSTRLEN];
    Status lStatus;

    (void)aController;

    lStatus = aIPAddress.ToString(lBuffer, sizeof (lBuffer));
    nlREQUIRE_SUCCESS(lStatus, done);

    Log::Info().Write("Did resolve \"%s\" to '%s'.\n", aHost, lBuffer);

 done:
    return;
}

void Client :: ControllerDidNotResolve(Controller &aController, const char *aHost, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not resolve \"%s\": %d (%s).\n", aHost, aError, strerror(-aError));
}

// Connect

void Client :: ControllerWillConnect(Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aController;

    Log::Info().Write("Will connect to %s with %u ms timeout.\n", CFString(CFURLGetString(aURLRef)).GetCString(), aTimeout.GetMilliseconds());
}

void Client :: ControllerIsConnecting(Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aController;

    Log::Info().Write("Connecting to %s with %u ms timeout.\n", CFString(CFURLGetString(aURLRef)).GetCString(), aTimeout.GetMilliseconds());
}

void Client :: ControllerDidConnect(Controller &aController, CFURLRef aURLRef)
{
    Status lStatus;

    (void)aController;

    Log::Info().Write("Connected to %s.\n", CFString(CFURLGetString(aURLRef)).GetCString());

    lStatus = mHLXClientController.Refresh();
    nlREQUIRE_SUCCESS(lStatus, done);

 done:
    return;
}

void Client :: ControllerDidNotConnect(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not connect to %s: %d (%s).\n", CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));

    Stop(aError);
}

// Disconnect

void Client :: ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will disconnect from %s.\n", CFString(CFURLGetString(aURLRef)).GetCString());
}

void Client :: ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    Status lStatus;

    if (aError >= kStatus_Success)
    {
        Log::Info().Write("Disconnected from %s.\n", CFString(CFURLGetString(aURLRef)).GetCString());
    }
    else
    {
        Log::Info().Write("Disconnected from %s: %d (%s).\n", CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
    }

    // At this point, the client has been disconnected. Set the delegate
    // to NULL to avoid any further state change notifications or other
    // delegations.

    lStatus = aController.SetDelegate(nullptr);
    nlREQUIRE_SUCCESS(lStatus, done);

    // Only call stop if we have non-error status; otherwise a
    // DidNot... or Error delegation already called it.

    if (aError != kStatus_Success)
    {
        Stop(aError);
    }

 done:
    return;
}

void Client :: ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not disconnect from %s: %d.\n", CFString(CFURLGetString(aURLRef)).GetCString(), aError);
}

// Refresh / Reload

void Client :: ControllerWillRefresh(Controller &aController)
{
    (void)aController;

    Log::Info().Write("Waiting for client data...\n");

    return;
}

void Client :: ControllerIsRefreshing(Controller &aController, const uint8_t &aPercentComplete)
{
    (void)aController;

    Log::Info().Write("%u%% of client data received.\n", aPercentComplete);
}

void Client :: ControllerDidRefresh(Controller &aController)
{
    Status lStatus;

    (void)aController;

    Log::Info().Write("Client data received.\n");

    // If the user provided both a class and a command argument, then
    // attempt to dispatch a command against those
    // arguments. Otherwise, the application was invoked in read-only,
    // query mode. In the latter case, just disconnect and quit.

    if (sOptFlags & (kOptHasObjectArg | kOptHasOperationArg))
    {
        lStatus = DispatchCommand(aController, sClientArgument, sOptFlags, sTimeout);

        if (lStatus != kStatus_Success)
        {
            Stop(lStatus);
        }
    }
    else
    {
        Stop();
    }

    return;
}

void Client :: ControllerDidNotRefresh(Controller &aController, const Error &aError)
{
    (void)aController;

    Stop(aError);
}

// State Change

void Client :: ControllerStateDidChange(Controller &aController, const StateChange::NotificationBasis &aStateChangeNotification)
{
    const StateChange::Type lType = aStateChangeNotification.GetType();

    (void)aController;

    switch (lType)
    {

    case StateChange::kStateChangeType_ConfigurationSavingToBackup:
        Log::Debug().Write("Saving configuration\n");
        break;

    case StateChange::kStateChangeType_ConfigurationSavedToBackup:
        Log::Debug().Write("Saved configuration\n");
        break;

    case StateChange::kStateChangeType_EqualizerPresetBand:
        {
            const StateChange::EqualizerPresetsBandNotification &lSCN = static_cast<const StateChange::EqualizerPresetsBandNotification &>(aStateChangeNotification);
            Log::Debug().Write("Equalizer preset %hhu equalizer band %hhu level is %hhd\n",
                               lSCN.GetIdentifier(), lSCN.GetBand(), lSCN.GetLevel());
        }
        break;

    case StateChange::kStateChangeType_EqualizerPresetName:
        {
            const StateChange::EqualizerPresetsNameNotification &lSCN = static_cast<const StateChange::EqualizerPresetsNameNotification &>(aStateChangeNotification);
            Log::Debug().Write("Name for equalizer preset %hhu is '%s'\n",
                               lSCN.GetIdentifier(), lSCN.GetName());
        }
        break;

    case StateChange::kStateChangeType_FavoriteName:
        {
            const StateChange::FavoritesNameNotification &lSCN = static_cast<const StateChange::FavoritesNameNotification &>(aStateChangeNotification);
            Log::Debug().Write("Name for favorite %hhu is '%s'\n",
                               lSCN.GetIdentifier(), lSCN.GetName());
        }
        break;

    case StateChange::kStateChangeType_FrontPanelBrightness:
        {
            const StateChange::FrontPanelBrightnessNotification &lSCN = static_cast<const StateChange::FrontPanelBrightnessNotification &>(aStateChangeNotification);
            Log::Debug().Write("Front panel brightness is %u\n",
                               lSCN.GetBrightness());
        }
        break;

    case StateChange::kStateChangeType_FrontPanelLocked:
        {
            const StateChange::FrontPanelLockedNotification &lSCN = static_cast<const StateChange::FrontPanelLockedNotification &>(aStateChangeNotification);
            Log::Debug().Write("Front panel is %s\n",
                               (lSCN.GetLocked() ? "locked" : "unlocked"));
        }
        break;

    case StateChange::kStateChangeType_GroupMute:
        {
            const StateChange::GroupsMuteNotification &lSCN = static_cast<const StateChange::GroupsMuteNotification &>(aStateChangeNotification);
            Log::Debug().Write("Volume for group %hhu is %smuted\n",
                               lSCN.GetIdentifier(), ((lSCN.GetMute()) ? "" : "not "));
        }
        break;

    case StateChange::kStateChangeType_GroupName:
        {
            const StateChange::GroupsNameNotification &lSCN = static_cast<const StateChange::GroupsNameNotification &>(aStateChangeNotification);
            Log::Debug().Write("Name for group %hhu is '%s'\n",
                               lSCN.GetIdentifier(), lSCN.GetName());
        }
        break;

    case StateChange::kStateChangeType_GroupSource:
        {
            const StateChange::GroupsSourceNotification &  lSCN = static_cast<const StateChange::GroupsSourceNotification &>(aStateChangeNotification);
            const GroupModel::Sources &                    lGroupSources = lSCN.GetSources();
            size_t                                         lGroupSourceCount = lSCN.GetCount();
            Detail::SourceIdentifiers                      lGroupSourceIdentifiers;
            Status                                         lStatus;
            std::string                                    lSourcesString;

            lGroupSourceIdentifiers.resize(lGroupSourceCount);

            lStatus = lGroupSources.GetIdentifiers(&lGroupSourceIdentifiers[0], lGroupSourceCount);
            nlREQUIRE_SUCCESS(lStatus, done);

            {
                Detail::SourceIdentifiers::const_iterator lGroupSourceCurrent = lGroupSourceIdentifiers.begin();
                Detail::SourceIdentifiers::const_iterator lGroupSourceEnd = lGroupSourceIdentifiers.end();
                size_t lCount = 0;

                while (lGroupSourceCurrent != lGroupSourceEnd)
                {
                    OutputStringStream  lGroupSourceIdentifierStream;

                    lCount++;

                    lGroupSourceIdentifierStream << *lGroupSourceCurrent;

                    lSourcesString += lGroupSourceIdentifierStream.str();

                    if (lGroupSourceCount > 2)
                    {
                        if ((lGroupSourceCount - lCount) >= 1)
                        {
                            lSourcesString += ",";
                        }

                        if ((lGroupSourceCount - lCount) > 1)
                        {
                            lSourcesString += " ";
                        }
                    }

                    if (lGroupSourceCount > 1)
                    {
                        if ((lGroupSourceCount - lCount) == 1)
                        {
                            lSourcesString += " and ";
                        }
                    }

                    lGroupSourceCurrent++;
                }
            }

            Log::Debug().Write("Source%s for group %hhu %s %s\n",
                               ((lGroupSourceCount == 1) ? "" : "s"),
                               lSCN.GetIdentifier(),
                               ((lGroupSourceCount == 1) ? "is" : "are"),
                               lSourcesString.c_str());
        }
        break;

    case StateChange::kStateChangeType_GroupVolume:
        {
            const StateChange::GroupsVolumeNotification &lSCN = static_cast<const StateChange::GroupsVolumeNotification &>(aStateChangeNotification);
            Log::Debug().Write("Volume for group %hhu is %d\n",
                               lSCN.GetIdentifier(), lSCN.GetVolume());
        }
        break;

    case StateChange::kStateChangeType_GroupZoneAdded:
    case StateChange::kStateChangeType_GroupZoneRemoved:
        {
            const bool lIsZoneAdded = (lType == StateChange::kStateChangeType_GroupZoneAdded);
            const StateChange::GroupsZoneNotificationBasis &lSCN = static_cast<const StateChange::GroupsZoneNotificationBasis &>(aStateChangeNotification);
            Log::Debug().Write("%s zone %hhu for group %hhu\n",
                               (lIsZoneAdded ? "Added" : "Removed"),
                               lSCN.GetZone(), lSCN.GetIdentifier());
        }
        break;

    case StateChange::kStateChangeType_InfraredDisabled:
        {
            const StateChange::InfraredDisabledNotification &lSCN = static_cast<const StateChange::InfraredDisabledNotification &>(aStateChangeNotification);
            Log::Debug().Write("Front panel infrared sensor is %s\n",
                               (lSCN.GetDisabled() ? "disabled" : "enabled"));
        }
        break;

    case StateChange::kStateChangeType_SourceName:
        {
            const StateChange::SourcesNameNotification &lSCN = static_cast<const StateChange::SourcesNameNotification &>(aStateChangeNotification);
            Log::Debug().Write("Name for source %hhu is '%s'\n",
                               lSCN.GetIdentifier(), lSCN.GetName());
        }
        break;

    case StateChange::kStateChangeType_ZoneBalance:
        {
            const StateChange::ZonesBalanceNotification &lSCN = static_cast<const StateChange::ZonesBalanceNotification &>(aStateChangeNotification);
            Log::Debug().Write("Balance for zone %hhu is %d\n",
                               lSCN.GetIdentifier(), lSCN.GetBalance());
        }
        break;

    case StateChange::kStateChangeType_ZoneEqualizerBand:
        {
            const StateChange::ZonesEqualizerBandNotification &lSCN = static_cast<const StateChange::ZonesEqualizerBandNotification &>(aStateChangeNotification);
            Log::Debug().Write("Zone %hhu equalizer band %hhu level is %hhd\n",
                               lSCN.GetIdentifier(), lSCN.GetBand(), lSCN.GetLevel());
        }
        break;

    case StateChange::kStateChangeType_ZoneEqualizerPreset:
        {
            const StateChange::ZonesEqualizerPresetNotification &lSCN = static_cast<const StateChange::ZonesEqualizerPresetNotification &>(aStateChangeNotification);
            Log::Debug().Write("Equalizer preset for zone %hhu is %hhu\n",
                               lSCN.GetIdentifier(), lSCN.GetEqualizerPreset());
        }
        break;

    case StateChange::kStateChangeType_ZoneHighpassCrossover:
        {
            const StateChange::ZonesHighpassCrossoverNotification &lSCN = static_cast<const StateChange::ZonesHighpassCrossoverNotification &>(aStateChangeNotification);
            Log::Debug().Write("Highpass crossover frequency for zone %hhu is %hu Hz\n",
                               lSCN.GetIdentifier(), lSCN.GetFrequency());
        }
        break;

    case StateChange::kStateChangeType_ZoneLowpassCrossover:
        {
            const StateChange::ZonesLowpassCrossoverNotification &lSCN = static_cast<const StateChange::ZonesLowpassCrossoverNotification &>(aStateChangeNotification);
            Log::Debug().Write("Lowpass crossover frequency for zone %hhu is %hu Hz\n",
                               lSCN.GetIdentifier(), lSCN.GetFrequency());
        }
        break;

    case StateChange::kStateChangeType_ZoneMute:
        {
            const StateChange::ZonesMuteNotification &lSCN = static_cast<const StateChange::ZonesMuteNotification &>(aStateChangeNotification);
            Log::Debug().Write("Volume for zone %hhu is %smuted\n",
                               lSCN.GetIdentifier(), ((lSCN.GetMute()) ? "" : "not "));
        }
        break;

    case StateChange::kStateChangeType_ZoneName:
        {
            const StateChange::ZonesNameNotification &lSCN = static_cast<const StateChange::ZonesNameNotification &>(aStateChangeNotification);
            Log::Debug().Write("Name for zone %hhu is '%s'\n",
                               lSCN.GetIdentifier(), lSCN.GetName());
        }
        break;

    case StateChange::kStateChangeType_ZoneSoundMode:
        {
            const StateChange::ZonesSoundModeNotification &lSCN = static_cast<const StateChange::ZonesSoundModeNotification &>(aStateChangeNotification);
            Log::Debug().Write("Sound mode for zone %hhu is %hhu\n",
                               lSCN.GetIdentifier(), lSCN.GetSoundMode());

        }
        break;

    case StateChange::kStateChangeType_ZoneSource:
        {
            const StateChange::ZonesSourceNotification &lSCN = static_cast<const StateChange::ZonesSourceNotification &>(aStateChangeNotification);
            Log::Debug().Write("Source for zone %hhu is %hhu\n",
                               lSCN.GetIdentifier(), lSCN.GetSource());
        }
        break;

    case StateChange::kStateChangeType_ZoneTone:
        {
            const StateChange::ZonesToneNotification &lSCN = static_cast<const StateChange::ZonesToneNotification &>(aStateChangeNotification);
            Log::Debug().Write("Bass and treble for zone %hhu are %hhd and %hhd\n",
                               lSCN.GetIdentifier(), lSCN.GetBass(), lSCN.GetTreble());
        }
        break;

    case StateChange::kStateChangeType_ZoneVolume:
        {
            const StateChange::ZonesVolumeNotification &lSCN = static_cast<const StateChange::ZonesVolumeNotification &>(aStateChangeNotification);
            Log::Debug().Write("Volume for zone %hhu is %d\n",
                               lSCN.GetIdentifier(), lSCN.GetVolume());
        }
        break;

    case StateChange::kStateChangeType_ZoneVolumeLocked:
        {
            const StateChange::ZonesVolumeLockedNotification &lSCN = static_cast<const StateChange::ZonesVolumeLockedNotification &>(aStateChangeNotification);
            Log::Debug().Write("Volume for zone %hhu is %slocked\n",
                               lSCN.GetIdentifier(), ((lSCN.GetLocked()) ? "" : "not "));
        }
        break;

    default:
        Log::Error().Write("Unhandled state change notification type %d\n", lType);
        break;
    }

 done:
    if (sClientArgument.mDidDispatch == true)
    {
        if (lType == sClientArgument.mExpectedClientArgumentCompletionEvent)
        {
            Stop();
        }
    }

    return;
}

// Error

void Client :: ControllerError(Controller &aController, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Error: %d (%s).\n", aError, strerror(-aError));

    Stop(aError);
}

void Client :: OnSignal(int aSignal)
{
    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);
}

static void OnSignal(int aSignal)
{
    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);

    if (sHLXClient != nullptr)
    {
        sHLXClient->Stop(-errno);
    }
}

static void SetSignalHandler(int aSignal, void (*aHandler)(int aSignal))
{
    struct sigaction sa;
    int              signals[] = { aSignal };

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = aHandler;

    for (size_t i = 0; i < ElementsOf(signals); i++)
    {
        if (sigaction(signals[i], &sa, nullptr) == -1)
        {
            perror("Can't catch signal");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 *  unsigned int SetLevel()
 *
 *  Description:
 *    This routine attempts to parse the specified argument as a debug
 *    or information/verbosity level and, if successful, sets the
 *    specified level value. Otherwise, if the specified argument is
 *    NULL, then the level is simply incremented.
 *
 *  Input(s):
 *    inLevel    - A reference to the value to set or increment.
 *    inArgument - An optional pointer to a NULL-terminated C string
 *                 representing a level to parse and set if valid.
 *
 *  Output(s):
 *    inLevel    - A reference to the set or incremented value.
 *
 *  Returns:
 *    Zero (0) if OK; otherwise, the number of errors encountered.
 *
 */
static unsigned int
SetLevel(Log::Level &inLevel, const char *inArgument)
{
    unsigned int errors = 0;

    if (inArgument != nullptr) {
        inLevel = static_cast<Log::Level>(strtoul(inArgument, nullptr, 10));

        if (inLevel == UINT32_MAX || errno == ERANGE) {
            Log::Error().Write("Invalid log level `%s'\n", inArgument);
            errors++;
        }

    } else {
        inLevel++;

    }

    return (errors);
}

/*
 *  void PrintUsage()
 *
 *  Description:
 *    This routine prints out the proper command line usage for this
 *    program.
 *
 *  Input(s):
 *    *inProgram - The name with which the program was invoked by
 *                 the parent process.
 *    inStatus   - The exit status returned to the parent process.
 *
 *  Output(s):
 *    N/A
 *
 *  Returns:
 *    N/A
 *
 */
static void
PrintUsage(const char *inProgram, int inStatus)
{
    const char * const theName = path(inProgram).leaf().c_str();

    // Regardless of the desired exit status, display a short usage
    // synopsis.

    printf(sShortUsageString, theName);

    // Depending on the desired exit status, display either a helpful
    // suggestion on obtaining more information or display a long
    // usage synopsis.

    if (inStatus != EXIT_SUCCESS)
        printf("Try `%s -h' for more information.\n", theName);

    if (inStatus != EXIT_FAILURE) {
        printf(sLongUsageString);
    }

    exit(inStatus);
}

static void
PrintVersion(const char *inProgram)
{
    const char * const theName = path(inProgram).leaf().c_str();

    printf("%s %s\n%s\n",
           theName,
           GetVersionString(),
           GetCopyrightString());

    exit(EXIT_SUCCESS);
}

static void
SetObjectOption(ClientArgument &aClientArgument, const Option &aOption, const char *aArgument, uint32_t &aOptFlags)
{
    aClientArgument.mObjectOptionArgument.mOption = aOption;
    aClientArgument.mObjectOptionArgument.mArgument.mString = aArgument;

    aOptFlags |= kOptHasObjectArg;
}

static void
SetSubobjectOption(ClientArgument &aClientArgument, const Option &aOption, const char *aArgument, uint32_t &aOptFlags)
{
    aClientArgument.mSubobjectOptionArgument.mOption = aOption;
    aClientArgument.mSubobjectOptionArgument.mArgument.mString = aArgument;

    aOptFlags |= kOptHasSubobjectArg;
}

static void
SetOperationOption(ClientArgument &aClientArgument, const Option &aOption, const char *aArgument, uint32_t &aOptFlags)
{
    aClientArgument.mOperationOptionArgument.mOption = aOption;
    aClientArgument.mOperationOptionArgument.mArgument.mString = aArgument;

    // Establish the received state change notification event that
    // will successfully terminate the command request.

    switch (aOption)
    {

    case OPT_DECREASE_EQUALIZER_BAND:
    case OPT_INCREASE_EQUALIZER_BAND:
    case OPT_SET_EQUALIZER_BAND:
        switch (aClientArgument.mObjectOptionArgument.mOption)
        {

        case OPT_EQUALIZER_PRESET:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_EqualizerPresetBand;
            break;

        case OPT_ZONE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneEqualizerBand;
            break;

        }
        break;

    case OPT_INCREASE_BALANCE_LEFT:
    case OPT_INCREASE_BALANCE_RIGHT:
    case OPT_SET_BALANCE:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneBalance;
        break;

    case OPT_SET_EQUALIZER_PRESET:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneEqualizerPreset;
        break;

    case OPT_SET_HIGHPASS_CROSSOVER:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneHighpassCrossover;
        break;

    case OPT_SET_LOWPASS_CROSSOVER:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneLowpassCrossover;
        break;

    case OPT_SET_MUTE:
    case OPT_TOGGLE_MUTE:
        switch (aClientArgument.mObjectOptionArgument.mOption)
        {

        case OPT_GROUP:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupMute;
            break;

        case OPT_ZONE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneMute;
            break;

        }
        break;

    case OPT_SET_NAME:
        switch (aClientArgument.mObjectOptionArgument.mOption)
        {
        case OPT_EQUALIZER_PRESET:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_EqualizerPresetName;
            break;

        case OPT_GROUP:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupName;
            break;

        case OPT_SOURCE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_SourceName;
            break;

        case OPT_ZONE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneName;
            break;

        }
        break;

    case OPT_SET_SOUND_MODE:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneSoundMode;
        break;

    case OPT_SET_SOURCE:
        switch (aClientArgument.mObjectOptionArgument.mOption)
        {

        case OPT_GROUP:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupSource;
            break;

        case OPT_ZONE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneSource;
            break;

        }
        break;

    case OPT_DECREASE_BASS:
    case OPT_DECREASE_TREBLE:
    case OPT_INCREASE_BASS:
    case OPT_INCREASE_TREBLE:
    case OPT_SET_BASS:
    case OPT_SET_TREBLE:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneTone;
        break;

    case OPT_DECREASE_VOLUME:
    case OPT_INCREASE_VOLUME:
    case OPT_SET_VOLUME:
        switch (aClientArgument.mObjectOptionArgument.mOption)
        {

        case OPT_GROUP:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupVolume;
            break;

        case OPT_ZONE:
            aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneVolume;
            break;

        }
        break;

    case OPT_SET_VOLUME_LOCKED:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_ZoneVolumeLocked;
        break;

    case OPT_ADD_ZONE:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupZoneAdded;
        break;

    case OPT_REMOVE_ZONE:
        aClientArgument.mExpectedClientArgumentCompletionEvent = StateChange::kStateChangeType_GroupZoneRemoved;
        break;

    default:
        break;

    }

    aOptFlags |= kOptHasOperationArg;
}

/*
 *  void DecodeOptions()
 *
 *  Description:
 *    This routine steps through the command-line arguments, parsing out
 *    recognzied options.
 *
 *  Input(s):
 *    inProgram - A pointer to a NULL-terminated C string of the name of
 *                the program.
 *    argc      - The number of arguments on the command line.
 *    argv      - A pointer to an array of C strings containing the
 *                command-line arguments.
 *    inOptions - A pointer to an options list enumerating the allowed/
 *                expected program options and arguments.
 *
 *  Output(s):
 *    N/A
 *
 *  Returns:
 *    N/A
 *
 */
static void
DecodeOptions(const char *inProgram,
              int argc,
              char * const argv[],
              const struct option *inOptions,
              size_t & outConsumed)
{
    const bool      posixly_correct = true;
    char const *    p;
    int             c;
    unsigned int    error = 0;
    string          shortOptions;
    Timeout::Value  timeoutMilliseconds;

    // Generate a list of those single-character options available as
    // a subset of the long option list.

    Nuovations::Utilities::GenerateShortOptions(!posixly_correct, inOptions, shortOptions);

    p = shortOptions.c_str();

    // Start parsing invocation options

    while (!error && (c = getopt_long(argc, argv, p, inOptions, nullptr)) != -1) {

        switch (c) {

        case OPT_DEBUG:
            error += SetLevel(sDebug, optarg);
            break;

        case OPT_HELP:
            PrintUsage(inProgram, EXIT_SUCCESS);
            break;

        case OPT_IPV4_ONLY:
            if (sOptFlags & kOptIPv6Only)
            {
                Log::Error().Write("The '-6' and '-4' options are mutually-exclusive. Please choose one or the other.\n");
                error++;
            }
            else
            {
                sOptFlags |= kOptIPv4Only;
            }
            break;

        case OPT_IPV6_ONLY:
            if (sOptFlags & kOptIPv4Only)
            {
                Log::Error().Write("The '-4' and '-6' options are mutually-exclusive. Please choose one or the other.\n");
                error++;
            }
            else
            {
                sOptFlags |= kOptIPv6Only;
            }
            break;

        case OPT_QUIET:
            sOptFlags |= kOptQuiet;
            break;

        case OPT_SYSLOG:
            sOptFlags |= kOptSyslog;
            break;

        case OPT_TIMEOUT:
            {
                sOptFlags |= kOptTimeout;
                timeoutMilliseconds = static_cast<Timeout::Value>(atoi(optarg));
            }
            break;

        case OPT_VERBOSE:
            error += SetLevel(sVerbose, optarg);
            break;

        case OPT_VERSION:
            PrintVersion(inProgram);
            break;

        case OPT_GROUP:
        case OPT_EQUALIZER_PRESET:
        case OPT_SOURCE:
        case OPT_ZONE:
            SetObjectOption(sClientArgument,
                            static_cast<Option>(c),
                            optarg,
                            sOptFlags);
            break;

        case OPT_EQUALIZER_BAND:
            SetSubobjectOption(sClientArgument,
                               static_cast<Option>(c),
                               optarg,
                               sOptFlags);
            break;

        case OPT_GET_BALANCE:
        case OPT_GET_BASS:
        case OPT_GET_EQUALIZER_BAND:
        case OPT_GET_MUTE:
        case OPT_GET_NAME:
        case OPT_GET_SOUND_MODE:
        case OPT_GET_SOURCE:
        case OPT_GET_TREBLE:
        case OPT_GET_VOLUME:
        case OPT_SET_BALANCE:
        case OPT_SET_BASS:
        case OPT_SET_EQUALIZER_BAND:
        case OPT_SET_EQUALIZER_PRESET:
        case OPT_SET_HIGHPASS_CROSSOVER:
        case OPT_SET_LOWPASS_CROSSOVER:
        case OPT_SET_MUTE:
        case OPT_SET_NAME:
        case OPT_SET_SOUND_MODE:
        case OPT_SET_SOURCE:
        case OPT_SET_TREBLE:
        case OPT_SET_VOLUME:
        case OPT_SET_VOLUME_LOCKED:
        case OPT_ADD_ZONE:
        case OPT_REMOVE_ZONE:
        case OPT_DECREASE_BASS:
        case OPT_DECREASE_EQUALIZER_BAND:
        case OPT_DECREASE_TREBLE:
        case OPT_DECREASE_VOLUME:
        case OPT_INCREASE_BALANCE_LEFT:
        case OPT_INCREASE_BALANCE_RIGHT:
        case OPT_INCREASE_BASS:
        case OPT_INCREASE_EQUALIZER_BAND:
        case OPT_INCREASE_TREBLE:
        case OPT_INCREASE_VOLUME:
        case OPT_TOGGLE_MUTE:
            SetOperationOption(sClientArgument,
                               static_cast<Option>(c),
                               optarg,
                               sOptFlags);
            break;

        default:
            Log::Error().Write("Unknown option '%d'!\n", optopt);
            error++;
            break;

        }
    }

    // If we have accumulated any errors at this point, bail out since
    // any further handling of arguments is likely to fail due to bad
    // user input.

    if (error) {
        goto exit;
    }

    // Update argument parameters to reflect those consumed by getopt.

    argc -= optind;
    argv += optind;

    outConsumed = static_cast<size_t>(optind);

    // Reset the optind value; otherwise, option processing in any
    // dispatched command will skip that many arguments before option
    // processing actually starts.

    optind = 0;

    // At this point, we should have exactly one other argument, the
    // URL, path, or host name and optional port to connect to.

    if (argc != 1) {
        error++;
        goto exit;
    }

    // Check that the timeout, if specified, makes sense.

    if (sOptFlags & kOptTimeout) {
        if (timeoutMilliseconds <= 0) {
            Log::Error().Write("The specified timeout `%d' is not greater "
                               "than zero. Please specify a non-zero, "
                               "positive timeout.\n", timeoutMilliseconds);
            error++;

        } else {
            const Timeout tempTimeout(timeoutMilliseconds);

            sTimeout = tempTimeout;

        }
    } else {
        sTimeout = kTimeoutDefault;

    }

    // If there were any errors parsing the command line arguments,
    // remind the user of proper invocation semantics and return an
    // error to the parent process.

exit:
    if (error) {
        PrintUsage(inProgram, EXIT_FAILURE);
    }

    return;
}

/*
 *  bool FilterSyslog()
 *
 *  Description:
 *    This routine filters any writers from the specified log chain
 *    that are not syslog writers.
 *
 *    Note that we have to be careful here. Chains are copied as a
 *    shared pointer, so they are effectively just aliases. So,
 *    we need to first find the writer(s) we are looking for, reset
 *    the chain and then add them back. Otherwise, if we simply try
 *    to copy the input chain and then reset it, we'll loose all the
 *    writers.
 *
 *  Input(s):
 *    inChain - A reference to the writer chain that should be
 *              scrubbed of all writers but those for the syslog.
 *
 *  Output(s):
 *    inChain - A reference to the writer chain scrubbed of all
 *              writers but those for the syslog.
 *
 *  Returns:
 *    True if syslog writers were successfully filtered from the
 *    writer chain.
 *
 */
static bool
FilterSyslog(Log::Writer::Chain &inChain)
{
    bool didFilter = false;
    const size_t theLinks = inChain.Size();
    vector<Log::Writer::Base *> savedWriters;

    // First, find and save the writers we would like to keep in the
    // writer chain, syslog writers.

    for (size_t theLink = 0; theLink < theLinks; theLink++) {
            Log::Writer::Base * theWriter =
                    inChain.Link<Log::Writer::Base>(theLink);

            if (theWriter != nullptr) {
                    if (typeid(*theWriter) == typeid(Log::Writer::Syslog)) {
                            savedWriters.push_back(theWriter);
                    }
            }
    }

    // Now, reset the input chain and add back any found syslog writers.

    inChain.Reset();

    BOOST_FOREACH(Log::Writer::Base * savedWriter, savedWriters) {
            inChain.Push(*static_cast<Log::Writer::Syslog *>(savedWriter));
    }

    didFilter = (savedWriters.size() && inChain.Size());

    return (didFilter);
}

/*
 *  bool FilterSyslog()
 *
 *  Description:
 *    This routine filters any writers from the specified logger that
 *    are not syslog writers.
 *
 *  Input(s):
 *    inLogger - A reference to the logger for which the writer chain
 *               should be scrubbed of all writers but those for the
 *               syslog.
 *
 *  Output(s):
 *    inLogger - A reference to the logger with its writer chain
 *               scrubbed of all writers but those for the syslog.
 *
 *  Returns:
 *    True if a syslog writer(s) was/were successfully filtered from
 *    the logger; otherwise, false.
 *
 */
static bool
FilterSyslog(Log::Logger &inLogger)
{
    Log::Writer::Base & theWriter = inLogger.GetWriter();

    if (typeid(theWriter) == typeid(Log::Writer::Chain))
    {
        return (FilterSyslog(static_cast<Log::Writer::Chain &>(theWriter)));
    }

    return (typeid(theWriter) == typeid(Log::Writer::Syslog));
}

static Status ParseIdentifier(const char *aObjectDescription, const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    Status lRetval;

    using HLX::Model::Utilities::ParseIdentifier;

    lRetval = ParseIdentifier(aString, aIdentifier);
    nlEXPECT_SUCCESS_ACTION(lRetval,
                            done,
                            Log::Error().Write("Error: unknown %s name or identifier \"%s\".\n",
                                               aObjectDescription, aString));

 done:
    return (lRetval);
}

static Status ParseEqualizerBandIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kObjectDescription = "equalizer band";

    return (ParseIdentifier(kObjectDescription, aString, aIdentifier));
}

static Status ParseEqualizerPresetIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kObjectDescription = "equalizer preset";

    return (ParseIdentifier(kObjectDescription, aString, aIdentifier));
}

static Status ParseGroupIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kObjectDescription = "group";

    return (ParseIdentifier(kObjectDescription, aString, aIdentifier));
}

static Status ParseSourceIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kObjectDescription = "source";

    return (ParseIdentifier(kObjectDescription, aString, aIdentifier));
}

static Status ParseZoneIdentifier(const char *aString, IdentifierModel::IdentifierType &aIdentifier)
{
    static const char * const kObjectDescription = "zone";

    return (ParseIdentifier(kObjectDescription, aString, aIdentifier));
}

static Status ParseObjectOption(Controller &aController, ClientArgument &aClientArgument)
{
    Status  lRetval = kStatus_Success;

    using HLX::Utilities::Parse;

    switch (aClientArgument.mObjectOptionArgument.mOption)
    {
    case OPT_EQUALIZER_PRESET:
        Log::Debug().Write("Attempting to convert equalizer preset \"%s\" into an identifier...\n", aClientArgument.mObjectOptionArgument.mArgument.mString);

        lRetval = aController.EqualizerPresetLookupIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset);
        if (lRetval != kStatus_Success)
        {
            lRetval = ParseEqualizerPresetIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset);
            nlEXPECT_SUCCESS(lRetval, done);
        }
        break;

    case OPT_GROUP:
        Log::Debug().Write("Attempting to convert group \"%s\" into an identifier...\n", aClientArgument.mObjectOptionArgument.mArgument.mString);

        lRetval = aController.GroupLookupIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup);
        if (lRetval != kStatus_Success)
        {
            lRetval = ParseGroupIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup);
            nlEXPECT_SUCCESS(lRetval, done);
        }
        break;

    case OPT_SOURCE:
        Log::Debug().Write("Attempting to convert source \"%s\" into an identifier...\n", aClientArgument.mObjectOptionArgument.mArgument.mString);

        lRetval = aController.SourceLookupIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mSource);
        if (lRetval != kStatus_Success)
        {
            lRetval = ParseSourceIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mSource);
            nlEXPECT_SUCCESS(lRetval, done);
        }
        break;

    case OPT_ZONE:
        Log::Debug().Write("Attempting to convert zone \"%s\" into an identifier...\n", aClientArgument.mObjectOptionArgument.mArgument.mString);

        lRetval = aController.ZoneLookupIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        if (lRetval != kStatus_Success)
        {
            lRetval = ParseZoneIdentifier(aClientArgument.mObjectOptionArgument.mArgument.mString, aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
            nlEXPECT_SUCCESS(lRetval, done);
        }
        break;

    default:
        Log::Error().Write("Unknown class '%d'!\n", aClientArgument.mObjectOptionArgument.mOption);
        lRetval = -EINVAL;
        break;
    }

 done:
    return (lRetval);
}

static Status ParseSubobjectOption(Controller &aController, ClientArgument &aClientArgument)
{
    Status  lRetval = kStatus_Success;

    using HLX::Utilities::Parse;

    (void)aController;

    switch (aClientArgument.mSubobjectOptionArgument.mOption)
    {

    case OPT_EQUALIZER_BAND:
        lRetval = ParseEqualizerBandIdentifier(aClientArgument.mSubobjectOptionArgument.mArgument.mString, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    default:
        Log::Error().Write("Unknown subclass '%d'!\n", aClientArgument.mSubobjectOptionArgument.mOption);
        lRetval = -EINVAL;
        break;

    }

 done:
    return (lRetval);
}

static Status ParseOperationOption(Controller &aController, ClientArgument &aClientArgument)
{
    Status  lRetval = kStatus_Success;

    using HLX::Utilities::Parse;


    switch (aClientArgument.mOperationOptionArgument.mOption)
    {
    case OPT_GET_BALANCE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_EQUALIZER_BAND:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_BASS:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_MUTE:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_NAME:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_SOURCE)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_SOUND_MODE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_SOURCE:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_TREBLE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_VOLUME:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_SET_BALANCE:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mBalance);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_EQUALIZER_BAND:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerBandLevel);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_BASS:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mBass);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_EQUALIZER_PRESET:
        {
            Log::Debug().Write("Attempting to convert equalizer preset \"%s\" into an identifier...\n", aClientArgument.mOperationOptionArgument.mArgument.mString);

            lRetval = aController.EqualizerPresetLookupIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerPreset);
            if (lRetval != kStatus_Success)
            {
                lRetval = ParseEqualizerPresetIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerPreset);
                nlEXPECT_SUCCESS(lRetval, done);
            }
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        break;

    case OPT_SET_HIGHPASS_CROSSOVER:
    case OPT_SET_LOWPASS_CROSSOVER:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mFrequency);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_MUTE:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mMute);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_NAME:
        aClientArgument.mOperationOptionArgument.mArgument.mUnion.mName = aClientArgument.mOperationOptionArgument.mArgument.mString;
        break;

    case OPT_SET_SOUND_MODE:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSoundMode);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_SOURCE:
        {
            Log::Debug().Write("Attempting to convert source \"%s\" into an identifier...\n", aClientArgument.mOperationOptionArgument.mArgument.mString);

            lRetval = aController.SourceLookupIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSource);
            if (lRetval != kStatus_Success)
            {
                lRetval = ParseSourceIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSource);
                nlEXPECT_SUCCESS(lRetval, done);
            }
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        break;

    case OPT_SET_TREBLE:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mTreble);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_VOLUME:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolume);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_SET_VOLUME_LOCKED:
        lRetval = Parse(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolumeLocked);
        nlREQUIRE_SUCCESS(lRetval, done);
        break;

    case OPT_ADD_ZONE:
    case OPT_REMOVE_ZONE:
        {
            Log::Debug().Write("Attempting to convert zone \"%s\" into an identifier...\n", aClientArgument.mOperationOptionArgument.mArgument.mString);

            lRetval = aController.ZoneLookupIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mZone);
            if (lRetval != kStatus_Success)
            {
                lRetval = ParseZoneIdentifier(aClientArgument.mOperationOptionArgument.mArgument.mString, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mZone);
                nlEXPECT_SUCCESS(lRetval, done);
            }
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        break;

    case OPT_DECREASE_EQUALIZER_BAND:
    case OPT_DECREASE_BASS:
    case OPT_DECREASE_TREBLE:
    case OPT_DECREASE_VOLUME:
    case OPT_INCREASE_BALANCE_LEFT:
    case OPT_INCREASE_BALANCE_RIGHT:
    case OPT_INCREASE_EQUALIZER_BAND:
    case OPT_INCREASE_BASS:
    case OPT_INCREASE_TREBLE:
    case OPT_INCREASE_VOLUME:
    case OPT_TOGGLE_MUTE:
        break;

    default:
        Log::Error().Write("Unknown command '%d'!\n", aClientArgument.mOperationOptionArgument.mOption);
        lRetval = -EINVAL;
        break;
    }

 done:
    return (lRetval);
}

static Status DispatchCommand(Controller &aController, ClientArgument &aClientArgument, const Timeout &aTimeout)
{
    Status  lRetval = kStatus_Success;

    (void)aTimeout;

    switch (aClientArgument.mOperationOptionArgument.mOption)
    {
    case OPT_GET_BALANCE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_EQUALIZER_BAND:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_BASS:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_MUTE:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_NAME:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_SOURCE)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_SOUND_MODE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_SOURCE:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_GET_TREBLE:
        lRetval = -ENOSYS;
        break;

    case OPT_GET_VOLUME:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = -ENOSYS;
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = -ENOSYS;
        else
            lRetval = -EINVAL;
        break;

    case OPT_SET_BALANCE:
        lRetval = aController.ZoneSetBalance(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mBalance);
        break;

    case OPT_SET_EQUALIZER_BAND:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_EQUALIZER_PRESET)
            lRetval = aController.EqualizerPresetSetBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerBandLevel);
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = aController.ZoneSetEqualizerBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerBandLevel);
        else
            lRetval = -EINVAL;
        break;

    case OPT_SET_BASS:
        lRetval = aController.ZoneSetBass(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mBass);
        break;

    case OPT_SET_EQUALIZER_PRESET:
        lRetval = aController.ZoneSetEqualizerPreset(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mEqualizerPreset);
        break;

    case OPT_SET_HIGHPASS_CROSSOVER:
        lRetval = aController.ZoneSetHighpassCrossover(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mFrequency);
        break;

    case OPT_SET_LOWPASS_CROSSOVER:
        lRetval = aController.ZoneSetLowpassCrossover(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mFrequency);
        break;

    case OPT_SET_MUTE:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupSetMute(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolume);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneSetMute(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mMute);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_SET_NAME:
        if (aClientArgument.mObjectOptionArgument.mOption == OPT_EQUALIZER_PRESET)
            lRetval = aController.EqualizerPresetSetName(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mName);
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
            lRetval = aController.GroupSetName(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mName);
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_SOURCE)
            lRetval = aController.SourceSetName(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mSource, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mName);
        else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
            lRetval = aController.ZoneSetName(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mName);
        else
            lRetval = -EINVAL;
        break;

    case OPT_SET_SOUND_MODE:
        lRetval = aController.ZoneSetSoundMode(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSoundMode);
        break;

    case OPT_SET_SOURCE:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupSetSource(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSource);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneSetSource(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mSource);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_SET_TREBLE:
        lRetval = aController.ZoneSetTreble(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mTreble);
        break;

    case OPT_SET_VOLUME:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupSetVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolume);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneSetVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolume);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_SET_VOLUME_LOCKED:
        lRetval = aController.ZoneSetVolumeLocked(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mVolumeLocked);
        break;

    case OPT_ADD_ZONE:
        lRetval = aController.GroupAddZone(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_REMOVE_ZONE:
        lRetval = aController.GroupRemoveZone(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup, aClientArgument.mOperationOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_DECREASE_EQUALIZER_BAND:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_EQUALIZER_PRESET)
                lRetval = aController.EqualizerPresetDecreaseBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneDecreaseEqualizerBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_DECREASE_BASS:
        lRetval = aController.ZoneDecreaseBass(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_DECREASE_TREBLE:
        lRetval = aController.ZoneDecreaseTreble(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_DECREASE_VOLUME:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupDecreaseVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneDecreaseVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_INCREASE_BALANCE_LEFT:
        lRetval = aController.ZoneIncreaseBalanceLeft(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_INCREASE_BALANCE_RIGHT:
        lRetval = aController.ZoneIncreaseBalanceRight(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_INCREASE_EQUALIZER_BAND:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_EQUALIZER_PRESET)
                lRetval = aController.EqualizerPresetIncreaseBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mEqualizerPreset, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneIncreaseEqualizerBand(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone, aClientArgument.mSubobjectOptionArgument.mArgument.mUnion.mEqualizerBand);
            else
                lRetval = -EINVAL;
        }
        break;

    case OPT_INCREASE_BASS:
        lRetval = aController.ZoneIncreaseBass(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_INCREASE_TREBLE:
        lRetval = aController.ZoneIncreaseTreble(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
        break;

    case OPT_INCREASE_VOLUME:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupIncreaseVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneIncreaseVolume(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
            else
                lRetval = -EINVAL;
            break;
        }

    case OPT_TOGGLE_MUTE:
        {
            if (aClientArgument.mObjectOptionArgument.mOption == OPT_GROUP)
                lRetval = aController.GroupToggleMute(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mGroup);
            else if (aClientArgument.mObjectOptionArgument.mOption == OPT_ZONE)
                lRetval = aController.ZoneToggleMute(aClientArgument.mObjectOptionArgument.mArgument.mUnion.mZone);
            else
                lRetval = -EINVAL;
        }
        break;

    default:
        Log::Error().Write("Unknown command '%d'!\n", aClientArgument.mOperationOptionArgument.mOption);
        lRetval = -EINVAL;
        break;
    }

    return (lRetval);
}

static Status DispatchCommand(Controller &aController, ClientArgument &aClientArgument, const uint32_t &aOptFlags, const Timeout &aTimeout)
{
    Status  lRetval = kStatus_Success;

#if DEBUG
    if ((aOptFlags & (kOptHasObjectArg | kOptHasSubobjectArg | kOptHasOperationArg)) == (kOptHasObjectArg | kOptHasSubobjectArg | kOptHasOperationArg))
    {
        Log::Debug().Write("Dispatching command with object %d, subobject %d, and operation %d...\n",
                           aClientArgument.mObjectOptionArgument.mOption,
                           aClientArgument.mSubobjectOptionArgument.mOption,
                           aClientArgument.mOperationOptionArgument.mOption);
    }
    else if ((aOptFlags & (kOptHasObjectArg | kOptHasOperationArg)) == (kOptHasObjectArg | kOptHasOperationArg))
    {
        Log::Debug().Write("Dispatching command with object %d and operation %d...\n",
                           aClientArgument.mObjectOptionArgument.mOption,
                           aClientArgument.mOperationOptionArgument.mOption);
    }
#endif // DEBUG

    if (aOptFlags & kOptHasObjectArg)
    {
        lRetval = ParseObjectOption(aController, aClientArgument);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (aOptFlags & kOptHasSubobjectArg)
    {
        lRetval = ParseSubobjectOption(aController, aClientArgument);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    if (aOptFlags & kOptHasOperationArg)
    {
        lRetval = ParseOperationOption(aController, aClientArgument);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    lRetval = DispatchCommand(aController, aClientArgument, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    aClientArgument.mDidDispatch = true;

 done:
    Log::Debug().Write("Command did%s dispatch with status %d, will%s wait for event %d.\n",
                       ((lRetval == kStatus_Success) ? "" : " not"),
                       lRetval,
                       ((lRetval == kStatus_Success) ? "" : " not"),
                       aClientArgument.mExpectedClientArgumentCompletionEvent);

    return (lRetval);
}

int main(int argc, char * const argv[])
{
    Client      lHLXClient;
    Status      lStatus;
    size_t      n = 0;
    const char *lMaybeURL = nullptr;

    // Cache the program invocation name for later use

    sProgram = argv[0];

    // Decode invocation parameters.

    DecodeOptions(sProgram, argc, argv, sOptions, n);

    lMaybeURL = argv[n];

    SetSignalHandler(SIGHUP,  OnSignal);
    SetSignalHandler(SIGINT,  OnSignal);
    SetSignalHandler(SIGQUIT, OnSignal);
    SetSignalHandler(SIGTERM, OnSignal);

    // Update logging streams, adjusting the filters and writers as
    // dictated by invocation options.

    Log::SetFilter(Log::Debug(), sDebug,   sOptFlags & kOptQuiet);
    Log::SetFilter(Log::Error(), sError,   false);
    Log::SetFilter(Log::Info(),  sVerbose, sOptFlags & kOptQuiet);

    if (sOptFlags & kOptSyslog) {
        FilterSyslog(Log::Debug());
        FilterSyslog(Log::Error());
        FilterSyslog(Log::Info());
    }

    {
        const bool lUseIPv4 = (((sOptFlags & kOptIPv6Only) == kOptIPv6Only) ? false : true);
        const bool lUseIPv6 = (((sOptFlags & kOptIPv4Only) == kOptIPv4Only) ? false : true);

        sHLXClient = &lHLXClient;

        lStatus = lHLXClient.Init();
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXClient.SetStatus(lStatus));

        lStatus = lHLXClient.Start(lMaybeURL, lUseIPv6, lUseIPv4, sTimeout);
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXClient.SetStatus(lStatus));
    }

    CFRunLoopRun();

 done:
    return((lHLXClient.GetStatus() == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
