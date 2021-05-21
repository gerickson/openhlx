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
 *      This file implements a unit test for HLX::Common::HostURL.
 *
 */

#include <utility>

#include <errno.h>
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Common/HostURL.hpp>


using namespace HLX;
using namespace HLX::Common;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    HostURL lHostURL;
}

static void TestInvalidValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char * lCStringValue_1  = nullptr;
    CFStringRef  lCFStringValue_1 = nullptr;
    CFURLRef     lCFURLValue      = nullptr;
    const char * lCStringValue_2  = "";
    CFStringRef  lCFStringValue_2 = CFSTR("");
    HostURL      lHostURL;
    Status       lStatus;

    lStatus = lHostURL.Init(lCStringValue_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lHostURL.Init(lCFStringValue_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lHostURL.Init(lCFURLValue);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lHostURL.Init(lCStringValue_2);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lHostURL.Init(lCFStringValue_2);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);
}

static void TestValidValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char * lCStringValue  = "http://127.0.0.1/";
    CFStringRef  lCFStringValue = CFSTR("http://127.0.0.1/");
    CFURLRef     lCFURLValue    = nullptr;
    HostURL      lHostURL_1;
    HostURL      lHostURL_2;
    HostURL      lHostURL_3;
    Status       lStatus;

    lStatus = lHostURL_1.Init(lCStringValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lHostURL_2.Init(lCFStringValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCFURLValue = CFURLCreateWithString(kCFAllocatorDefault,
                                        lCFStringValue,
                                        nullptr);
    NL_TEST_ASSERT(inSuite, lCFURLValue != nullptr);

    lStatus = lHostURL_3.Init(lCFURLValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    TestInvalidValueInitialization(inSuite, inContext);
    TestValidValueInitialization(inSuite, inContext);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestValueInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    HostURL  lHostURL_1;
    HostURL  lHostURL_2;
    CFURLRef lCFURL = nullptr;
    Status   lStatus;


    // 1: Test uninitialized host URL.

    lCFURL = lHostURL_1.GetURL();
    NL_TEST_ASSERT(inSuite, lCFURL == nullptr);

    // 2: Test initialized host URL.

    lStatus = lHostURL_2.Init("http://127.0.0.1/");
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCFURL = lHostURL_2.GetURL();
    NL_TEST_ASSERT(inSuite, lCFURL != nullptr);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    HostURL  lHostURL_1;
    HostURL  lHostURL_2;
    HostURL  lHostURL_3;
    HostURL  lHostURL_4;
    HostURL  lHostURL_5;
    Status   lStatus;
    bool     lAreEqual;

    lStatus = lHostURL_3.Init("http://127.0.0.1/");
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lHostURL_4.Init("http://[::1]/");
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lHostURL_5.Init("http://127.0.0.1/");
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1: Test that any arbitrary host URL is equal to itself,
    //    initialized or not.

    lAreEqual = (lHostURL_1 == lHostURL_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lAreEqual = (lHostURL_3 == lHostURL_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // 2: Test that two uninitialized host URLs are equal.

    lAreEqual = (lHostURL_1 == lHostURL_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // 3: Test that an initialized and an uninitialized host URL are NOT equal.

    lAreEqual = (lHostURL_2 == lHostURL_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lHostURL_4 == lHostURL_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // 4: Test that two initialized host URLs with the different URLs
    //    are NOT equal.

    lAreEqual = (lHostURL_4 == lHostURL_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // 5: Test that two initialized host URLs with the same URL are equal.

    lAreEqual = (lHostURL_3 == lHostURL_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *     kURLConstant = "http://127.0.0.1/";
    HostURL          lHostURL_1;
    HostURL          lHostURL_2;
    HostURL          lHostURL_3;
    HostURL          lHostURL_4;
    HostURL *        lHostURL;
    Status           lStatus;
    bool             lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lHostURL_2 = lHostURL_1;

    lAreEqual = (lHostURL_1 == lHostURL_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lHostURL_3.Init(kURLConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHostURL_4 = lHostURL_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lHostURL_3 == lHostURL_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that self-assignment is a no-op.

    lHostURL = &lHostURL_1;

    lHostURL_1 = *lHostURL;

    lAreEqual = (lHostURL_1 == lHostURL_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestMovement(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *     kURLConstant = "http://127.0.0.1/";
    HostURL          lHostURL_1;
    HostURL          lHostURL_2;
    HostURL          lHostURL_3;
    HostURL          lHostURL_4;
    HostURL *        lHostURL;
    Status           lStatus;
    bool             lAreEqual;

    // Test 1: Test one uninitialized object and that object
    //         moved to another object are equivalent.

    lHostURL_2 = std::move(lHostURL_1);

    lAreEqual = (lHostURL_1 == lHostURL_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that object
    // assigned to another uninitialized object are not equivalent.

    lStatus = lHostURL_3.Init(kURLConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHostURL_4 = std::move(lHostURL_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lHostURL_3 == lHostURL_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 3: Test that self-movement is a no-op.

    lHostURL = &lHostURL_1;

    lHostURL_1 = std::move(*lHostURL);

    lAreEqual = (lHostURL_1 == lHostURL_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Equality",       TestEquality),
    NL_TEST_DEF("Assignment",     TestAssignment),
    NL_TEST_DEF("Movement",       TestMovement),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Host URL",
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
