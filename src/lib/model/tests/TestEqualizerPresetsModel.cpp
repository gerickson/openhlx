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

#include <OpenHLX/Model/EqualizerPresetsModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;

static constexpr size_t kEqualizerPresetsMax = 7;

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel lEqualizerPresetsModel_1;
    EqualizerPresetsModel lEqualizerPresetsModel_2(lEqualizerPresetsModel_1);
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel lEqualizerPresetsModel;
    Status                lStatus;

    lStatus = lEqualizerPresetsModel.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel lEqualizerPresetsModel_1;
    EqualizerPresetsModel lEqualizerPresetsModel_2;
    Status                lStatus;

    lStatus = lEqualizerPresetsModel_1.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerPresetsModel_2.Init(lEqualizerPresetsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel        lEqualizerPresetsModel;
    const char *                 lNameConstant_1 = nullptr;
    const char *                 lNameConstant_2 = "Test Name";
    const EqualizerPresetModel * lImmutableEqualizerPresetModel;
    EqualizerPresetModel *       lMutableEqualizerPresetModel;
    Status                       lStatus;


    lStatus = lEqualizerPresetsModel.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid or out-of-bounds observation conditions.

    // Test 1.1: Test invalid observation conditions.

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(IdentifierModel::kIdentifierInvalid, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(IdentifierModel::kIdentifierInvalid, lMutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(lNameConstant_1, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test out-of-bounds observation conditions.

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(kEqualizerPresetsMax + 1, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(kEqualizerPresetsMax + 1, lMutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(lNameConstant_2, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test inbounds observations conditions.

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(kEqualizerPresetsMax - 1, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(kEqualizerPresetsMax - 1, lMutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerPresetModel::IdentifierType lIdentifierConstant = 5;
    const char *                               lNameConstant_1 = "Test Name 1";
    const char *                               lNameConstant_2 = "Test Name 2";
    EqualizerPresetsModel                      lEqualizerPresetsModel;
    EqualizerPresetModel                       lEqualizerPresetModel_1;
    const EqualizerPresetModel *               lImmutableEqualizerPresetModel;
    Status                                     lStatus;

    // Initialize the source model as a test value.

    lStatus = lEqualizerPresetModel_1.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Initialize the sources model to test.

    lStatus = lEqualizerPresetsModel.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid mutations

    lStatus = lEqualizerPresetsModel.SetEqualizerPreset(IdentifierModel::kIdentifierInvalid, lEqualizerPresetModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerPresetsModel.SetEqualizerPreset(kEqualizerPresetsMax + 1, lEqualizerPresetModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Test valid mutation

    lStatus = lEqualizerPresetsModel.SetEqualizerPreset(lIdentifierConstant, lEqualizerPresetModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Ensure that setting a source twice results in
    //           non-error / non-success status.

    lStatus = lEqualizerPresetsModel.SetEqualizerPreset(lIdentifierConstant, lEqualizerPresetModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Test that we can successfully observe the set source.

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(lNameConstant_1, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: Test that a different name does not observe the set source.

    lStatus = lEqualizerPresetsModel.GetEqualizerPreset(lNameConstant_2, lImmutableEqualizerPresetModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel lEqualizerPresetsModel_1;
    EqualizerPresetsModel lEqualizerPresetsModel_2;
    EqualizerPresetsModel lEqualizerPresetsModel_3;
    EqualizerPresetsModel lEqualizerPresetsModel_4;
    Status                lStatus;
    bool                  lAreEqual;

    // Test 1: Test that two value initialized objects are
    //         equivalent.

    lStatus = lEqualizerPresetsModel_1.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerPresetsModel_2.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerPresetsModel_1 == lEqualizerPresetsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two object initialized objects are
    //         equivalent.

    lStatus = lEqualizerPresetsModel_3.Init(lEqualizerPresetsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerPresetsModel_3 == lEqualizerPresetsModel_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that a value initialized and an uninitialized
    //         object are not equal.

    lAreEqual = (lEqualizerPresetsModel_1 == lEqualizerPresetsModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerPresetsModel lEqualizerPresetsModel_1;
    EqualizerPresetsModel lEqualizerPresetsModel_2;
    EqualizerPresetsModel lEqualizerPresetsModel_3;
    EqualizerPresetsModel lEqualizerPresetsModel_4;
    EqualizerPresetsModel lEqualizerPresetsModel_5;
    Status                lStatus;
    bool                  lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lEqualizerPresetsModel_2 = lEqualizerPresetsModel_1;

    lAreEqual = (lEqualizerPresetsModel_1 == lEqualizerPresetsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerPresetsModel_1.Init(kEqualizerPresetsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerPresetsModel_2 = lEqualizerPresetsModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerPresetsModel_1 == lEqualizerPresetsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerPresetsModel_4.Init(lEqualizerPresetsModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerPresetsModel_5 = lEqualizerPresetsModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerPresetsModel_4 == lEqualizerPresetsModel_5);
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
        "Equalizer Presets Model",
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
