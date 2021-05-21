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

#include <OpenHLX/Model/BalanceModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)), void *inContext __attribute__((unused)))
{
    BalanceModel  lBalanceModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    BalanceModel                       lBalanceModel;
    Status                             lStatus;

    lStatus = lBalanceModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const BalanceModel::BalanceType  lBalanceConstant = (((BalanceModel::kBalanceMax - BalanceModel::kBalanceMin) / 2) + BalanceModel::kBalanceMin);
    BalanceModel                     lBalanceModel;
    Status                           lStatus;

    // Test 1: Test in range value initialization.

    lStatus = lBalanceModel.Init(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2: Test out of range value initialization

    lStatus = lBalanceModel.Init(BalanceModel::kBalanceMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    BalanceModel                     lBalanceModel_1;
    BalanceModel                     lBalanceModel_2;
    Status                           lStatus;

    lStatus = lBalanceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_2.Init(lBalanceModel_1);
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
    const BalanceModel::BalanceType  lBalanceConstant = BalanceModel::kBalanceMin;
    BalanceModel                     lBalanceModel_1;
    BalanceModel                     lBalanceModel_2;
    BalanceModel::BalanceType        lBalanceValue;
    Status                           lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lBalanceModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lBalanceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lBalanceModel_1.Init(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == lBalanceConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lBalanceModel_2.Init(lBalanceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_2.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == lBalanceConstant);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const BalanceModel::BalanceType  lBalanceConstant = BalanceModel::kBalanceMax - 1;
    BalanceModel                     lBalanceModel;
    BalanceModel                     lBalanceModel_2;
    BalanceModel::BalanceType        lBalanceValue;
    Status                           lStatus;

    // Initialize the model.

    lStatus = lBalanceModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lBalanceModel.SetBalance(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == lBalanceConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lBalanceModel.SetBalance(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that increase balance left/right work

    // Test 4.1: Ensure that increase balance right works

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceCenter);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter);

    lStatus = lBalanceModel.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 1);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 1);

    lStatus = lBalanceModel.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 2);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 2);

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMax);

    lStatus = lBalanceModel.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMax);

    lStatus = lBalanceModel_2.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that increase balance left works

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceCenter);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter);

    lStatus = lBalanceModel.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 1);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 1);

    lStatus = lBalanceModel.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 2);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 2);

    lStatus = lBalanceModel.SetBalance(BalanceModel::kBalanceMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMin);

    lStatus = lBalanceModel.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lBalanceModel.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMin);

    lStatus = lBalanceModel_2.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const BalanceModel::BalanceType  lBalanceConstant = (((BalanceModel::kBalanceMax - BalanceModel::kBalanceMin) / 2) + BalanceModel::kBalanceMin);
    BalanceModel                     lBalanceModel_1;
    BalanceModel                     lBalanceModel_2;
    BalanceModel                     lBalanceModel_3;
    BalanceModel                     lBalanceModel_4;
    BalanceModel                     lBalanceModel_5;
    BalanceModel                     lBalanceModel_6;
    Status                           lStatus;
    bool                             lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lBalanceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_1 == lBalanceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lBalanceModel_3.Init(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lBalanceModel_4.Init(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_3 == lBalanceModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lBalanceModel_5.Init(lBalanceModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_5 == lBalanceModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a value initialized and an uninitialized object
    //         are not equal.

    lAreEqual = (lBalanceModel_3 == lBalanceModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const BalanceModel::BalanceType  lBalanceConstant = (((BalanceModel::kBalanceMax - BalanceModel::kBalanceMin) / 2) + BalanceModel::kBalanceMin);
    BalanceModel                     lBalanceModel_1;
    BalanceModel                     lBalanceModel_2;
    BalanceModel                     lBalanceModel_3;
    BalanceModel                     lBalanceModel_4;
    BalanceModel                     lBalanceModel_5;
    Status                           lStatus;
    bool                             lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lBalanceModel_2 = lBalanceModel_1;

    lAreEqual = (lBalanceModel_1 == lBalanceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lBalanceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lBalanceModel_2 = lBalanceModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_1 == lBalanceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lBalanceModel_3.Init(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lBalanceModel_4 = lBalanceModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_3 == lBalanceModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lBalanceModel_4.Init(lBalanceModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lBalanceModel_5 = lBalanceModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lBalanceModel_4 == lBalanceModel_5);
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
        "Balance Model",
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
