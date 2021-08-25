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
 *      This file implements a HLX control caching proxy daemon program
 *      executable.
 *
 */

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

#include <OpenHLX/Common/ConnectionManagerBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Version.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

#include "HLXProxyController.hpp"


using namespace HLX;
using namespace HLX::Client;
using namespace HLX::Common;
using namespace HLX::Proxy;
using namespace HLX::Proxy::Application;
using namespace HLX::Utilities;
using namespace Nuovations;
using namespace boost::filesystem;
using namespace std;


// Preprocessor Definitions

#define OPT_BASE                     0x00001000

#define OPT_CONNECT                  'c'
#define OPT_DEBUG                    'd'
#define OPT_HELP                     'h'
#define OPT_INITIAL_REFRESH          (OPT_BASE + 1)
#define OPT_IPV4_ONLY                '4'
#define OPT_IPV6_ONLY                '6'
#define OPT_LISTEN                   'l'
#define OPT_NO_INITIAL_REFRESH       (OPT_BASE + 2)
#define OPT_QUIET                    'q'
#define OPT_SYSLOG                   's'
#define OPT_TIMEOUT                  't'
#define OPT_VERBOSE                  'v'
#define OPT_VERSION                  'V'

// Type Declarations

enum OptFlags {
    kOptNone             = 0x00000000,
    kOptIPv4Only         = 0x00000001,
    kOptIPv6Only         = 0x00000002,
    kOptPriority         = 0x00000004,
    kOptQuiet            = 0x00000008,
    kOptSyslog           = 0x00000010,

    kOptTimeout          = 0x00000080,

    kOptNoInitialRefresh = 0x00000100
};

class HLXProxy;

// Function Prototypes

// Global Variables

static uint32_t             sOptFlags            = 0;
static Log::Level           sDebug               = 0;
static Log::Level           sError               = 0;
static Log::Level           sVerbose             = 0;

static const char *         sProgram             = nullptr;

static Timeout              sTimeout;

static const char *         sConnectMaybeURL     = nullptr;
static const char *         sListenMaybeURL      = nullptr;

static HLXProxy *           sHLXProxy            = nullptr;

static const struct option  sOptions[] = {
    { "connect",                 required_argument,  nullptr,   OPT_CONNECT                 },
    { "debug",                   optional_argument,  nullptr,   OPT_DEBUG                   },
    { "help",                    no_argument,        nullptr,   OPT_HELP                    },
    { "initial-refresh",         no_argument,        nullptr,   OPT_INITIAL_REFRESH         },
    { "ipv4-only",               no_argument,        nullptr,   OPT_IPV4_ONLY               },
    { "ipv6-only",               no_argument,        nullptr,   OPT_IPV6_ONLY               },
    { "listen",                  required_argument,  nullptr,   OPT_LISTEN                  },
    { "no-initial-refresh",      no_argument,        nullptr,   OPT_NO_INITIAL_REFRESH      },
    { "quiet",                   no_argument,        nullptr,   OPT_QUIET                   },
    { "timeout",                 required_argument,  nullptr,   OPT_TIMEOUT                 },
    { "verbose",                 optional_argument,  nullptr,   OPT_VERBOSE                 },
    { "version",                 no_argument,        nullptr,   OPT_VERSION                 },

    { nullptr,                   0,                  nullptr,   0                           }
};

static const char * const   sShortUsageString =
"Usage: %s [ options ] TBD\n";

static const char * const   sLongUsageString =
"\n"
" General options:\n"
"\n"
"  -d, --debug[=LEVEL]         Enable diagnostic output, optionally at level \n"
"                              LEVEL.\n"
"  -h, --help                  Print this help, then exit.\n"
"  -q, --quiet                 Run silently, suppressing all diagnostic and \n"
"                              informative output.\n"
"  -s, --syslog                Write all error, diagnostic and informative \n"
"                              output only to the system log, rather than to \n"
"                              both the system log as well as standard error \n"
"                              and standard output.\n"
"  -v, --verbose[=LEVEL]       Enable verbose output, optionally at level LEVEL.\n"
"  -V, --version               Print version and copyright information, then\n"
"                              exit.\n"
"\n"
" Proxy Options:\n"
"\n"
"  -4, --ipv4-only             Force hlxproxyd to use IPv4 addresses only.\n"
"  -6, --ipv6-only             Force hlxproxyd to use IPv6 addresses only.\n"
"  -c, --connect=HOST          TBD.\n"
"  -l, --listen=HOST           TBD.\n"
"  -t, --timeout=MILLISECONDS  Set a connection timeout of MILLISECONDS \n"
"                              milliseconds.\n"
"\n";

class HLXProxy :
    public Proxy::Application::ControllerDelegate
{
public:
    HLXProxy(void);
    ~HLXProxy(void);

    Status Init(const char *aConnectMaybeURL,
                const char *aListenMaybeURL,
                const bool &aUseIPv6,
                const bool &aUseIPv4);

    Status Start(void);
    Status Listen(void);
    Status Stop(void);
    Status Stop(const Status &aStatus);

    const Proxy::Application::Controller &GetController(void) const;
    Proxy::Application::Controller &GetController(void);

    Status GetStatus(void) const;
    void SetStatus(const Status &aStatus);

    void SetVersions(const bool &aUseIPv6,
                     const bool &aUseIPv4);
    const ConnectionManagerBasis::Versions &GetVersions(void) const;

    static bool IsRole(const Roles &aFirst, const Roles &aSecond);
    static bool IsClient(const Roles &aRole);
    static bool IsServer(const Roles &aRole);

private:
    // Resolve

    void ControllerWillResolve(Proxy::Application::Controller &aController, const char *aHost) final;
    void ControllerIsResolving(Proxy::Application::Controller &aController, const char *aHost) final;
    void ControllerDidResolve(Proxy::Application::Controller &aController, const char *aHost, const IPAddress &aIPAddress) final;
    void ControllerDidNotResolve(Proxy::Application::Controller &aController, const char *aHost, const Error &aError) final;

    // Client-facing Server Listen

    void ControllerWillListen(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerIsListening(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidListen(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotListen(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Common::Error &aError) final;

    // Client-facing Server Accept

    void ControllerWillAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerIsAccepting(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Server-facing Client Connect

    void ControllerWillConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout) final;
    void ControllerIsConnecting(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout) final;
    void ControllerDidConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Disconnect

    void ControllerWillDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef) final;
    void ControllerDidDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef, const Error &aError) final;
    void ControllerDidNotDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef, const Error &aError) final;

    // Server-facing Client Refresh / Reload

    void ControllerWillRefresh(Client::Application::ControllerBasis &aController) final;
    void ControllerIsRefreshing(Client::Application::ControllerBasis &aController, const uint8_t &aPercentComplete) final;
    void ControllerDidRefresh(Client::Application::ControllerBasis &aController) final;
    void ControllerDidNotRefresh(Client::Application::ControllerBasis &aController, const Error &aError) final;

    // Server-facing Client State Change

    void ControllerStateDidChange(Proxy::Application::Controller &aController, const StateChange::NotificationBasis &aStateChangeNotification) final;

    // Error

    void ControllerError(Proxy::Application::Controller &aController, const Roles &aRoles, const Error &aError) final;

    static void OnSignal(int aSignal);

private:
    RunLoopParameters                mRunLoopParameters;
    Proxy::Application::Controller   mHLXProxyController;
    Status                           mStatus;
    const char *                     mConnectMaybeURL;
    const char *                     mListenMaybeURL;
    ConnectionManagerBasis::Versions mVersions;
};

static const char *
GetString(const Proxy::Application::ControllerDelegate::Roles &aRoles, const bool &aTitleCase)
{
    constexpr auto kRoleClient = HLX::Common::ConnectionManagerBasis::kRoleClient;
    constexpr auto kRoleServer = HLX::Common::ConnectionManagerBasis::kRoleServer;
    const char *   lRetval     = nullptr;


    if ((aRoles & kRoleClient) == kRoleClient)
    {
        lRetval = ((aTitleCase) ? "Client" : "client");
    }
    else if ((aRoles & kRoleServer) == kRoleServer)
    {
        lRetval = ((aTitleCase) ? "Server" : "server");
    }

    return (lRetval);
}

static const char *
GetString(const Proxy::Application::ControllerDelegate::Roles &aRoles)
{
    static const bool kTitleCase = true;

    return (GetString(aRoles, !kTitleCase));
}

HLXProxy :: HLXProxy(void) :
    ControllerDelegate(),
    mRunLoopParameters(),
    mHLXProxyController(),
    mStatus(kStatus_Success),
    mConnectMaybeURL(nullptr),
    mListenMaybeURL(nullptr),
    mVersions(0)
{
    DeclareScopedFunctionTracer(lTracer);
}

HLXProxy :: ~HLXProxy(void)
{
    return;
}

Status HLXProxy :: Init(const char *aConnectMaybeURL,
                        const char *aListenMaybeURL,
                        const bool &aUseIPv6,
                        const bool &aUseIPv4)
{
    Status lRetval = kStatus_Success;

    lRetval = mRunLoopParameters.Init(CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXProxyController.Init(mRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXProxyController.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    mConnectMaybeURL = aConnectMaybeURL;
    mListenMaybeURL  = aListenMaybeURL;

    SetVersions(aUseIPv6, aUseIPv4);

 done:
    return (lRetval);
}

Status
HLXProxy :: Start(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mHLXProxyController.Connect(mConnectMaybeURL, GetVersions());
    nlREQUIRE_SUCCESS(lRetval, done);

    if ((sOptFlags & kOptNoInitialRefresh) == kOptNoInitialRefresh)
    {
        lRetval = Listen();
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

Status
HLXProxy :: Listen(void)
{
    Status lRetval = kStatus_Success;


    if (mListenMaybeURL == nullptr)
    {
        lRetval = mHLXProxyController.Listen(GetVersions());
        nlREQUIRE_SUCCESS(lRetval, done);
    }
    else
    {
        lRetval = mHLXProxyController.Listen(mListenMaybeURL, GetVersions());
        nlREQUIRE_SUCCESS(lRetval, done);
    }

done:
    return (lRetval);
}

Status HLXProxy :: Stop(void)
{
    return (Stop(kStatus_Success));
}

Status HLXProxy :: Stop(const Status &aStatus)
{
    Status lStatus = kStatus_Success;

    SetStatus(aStatus);

    CFRunLoopStop(mRunLoopParameters.GetRunLoop());

    return (lStatus);
}

const Proxy::Application::Controller &
HLXProxy :: GetController(void) const
{
    return (mHLXProxyController);
}

Proxy::Application::Controller &
HLXProxy :: GetController(void)
{
    return (mHLXProxyController);
}

Status HLXProxy :: GetStatus(void) const
{
    return (mStatus);
}

void HLXProxy :: SetStatus(const Status &aStatus)
{
    mStatus = aStatus;
}

void
HLXProxy :: SetVersions(const bool &aUseIPv6,
                        const bool &aUseIPv4)
{
    using Common::Utilities::GetVersions;

    mVersions = GetVersions(aUseIPv6, aUseIPv4);
}

const ConnectionManagerBasis::Versions &
HLXProxy :: GetVersions(void) const
{
    return (mVersions);
}

bool
HLXProxy :: IsRole(const Roles &aFirst, const Roles &aSecond)
{
    return ((aFirst & aSecond) == aSecond);
}

bool
HLXProxy :: IsClient(const Roles &aRole)
{
    constexpr auto kRoleClient = HLX::Common::ConnectionManagerBasis::kRoleClient;

    return (IsRole(aRole, kRoleClient));
}

bool
HLXProxy :: IsServer(const Roles &aRole)
{
    constexpr auto kRoleServer = HLX::Common::ConnectionManagerBasis::kRoleServer;

    return (IsRole(aRole, kRoleServer));
}

// Controller Delegate Methods

// Resolve

void HLXProxy :: ControllerWillResolve(Proxy::Application::Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Will resolve \"%s\".\n", aHost);
}

void HLXProxy :: ControllerIsResolving(Proxy::Application::Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Is resolving \"%s\".\n", aHost);
}

void HLXProxy :: ControllerDidResolve(Proxy::Application::Controller &aController, const char *aHost, const IPAddress &aIPAddress)
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

void HLXProxy :: ControllerDidNotResolve(Proxy::Application::Controller &aController, const char *aHost, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not resolve \"%s\": %d (%s).\n", aHost, aError, strerror(-aError));
}

// Client-facing Server Listen

void HLXProxy :: ControllerWillListen(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will listen at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerIsListening(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Listening at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerDidListen(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Listened at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerDidNotListen(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not listen at %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

// Client-facing Server Accept

void HLXProxy :: ControllerWillAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will accept from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerIsAccepting(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Accepting from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerDidAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Accepted from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerDidNotAccept(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not accept from %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

// Server-facing Client Connect

void HLXProxy :: ControllerWillConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aController;

    Log::Info().Write("Will connect to %s with %u ms timeout.\n", CFString(CFURLGetString(aURLRef)).GetCString(), aTimeout.GetMilliseconds());
}

void HLXProxy :: ControllerIsConnecting(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Timeout &aTimeout)
{
    (void)aController;

    Log::Info().Write("Connecting to %s with %u ms timeout.\n", CFString(CFURLGetString(aURLRef)).GetCString(), aTimeout.GetMilliseconds());
}

void HLXProxy :: ControllerDidConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef)
{
    Status lStatus;

    (void)aController;

    Log::Info().Write("Connected to %s.\n", CFString(CFURLGetString(aURLRef)).GetCString());

    if ((sOptFlags & kOptNoInitialRefresh) != kOptNoInitialRefresh)
    {
        lStatus = mHLXProxyController.Refresh();
        nlREQUIRE_SUCCESS(lStatus, done);
    }

 done:
    return;
}

void HLXProxy :: ControllerDidNotConnect(Proxy::Application::Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not connect to %s: %d (%s).\n", CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));

    Stop(aError);
}

// Disconnect

void HLXProxy :: ControllerWillDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will disconnect %s from %s.\n", GetString(aRoles), CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXProxy :: ControllerDidDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    if (aError >= kStatus_Success)
    {
        Log::Info().Write("Disconnected %s from %s.\n", GetString(aRoles), CFString(CFURLGetString(aURLRef)).GetCString());
    }
    else
    {
        Log::Info().Write("Disconnected %s from %s: %d (%s).\n", GetString(aRoles), CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
    }

    // Only call stop if we have non-error status; otherwise a
    // DidNot... or Error delegation already called it.

    // XXX - We may need to know whether this connection was on the
    // server-facing client side versus the client-facing server side
    // of the proxy since only the former should trigger a stop (if at
    // all rather than a retry) and not the latter.

    switch (aError)
    {

    case kStatus_Success:
        break;

    case -ECONNRESET:
        break;

    default:
        Stop(aError);
        break;

    }

    return;
}

void HLXProxy :: ControllerDidNotDisconnect(Proxy::Application::Controller &aController, const Roles &aRoles, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not disconnect %s from %s: %d.\n", GetString(aRoles), CFString(CFURLGetString(aURLRef)).GetCString(), aError);
}

// Server-facing Client Refresh / Reload

void HLXProxy :: ControllerWillRefresh(Client::Application::ControllerBasis &aController)
{
    (void)aController;

    Log::Info().Write("Waiting for client data...\n");

    return;
}

void HLXProxy :: ControllerIsRefreshing(Client::Application::ControllerBasis &aController, const uint8_t &aPercentComplete)
{
    (void)aController;

    Log::Info().Write("%u%% of client data received.\n", aPercentComplete);
}

void HLXProxy :: ControllerDidRefresh(Client::Application::ControllerBasis &aController)
{
    Status lStatus;


    (void)aController;

    Log::Info().Write("Client data received.\n");

    if ((sOptFlags & kOptNoInitialRefresh) != kOptNoInitialRefresh)
    {
        lStatus = Listen();
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, SetStatus(lStatus));
    }

 done:
    return;
}

void HLXProxy :: ControllerDidNotRefresh(Client::Application::ControllerBasis &aController, const Error &aError)
{
    (void)aController;

    Stop(aError);
}

// Server-facing Client State Change

void HLXProxy :: ControllerStateDidChange(Proxy::Application::Controller &aController, const Client::StateChange::NotificationBasis &aStateChangeNotification)
{
    const StateChange::Type lType = aStateChangeNotification.GetType();

    (void)aController;

    switch (lType)
    {
    default:
        Log::Error().Write("Unhandled state change notification type %d\n", lType);
        break;
    }

    return;
}

// Error

void HLXProxy :: ControllerError(Proxy::Application::Controller &aController, const Roles &aRoles, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Proxy %s error: %d (%s).\n",
                       GetString(aRoles),
                       aError,
                       strerror(-aError));

    switch (aError)
    {

    case -ECONNRESET:
        break;

    default:
        Stop(aError);
        break;

    }
}

void HLXProxy :: OnSignal(int aSignal)
{
    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);
}

static void OnSignal(int aSignal)
{
    DeclareScopedFunctionTracer(lTracer);

    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);

    if (sHLXProxy != nullptr)
    {
        sHLXProxy->Stop(-errno);
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

        case OPT_CONNECT:
            sConnectMaybeURL = optarg;
            break;

        case OPT_DEBUG:
            error += SetLevel(sDebug, optarg);
            break;

        case OPT_HELP:
            PrintUsage(inProgram, EXIT_SUCCESS);
            break;

        case OPT_INITIAL_REFRESH:
            if (sOptFlags & kOptNoInitialRefresh)
            {
                Log::Error().Write("The '--initial-refresh' and '--no-initial-refresh' options are mutually-exclusive. Please choose one or the other.\n");
                error++;
            }
            else
            {
                sOptFlags &= static_cast<uint32_t>(~kOptNoInitialRefresh);
            }
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

        case OPT_LISTEN:
            sListenMaybeURL = optarg;
            break;

        case OPT_NO_INITIAL_REFRESH:
            sOptFlags |= kOptNoInitialRefresh;
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

int main(int argc, char * const argv[])
{
    DeclareScopedFunctionTracer(lTracer);
    HLXProxy     lHLXProxy;
    Status       lStatus;
    size_t       n = 0;

    // Cache the program invocation name for later use

    sProgram = argv[0];

    // Decode invocation parameters.

    DecodeOptions(sProgram, argc, argv, sOptions, n);

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

        sHLXProxy = &lHLXProxy;

        lStatus = lHLXProxy.Init(sConnectMaybeURL,
                                 sListenMaybeURL,
                                 lUseIPv6,
                                 lUseIPv4);
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXProxy.SetStatus(lStatus));

        lStatus = lHLXProxy.Start();
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXProxy.SetStatus(lStatus));

        Log::Debug().Write("Proxy started with status %d\n", lStatus);
    }

    CFRunLoopRun();

 done:
    return((lHLXProxy.GetStatus() == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
