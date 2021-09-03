/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file implements a unit test for
 *      HLX::Server::ConnectionSchemeIdentifierManager.
 *
 */

#include <utility>

#include <errno.h>
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Server/ConnectionSchemeIdentifierManager.hpp>


using namespace HLX;
using namespace HLX::Server;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    ConnectionSchemeIdentifierManager  lSchemeIdentifierManager;
}

static void TestObservation(nlTestSuite *inSuite, const char *inScheme)
{
    ConnectionSchemeIdentifierManager  lSchemeIdentifierManager;
    bool                               lClaimed;

    // 1: Test introspection / observation for unseen / unclaimed schemes

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, std::numeric_limits<ConnectionSchemeIdentifierManager::IdentifierType>::min());
    NL_TEST_ASSERT(inSuite, lClaimed == false);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, 1);
    NL_TEST_ASSERT(inSuite, lClaimed == false);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, 3);
    NL_TEST_ASSERT(inSuite, lClaimed == false);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, 7);
    NL_TEST_ASSERT(inSuite, lClaimed == false);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, std::numeric_limits<ConnectionSchemeIdentifierManager::IdentifierType>::max());
    NL_TEST_ASSERT(inSuite, lClaimed == false);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    // 1: Test introspection / observation for empty scheme

    TestObservation(inSuite, "");

    // 2: Test introspection / observation for unseen / unclaimed schemes

    // 2.1: Test unseen / unclaimed "telnet" scheme

    TestObservation(inSuite, "telnet");

    // 2.2: Test unseen / unclaimed "serial" scheme

    TestObservation(inSuite, "serial");
}

static void TestRelease(nlTestSuite *inSuite, const char *inScheme)
{
    ConnectionSchemeIdentifierManager  lSchemeIdentifierManager;
    bool                               lReleased;


    lReleased = lSchemeIdentifierManager.ReleaseSchemeIdentifier(inScheme, std::numeric_limits<ConnectionSchemeIdentifierManager::IdentifierType>::min());
    NL_TEST_ASSERT(inSuite, lReleased == false);

    lReleased = lSchemeIdentifierManager.ReleaseSchemeIdentifier(inScheme, 1);
    NL_TEST_ASSERT(inSuite, lReleased == false);

    lReleased = lSchemeIdentifierManager.ReleaseSchemeIdentifier(inScheme, 3);
    NL_TEST_ASSERT(inSuite, lReleased == false);

    lReleased = lSchemeIdentifierManager.ReleaseSchemeIdentifier(inScheme, 7);
    NL_TEST_ASSERT(inSuite, lReleased == false);

    lReleased = lSchemeIdentifierManager.ReleaseSchemeIdentifier(inScheme, std::numeric_limits<ConnectionSchemeIdentifierManager::IdentifierType>::max());
    NL_TEST_ASSERT(inSuite, lReleased == false);
}

static void TestClaim(nlTestSuite *inSuite, const char *inScheme)
{
    ConnectionSchemeIdentifierManager                  lSchemeIdentifierManager;
    ConnectionSchemeIdentifierManager::IdentifierType  lIdentifier;
    bool                                               lClaimed;


    lIdentifier = lSchemeIdentifierManager.ClaimSchemeIdentifier(inScheme);
    NL_TEST_ASSERT(inSuite, lIdentifier != ConnectionSchemeIdentifierManager::kInvalidIdentifier);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, lIdentifier);
    NL_TEST_ASSERT(inSuite, lClaimed == true);

    lIdentifier = lSchemeIdentifierManager.ClaimSchemeIdentifier(inScheme);
    NL_TEST_ASSERT(inSuite, lIdentifier != ConnectionSchemeIdentifierManager::kInvalidIdentifier);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, lIdentifier);
    NL_TEST_ASSERT(inSuite, lClaimed == true);

    lIdentifier = lSchemeIdentifierManager.ClaimSchemeIdentifier(inScheme);
    NL_TEST_ASSERT(inSuite, lIdentifier != ConnectionSchemeIdentifierManager::kInvalidIdentifier);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, lIdentifier);
    NL_TEST_ASSERT(inSuite, lClaimed == true);

    lIdentifier = lSchemeIdentifierManager.ClaimSchemeIdentifier(inScheme);
    NL_TEST_ASSERT(inSuite, lIdentifier != ConnectionSchemeIdentifierManager::kInvalidIdentifier);

    lClaimed = lSchemeIdentifierManager.IsSchemeIdentifierClaimed(inScheme, lIdentifier);
    NL_TEST_ASSERT(inSuite, lClaimed == true);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    // 1: Test release for empty scheme

    TestRelease(inSuite, "");

    // 2: Test release for unseen / unclaimed schemes

    // 2.1: Test release for unseen / unclaimed "telnet" scheme

    TestRelease(inSuite, "telnet");

    // 2.2: Test release for unseen / unclaimed "serial" scheme

    TestRelease(inSuite, "serial");

    // 3: Test claiming for an empty scheme

    {
        ConnectionSchemeIdentifierManager                  lSchemeIdentifierManager;
        ConnectionSchemeIdentifierManager::IdentifierType  lIdentifier;


        lIdentifier = lSchemeIdentifierManager.ClaimSchemeIdentifier("");
        NL_TEST_ASSERT(inSuite, lIdentifier == ConnectionSchemeIdentifierManager::kInvalidIdentifier);
    }

    // 4: Test claiming some identifiers across various schemes and
    //    then introspecting that they are claimed.

    // 4.1: Test claiming and confirming for "telnet" scheme

    TestClaim(inSuite, "telnet");

    // 4.1: Test claiming and confirming for "telnet" scheme

    TestClaim(inSuite, "serial");

    // 5: Test claiming some identifiers across various schemes,
    //    introspecting they are claimed, releasing one (1) of them,
    //    confirming it is no longer claimed, reclaiming it,
    //    confirming it is claimed, and then claiming and confirming
    //    another identifier.

    // 6: Test claiming some identifiers across various schemes,
    //    introspecting they are claimed, releasing two (2) of them,
    //    confirming they are no longer claimed, reclaiming them,
    //    confirming they are claimed, and then claiming and
    //    confirming another identifier.

    // 7: Test claiming some identifiers across various schemes,
    //    introspecting they are claimed, releasing two (2) of them in
    //    a different order, confirming they are no longer claimed,
    //    reclaiming them, confirming they are claimed, and then
    //    claiming and confirming another identifier.
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Mutation",       TestMutation),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Connection Scheme Identifier Manager",
        &sTests[0],
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        0,
        0,
        0,
        0
    };

    // Generate human-readable output.
    nlTestSetOutputStyle(OUTPUT_DEF);

    // Run test suit againt one context.
    nlTestRunner(&theSuite, nullptr);

    return nlTestRunnerStats(&theSuite);
}
