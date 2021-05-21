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

#include <OpenHLX/Model/IdentifiersCollection.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    IdentifiersCollection lIdentifiersCollection;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection              lIdentifiersCollection;
    Status                             lStatus;

    lStatus = lIdentifiersCollection.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                    lIdentifiersCollection;
    Status                                   lStatus;
    IdentifiersCollection::IdentifierType *  lIdentifiers;
    size_t                                   lCount;
    IdentifiersCollection::IdentifierType    lOneIdentifier[] = { 1 };
    IdentifiersCollection::IdentifierType    lTwoIdentifiers[] = { 1, 2 };
    IdentifiersCollection::IdentifierType    lThreeIdentifiers[] = { 1, 2, 3 };
    IdentifiersCollection::IdentifierType    lFiveIdentifiers[] = { 1, 2, 3, 5, 8 };

    // Test 1: Empty (no) values

    lIdentifiers = nullptr;
    lCount = 0;

    lStatus = lIdentifiersCollection.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2: One value

    lIdentifiers = &lOneIdentifier[0];
    lCount = ElementsOf(lOneIdentifier);

    lStatus = lIdentifiersCollection.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3: Two values

    lIdentifiers = &lTwoIdentifiers[0];
    lCount = ElementsOf(lTwoIdentifiers);

    lStatus = lIdentifiersCollection.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: Three values

    lIdentifiers = &lThreeIdentifiers[0];
    lCount = ElementsOf(lThreeIdentifiers);

    lStatus = lIdentifiersCollection.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 5: Five values

    lIdentifiers = &lFiveIdentifiers[0];
    lCount = ElementsOf(lFiveIdentifiers);

    lStatus = lIdentifiersCollection.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                    lIdentifiersCollection_1;
    IdentifiersCollection                    lIdentifiersCollection_2;
    Status                                   lStatus;
    IdentifiersCollection::IdentifierType *  lIdentifiers;
    size_t                                   lCount;
    IdentifiersCollection::IdentifierType    lOneIdentifier[] = { 1 };
    IdentifiersCollection::IdentifierType    lTwoIdentifiers[] = { 1, 2 };
    IdentifiersCollection::IdentifierType    lThreeIdentifiers[] = { 1, 2, 3 };
    IdentifiersCollection::IdentifierType    lFiveIdentifiers[] = { 1, 2, 3, 5, 8 };

    // Test 1: One object from an uninitialized object

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: One object from an empty (no) values object

    lIdentifiers = nullptr;
    lCount = 0;

    lStatus = lIdentifiersCollection_1.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3: One object from a one value object

    lIdentifiers = &lOneIdentifier[0];
    lCount = ElementsOf(lOneIdentifier);

    lStatus = lIdentifiersCollection_1.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4: One object from a two values object

    lIdentifiers = &lTwoIdentifiers[0];
    lCount = ElementsOf(lTwoIdentifiers);

    lStatus = lIdentifiersCollection_1.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 5: One object from a three values object

    lIdentifiers = &lThreeIdentifiers[0];
    lCount = ElementsOf(lThreeIdentifiers);

    lStatus = lIdentifiersCollection_1.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 6: One object from a five values object

    lIdentifiers = &lFiveIdentifiers[0];
    lCount = ElementsOf(lFiveIdentifiers);

    lStatus = lIdentifiersCollection_1.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)), const IdentifiersCollection::IdentifierType *aIdentifiers, const size_t &aCount, const IdentifiersCollection &aCollection)
{
    Status                                   lStatus;
    IdentifiersCollection::IdentifierType *  lIdentifiers;
    IdentifiersCollection::IdentifierType    lIdentifier;
    size_t                                   lInitialCount;
    size_t                                   lExpectedCount;
    size_t                                   lActualCount;
    bool                                     lDoesContainIdentifier;
    IdentifiersCollection::IdentifierType    lExpectedIdentifiers[8];

    lInitialCount = aCount;

    // Test 1: Test count observation

    lStatus = aCollection.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lInitialCount);

    // Test 2: Test containment observation

    // Test 2.1: Negative test

    lIdentifier = 13;

    lDoesContainIdentifier = aCollection.ContainsIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == false);

    // Test 2.2: Positive tests

    for (size_t i = 0; i < lInitialCount; i++)
    {
        lIdentifier = aIdentifiers[i];

        lDoesContainIdentifier = aCollection.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }

    // Test 3: Test value observation

    lIdentifiers = nullptr;
    lActualCount = 0;

    lStatus = aCollection.GetIdentifiers(lIdentifiers, lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    // Test 4: Test value observation with non-NULL container.

    // Test 4.1: Test value observation with undersized non-NULL container.

    if (lInitialCount > 0)
    {
        lIdentifiers = &lExpectedIdentifiers[0];
        lExpectedCount = std::min(lInitialCount - 1, ElementsOf(lExpectedIdentifiers));
        lActualCount = lExpectedCount;

        lStatus = aCollection.GetIdentifiers(lIdentifiers, lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);
    }

    // Test 4.2: Test value observation with oversized non-NULL container.

    lIdentifiers = &lExpectedIdentifiers[0];
    lExpectedCount = std::min(lInitialCount + 1, ElementsOf(lExpectedIdentifiers));
    lActualCount = lExpectedCount;

    lStatus = aCollection.GetIdentifiers(lIdentifiers, lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lInitialCount);

    // Test 4.3: Test value observation with exactly-sized non-NULL container.

    lIdentifiers = &lExpectedIdentifiers[0];
    lExpectedCount = std::min(lInitialCount, ElementsOf(lExpectedIdentifiers));
    lActualCount = lExpectedCount;

    lStatus = aCollection.GetIdentifiers(lIdentifiers, lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lInitialCount);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext, const IdentifiersCollection::IdentifierType *aIdentifiers, const size_t &aCount)
{
    IdentifiersCollection                    lIdentifiersCollection_1;
    IdentifiersCollection                    lIdentifiersCollection_2;
    Status                                   lStatus;

    // Test 1: Observation with value initialized object

    lStatus = lIdentifiersCollection_1.Init(aIdentifiers, aCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(inSuite, inContext, aIdentifiers, aCount, lIdentifiersCollection_1);

    // Test 2: Observation with object initialized object

    lStatus = lIdentifiersCollection_2.Init(lIdentifiersCollection_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(inSuite, inContext, aIdentifiers, aCount, lIdentifiersCollection_1);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                    lIdentifiersCollection_1;
    IdentifiersCollection                    lIdentifiersCollection_2;
    Status                                   lStatus;
    IdentifiersCollection::IdentifierType *  lIdentifiers;
    IdentifiersCollection::IdentifierType    lIdentifier;
    size_t                                   lInitialCount;
    size_t                                   lActualCount;
    bool                                     lDoesContainIdentifier;
    IdentifiersCollection::IdentifierType    lOneIdentifier[] = { 1 };
    IdentifiersCollection::IdentifierType    lTwoIdentifiers[] = { 1, 2 };
    IdentifiersCollection::IdentifierType    lThreeIdentifiers[] = { 1, 2, 3 };
    IdentifiersCollection::IdentifierType    lFiveIdentifiers[] = { 1, 2, 3, 5, 8 };

    // Test 1: Observation of an uninitialized object

    // Test 1.1: Test count observation with an uninitialized object

    lStatus = lIdentifiersCollection_1.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Test containment observation with an uninitialized object

    // Test 1.2.1: Negative test

    lIdentifier = 13;

    lDoesContainIdentifier = lIdentifiersCollection_1.ContainsIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == false);

    // Test 1.3: Test value observation with an unitialized object

    lIdentifiers = nullptr;
    lActualCount = 0;

    lStatus = lIdentifiersCollection_1.GetIdentifiers(lIdentifiers, lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Observation of one object from an empty (no) values object

    lIdentifiers = nullptr;
    lInitialCount = 0;

    TestObservation(inSuite, inContext, lIdentifiers, lInitialCount);

    // Test 3: Observation of one object from a one value object

    lIdentifiers = &lOneIdentifier[0];
    lInitialCount = ElementsOf(lOneIdentifier);

    TestObservation(inSuite, inContext, lIdentifiers, lInitialCount);

    // Test 4: Observation of one object from a two value object

    lIdentifiers = &lTwoIdentifiers[0];
    lInitialCount = ElementsOf(lTwoIdentifiers);

    TestObservation(inSuite, inContext, lIdentifiers, lInitialCount);

    // Test 5: Observation of one object from a three value object

    lIdentifiers = &lThreeIdentifiers[0];
    lInitialCount = ElementsOf(lThreeIdentifiers);

    TestObservation(inSuite, inContext, lIdentifiers, lInitialCount);

    // Test 6: Observation of one object from a five value object

    lIdentifiers = &lFiveIdentifiers[0];
    lInitialCount = ElementsOf(lFiveIdentifiers);

    TestObservation(inSuite, inContext, lIdentifiers, lInitialCount);
}

static void TestAdd(nlTestSuite *inSuite, void *inContext __attribute__((unused)), const IdentifiersCollection::IdentifierType *aInitialIdentifiers, const size_t &aInitialCount, IdentifiersCollection &aCollection)
{
    Status                                       lStatus;
    size_t                                       lActualCount;
    size_t                                       lExpectedCount;
    size_t                                       lCount;
    const IdentifiersCollection::IdentifierType  lAddedIdentifiers[] = { 5, 8, 13, 21 };

    // Test 1: Test adding identifiers known to be present in the
    //         collection. Verify non-success status and that the size
    //         remains constant.

    if ((aInitialIdentifiers != nullptr) && (aInitialCount > 0))
    {
        for (size_t i = 0; i < aInitialCount; i++)
        {
            lStatus = aCollection.AddIdentifier(aInitialIdentifiers[i]);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

            lStatus = aCollection.GetCount(lActualCount);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
            NL_TEST_ASSERT(inSuite, lActualCount == aInitialCount);
        }
    }

    // Test 2: Test adding an identifier known to be absent from the
    //         collection. Verify that the size increases.

    lCount = ElementsOf(lAddedIdentifiers);
    lExpectedCount = aInitialCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lAddedIdentifiers[i];
        bool                                         lDoesContainIdentifier;


        lStatus = aCollection.AddIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount++;

        lStatus = aCollection.GetCount(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

        lDoesContainIdentifier = aCollection.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }
}

static void TestAdd(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                    lIdentifiersCollection_1;
    IdentifiersCollection                    lIdentifiersCollection_2;
    IdentifiersCollection                    lIdentifiersCollection_3;
    IdentifiersCollection                    lIdentifiersCollection_4;
    Status                                   lStatus;
    const IdentifiersCollection::IdentifierType  lInitialIdentifiers[] = { 1, 2, 3 };
    size_t lCount;

    // Test 1: Test adding to an uninitialized object

    lStatus = lIdentifiersCollection_1.AddIdentifier(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test adding to a default initialized object

    lStatus = lIdentifiersCollection_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestAdd(inSuite, inContext, nullptr, 0, lIdentifiersCollection_1);

    // Test 3: Test adding to a value initialized object

    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_2.Init(lInitialIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestAdd(inSuite, inContext, lInitialIdentifiers, lCount, lIdentifiersCollection_2);

    // Test 4: Test adding to an object initialized object

    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_3.Init(lInitialIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_4.Init(lIdentifiersCollection_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestAdd(inSuite, inContext, lInitialIdentifiers, lCount, lIdentifiersCollection_4);
}

static void TestRemove(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                        lIdentifiersCollection_1;
    IdentifiersCollection                        lIdentifiersCollection_2;
    IdentifiersCollection                        lIdentifiersCollection_3;
    IdentifiersCollection                        lIdentifiersCollection_4;
    Status                                       lStatus;
    const IdentifiersCollection::IdentifierType  lInitialIdentifiers[] = { 1, 2, 3 };
    size_t                                       lActualCount;
    size_t                                       lExpectedCount;
    size_t                                       lCount;

    // Test 1: Test removing from an uninitialized object

    lStatus = lIdentifiersCollection_1.RemoveIdentifier(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test removing from a default initialized object

    lStatus = lIdentifiersCollection_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_1.RemoveIdentifier(0);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test removing from a value initialized object

    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_2.Init(lInitialIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lInitialIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lStatus = lIdentifiersCollection_2.RemoveIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount--;

        lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

        lDoesContainIdentifier = lIdentifiersCollection_2.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == false);
    }
}

static void TestSet(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                          lIdentifiersCollection_1;
    IdentifiersCollection                          lIdentifiersCollection_2;
    IdentifiersCollection                          lIdentifiersCollection_3;
    Status                                         lStatus;
    const IdentifiersCollection::IdentifierType *  lIdentifiers;
    const IdentifiersCollection::IdentifierType    lInitialIdentifiers[] = { 1, 2, 3 };
    const IdentifiersCollection::IdentifierType    lReplacementIdentifiers[] = { 5, 8, 13, 21 };
    size_t                                         lActualCount;
    size_t                                         lExpectedCount;
    size_t                                         lCount;


    // Test 1: Test setting by value to an uninitialized object

    lIdentifiers = nullptr;
    lCount = 0;

    lStatus = lIdentifiersCollection_1.SetIdentifiers(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test setting to a default initialized object

    lStatus = lIdentifiersCollection_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_2.SetIdentifiers(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lInitialIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lDoesContainIdentifier = lIdentifiersCollection_2.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }

    // Test 3: Test setting by value to a value initialized object
    //         that is equivalent.

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_3.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_3.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_3.SetIdentifiers(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lExpectedCount = lCount;

    lStatus = lIdentifiersCollection_3.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lInitialIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lDoesContainIdentifier = lIdentifiersCollection_3.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }

    // Test 4: Test setting by value to a value initialized object
    //         that is not equivalent.

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_3.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_3.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lIdentifiers = lReplacementIdentifiers;
    lCount = ElementsOf(lReplacementIdentifiers);

    lStatus = lIdentifiersCollection_3.SetIdentifiers(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    lStatus = lIdentifiersCollection_3.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lReplacementIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lDoesContainIdentifier = lIdentifiersCollection_3.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }

    // Test 5: Test setting by object to an uintialized object.

    lStatus = lIdentifiersCollection_1.SetIdentifiers(lIdentifiersCollection_3);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 6: Test setting by object to a value intialized object.

    lStatus = lIdentifiersCollection_2.SetIdentifiers(lIdentifiersCollection_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    for (size_t i = 0; i < lCount; i++)
    {
        const IdentifiersCollection::IdentifierType  lIdentifier = lReplacementIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lDoesContainIdentifier = lIdentifiersCollection_2.ContainsIdentifier(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }

    // Test 7: Test setting by object to self.

    lStatus = lIdentifiersCollection_2.SetIdentifiers(lIdentifiersCollection_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 8: Test setting by object to an equivalent object.

    lStatus = lIdentifiersCollection_2.SetIdentifiers(lIdentifiersCollection_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestClear(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    IdentifiersCollection                        lIdentifiersCollection_1;
    IdentifiersCollection                        lIdentifiersCollection_2;
    Status                                       lStatus;
    const IdentifiersCollection::IdentifierType  lInitialIdentifiers[] = { 1, 2, 3 };
    size_t                                       lActualCount;
    size_t                                       lCount;

    // Test 1: Test clearing from an uninitialized object

    lStatus = lIdentifiersCollection_1.ClearIdentifiers();
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test clearing from a default initialized object

    lStatus = lIdentifiersCollection_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_1.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    lStatus = lIdentifiersCollection_1.ClearIdentifiers();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_1.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    // Test 3: Test clearing from a value initialized object

    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lIdentifiersCollection_2.Init(lInitialIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lStatus = lIdentifiersCollection_2.ClearIdentifiers();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_2.GetCount(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestAdd(inSuite, inContext);
    TestRemove(inSuite, inContext);
    TestSet(inSuite, inContext);
    TestClear(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifiersCollection::IdentifierType    lFiveIdentifiers[] = { 1, 2, 3, 5, 8 };
    const IdentifiersCollection::IdentifierType *  lIdentifiers;
    IdentifiersCollection                          lIdentifiersCollection_1;
    IdentifiersCollection                          lIdentifiersCollection_2;
    IdentifiersCollection                          lIdentifiersCollection_3;
    IdentifiersCollection                          lIdentifiersCollection_4;
    IdentifiersCollection                          lIdentifiersCollection_5;
    Status                                         lStatus;
    bool                                           lAreEqual;
    size_t                                         lCount;

    // Test 1: Test that an uninitialized and a default initialized
    //         object are NOT equivalent.

    lStatus = lIdentifiersCollection_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_1 == lIdentifiersCollection_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 2: Test that two default initialized objects are
    //         equivalent.

    lStatus = lIdentifiersCollection_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_1 == lIdentifiersCollection_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two value initialized objects are equivalent.

    lIdentifiers = &lFiveIdentifiers[0];
    lCount = ElementsOf(lFiveIdentifiers);

    lStatus = lIdentifiersCollection_3.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIdentifiersCollection_4.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_3 == lIdentifiersCollection_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that two object initialized objects are
    //         equivalent.

    lStatus = lIdentifiersCollection_5.Init(lIdentifiersCollection_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_5 == lIdentifiersCollection_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const IdentifiersCollection::IdentifierType    lFiveIdentifiers[] = { 1, 2, 3, 5, 8 };
    const IdentifiersCollection::IdentifierType *  lIdentifiers;
    IdentifiersCollection                          lIdentifiersCollection_1;
    IdentifiersCollection                          lIdentifiersCollection_2;
    IdentifiersCollection                          lIdentifiersCollection_3;
    IdentifiersCollection                          lIdentifiersCollection_4;
    IdentifiersCollection                          lIdentifiersCollection_5;
    Status                                         lStatus;
    bool                                           lAreEqual;
    size_t                                         lCount;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lIdentifiersCollection_2 = lIdentifiersCollection_1;

    lAreEqual = (lIdentifiersCollection_1 == lIdentifiersCollection_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lIdentifiersCollection_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiersCollection_2 = lIdentifiersCollection_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_1 == lIdentifiersCollection_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lIdentifiers = &lFiveIdentifiers[0];
    lCount = ElementsOf(lFiveIdentifiers);

    lStatus = lIdentifiersCollection_3.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiersCollection_4 = lIdentifiersCollection_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_3 == lIdentifiersCollection_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lIdentifiersCollection_4.Init(lIdentifiersCollection_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiersCollection_5 = lIdentifiersCollection_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lIdentifiersCollection_4 == lIdentifiersCollection_5);
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
        "Identifiers Collection",
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
