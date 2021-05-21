/*
 *    Copyright (c) 2020-2021 Grant Erickson
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

#include <OpenHLX/Model/CrossoverModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;

// Global Variables

static constexpr CrossoverModel::FrequencyType  kFrequencyMin_1 = 1;
static constexpr CrossoverModel::FrequencyType  kFrequencyMax_1 = 19997;
static constexpr CrossoverModel::FrequencyType  kFrequency_1    = 9973;

static constexpr CrossoverModel::FrequencyLimits kFrequencyLimits_1 =
{
    kFrequencyMin_1,
    kFrequencyMax_1
};

static constexpr CrossoverModel::FrequencyType  kFrequencyMin_2 = 3;
static constexpr CrossoverModel::FrequencyType  kFrequencyMax_2 = 19993;
static constexpr CrossoverModel::FrequencyType  kFrequency_2    = 20011;

static constexpr CrossoverModel::FrequencyLimits kFrequencyLimits_2 =
{
    kFrequencyMin_2,
    kFrequencyMax_2
};

static constexpr CrossoverModel::FrequencyLimits kFrequencyLimits_3 =
{
    kFrequencyMin_1,
    kFrequencyMax_2
};

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel(kFrequencyLimits_1);
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel(kFrequencyLimits_1);
    Status                                          lStatus;

    lStatus = lCrossoverModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel(kFrequencyLimits_1);
    Status                                          lStatus;

    // Test 1: Test in range value initialization.

    lStatus = lCrossoverModel.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test out of range value initialization.

    lStatus = lCrossoverModel.Init(kFrequency_2);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel_1(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_2(kFrequencyLimits_1);
    Status                                          lStatus;

    lStatus = lCrossoverModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_2.Init(lCrossoverModel_1);
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
    CrossoverModel                                  lCrossoverModel_1(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_2(kFrequencyLimits_1);
    CrossoverModel::FrequencyType                   lFrequency;
    CrossoverModel::FrequencyLimits                 lFrequencyLimits;
    Status                                          lStatus;

    // Test 1.1: Ensure that an uninitialized model returns an error on
    //           frequency observation.

    lStatus = lCrossoverModel_1.GetFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Ensure that an uninitialized model returns success
    //           frequency limits observation.

    lStatus = lCrossoverModel_1.GetFrequencyLimits(lFrequencyLimits);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMin == kFrequencyLimits_1.mMin);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMax == kFrequencyLimits_1.mMax);

    // Test 2.1: Ensure that a default initialized model returns an
    //           error on frequency observation.

    lStatus = lCrossoverModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_1.GetFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Ensure that a default initialized model returns
    //           success on frequency limit observation.

    lStatus = lCrossoverModel_1.GetFrequencyLimits(lFrequencyLimits);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMin == kFrequencyLimits_1.mMin);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMax == kFrequencyLimits_1.mMax);

    // Test 3.1: Ensure that a value initialized model returns success
    //           and the initialized value on frequency observation.

    lStatus = lCrossoverModel_1.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_1.GetFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kFrequency_1);

    // Test 3.2: Ensure that a value initialized model returns success
    //           on frequency limit observation.

    lStatus = lCrossoverModel_1.GetFrequencyLimits(lFrequencyLimits);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMin == kFrequencyLimits_1.mMin);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMax == kFrequencyLimits_1.mMax);

    // Test 4.1: Ensure that an object initialized model returns
    //           success and the original initializing object value on
    //           frequency observation.

    lStatus = lCrossoverModel_2.Init(lCrossoverModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_2.GetFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kFrequency_1);

    // Test 4.2: Ensure that an object initialized model returns
    //           success on frequency limit observation.

    lStatus = lCrossoverModel_2.GetFrequencyLimits(lFrequencyLimits);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMin == kFrequencyLimits_1.mMin);
    NL_TEST_ASSERT(inSuite, lFrequencyLimits.mMax == kFrequencyLimits_1.mMax);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel(kFrequencyLimits_1);
    CrossoverModel::FrequencyType                   lFrequency;
    Status                                          lStatus;

    // Initialize the model.

    lStatus = lCrossoverModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lCrossoverModel.SetFrequency(kFrequencyMax_1 + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lCrossoverModel.SetFrequency(kFrequencyMin_1 - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lCrossoverModel.SetFrequency(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel.GetFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kFrequency_1);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lCrossoverModel.SetFrequency(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel_1(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_2(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_3(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_4(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_5(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_6(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_7(kFrequencyLimits_2);
    CrossoverModel                                  lCrossoverModel_8(kFrequencyLimits_3);
    Status                                          lStatus;
    bool                                            lAreEqual;


    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lCrossoverModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_1 == lCrossoverModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lCrossoverModel_3.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lCrossoverModel_4.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_3 == lCrossoverModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lCrossoverModel_5.Init(lCrossoverModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_5 == lCrossoverModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a value initialized and an uninitialized object
    //         are not equal.

    lAreEqual = (lCrossoverModel_3 == lCrossoverModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that initialized objects with different limits are
    //         not equal.

    lStatus = lCrossoverModel_7.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_3 == lCrossoverModel_7);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that initialized objects with the same minimium
    //         but different maximum limit are not equal.

    lStatus = lCrossoverModel_8.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_3 == lCrossoverModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    CrossoverModel                                  lCrossoverModel_1(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_2(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_3(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_4(kFrequencyLimits_1);
    CrossoverModel                                  lCrossoverModel_5(kFrequencyLimits_1);
    Status                                          lStatus;
    bool                                            lAreEqual;


    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lCrossoverModel_2 = lCrossoverModel_1;

    lAreEqual = (lCrossoverModel_1 == lCrossoverModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lCrossoverModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCrossoverModel_2 = lCrossoverModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_1 == lCrossoverModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lCrossoverModel_3.Init(kFrequency_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCrossoverModel_4 = lCrossoverModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_3 == lCrossoverModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lCrossoverModel_4.Init(lCrossoverModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCrossoverModel_5 = lCrossoverModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lCrossoverModel_4 == lCrossoverModel_5);
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
        "Crossover Model",
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
