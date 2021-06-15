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
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Model/SourceModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    SourceModel                      lSourceModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourceModel                      lSourceModel;
    Status                           lStatus;

    lStatus = lSourceModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *                  lNameConstant_1 = nullptr;
    const char *                  lNameConstant_2 = "0123456789ABCDEF0";
    const std::string             lNameConstant_3(lNameConstant_2);
    const char *                  lNameConstant_4 = "Test Name";
    const std::string             lNameConstant_5(lNameConstant_4);
    const char *                  lNameConstant_6 = "0123456789ABCDEF";
    const std::string             lNameConstant_7(lNameConstant_6);
    SourceModel::IdentifierType lIdentifier;
    SourceModel                 lSourceModel_1;
    SourceModel                 lSourceModel_2;
    SourceModel                 lSourceModel_3;
    SourceModel                 lSourceModel_4;
    SourceModel                 lSourceModel_5;
    SourceModel                 lSourceModel_6;
    Status                        lStatus;

    // Test 1: Test invalid names and valid identifiers.

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 1.1: Test a null value.

    lStatus = lSourceModel_1.Init(lNameConstant_1, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSourceModel_2.Init(lNameConstant_1, 0, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test a too-long value with a null-terminated C string.

    lStatus = lSourceModel_1.Init(lNameConstant_2, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.3: Test a too-long value with a pointer and a length.

    lStatus = lSourceModel_2.Init(lNameConstant_2, strlen(lNameConstant_2), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.4: Test a too-long value with a C++ Standard Library string.

    lStatus = lSourceModel_3.Init(lNameConstant_3, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 2: Test valid names and invalid identifiers

    lIdentifier = IdentifierModel::kIdentifierInvalid;

    // Test 2.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lSourceModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lSourceModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lSourceModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lSourceModel_1.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lSourceModel_2.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lSourceModel_3.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 3: Test valid names and valid identifiers

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 3.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lSourceModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lSourceModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lSourceModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lSourceModel_4.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lSourceModel_5.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lSourceModel_6.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourceModel                      lSourceModel_1;
    SourceModel                      lSourceModel_2;
    Status                           lStatus;

    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_2.Init(lSourceModel_1);
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
    const SourceModel::IdentifierType  lIdentifierConstant = 13;
    const char *                       lNameConstant = "Test Name";
    SourceModel                        lSourceModel_1;
    SourceModel                        lSourceModel_2;
    const char *                       lNameValue;
    SourceModel::IdentifierType        lIdentifierValue;
    Status                             lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    // Test 1.1: Test name observation

    lStatus = lSourceModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Test identifier observation

    lStatus = lSourceModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Test name observation

    lStatus = lSourceModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Test identifier observation

    lStatus = lSourceModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lSourceModel_1.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.1: Test name observation

    lStatus = lSourceModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);

    // Test 3.2: Test identifier observation

    lStatus = lSourceModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lSourceModel_2.Init(lSourceModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4.1: Test name observation

    lStatus = lSourceModel_2.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);

    // Test 4.2: Test identifier observation

    lStatus = lSourceModel_2.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);
}

static void TestNameMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *  lNameConstant_1 = nullptr;
    const char *  lNameConstant_2 = "0123456789ABCDEF0";
    const char *  lNameConstant_3 = "Test Name";
    const char *  lNameConstant_4 = "0123456789ABCDEF";
    const char *  lNameValue;
    SourceModel lSourceModel_1;
    SourceModel lSourceModel_2;
    SourceModel lSourceModel_3;
    SourceModel lSourceModel_4;
    Status        lStatus;


    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_3.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_4.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test name mutation

    // Test 1.1: Test invalid values

    // Test 1.1.1: Test a null value

    lStatus = lSourceModel_1.SetName(lNameConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSourceModel_2.SetName(lNameConstant_1, 0);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.2: Test a too-long value with a null-terminated C string.

    lStatus = lSourceModel_1.SetName(lNameConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.1.3: Test a too-long value with a pointer and a length.

    lStatus = lSourceModel_2.SetName(lNameConstant_2, strlen(lNameConstant_2));
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.2: Test valid values

    // Test 1.2.1: Test a null-terminated C string that is under the
    //             maximum length.

    lStatus = lSourceModel_1.SetName(lNameConstant_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    // Test 1.2.2: Test a pointer and a length that is under the maximum
    //             length.

    lStatus = lSourceModel_2.SetName(lNameConstant_3, strlen(lNameConstant_3));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_2.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    // Test 1.2.3: Test a null-terminated C string that is exactly the
    //             maximum length.

    lStatus = lSourceModel_3.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_3.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 1.2.4: Test a pointer and a length that is exactly the maximum
    //             length.

    lStatus = lSourceModel_4.SetName(lNameConstant_4, strlen(lNameConstant_4));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_4.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 1.3: Ensure that setting a name value twice results in non-error
    //           / non-success status.

    lStatus = lSourceModel_4.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lSourceModel_4.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);
}

static void TestIdentifierMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourceModel::IdentifierType lIdentifier;
    SourceModel::IdentifierType lIdentifierValue;
    SourceModel                 lSourceModel_1;
    Status                        lStatus;


    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test identifier mutation

    // Test 1.1: Test an invalid identifier

    lIdentifier = IdentifierModel::kIdentifierInvalid;

    lStatus = lSourceModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test a valid identifier

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lSourceModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifier == lIdentifierValue);

    // Test 1.3: Ensure that setting an identifier value twice results
    //           in non-error / non-success status.

    lStatus = lSourceModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lSourceModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifier == lIdentifierValue);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestNameMutation(inSuite, inContext);
    TestIdentifierMutation(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SourceModel::IdentifierType lIdentifierConstant_1 = IdentifierModel::kIdentifierMin;
    SourceModel::IdentifierType lIdentifierConstant_2 = IdentifierModel::kIdentifierMin + 1;
    const char *                  lNameConstant_1 = "Test Name 1";
    const char *                  lNameConstant_2 = "Test Name 2";
    SourceModel                 lSourceModel_1;
    SourceModel                 lSourceModel_2;
    SourceModel                 lSourceModel_3;
    SourceModel                 lSourceModel_4;
    SourceModel                 lSourceModel_5;
    SourceModel                 lSourceModel_6;
    SourceModel                 lSourceModel_7;
    SourceModel                 lSourceModel_8;
    Status                        lStatus;
    bool                          lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_1 == lSourceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lSourceModel_3.Init(lNameConstant_1, lIdentifierConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_4.Init(lNameConstant_1, lIdentifierConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_3 == lSourceModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two objects with the same name but different
    //         identifiers are not equivalent.

    lStatus = lSourceModel_5.Init(lNameConstant_1, lIdentifierConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_6.Init(lNameConstant_1, lIdentifierConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_5 == lSourceModel_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 4: Test that two objects with different names but the same
    //         identifier are not equivalent.

    lStatus = lSourceModel_7.Init(lNameConstant_1, lIdentifierConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourceModel_8.Init(lNameConstant_2, lIdentifierConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_7 == lSourceModel_8);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SourceModel::IdentifierType  lIdentifierConstant = 13;
    const char *                       lNameConstant = "Test Name";
    SourceModel                        lSourceModel_1;
    SourceModel                        lSourceModel_2;
    SourceModel                        lSourceModel_3;
    SourceModel                        lSourceModel_4;
    SourceModel                        lSourceModel_5;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lSourceModel_2 = lSourceModel_1;

    lAreEqual = (lSourceModel_1 == lSourceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSourceModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSourceModel_2 = lSourceModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_1 == lSourceModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSourceModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSourceModel_4 = lSourceModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_3 == lSourceModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSourceModel_4.Init(lSourceModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSourceModel_5 = lSourceModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSourceModel_4 == lSourceModel_5);
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
        "Source Model",
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
