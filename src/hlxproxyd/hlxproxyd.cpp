/*
 *    Copyright (c) 2018-2019 Grant Erickson
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
 *    Description:
 *      This file...
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

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;
using namespace boost::filesystem;
using namespace std;

// Preprocessor Definitions

#define OPT_BASE                    0x00001000

#define OPT_DEBUG                   'd'
#define OPT_HELP                    'h'
#define OPT_QUIET                   'q'
#define OPT_SYSLOG                  's'
#define OPT_VERBOSE                 'v'

// Type Declarations

enum OptFlags {
    kOptNone          = 0x00000000,
    kOptPriority      = 0x00000001,
    kOptQuiet         = 0x00000002,
    kOptSyslog        = 0x00000004
};

// Global Variables

static uint32_t             sOptFlags            = 0;
static Log::Level           sDebug               = 0;
static Log::Level           sError               = 0;
static Log::Level           sVerbose             = 0;

static const char *         sProgram             = nullptr;

static const struct option  sOptions[] = {
    { "debug",                  optional_argument,  nullptr,   OPT_DEBUG                  },
    { "help",                   no_argument,        nullptr,   OPT_HELP                   },
    { "quiet",                  no_argument,        nullptr,   OPT_QUIET                  },
    { "verbose",                optional_argument,  nullptr,   OPT_VERBOSE                },

    { nullptr,                  0,                  nullptr,   0                          }
};

static const char * const   sShortUsageString =
"Usage: %s [ options ] { <URL> | <host[:port]> | <file> }\n";

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
"\n"
" Client Options:\n"
"\n"
"  -t, --timeout=MILLISECONDS  Set a connection timeout of MILLISECONDS \n"
"                              milliseconds.\n"
"\n";

static void OnSignal(int aSignal)
{
    DeclareScopedFunctionTracer(lTracer);

    Log::Debug().Write("%s: caught signal %d\n", __func__, aSignal);
}

static void SetSignalHandler(int aSignal, void (*aHandler)(int aSignal))
{
    struct sigaction sa;
    int signals[] = { aSignal };

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

        case OPT_DEBUG:
            error += SetLevel(sDebug, optarg);
            break;

        case OPT_HELP:
            PrintUsage(inProgram, EXIT_SUCCESS);
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
    // URL, path, or host and optional port to connect to.

    if (argc != 1) {
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
    DeclareScopedFunctionTracer(lTracer);
    size_t n = 0;

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

    return((true) ? EXIT_SUCCESS : EXIT_FAILURE);
}
