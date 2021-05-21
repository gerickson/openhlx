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

#include <OpenHLX/Model/GroupsModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;

static constexpr size_t kGroupsMax = 7;

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    GroupsModel lGroupsModel_1;
    GroupsModel lGroupsModel_2(lGroupsModel_1);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupsModel lGroupsModel;
    Status      lStatus;

    lStatus = lGroupsModel.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupsModel lGroupsModel_1;
    GroupsModel lGroupsModel_2;
    Status      lStatus;

    lStatus = lGroupsModel_1.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupsModel_2.Init(lGroupsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupsModel                         lGroupsModel;
    const char *                        lNameConstant_1 = nullptr;
    const char *                        lNameConstant_2 = "Test Name";
    const GroupModel *                  lImmutableGroupModel;
    GroupModel *                        lMutableGroupModel;
    Status                              lStatus;


    lStatus = lGroupsModel.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid or out-of-bounds observation conditions.

    // Test 1.1: Test invalid observation conditions.

    lStatus = lGroupsModel.GetGroup(IdentifierModel::kIdentifierInvalid, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lGroupsModel.GetGroup(IdentifierModel::kIdentifierInvalid, lMutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lGroupsModel.GetGroup(lNameConstant_1, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test out-of-bounds observation conditions.

    lStatus = lGroupsModel.GetGroup(kGroupsMax + 1, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lGroupsModel.GetGroup(kGroupsMax + 1, lMutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lGroupsModel.GetGroup(lNameConstant_2, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test inbounds observations conditions.

    lStatus = lGroupsModel.GetGroup(kGroupsMax - 1, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupsModel.GetGroup(kGroupsMax - 1, lMutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const GroupModel::IdentifierType    lIdentifierConstant = 7;
    const char *                        lNameConstant_1 = "Test Name 1";
    const char *                        lNameConstant_2 = "Test Name 2";
    GroupsModel                         lGroupsModel;
    GroupModel                          lGroupModel_1;
    const GroupModel *                  lImmutableGroupModel;
    Status                              lStatus;

    // Initialize the source model as a test value.

    lStatus = lGroupModel_1.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Initialize the sources model to test.

    lStatus = lGroupsModel.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test invalid mutations

    lStatus = lGroupsModel.SetGroup(IdentifierModel::kIdentifierInvalid, lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lGroupsModel.SetGroup(kGroupsMax + 1, lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Test valid mutation

    lStatus = lGroupsModel.SetGroup(lIdentifierConstant, lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Ensure that setting a source twice results in
    //           non-error / non-success status.

    lStatus = lGroupsModel.SetGroup(lIdentifierConstant, lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Test that we can successfully observe the set source.

    lStatus = lGroupsModel.GetGroup(lNameConstant_1, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: Test that a different name does not observe the set source.

    lStatus = lGroupsModel.GetGroup(lNameConstant_2, lImmutableGroupModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupsModel    lGroupsModel_1;
    GroupsModel    lGroupsModel_2;
    GroupsModel    lGroupsModel_3;
    GroupsModel    lGroupsModel_4;
    Status         lStatus;
    bool           lAreEqual;

    // Test 1: Test that two value initialized objects are
    //         equivalent.

    lStatus = lGroupsModel_1.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupsModel_2.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupsModel_1 == lGroupsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two object initialized objects are
    //         equivalent.

    lStatus = lGroupsModel_3.Init(lGroupsModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupsModel_3 == lGroupsModel_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that a value initialized and an uninitialized
    //         object are not equal.

    lAreEqual = (lGroupsModel_1 == lGroupsModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupsModel lGroupsModel_1;
    GroupsModel lGroupsModel_2;
    GroupsModel lGroupsModel_3;
    GroupsModel lGroupsModel_4;
    GroupsModel lGroupsModel_5;
    Status         lStatus;
    bool           lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lGroupsModel_2 = lGroupsModel_1;

    lAreEqual = (lGroupsModel_1 == lGroupsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lGroupsModel_1.Init(kGroupsMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lGroupsModel_2 = lGroupsModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupsModel_1 == lGroupsModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lGroupsModel_4.Init(lGroupsModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lGroupsModel_5 = lGroupsModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupsModel_4 == lGroupsModel_5);
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
        "Groups Model",
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
