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

#include <OpenHLX/Model/InfraredModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    InfraredModel  lInfraredModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    InfraredModel                      lInfraredModel;
    Status                             lStatus;

    lStatus = lInfraredModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const InfraredModel::DisabledType  lDisabledConstant = false;
    InfraredModel                      lInfraredModel;
    Status                             lStatus;

    lStatus = lInfraredModel.Init(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    InfraredModel                      lInfraredModel_1;
    InfraredModel                      lInfraredModel_2;
    Status                             lStatus;

    lStatus = lInfraredModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_2.Init(lInfraredModel_1);
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
    const InfraredModel::DisabledType  lDisabledConstant = false;
    InfraredModel                      lInfraredModel_1;
    InfraredModel                      lInfraredModel_2;
    InfraredModel::DisabledType        lDisabledValue;
    Status                             lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lInfraredModel_1.GetDisabled(lDisabledValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lInfraredModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_1.GetDisabled(lDisabledValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lInfraredModel_1.Init(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_1.GetDisabled(lDisabledValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lDisabledValue == lDisabledConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lInfraredModel_2.Init(lInfraredModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_2.GetDisabled(lDisabledValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lDisabledValue == lDisabledConstant);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const InfraredModel::DisabledType  lDisabledConstant = false;
    InfraredModel                      lInfraredModel;
    InfraredModel::DisabledType        lDisabledValue;
    Status                             lStatus;

    // Initialize the model.

    lStatus = lInfraredModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lInfraredModel.SetDisabled(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel.GetDisabled(lDisabledValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lDisabledValue == lDisabledConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lInfraredModel.SetDisabled(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const InfraredModel::DisabledType  lDisabledConstant = false;
    InfraredModel                      lInfraredModel_1;
    InfraredModel                      lInfraredModel_2;
    InfraredModel                      lInfraredModel_3;
    InfraredModel                      lInfraredModel_4;
    InfraredModel                      lInfraredModel_5;
    InfraredModel                      lInfraredModel_6;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lInfraredModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_1 == lInfraredModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lInfraredModel_3.Init(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInfraredModel_4.Init(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_3 == lInfraredModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lInfraredModel_5.Init(lInfraredModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_5 == lInfraredModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a value initialized and an uninitialized
    //         objects are not equivalent.

    lAreEqual = (lInfraredModel_3 == lInfraredModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that two objects with different disabled values
    //         are not equivalent.

    lStatus = lInfraredModel_4.SetDisabled(!lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_5 == lInfraredModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const InfraredModel::DisabledType  lDisabledConstant = false;
    InfraredModel                     lInfraredModel_1;
    InfraredModel                     lInfraredModel_2;
    InfraredModel                     lInfraredModel_3;
    InfraredModel                     lInfraredModel_4;
    InfraredModel                     lInfraredModel_5;
    Status                           lStatus;
    bool                             lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lInfraredModel_2 = lInfraredModel_1;

    lAreEqual = (lInfraredModel_1 == lInfraredModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lInfraredModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lInfraredModel_2 = lInfraredModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_1 == lInfraredModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lInfraredModel_3.Init(lDisabledConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lInfraredModel_4 = lInfraredModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_3 == lInfraredModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lInfraredModel_4.Init(lInfraredModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lInfraredModel_5 = lInfraredModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lInfraredModel_4 == lInfraredModel_5);
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
        "Infrared Model",
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
