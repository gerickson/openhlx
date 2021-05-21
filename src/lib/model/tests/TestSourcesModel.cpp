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

#include <OpenHLX/Model/SourcesModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;

static constexpr size_t kSourcesMax = 7;

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    SourcesModel lSourcesModel_1;
    SourcesModel lSourcesModel_2(lSourcesModel_1);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourcesModel lSourcesModel;
    Status       lStatus;

    lStatus = lSourcesModel.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourcesModel lSourcesModel_1;
    SourcesModel lSourcesModel_2;
    Status      lStatus;

    lStatus = lSourcesModel_1.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesModel_2.Init(lSourcesModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourcesModel                        lSourcesModel;
    const char *                        lNameConstant_1 = nullptr;
    const char *                        lNameConstant_2 = "Test Name";
    const SourceModel *                 lImmutableSourceModel;
    SourceModel *                       lMutableSourceModel;
    Status                              lStatus;


    lStatus = lSourcesModel.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid or out-of-bounds observation conditions.

    // Test 1.1: Test invalid observation conditions.

    lStatus = lSourcesModel.GetSource(IdentifierModel::kIdentifierInvalid, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSourcesModel.GetSource(IdentifierModel::kIdentifierInvalid, lMutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSourcesModel.GetSource(lNameConstant_1, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test out-of-bounds observation conditions.

    lStatus = lSourcesModel.GetSource(kSourcesMax + 1, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSourcesModel.GetSource(kSourcesMax + 1, lMutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSourcesModel.GetSource(lNameConstant_2, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test inbounds observations conditions.

    lStatus = lSourcesModel.GetSource(kSourcesMax - 1, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesModel.GetSource(kSourcesMax - 1, lMutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SourceModel::IdentifierType   lIdentifierConstant = 7;
    const char *                        lNameConstant_1 = "Test Name";
    const char *                        lNameConstant_2 = "Test Name 2";
    SourcesModel                        lSourcesModel;
    SourceModel                         lSourceModel_1;
    const SourceModel *                 lImmutableSourceModel;
    Status                              lStatus;

    // Initialize the source model as a test value.

    lStatus = lSourceModel_1.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Initialize the sources model to test.

    lStatus = lSourcesModel.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid mutations

    lStatus = lSourcesModel.SetSource(IdentifierModel::kIdentifierInvalid, lSourceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSourcesModel.SetSource(kSourcesMax + 1, lSourceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Test valid mutation

    lStatus = lSourcesModel.SetSource(lIdentifierConstant, lSourceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Ensure that setting a source twice results in
    //           non-error / non-success status.

    lStatus = lSourcesModel.SetSource(lIdentifierConstant, lSourceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Test that we can successfully observe the set source.

    lStatus = lSourcesModel.GetSource(lNameConstant_1, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: Test that a different name does not observe the set source.

    lStatus = lSourcesModel.GetSource(lNameConstant_2, lImmutableSourceModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourcesModel   lSourcesModel_1;
    SourcesModel   lSourcesModel_2;
    SourcesModel   lSourcesModel_3;
    SourcesModel   lSourcesModel_4;
    Status         lStatus;
    bool           lAreEqual;

    // Test 1: Test that two value initialized objects are
    //         equivalent.

    lStatus = lSourcesModel_1.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesModel_2.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourcesModel_1 == lSourcesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two object initialized objects are
    //         equivalent.

    lStatus = lSourcesModel_3.Init(lSourcesModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourcesModel_3 == lSourcesModel_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that a value initialized and an uninitialized
    //         object are not equal.

    lAreEqual = (lSourcesModel_1 == lSourcesModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourcesModel   lSourcesModel_1;
    SourcesModel   lSourcesModel_2;
    SourcesModel   lSourcesModel_3;
    SourcesModel   lSourcesModel_4;
    SourcesModel   lSourcesModel_5;
    Status         lStatus;
    bool           lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lSourcesModel_2 = lSourcesModel_1;

    lAreEqual = (lSourcesModel_1 == lSourcesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSourcesModel_1.Init(kSourcesMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSourcesModel_2 = lSourcesModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourcesModel_1 == lSourcesModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSourcesModel_4.Init(lSourcesModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSourcesModel_5 = lSourcesModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourcesModel_4 == lSourcesModel_5);
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
        "Sources Model",
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
