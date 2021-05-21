/*
 *    Copyright (c) 2021 Grant Erickson
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

#include <OpenHLX/Model/EqualizerBandsModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    EqualizerBandsModel lEqualizerBandsModel_1;
    EqualizerBandsModel lEqualizerBandsModel_2(lEqualizerBandsModel_1);
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandModel lEqualizerBandModel;
    Status             lStatus;

    lStatus = lEqualizerBandModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandsModel lEqualizerBandsModel_1;
    EqualizerBandsModel lEqualizerBandsModel_2;
    Status              lStatus;

    lStatus = lEqualizerBandsModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandsModel_2.Init(lEqualizerBandsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandsModel                  lEqualizerBandsModel_1;
    EqualizerBandsModel                  lEqualizerBandsModel_2;
    EqualizerBandsModel::IdentifierType  lIdentifier;
    const EqualizerBandModel *           lImmutableEqualizerBandModel;
    EqualizerBandModel *                 lMutableEqualizerBandModel;
    Status                               lStatus;
    bool                                 lIsValid;

    lStatus = lEqualizerBandsModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test in range identifier validation with status

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lEqualizerBandsModel_1.ValidateIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax;

    lStatus = lEqualizerBandsModel_1.ValidateIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2: Test in range identifier validation with status

    lIdentifier = IdentifierModel::kIdentifierMin - 1;

    lStatus = lEqualizerBandsModel_1.ValidateIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lStatus = lEqualizerBandsModel_1.ValidateIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 3: Test in range identifier validation with Boolean

    lIdentifier = IdentifierModel::kIdentifierMin;

    lIsValid = lEqualizerBandsModel_1.IsValidIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lIsValid == true);

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax;

    lIsValid = lEqualizerBandsModel_1.IsValidIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lIsValid == true);

    // Test 4: Test in range identifier validation with Boolean

    lIdentifier = IdentifierModel::kIdentifierMin - 1;

    lIsValid = lEqualizerBandsModel_1.IsValidIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lIsValid == false);

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lIsValid = lEqualizerBandsModel_1.IsValidIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lIsValid == false);

    // Test 5: Test getting an in range mutable and immutable
    //         equalizer band from an uninitialized bands model.

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lEqualizerBandsModel_2.GetEqualizerBand(lIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lEqualizerBandsModel_2.GetEqualizerBand(lIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 6: Test getting an out of range mutable and immutable equalizer band
    //         from an initialized bands model.

    lIdentifier = IdentifierModel::kIdentifierMin - 1;

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 7: Test getting a mutable and an immutable equalizer band
    //         from an initialized bands model.

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandsModel_1.GetEqualizerBand(lIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const EqualizerBandModel::FrequencyType  lFrequencyConstant = 42;
    const EqualizerBandModel::LevelType      lLevelConstant = EqualizerBandModel::kLevelFlat;
    EqualizerBandModel                       lEqualizerBandModel;
    EqualizerBandsModel                      lEqualizerBandsModel;
    EqualizerBandsModel::IdentifierType      lIdentifier;
    const EqualizerBandModel *               lImmutableEqualizerBandModel;
    Status                                   lStatus;


    lStatus = lEqualizerBandModel.Init(lFrequencyConstant, lLevelConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandsModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test that setting a band with an  out of range identifier fails.

    lIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lStatus = lEqualizerBandsModel.SetEqualizerBand(lIdentifier, lEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Test that setting a band that has already been set responds as expected.

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lEqualizerBandsModel.GetEqualizerBand(lIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandsModel.SetEqualizerBand(lIdentifier, *lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Test that setting a new band succeeds.

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lEqualizerBandsModel.SetEqualizerBand(lIdentifier, lEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandsModel lEqualizerBandsModel_1;
    EqualizerBandsModel lEqualizerBandsModel_2;
    EqualizerBandsModel lEqualizerBandsModel_3;
    EqualizerBandsModel lEqualizerBandsModel_4;
    Status              lStatus;
    bool                lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lEqualizerBandsModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEqualizerBandsModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandsModel_1 == lEqualizerBandsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two object initialized objects are
    //         equivalent.

    lStatus = lEqualizerBandsModel_3.Init(lEqualizerBandsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandsModel_3 == lEqualizerBandsModel_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that a default initialized and an uninitialized
    //         object are not equal.

    lAreEqual = (lEqualizerBandsModel_1 == lEqualizerBandsModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    EqualizerBandsModel lEqualizerBandsModel_1;
    EqualizerBandsModel lEqualizerBandsModel_2;
    EqualizerBandsModel lEqualizerBandsModel_3;
    EqualizerBandsModel lEqualizerBandsModel_4;
    EqualizerBandsModel lEqualizerBandsModel_5;
    Status              lStatus;
    bool                lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lEqualizerBandsModel_2 = lEqualizerBandsModel_1;

    lAreEqual = (lEqualizerBandsModel_1 == lEqualizerBandsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerBandsModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerBandsModel_2 = lEqualizerBandsModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandsModel_1 == lEqualizerBandsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lEqualizerBandsModel_4.Init(lEqualizerBandsModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lEqualizerBandsModel_5 = lEqualizerBandsModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lEqualizerBandsModel_4 == lEqualizerBandsModel_5);
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
        "Equalizer Bands Model",
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
