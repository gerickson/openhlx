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
 *    Description:
 *      This file...
 */

#include <errno.h>

#include <nlunit-test.h>

#include <OpenHLX/Model/ZonesModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;

static constexpr size_t kZonesMax = 7;

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    ZonesModel lZonesModel_1;
    ZonesModel lZonesModel_2(lZonesModel_1);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZonesModel lZonesModel;
    Status     lStatus;

    lStatus = lZonesModel.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZonesModel lZonesModel_1;
    ZonesModel lZonesModel_2;
    Status     lStatus;

    lStatus = lZonesModel_1.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZonesModel_2.Init(lZonesModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZonesModel                         lZonesModel;
    const char *                       lNameConstant_1 = nullptr;
    const char *                       lNameConstant_2 = "Test Name";
    const ZoneModel *                  lImmutableZoneModel;
    ZoneModel *                        lMutableZoneModel;
    Status                             lStatus;


    lStatus = lZonesModel.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid or out-of-bounds observation conditions.

    // Test 1.1: Test invalid observation conditions.

    lStatus = lZonesModel.GetZone(IdentifierModel::kIdentifierInvalid, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZonesModel.GetZone(IdentifierModel::kIdentifierInvalid, lMutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZonesModel.GetZone(lNameConstant_1, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test out-of-bounds observation conditions.

    lStatus = lZonesModel.GetZone(kZonesMax + 1, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZonesModel.GetZone(kZonesMax + 1, lMutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZonesModel.GetZone(lNameConstant_2, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test inbounds observations conditions.

    lStatus = lZonesModel.GetZone(kZonesMax - 1, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZonesModel.GetZone(kZonesMax - 1, lMutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ZoneModel::IdentifierType    lIdentifierConstant = 7;
    const char *                       lNameConstant_1 = "Test Name 1";
    const char *                       lNameConstant_2 = "Test Name 2";
    ZonesModel                         lZonesModel;
    ZoneModel                          lZoneModel_1;
    const ZoneModel *                  lImmutableZoneModel;
    Status                              lStatus;

    // Initialize the source model as a test value.

    lStatus = lZoneModel_1.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Initialize the sources model to test.

    lStatus = lZonesModel.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid mutations

    lStatus = lZonesModel.SetZone(IdentifierModel::kIdentifierInvalid, lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZonesModel.SetZone(kZonesMax + 1, lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Test valid mutation

    lStatus = lZonesModel.SetZone(lIdentifierConstant, lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Ensure that setting a source twice results in
    //           non-error / non-success status.

    lStatus = lZonesModel.SetZone(lIdentifierConstant, lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Test that we can successfully observe the set source.

    lStatus = lZonesModel.GetZone(lNameConstant_1, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: Test that a different name does not observe the set source.

    lStatus = lZonesModel.GetZone(lNameConstant_2, lImmutableZoneModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZonesModel    lZonesModel_1;
    ZonesModel    lZonesModel_2;
    ZonesModel    lZonesModel_3;
    ZonesModel    lZonesModel_4;
    Status        lStatus;
    bool          lAreEqual;

    // Test 1: Test that two value initialized objects are
    //         equivalent.

    lStatus = lZonesModel_1.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZonesModel_2.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZonesModel_1 == lZonesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two object initialized objects are
    //         equivalent.

    lStatus = lZonesModel_3.Init(lZonesModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZonesModel_3 == lZonesModel_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that a value initialized and an uninitialized
    //         object are not equal.

    lAreEqual = (lZonesModel_1 == lZonesModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZonesModel lZonesModel_1;
    ZonesModel lZonesModel_2;
    ZonesModel lZonesModel_3;
    ZonesModel lZonesModel_4;
    ZonesModel lZonesModel_5;
    Status     lStatus;
    bool       lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lZonesModel_2 = lZonesModel_1;

    lAreEqual = (lZonesModel_1 == lZonesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lZonesModel_1.Init(kZonesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lZonesModel_2 = lZonesModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZonesModel_1 == lZonesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lZonesModel_4.Init(lZonesModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lZonesModel_5 = lZonesModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZonesModel_4 == lZonesModel_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Mutation",       TestMutation),
    NL_TEST_DEF("Equality",       TestEquality),
    NL_TEST_DEF("Assignment",     TestAssignment),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Zones Model",
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
