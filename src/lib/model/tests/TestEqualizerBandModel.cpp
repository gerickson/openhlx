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

#include <OpenHLX/Model/EqualizerBandModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    EqualizerBandModel lEqualizerBandModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandModel                 lEqualizerBandModel;
    Status                             lStatus;

    lStatus = lEqualizerBandModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType   lFrequencyConstant_1 = 42;
    const EqualizerBandModel::LevelType       lLevelConstant_1     = EqualizerBandModel::kLevelFlat;
    const EqualizerBandModel::FrequencyType   lFrequencyConstant_2 = 37;
    const EqualizerBandModel::LevelType       lLevelConstant_2     = 7;
    EqualizerBandModel                        lEqualizerBandModel_1;
    EqualizerBandModel                        lEqualizerBandModel_2;
    Status                                    lStatus;

    // Test 1: Test in range value frequency-only initialization.

    lStatus = lEqualizerBandModel_1.Init(lFrequencyConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1.1: Test it again to ensure it is still successful.

    lStatus = lEqualizerBandModel_1.Init(lFrequencyConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1.2: Test it with a different value and ensure it is still
    //           successful.

    lStatus = lEqualizerBandModel_1.Init(lFrequencyConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2: Test in range value frequency and level initialization.

    lStatus = lEqualizerBandModel_2.Init(lFrequencyConstant_1,
                                         lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Test it again to ensure it is still successful.

    lStatus = lEqualizerBandModel_2.Init(lFrequencyConstant_1,
                                         lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.2: Test it again with different values and ensure it is
    //           still successful.

    lStatus = lEqualizerBandModel_2.Init(lFrequencyConstant_2,
                                         lLevelConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandModel             lEqualizerBandModel_1;
    EqualizerBandModel             lEqualizerBandModel_2;
    Status                         lStatus;

    lStatus = lEqualizerBandModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_2.Init(lEqualizerBandModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType  lFrequencyConstant = 42;
    const EqualizerBandModel::LevelType      lLevelConstant = EqualizerBandModel::kLevelFlat;
    EqualizerBandModel                       lEqualizerBandModel_1;
    EqualizerBandModel                       lEqualizerBandModel_2;
    EqualizerBandModel::FrequencyType        lFrequencyValue;
    EqualizerBandModel::LevelType            lLevelValue;
    Status                                   lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lEqualizerBandModel_1.GetFrequency(lFrequencyValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lEqualizerBandModel_1.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lEqualizerBandModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_1.GetFrequency(lFrequencyValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lEqualizerBandModel_1.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lEqualizerBandModel_1.Init(lFrequencyConstant, lLevelConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_1.GetFrequency(lFrequencyValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyValue == lFrequencyConstant);

    lStatus = lEqualizerBandModel_1.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == lLevelConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lEqualizerBandModel_2.Init(lEqualizerBandModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_2.GetFrequency(lFrequencyValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyValue == lFrequencyConstant);

    lStatus = lEqualizerBandModel_2.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == lLevelConstant);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType  lFrequencyConstant = 42;
    const EqualizerBandModel::LevelType      lLevelConstant = EqualizerBandModel::kLevelMax - 1;
    EqualizerBandModel                       lEqualizerBandModel;
    EqualizerBandModel                       lEqualizerBandModel_2;
    EqualizerBandModel::LevelType            lLevelValue;
    Status                                   lStatus;

    // Initialize the model.

    lStatus = lEqualizerBandModel.Init(lFrequencyConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lEqualizerBandModel.SetLevel(lLevelConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == lLevelConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lEqualizerBandModel.SetLevel(lLevelConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel.IncreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelFlat + 1);

    lStatus = lEqualizerBandModel.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelFlat + 1);

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel.DecreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelFlat - 1);

    lStatus = lEqualizerBandModel.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelFlat - 1);

    // Test 5: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel.IncreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerBandModel.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelMax);

    lStatus = lEqualizerBandModel.SetLevel(EqualizerBandModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel.DecreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerBandModel.GetLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLevelValue == EqualizerBandModel::kLevelMin);

    // Test 6: Ensure that decrement and increment work correctly on
    //         an unintialized object.

    lStatus = lEqualizerBandModel_2.DecreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lEqualizerBandModel_2.DecreaseLevel(lLevelValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType  lFrequencyConstant_1 = 42;
    const EqualizerBandModel::FrequencyType  lFrequencyConstant_2 = 53;
    const EqualizerBandModel::LevelType      lLevelConstant_1 = (((EqualizerBandModel::kLevelMax - EqualizerBandModel::kLevelMin) / 2) + EqualizerBandModel::kLevelMin);
    const EqualizerBandModel::LevelType      lLevelConstant_2 = 5;
    EqualizerBandModel                       lEqualizerBandModel_1;
    EqualizerBandModel                       lEqualizerBandModel_2;
    EqualizerBandModel                       lEqualizerBandModel_3;
    EqualizerBandModel                       lEqualizerBandModel_4;
    EqualizerBandModel                       lEqualizerBandModel_5;
    EqualizerBandModel                       lEqualizerBandModel_6;
    EqualizerBandModel                       lEqualizerBandModel_7;
    EqualizerBandModel                       lEqualizerBandModel_8;
    Status                                   lStatus;
    bool                                     lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lEqualizerBandModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_1 == lEqualizerBandModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lEqualizerBandModel_3.Init(lFrequencyConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_4.Init(lFrequencyConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_3 == lEqualizerBandModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lStatus = lEqualizerBandModel_5.Init(lFrequencyConstant_1, lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandModel_6.Init(lFrequencyConstant_1, lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_5 == lEqualizerBandModel_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lEqualizerBandModel_7.Init(lEqualizerBandModel_6);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_7 == lEqualizerBandModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a fully value-initialized and an uninitialized object
    //         are not equal.

    lAreEqual = (lEqualizerBandModel_5 == lEqualizerBandModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that a fully value-initialized and a
    //         frequency-only initialized object are not equal.

    lAreEqual = (lEqualizerBandModel_5 == lEqualizerBandModel_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that fully value-initialized objects but with
    //         different frequencies are not equal.

    lStatus = lEqualizerBandModel_8.Init(lFrequencyConstant_2, lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_5 == lEqualizerBandModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 7: Test that fully value-initialized objects but with
    //         different levels are not equal.

    lStatus = lEqualizerBandModel_5.SetLevel(lLevelConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lEqualizerBandModel_6.SetLevel(lLevelConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_5 == lEqualizerBandModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType  lFrequencyConstant = 42;
    const EqualizerBandModel::LevelType      lLevelConstant = (((EqualizerBandModel::kLevelMax - EqualizerBandModel::kLevelMin) / 2) + EqualizerBandModel::kLevelMin);
    EqualizerBandModel                       lEqualizerBandModel_1;
    EqualizerBandModel                       lEqualizerBandModel_2;
    EqualizerBandModel                       lEqualizerBandModel_3;
    EqualizerBandModel                       lEqualizerBandModel_4;
    EqualizerBandModel                       lEqualizerBandModel_5;
    Status                                   lStatus;
    bool                                     lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lEqualizerBandModel_2 = lEqualizerBandModel_1;

    lAreEqual = (lEqualizerBandModel_1 == lEqualizerBandModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerBandModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerBandModel_2 = lEqualizerBandModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_1 == lEqualizerBandModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerBandModel_3.Init(lFrequencyConstant, lLevelConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerBandModel_4 = lEqualizerBandModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_3 == lEqualizerBandModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerBandModel_4.Init(lEqualizerBandModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerBandModel_5 = lEqualizerBandModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandModel_4 == lEqualizerBandModel_5);
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
        "Equalizer Band Model",
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
