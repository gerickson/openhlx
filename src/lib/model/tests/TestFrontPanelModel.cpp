/*
 *    Copyright (c) 2019-2020 Grant Erickson
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

#include <OpenHLX/Model/FrontPanelModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    FrontPanelModel  lFrontPanelModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    FrontPanelModel                        lFrontPanelModel;
    Status                                 lStatus;

    lStatus = lFrontPanelModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const FrontPanelModel::BrightnessType  lBrightnessConstant = (((FrontPanelModel::kBrightnessMax - FrontPanelModel::kBrightnessMin) / 2) + FrontPanelModel::kBrightnessMin);
    const FrontPanelModel::LockedType      lLockedConstant = true;
    FrontPanelModel                        lFrontPanelModel;
    Status                                 lStatus;

    lStatus = lFrontPanelModel.Init(lBrightnessConstant, lLockedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    FrontPanelModel                        lFrontPanelModel_1;
    FrontPanelModel                        lFrontPanelModel_2;
    Status                                 lStatus;

    lStatus = lFrontPanelModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_2.Init(lFrontPanelModel_1);
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
    const FrontPanelModel::BrightnessType  lBrightnessConstant = FrontPanelModel::kBrightnessMin;
    const FrontPanelModel::LockedType      lLockedConstant = true;
    FrontPanelModel                        lFrontPanelModel_1;
    FrontPanelModel                        lFrontPanelModel_2;
    FrontPanelModel::BrightnessType        lBrightnessValue;
    FrontPanelModel::LockedType            lLockedValue;
    Status                                 lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lFrontPanelModel_1.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lFrontPanelModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_1.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lFrontPanelModel_1.GetLocked(lLockedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lFrontPanelModel_1.Init(lBrightnessConstant, lLockedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_1.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == lBrightnessConstant);

    lStatus = lFrontPanelModel_1.GetLocked(lLockedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLockedValue == lLockedConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lFrontPanelModel_2.Init(lFrontPanelModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_2.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == lBrightnessConstant);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const FrontPanelModel::BrightnessType  lBrightnessConstant = FrontPanelModel::kBrightnessMax - 1;
    const FrontPanelModel::LockedType      lLockedConstant = false;
    FrontPanelModel                        lFrontPanelModel;
    FrontPanelModel::BrightnessType        lBrightnessValue;
    FrontPanelModel::LockedType            lLockedValue;
    Status                                 lStatus;

    // Initialize the model.

    lStatus = lFrontPanelModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range values can be successfully set
    //         and observed.

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == FrontPanelModel::kBrightnessMax);

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == FrontPanelModel::kBrightnessMin);

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMax - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == FrontPanelModel::kBrightnessMax - 1);

    lStatus = lFrontPanelModel.SetBrightness(FrontPanelModel::kBrightnessMin + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == FrontPanelModel::kBrightnessMin + 1);

    lStatus = lFrontPanelModel.SetBrightness(lBrightnessConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetBrightness(lBrightnessValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBrightnessValue == lBrightnessConstant);

    lStatus = lFrontPanelModel.SetLocked(lLockedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel.GetLocked(lLockedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lLockedValue == lLockedConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lFrontPanelModel.SetBrightness(lBrightnessConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lFrontPanelModel.SetLocked(lLockedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const FrontPanelModel::BrightnessType  lBrightnessConstant_1 = 1;
    const FrontPanelModel::BrightnessType  lBrightnessConstant_2 = 3;
    const FrontPanelModel::LockedType      lLockedConstant_1 = true;
    const FrontPanelModel::LockedType      lLockedConstant_2 = false;
    FrontPanelModel                        lFrontPanelModel_1;
    FrontPanelModel                        lFrontPanelModel_2;
    FrontPanelModel                        lFrontPanelModel_3;
    FrontPanelModel                        lFrontPanelModel_4;
    FrontPanelModel                        lFrontPanelModel_5;
    FrontPanelModel                        lFrontPanelModel_6;
    FrontPanelModel                        lFrontPanelModel_7;
    FrontPanelModel                        lFrontPanelModel_8;
    Status                                 lStatus;
    bool                                   lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lFrontPanelModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_1 == lFrontPanelModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lFrontPanelModel_3.Init(lBrightnessConstant_1, lLockedConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lFrontPanelModel_4.Init(lBrightnessConstant_1, lLockedConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_3 == lFrontPanelModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lFrontPanelModel_5.Init(lFrontPanelModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_5 == lFrontPanelModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a fully value-initialized and an uninitialized object
    //         are not equal.

    lAreEqual = (lFrontPanelModel_5 == lFrontPanelModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that a brightness-only initialized object and a
    //         fully value-initialized object are not equal.

    lStatus = lFrontPanelModel_7.SetBrightness(lBrightnessConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_7 == lFrontPanelModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that fully value-initialized objects but with
    //         different frequencies are not equal.

    lStatus = lFrontPanelModel_8.Init(lBrightnessConstant_2, lLockedConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_5 == lFrontPanelModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 7: Test that fully value-initialized objects but with
    //         different levels are not equal.

    lStatus = lFrontPanelModel_5.SetLocked(lLockedConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lFrontPanelModel_6.SetLocked(lLockedConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_5 == lFrontPanelModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const FrontPanelModel::BrightnessType  lBrightnessConstant = (((FrontPanelModel::kBrightnessMax - FrontPanelModel::kBrightnessMin) / 2) + FrontPanelModel::kBrightnessMin);
    const FrontPanelModel::LockedType      lLockedConstant = true;
    FrontPanelModel                        lFrontPanelModel_1;
    FrontPanelModel                        lFrontPanelModel_2;
    FrontPanelModel                        lFrontPanelModel_3;
    FrontPanelModel                        lFrontPanelModel_4;
    FrontPanelModel                        lFrontPanelModel_5;
    Status                                 lStatus;
    bool                                   lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lFrontPanelModel_2 = lFrontPanelModel_1;

    lAreEqual = (lFrontPanelModel_1 == lFrontPanelModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lFrontPanelModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lFrontPanelModel_2 = lFrontPanelModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_1 == lFrontPanelModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lFrontPanelModel_3.Init(lBrightnessConstant, lLockedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lFrontPanelModel_4 = lFrontPanelModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_3 == lFrontPanelModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lFrontPanelModel_4.Init(lFrontPanelModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lFrontPanelModel_5 = lFrontPanelModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lFrontPanelModel_4 == lFrontPanelModel_5);
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
        "Front Panel Model",
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
