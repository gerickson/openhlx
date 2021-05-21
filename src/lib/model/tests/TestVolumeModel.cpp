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

#include <OpenHLX/Model/VolumeModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    VolumeModel  lVolumeModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    VolumeModel                       lVolumeModel;
    Status                             lStatus;


    lStatus = lVolumeModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel;
    Status                         lStatus;

    lStatus = lVolumeModel.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    Status                         lStatus;

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_2.Init(lVolumeModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestVolumeObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    VolumeModel::LevelType        lVolumeValue;
    Status                         lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lVolumeModel_1.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_1.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lVolumeModel_1.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_1.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lVolumeModel_2.Init(lVolumeModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_2.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);
}

static void TestMuteObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    VolumeModel::MuteType          lMuteValue;
    Status                         lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lVolumeModel_1.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_1.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lVolumeModel_1.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_1.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lVolumeModel_2.Init(lVolumeModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_2.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);
}

static void TestFixedObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    VolumeModel::FixedType         lFixedValue;
    Status                         lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lVolumeModel_1.GetFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_1.GetFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lVolumeModel_1.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // There is no initializer for the fixed property; this should
    // still return an error on observation.

    lStatus = lVolumeModel_1.GetFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lVolumeModel_2.Init(lVolumeModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // The fixed property should still be uninitialized and should
    // still return an error on observation.

    lStatus = lVolumeModel_2.GetFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext)
{
    TestVolumeObservation(inSuite, inContext);
    TestMuteObservation(inSuite, inContext);
    TestFixedObservation(inSuite, inContext);
}

static void TestVolumeMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    VolumeModel                    lVolumeModel;
    VolumeModel::LevelType        lVolumeValue;
    Status                         lStatus;

    // Test adjustment on an uninitialized model

    lStatus = lVolumeModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lVolumeModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Initialize the model.

    lStatus = lVolumeModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lVolumeModel.SetVolume(VolumeModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lVolumeModel.SetVolume(VolumeModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lVolumeModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lVolumeModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lVolumeModel.SetVolume(lVolumeConstant + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lVolumeModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lVolumeModel.SetVolume(lVolumeConstant - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    lStatus = lVolumeModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    // Test 5: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lVolumeModel.SetVolume(VolumeModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lVolumeModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMax);

    lStatus = lVolumeModel.SetVolume(VolumeModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lVolumeModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMin);
}

static void TestMuteMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel;
    VolumeModel::MuteType          lMuteValue;
    Status                         lStatus;


    // Test toggling on an uninitialized model

    lStatus = lVolumeModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Initialize the model.

    lStatus = lVolumeModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lVolumeModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lVolumeModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Ensure that mute toggling works.

    lStatus = lVolumeModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lVolumeModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lVolumeModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lVolumeModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lVolumeModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lVolumeModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);
}

static void TestFixedMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::FixedType   lFixedConstant = true;
    VolumeModel                    lVolumeModel;
    VolumeModel::FixedType         lFixedValue;
    Status                         lStatus;


    // Initialize the model.

    lStatus = lVolumeModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lVolumeModel.SetFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel.GetFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFixedValue == lFixedConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lVolumeModel.SetFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestVolumeMutation(inSuite, inContext);
    TestMuteMutation(inSuite, inContext);
    TestFixedMutation(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    const VolumeModel::FixedType   lFixedConstant = true;
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    VolumeModel                    lVolumeModel_3;
    VolumeModel                    lVolumeModel_4;
    VolumeModel                    lVolumeModel_5;
    VolumeModel                    lVolumeModel_6;
    Status                         lStatus;
    bool                           lAreEqual;


    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_1 == lVolumeModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lVolumeModel_3.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_4.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_3 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lVolumeModel_5.Init(lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_5 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a value initialized and an uninitialized
    //         object are not equivalent.

    lAreEqual = (lVolumeModel_3 == lVolumeModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that objects with the different volumes are not
    //         equivalent.

    lStatus = lVolumeModel_4.SetVolume(lVolumeConstant + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_5 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that objects with the same volume but different
    //         mute values are not equivalent.

    lStatus = lVolumeModel_4.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_4.SetMute(!lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_5 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 7: Test that objects with the same volume and mute values
    //         but different fixed values are not equivalent.

    lStatus = lVolumeModel_4.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_4.SetFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_5.SetFixed(!lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_5 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 8: Test that objects with the same volume and fixed values
    //         but one with an initialized mute and another with an
    //         uninitialized mute are not equivalent.

    lStatus = lVolumeModel_6.SetFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lVolumeModel_6.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_6 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    const VolumeModel::MuteType    lMuteConstant = true;
    VolumeModel                    lVolumeModel_1;
    VolumeModel                    lVolumeModel_2;
    VolumeModel                    lVolumeModel_3;
    VolumeModel                    lVolumeModel_4;
    VolumeModel                    lVolumeModel_5;
    Status                         lStatus;
    bool                           lAreEqual;


    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lVolumeModel_2 = lVolumeModel_1;

    lAreEqual = (lVolumeModel_1 == lVolumeModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lVolumeModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lVolumeModel_2 = lVolumeModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_1 == lVolumeModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lVolumeModel_3.Init(lVolumeConstant, lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lVolumeModel_4 = lVolumeModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_3 == lVolumeModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lVolumeModel_4.Init(lVolumeModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lVolumeModel_5 = lVolumeModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lVolumeModel_4 == lVolumeModel_5);
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
        "Volume Model",
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
