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

#include <OpenHLX/Model/ToneModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    ToneModel  lToneModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ToneModel  lToneModel;
    Status     lStatus;


    lStatus = lToneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const ToneModel::LevelType  kBassConstant_1 = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    static const ToneModel::LevelType  kTrebleConstant_1 = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    ToneModel                          lToneModel;
    Status                             lStatus;

    // Test 1: Ensure that out of range values return an error on
    //         initialization.

    lStatus = lToneModel.SetTone(ToneModel::kLevelMin - 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(kBassConstant_1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMin - 1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMax + 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(kBassConstant_1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMax + 1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that in range value return success on
    //         initialization.

    lStatus = lToneModel.Init(kBassConstant_1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3: Ensure that double-initialization with in range value
    //         return success on initialization.

    lStatus = lToneModel.Init(kBassConstant_1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ToneModel  lToneModel_1;
    ToneModel  lToneModel_2;
    Status     lStatus;


    lStatus = lToneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_2.Init(lToneModel_1);
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
    const ToneModel::LevelType  kBassLevel = ToneModel::kLevelMin;
    const ToneModel::LevelType  kTrebleLevel = ToneModel::kLevelMin;
    ToneModel                   lToneModel_1;
    ToneModel                   lToneModel_2;
    ToneModel::LevelType        lBassLevel;
    ToneModel::LevelType        lTrebleLevel;
    Status                      lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lToneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lToneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lToneModel_1.Init(kBassLevel, kTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassLevel == kBassLevel);

    lStatus = lToneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleLevel == kTrebleLevel);

    lStatus = lToneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassLevel == kBassLevel);
    NL_TEST_ASSERT(inSuite, lTrebleLevel == kTrebleLevel);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lToneModel_2.Init(lToneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassLevel == kBassLevel);

    lStatus = lToneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleLevel == kTrebleLevel);

    lStatus = lToneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassLevel == kBassLevel);
    NL_TEST_ASSERT(inSuite, lTrebleLevel == kTrebleLevel);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ToneModel::LevelType  kBassConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kTrebleConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kBassConstant_2 = kBassConstant_1 + 1;
    const ToneModel::LevelType  kTrebleConstant_2 = kTrebleConstant_1 + 1;
    ToneModel                   lToneModel;
    ToneModel::LevelType        lBassValue;
    ToneModel::LevelType        lTrebleValue;
    Status                      lStatus;

    // Test adjustment on an uninitialized model.

    lStatus = lToneModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lToneModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Initialize the model.

    lStatus = lToneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lToneModel.SetBass(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetBass(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMin - 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(kBassConstant_1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMin - 1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMax + 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(kBassConstant_1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.SetTone(ToneModel::kLevelMax + 1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lToneModel.SetBass(kBassConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_1);

    lStatus = lToneModel.SetTreble(kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_1);

    // Careful to avoid setting the same value for SetTone as SetBass
    // and SetTreble above as we do not want to test for
    // 'kStatus_ValueAlreadySet' until later.

    lStatus = lToneModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.GetTone(lBassValue, lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_2);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lToneModel.SetBass(kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lToneModel.SetTreble(kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lToneModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that when one of bass or treble are already
    //         set but the other changes that we get success (rather than
    //         already set status).

    lStatus = lToneModel.SetTone(kBassConstant_1, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTone(kBassConstant_2, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 5: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lToneModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lToneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lToneModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lToneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lToneModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lToneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lToneModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    lStatus = lToneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    // Test 6: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lToneModel.SetBass(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMax);

    lStatus = lToneModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMax);

    lStatus = lToneModel.SetBass(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.SetTreble(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMin);

    lStatus = lToneModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lToneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMin);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ToneModel::LevelType  kBassLevel = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    const ToneModel::LevelType  kTrebleLevel = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    ToneModel                   lToneModel_1;
    ToneModel                   lToneModel_2;
    ToneModel                   lToneModel_3;
    ToneModel                   lToneModel_4;
    ToneModel                   lToneModel_5;
    ToneModel                   lToneModel_6;
    Status                      lStatus;
    bool                        lAreEqual;


    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lToneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_1 == lToneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lToneModel_3.Init(kBassLevel, kTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_4.Init(kBassLevel, kTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_3 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lToneModel_5.Init(lToneModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_5 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that a value initialized and an uninitialized
    //         object are not equivalent.

    lAreEqual = (lToneModel_3 == lToneModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that objects with the different bass are not
    //         equivalent.

    lStatus = lToneModel_4.SetBass(kBassLevel + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_5 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that objects with the same bass but different
    //         treble are not equivalent.

    lStatus = lToneModel_4.SetBass(kBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lToneModel_4.SetTreble(kTrebleLevel + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_5 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 8: Test that objects with the same bass but one with an
	//         initialized treble and another with an uninitialized
	//         treble are not equivalent.

    lStatus = lToneModel_6.SetBass(kBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_6 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ToneModel::LevelType  kBassLevel = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    const ToneModel::LevelType  kTrebleLevel = (((ToneModel::kLevelMax - ToneModel::kLevelMin) / 2) + ToneModel::kLevelMin);
    ToneModel                   lToneModel_1;
    ToneModel                   lToneModel_2;
    ToneModel                   lToneModel_3;
    ToneModel                   lToneModel_4;
    ToneModel                   lToneModel_5;
    Status                      lStatus;
    bool                        lAreEqual;


    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lToneModel_2 = lToneModel_1;

    lAreEqual = (lToneModel_1 == lToneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lToneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lToneModel_2 = lToneModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_1 == lToneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lToneModel_3.Init(kBassLevel, kTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lToneModel_4 = lToneModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_3 == lToneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lToneModel_4.Init(lToneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lToneModel_5 = lToneModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lToneModel_4 == lToneModel_5);
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
        "Tone Model",
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
