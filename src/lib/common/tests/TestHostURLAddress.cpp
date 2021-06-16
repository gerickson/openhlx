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
 *      This file implements a unit test for HLX::Common::HostURLAddress.
 *
 */

#include <errno.h>
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Common/HostURLAddress.hpp>


using namespace HLX;
using namespace HLX::Common;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    return;
}

static void TestDefaultInitialization(nlTestSuite *inSuite __attribute__((unused)),
									  void *inContext __attribute__((unused)))
{
    return;
}

static void TestValueInitialization(nlTestSuite *inSuite __attribute__((unused)),
									void *inContext __attribute__((unused)))
{
    return;
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite __attribute__((unused)),
							void *inContext __attribute__((unused)))
{
    return;
}

static void TestMutation(nlTestSuite *inSuite __attribute__((unused)),
						 void *inContext __attribute__((unused)))
{
    return;
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Mutation",       TestMutation),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Host URL Address",
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
