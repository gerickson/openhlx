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

#include <OpenHLX/Model/IdentifierModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    IdentifierModel  lIdentifierModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifierModel                        lIdentifierModel;
    Status                                 lStatus;

    lStatus = lIdentifierModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifierModel::IdentifierType  lIdentifierConstant = 13;
    IdentifierModel                        lIdentifierModel;
    Status                                 lStatus;

    lStatus = lIdentifierModel.Init(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifierModel                        lIdentifierModel_1;
    IdentifierModel                        lIdentifierModel_2;
    Status                                 lStatus;

    lStatus = lIdentifierModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_2.Init(lIdentifierModel_1);
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
    const IdentifierModel::IdentifierType  lIdentifierConstant = 13;
    IdentifierModel                        lIdentifierModel_1;
    IdentifierModel                        lIdentifierModel_2;
    IdentifierModel::IdentifierType        lIdentifierValue;
    Status                                 lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lIdentifierModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lIdentifierModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lIdentifierModel_1.Init(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lIdentifierModel_2.Init(lIdentifierModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_2.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifierModel::IdentifierType  lIdentifierConstant = 13;
    IdentifierModel                        lIdentifierModel;
    IdentifierModel::IdentifierType        lIdentifierValue;
    Status                                 lStatus;

    // Initialize the model.

    lStatus = lIdentifierModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an invalid value returns an error on
    //         mutation.

    lStatus = lIdentifierModel.SetIdentifier(IdentifierModel::kIdentifierInvalid);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lIdentifierModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lIdentifierModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifierModel::IdentifierType  lIdentifierConstant = 13;
    IdentifierModel                        lIdentifierModel_1;
    IdentifierModel                        lIdentifierModel_2;
    IdentifierModel                        lIdentifierModel_3;
    IdentifierModel                        lIdentifierModel_4;
    IdentifierModel                        lIdentifierModel_5;
    Status                                 lStatus;
    bool                                   lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lIdentifierModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_1 == lIdentifierModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lIdentifierModel_3.Init(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifierModel_4.Init(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_3 == lIdentifierModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lIdentifierModel_5.Init(lIdentifierModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_5 == lIdentifierModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifierModel::IdentifierType  lIdentifierConstant = 13;
    IdentifierModel                        lIdentifierModel_1;
    IdentifierModel                        lIdentifierModel_2;
    IdentifierModel                        lIdentifierModel_3;
    IdentifierModel                        lIdentifierModel_4;
    IdentifierModel                        lIdentifierModel_5;
    Status                                 lStatus;
    bool                                   lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lIdentifierModel_2 = lIdentifierModel_1;

    lAreEqual = (lIdentifierModel_1 == lIdentifierModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lIdentifierModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifierModel_2 = lIdentifierModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_1 == lIdentifierModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lIdentifierModel_3.Init(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifierModel_4 = lIdentifierModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_3 == lIdentifierModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lIdentifierModel_4.Init(lIdentifierModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifierModel_5 = lIdentifierModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifierModel_4 == lIdentifierModel_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestUtilities(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char    lBuffer_1[1] = { '1' };
    const char *  lBuffer_2 = "3";
    const uint8_t lBuffer_3[2] = { 0x31, 0x33 };
    const uint8_t lBuffer_4[3] = { 0x31, 0x37, 0x00 };
    const IdentifierModel::IdentifierType lExpectedIdentifier_1 = 1;
    const IdentifierModel::IdentifierType lExpectedIdentifier_2 = 3;
    const IdentifierModel::IdentifierType lExpectedIdentifier_3 = 13;
    const IdentifierModel::IdentifierType lExpectedIdentifier_4 = 17;
    IdentifierModel::IdentifierType lActualIdentifier;
    Status lStatus;

    lStatus = Model::Utilities::ParseIdentifier(lBuffer_1, sizeof (lBuffer_1), lActualIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lExpectedIdentifier_1 == lActualIdentifier);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer_2, lActualIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lExpectedIdentifier_2 == lActualIdentifier);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer_3, sizeof (lBuffer_3), lActualIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lExpectedIdentifier_3 == lActualIdentifier);

    lStatus = Model::Utilities::ParseIdentifier(lBuffer_4, lActualIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lExpectedIdentifier_4 == lActualIdentifier);
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
    NL_TEST_DEF("Utilities",      TestUtilities),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Identifier Model",
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
