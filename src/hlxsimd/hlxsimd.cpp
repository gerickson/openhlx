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
 *      This file implements a HLX control server simulator daemon
 *      program executable.
 *
 */

#include <iterator>
#include <memory>
#include <set>

#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/socket.h>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <libtelnet.h>

#include <CoreFoundation/CFSocket.h>
#include <CoreFoundation/CFStream.h>
#include <CoreFoundation/CFURL.h>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>

#include <LogUtilities/LogUtilities.hpp>
#include <NuovationsUtilities/GenerateShortOptions.hpp>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>
#include <OpenHLX/Common/Version.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>
#include <OpenHLX/Utilities/Utilities.hpp>

#include <CommandManager.hpp>
#include <CommandRegularExpression.hpp>
#include <CommandRequestBasis.hpp>
#include <ConnectionBasisDelegate.hpp>
#include <ConnectionTelnet.hpp>
#include <HLXServerController.hpp>
#include <SourcesControllerCommands.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace HLX::Utilities;
using namespace Nuovations;
using namespace boost::filesystem;
using namespace std;

// Preprocessor Definitions

#define OPT_BASE                    0x00001000

#define OPT_DEBUG                   'd'
#define OPT_HELP                    'h'
#define OPT_IPV4_ONLY               '4'
#define OPT_IPV6_ONLY               '6'
#define OPT_QUIET                   'q'
#define OPT_SYSLOG                  's'
#define OPT_VERBOSE                 'v'
#define OPT_VERSION                 'V'

#define OPT_CONFIGURATION_FILE      (OPT_BASE +  1)

// Type Declarations

enum OptFlags {
    kOptNone          = 0x00000000,
    kOptIPv4Only      = 0x00000001,
    kOptIPv6Only      = 0x00000002,
    kOptPriority      = 0x00000004,
    kOptQuiet         = 0x00000008,
    kOptSyslog        = 0x00000010
};

class HLXServer;

// Function Prototypes

// Global Variables

static uint32_t             sOptFlags            = 0;
static Log::Level           sDebug               = 0;
static Log::Level           sError               = 0;
static Log::Level           sVerbose             = 0;

static const char *         sProgram             = nullptr;

static const char *         sConfigurationFile   = HLXSIMD_DEFAULT_CONFIG_PATH;

static HLXServer *          sHLXServer           = nullptr;

static const struct option  sOptions[] = {
    { "configuration-file",     required_argument,  nullptr,   OPT_CONFIGURATION_FILE     },
    { "debug",                  optional_argument,  nullptr,   OPT_DEBUG                  },
    { "help",                   no_argument,        nullptr,   OPT_HELP                   },
    { "ipv4-only",              no_argument,        nullptr,   OPT_IPV4_ONLY              },
    { "ipv6-only",              no_argument,        nullptr,   OPT_IPV6_ONLY              },
    { "quiet",                  no_argument,        nullptr,   OPT_QUIET                  },
    { "verbose",                optional_argument,  nullptr,   OPT_VERBOSE                },
    { "version",                no_argument,        nullptr,   OPT_VERSION                },

    { nullptr,                  0,                  nullptr,   0                          }
};

static const char * const   sShortUsageString =
"Usage: %s [ options ] [ { <URL> | <host[:port]> } ]\n";

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
" Server Options:\n"
"\n"
"  -4, --ipv4-only             Force hlxsimd to use IPv4 addresses only.\n"
"  -6, --ipv6-only             Force hlxsimd to use IPv6 addresses only.\n"
"  --configuration-file=FILE   Use file FILE as the configuration backing store\n"
"                              (default: " HLXSIMD_DEFAULT_CONFIG_PATH ").\n"
"\n";

/**
 *  @brief
 *    An object that effects the desired HLX control server simulator
 *    daemon.
 *
 *  This effects the core of the control server simulator by
 *  instantiating an instance of the HLX server controller and that
 *  implements the required server delegations for that controller.
 *
 */
class HLXServer :
    public ControllerDelegate
{
public:
    HLXServer(void);
    ~HLXServer(void);

    Status Init(void);

    Status Start(const bool &aUseIPv6, const bool &aUseIPv4);
    Status Start(const char *aMaybeURL, const bool &aUseIPv6, const bool &aUseIPv4);
    Status Stop(void);
    Status Stop(const Status &aStatus);

    const HLX::Server::Controller &GetController(void) const;
    HLX::Server::Controller &GetController(void);
    Status GetStatus(void) const;
    void SetStatus(const Status &aStatus);

private:
    // Resolve

    void ControllerWillResolve(Controller &aController, const char *aHost) final;
    void ControllerIsResolving(Controller &aController, const char *aHost) final;
    void ControllerDidResolve(Controller &aController, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ControllerDidNotResolve(Controller &aController, const char *aHost, const Common::Error &aError) final;

    // Listen

    void ControllerWillListen(Controller &aController, CFURLRef aURLRef) final;
    void ControllerIsListening(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidListen(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotListen(Controller &aController, CFURLRef aURLRef, const Common::Error &aError) final;

    // Accept

    void ControllerWillAccept(Controller &aController, CFURLRef aURLRef) final;
    void ControllerIsAccepting(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidAccept(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidNotAccept(Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Disconnect

    void ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef) final;
    void ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError) final;
    void ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError) final;

    // Error

    void ControllerError(Controller &aController, const Error &aError) final;

    static void OnSignal(int aSignal);

private:
    RunLoopParameters                mRunLoopParameters;
    HLX::Server::Controller          mHLXServerController;
    Status                           mStatus;
};

HLXServer :: HLXServer(void) :
    ControllerDelegate(),
    mRunLoopParameters(),
    mHLXServerController(),
    mStatus(kStatus_Success)
{
    return;
}

HLXServer :: ~HLXServer(void)
{
    return;
}

Status HLXServer :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mRunLoopParameters.Init(CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXServerController.Init(mRunLoopParameters, sConfigurationFile);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mHLXServerController.SetDelegate(this, nullptr);
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

Status HLXServer :: Start(const bool &aUseIPv6, const bool &aUseIPv4)
{
    Status lRetval = kStatus_Success;

    lRetval = mHLXServerController.Listen(GetVersions(aUseIPv6, aUseIPv4));

    return (lRetval);
}

Status HLXServer :: Start(const char *aMaybeURL, const bool &aUseIPv6, const bool &aUseIPv4)
{
    Status lRetval = kStatus_Success;

    lRetval = mHLXServerController.Listen(aMaybeURL, GetVersions(aUseIPv6, aUseIPv4));

    return (lRetval);
}

Status HLXServer :: Stop(void)
{
    return (Stop(kStatus_Success));
}

Status HLXServer :: Stop(const Status &aStatus)
{
    Status lStatus = kStatus_Success;

    SetStatus(aStatus);

    CFRunLoopStop(mRunLoopParameters.GetRunLoop());

    return (lStatus);
}

const HLX::Server::Controller &HLXServer :: GetController(void) const
{
    return (mHLXServerController);
}

HLX::Server::Controller &HLXServer :: GetController(void)
{
    return (mHLXServerController);
}

Status HLXServer :: GetStatus(void) const
{
    return (mStatus);
}

void HLXServer :: SetStatus(const Status &aStatus)
{
    mStatus = aStatus;
}

// Controller Delegate Methods

// Resolve

void HLXServer :: ControllerWillResolve(Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Will resolve \"%s\".\n", aHost);
}

void HLXServer :: ControllerIsResolving(Controller &aController, const char *aHost)
{
    (void)aController;

    Log::Info().Write("Is resolving \"%s\".\n", aHost);
}

void HLXServer :: ControllerDidResolve(Controller &aController, const char *aHost, const IPAddress &aIPAddress)
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

void HLXServer :: ControllerDidNotResolve(Controller &aController, const char *aHost, const Common::Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not resolve \"%s\": %d (%s).\n", aHost, aError, strerror(-aError));
}

// Listen

void HLXServer :: ControllerWillListen(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will listen at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerIsListening(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Listening at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerDidListen(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Listened at %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerDidNotListen(Controller &aController, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not listen at %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

// Accept

void HLXServer :: ControllerWillAccept(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will accept from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerIsAccepting(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Accepting from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerDidAccept(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Accepted from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerDidNotAccept(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not accept from %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

// Disconnect

void HLXServer :: ControllerWillDisconnect(Controller &aController, CFURLRef aURLRef)
{
    (void)aController;

    Log::Info().Write("Will disconnect from %s.\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString());
}

void HLXServer :: ControllerDidDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Info().Write("Disconnected from %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

void HLXServer :: ControllerDidNotDisconnect(Controller &aController, CFURLRef aURLRef, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Did not disconnect from %s: %d (%s).\n", (aURLRef == nullptr) ? "(null)" : CFString(CFURLGetString(aURLRef)).GetCString(), aError, strerror(-aError));
}

// Error

void HLXServer :: ControllerError(Controller &aController, const Error &aError)
{
    (void)aController;

    Log::Error().Write("Error: %d (%s).\n", aError, strerror(-aError));

    switch (aError)
    {

    case -ECONNRESET:
        break;

    default:
        Stop(aError);
        break;

    }
}

void HLXServer :: OnSignal(int aSignal)
{
    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);
}

static void OnSignal(int aSignal)
{
    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);

    if (sHLXServer != nullptr)
    {
        sHLXServer->Stop(-errno);
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

    // Generate a list of those single-character options available as
    // a subset of the long option list.

    Nuovations::Utilities::GenerateShortOptions(!posixly_correct, inOptions, shortOptions);

    p = shortOptions.c_str();

    // Start parsing invocation options

    while (!error && (c = getopt_long(argc, argv, p, inOptions, nullptr)) != -1) {

        switch (c) {

        case OPT_CONFIGURATION_FILE:
            sConfigurationFile = optarg;
            break;

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

    // At this point, we should have zero or one additional argument,
    // which is a URL, network host name (including optional port), or
    // network address which needs to be resolved, filtered against
    // other options, and then listened on.

    if (argc > 1) {
        error++;
        goto exit;
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

int main(int argc, char * const argv[])
{
    HLXServer    lHLXServer;
    Status       lStatus;
    size_t       n = 0;
    const char * lMaybeURL = nullptr;

    // Cache the program invocation name for later use

    sProgram = argv[0];

    // Decode invocation parameters.

    DecodeOptions(sProgram, argc, argv, sOptions, n);

    if ((static_cast<size_t>(argc) - n) > 0)
    {
        lMaybeURL = argv[n];
    }

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

        sHLXServer = &lHLXServer;

        lStatus = lHLXServer.Init();
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXServer.SetStatus(lStatus));

        if (lMaybeURL != nullptr)
        {
            lStatus = lHLXServer.Start(lMaybeURL, lUseIPv6, lUseIPv4);
        }
        else
        {
            lStatus = lHLXServer.Start(lUseIPv6, lUseIPv4);
        }
        nlREQUIRE_SUCCESS_ACTION(lStatus, done, lHLXServer.SetStatus(lStatus));

        Log::Debug().Write("Server started with status %d\n", lStatus);
    }

    CFRunLoopRun();

 done:
    return((lHLXServer.GetStatus() == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
