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
 */

/**
 *    @file
 *      This file implements a unit test for
 *      HLX::Common::ConnectionBuffer.
 *
 */

#include <errno.h>
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>


using namespace HLX;
using namespace HLX::Common;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    ConnectionBuffer lConnectionBuffer;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ConnectionBuffer lConnectionBuffer_1;
    Status           lStatus;

    lStatus = lConnectionBuffer_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    ConnectionBuffer   lConnectionBuffer_3;
    ConnectionBuffer   lConnectionBuffer_4;
    ConnectionBuffer   lConnectionBuffer_5;
    ConnectionBuffer   lConnectionBuffer_6;
    size_t             lCapacity;
    uint8_t *          lOurData;
    uint8_t            lBackingBuffer[kCapacity];
    Status             lStatus;

    // 1: Test capacity-only

    // 1.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_1.Init(lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_2.Init(lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 2: Test pointer and capacity

    // 2.1: Test null (their-owned buffer) pointer and capacity.

    lOurData = nullptr;

    // 2.1.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_3.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 2.1.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_4.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 2.2: Test non-null (our owned buffer) pointer and capacity.

    lOurData = &lBackingBuffer[0];

    // 2.2.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_5.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 2.2.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_6.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
}

static void TestObservation(const ConnectionBuffer &inBuffer, nlTestSuite *inSuite, const size_t &inExpectedCapacity)
{
    size_t    lCapacity;
    size_t    lSize;
    uint8_t * lHead;
    uint8_t * lTail;

    lCapacity = inBuffer.GetCapacity();
    NL_TEST_ASSERT(inSuite, lCapacity == inExpectedCapacity);

    lSize = inBuffer.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lHead = inBuffer.GetHead();
    lTail = inBuffer.GetTail();
    NL_TEST_ASSERT(inSuite, lHead != nullptr);
    NL_TEST_ASSERT(inSuite, lTail != nullptr);
    NL_TEST_ASSERT(inSuite, lHead == lTail);
}


static void TestObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    ConnectionBuffer   lConnectionBuffer_3;
    ConnectionBuffer   lConnectionBuffer_4;
    ConnectionBuffer   lConnectionBuffer_5;
    ConnectionBuffer   lConnectionBuffer_6;
    size_t             lCapacity;
    uint8_t *          lOurData;
    uint8_t            lBackingBuffer[kCapacity];
    Status             lStatus;

    // 1: Test capacity-only

    // 1.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_1.Init(lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_1, inSuite, lCapacity);

    // 1.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_2.Init(lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_2, inSuite, lCapacity);

    // 2: Test pointer and capacity

    // 2.1: Test null (their-owned buffer) pointer and capacity.

    lOurData = nullptr;

    // 2.1.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_3.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_3, inSuite, lCapacity);

    // 2.1.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_4.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_4, inSuite, lCapacity);

    // 2.2: Test non-null (our owned buffer) pointer and capacity.

    lOurData = &lBackingBuffer[0];

    // 2.2.1: Test zero capacity.

    lCapacity = 0;

    lStatus = lConnectionBuffer_5.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_5, inSuite, lCapacity);

    // 2.2.2: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_6.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestObservation(lConnectionBuffer_6, inSuite, lCapacity);
}

static void TestDestroy(ConnectionBuffer &inBuffer, nlTestSuite *inSuite)
{
    size_t    lCapacity;
    size_t    lSize;
    uint8_t * lHead;
    uint8_t * lTail;

    inBuffer.Destroy();

    lCapacity = inBuffer.GetCapacity();
    NL_TEST_ASSERT(inSuite, lCapacity == 0);

    lSize = inBuffer.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lHead = inBuffer.GetHead();
    lTail = inBuffer.GetTail();
    NL_TEST_ASSERT(inSuite, lHead == nullptr);
    NL_TEST_ASSERT(inSuite, lTail == nullptr);
    NL_TEST_ASSERT(inSuite, lHead == lTail);
}

static void TestDestroy(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    size_t             lCapacity;
    uint8_t *          lOurData;
    uint8_t            lBackingBuffer[kCapacity];
    Status             lStatus;


    // 1: Test pointer and capacity

    // 1.1: Test null (their-owned buffer) pointer and capacity.

    lOurData = nullptr;

    // 1.1.1: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_1.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.1.2: Test destroy.

    TestDestroy(lConnectionBuffer_1, inSuite);

    // 1.2: Test non-null (our owned buffer) pointer and capacity.

    lOurData = &lBackingBuffer[0];

    // 1.2.1: Test arbitrary capacity.

    lCapacity = 257;

    lStatus = lConnectionBuffer_2.Init(lOurData, lCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.2.2: Test destroy.

    TestDestroy(lConnectionBuffer_2, inSuite);
}

static void TestPut(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    ConnectionBuffer   lConnectionBuffer_3;
    ConnectionBuffer   lConnectionBuffer_4;
    ConnectionBuffer   lConnectionBuffer_5;
    ConnectionBuffer   lConnectionBuffer_6;
    ConnectionBuffer   lConnectionBuffer_7;
    ConnectionBuffer   lConnectionBuffer_8;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    uint8_t *          lHead;
    int                lComparison;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test put with a data pointer

    // 1.1.1: Test put on a caller-owned backing store

    // 1.1.1.1: Test put with insufficient head room

    NL_TEST_ASSERT(inSuite, lOurSize - 1 <= sizeof (lBackingBuffer));

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_1.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead == nullptr);

    // 1.1.1.2: Test put with sufficient head room

    NL_TEST_ASSERT(inSuite, lOurSize + 1 <= sizeof (lBackingBuffer));

    lStatus = lConnectionBuffer_2.Init(&lBackingBuffer[0], lOurSize + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_2.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);
    NL_TEST_ASSERT(inSuite, lHead == &lBackingBuffer[0]);

    lComparison = memcmp(lHead, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.1.2: Test put on a buffer-owned backing store

    // 1.1.2.1: Test put with insufficient head room

    lStatus = lConnectionBuffer_3.Init(nullptr, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_3.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead == nullptr);

    // 1.1.2.2: Test put with sufficient head room

    lStatus = lConnectionBuffer_4.Init(nullptr, lOurSize + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_4.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lComparison = memcmp(lHead, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.2: Test put without a data pointer

    // 1.2.1: Test put on a caller-owned backing store

    // 1.2.1.1: Test put with insufficient head room

    NL_TEST_ASSERT(inSuite, lOurSize - 1 <= sizeof (lBackingBuffer));

    lStatus = lConnectionBuffer_5.Init(&lBackingBuffer[0], lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_5.Put(lOurSize);
    NL_TEST_ASSERT(inSuite, lHead == nullptr);

    // 1.2.1.2: Test put with sufficient head room

    NL_TEST_ASSERT(inSuite, lOurSize + 1 <= sizeof (lBackingBuffer));

    lStatus = lConnectionBuffer_6.Init(&lBackingBuffer[0], lOurSize + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_6.Put(lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);
    NL_TEST_ASSERT(inSuite, lHead == &lBackingBuffer[0]);

    // 1.2.2: Test put on a buffer-owned backing store

    // 1.2.2.1: Test put with insufficient head room

    lStatus = lConnectionBuffer_7.Init(nullptr, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_7.Put(lOurSize);
    NL_TEST_ASSERT(inSuite, lHead == nullptr);

    // 1.2.2.2: Test put with sufficient head room

    lStatus = lConnectionBuffer_8.Init(nullptr, lOurSize + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_8.Put(lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);
}

static void TestGet(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    uint8_t            lCopyBuffer[16];
    size_t             lSize;
    uint8_t *          lResult;
    int                lComparison;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test get on a caller-owned backing store

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.1.1: Test get with a pointer and a size

    // 1.1.1.1: Test get with insufficient data

    lResult = lConnectionBuffer_1.Put(lOurData, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    lResult = lConnectionBuffer_1.Get(lCopyBuffer, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    // 1.1.1.2: Test get with sufficient data

    // 1.1.1.2.1: Test get with complete retrieval of data

    lResult = lConnectionBuffer_1.Put(lOurData + lOurSize - 1, 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_1.Get(lCopyBuffer, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lComparison = memcmp(lResult, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.1.1.2.2: Test get with partial retrieval of data

    lResult = lConnectionBuffer_1.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_1.Get(lCopyBuffer, lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 3);

    lComparison = memcmp(lResult, lOurData, lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    lResult = lConnectionBuffer_1.Get(lCopyBuffer + lOurSize - 3, 3);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lComparison = memcmp(lResult, lOurData + lOurSize - 3, 3);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.1.2: Test get with a size only

    // 1.1.2.1: Test get with insufficient data

    lResult = lConnectionBuffer_1.Put(lOurData, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    lResult = lConnectionBuffer_1.Get(lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    // 1.1.2.2: Test get with sufficient data

    // 1.1.2.2.1: Test get with complete retrieval of data

    lResult = lConnectionBuffer_1.Put(lOurData + lOurSize - 1, 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_1.Get(lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    // 1.1.2.2.2: Test get with partial retrieval of data

    lResult = lConnectionBuffer_1.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_1.Get(lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 3);

    lResult = lConnectionBuffer_1.Get(3);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    // 1.2: Test get on a buffer-owned backing store

    lStatus = lConnectionBuffer_2.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.2.1: Test get with a pointer and a size

    // 1.2.1.1: Test get with insufficient data

    lResult = lConnectionBuffer_2.Put(lOurData, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    lResult = lConnectionBuffer_2.Get(lCopyBuffer, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    // 1.2.1.2: Test get with sufficient data

    // 1.2.1.2.1: Test get with complete retrieval of data

    lResult = lConnectionBuffer_2.Put(lOurData + lOurSize - 1, 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_2.Get(lCopyBuffer, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lComparison = memcmp(lResult, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.2.1.2.2: Test get with partial retrieval of data

    lResult = lConnectionBuffer_2.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_2.Get(lCopyBuffer, lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 3);

    lComparison = memcmp(lResult, lOurData, lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    lResult = lConnectionBuffer_2.Get(lCopyBuffer + lOurSize - 3, 3);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lComparison = memcmp(lResult, lOurData + lOurSize - 3, 3);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.2.2: Test get with a size only

    // 1.2.2.1: Test get with insufficient data

    lResult = lConnectionBuffer_2.Put(lOurData, lOurSize - 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    lResult = lConnectionBuffer_2.Get(lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize - 1);

    // 1.2.2.2: Test get with sufficient data

    // 1.2.2.2.1: Test get with complete retrieval of data

    lResult = lConnectionBuffer_2.Put(lOurData + lOurSize - 1, 1);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_2.Get(lOurSize);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    // 1.2.2.2.2: Test get with partial retrieval of data

    lResult = lConnectionBuffer_2.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lResult != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lResult = lConnectionBuffer_2.Get(lOurSize - 3);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 3);

    lResult = lConnectionBuffer_2.Get(3);
    NL_TEST_ASSERT(inSuite, lResult == nullptr);
}

static void TestSetSize(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    size_t             lSize;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test set size on a caller-owned backing store

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.1.1: Test set size with a size greater than capacity

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lStatus = lConnectionBuffer_1.SetSize(kCapacity + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOSPC);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    // 1.1.2: Test set size with a size less than or equal to capacity

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lStatus = lConnectionBuffer_1.SetSize(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 7);

    // 1.2: Test set on a buffer-owned backing store

    lStatus = lConnectionBuffer_2.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // 1.2.1: Test set size with a size greater than capacity

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lStatus = lConnectionBuffer_2.SetSize(kCapacity + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOSPC);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    // 1.2.2: Test set size with a size less than or equal to capacity

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 0);

    lStatus = lConnectionBuffer_2.SetSize(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == 7);
}

static void TestFlush(ConnectionBuffer &inBuffer, nlTestSuite *inSuite, const size_t &inExpectedCapacity)
{
    inBuffer.Flush();

    TestObservation(inBuffer, inSuite, inExpectedCapacity);
}

static void TestFlush(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    size_t             lSize;
    uint8_t *          lHead;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test flush on a caller-owned backing store

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_1.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    TestFlush(lConnectionBuffer_1, inSuite, kCapacity);

    // 1.2: Test flush on a buffer-owned backing store

    lStatus = lConnectionBuffer_2.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_2.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    TestFlush(lConnectionBuffer_2, inSuite, kCapacity);
}

static void TestReserve(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    ConnectionBuffer   lConnectionBuffer_3;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    size_t             lSize;
    size_t             lReserveCapacity;
    size_t             lActualCapacity;
    uint8_t *          lHead;
    int                lComparison;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test reserve on a caller-owned backing store

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_1.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lSize = lConnectionBuffer_1.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    lStatus = lConnectionBuffer_1.Reserve(kCapacity << 1);
    NL_TEST_ASSERT(inSuite, lStatus == kError_BufferNotOwned);

    // 1.2: Test reserve on a buffer-owned backing store

    // 1.2.1: Test reserve at less than or equal to current capacity

    lStatus = lConnectionBuffer_3.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_3.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lSize = lConnectionBuffer_3.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    lReserveCapacity = kCapacity - 1;

    lStatus = lConnectionBuffer_3.Reserve(lReserveCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_3.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    // Note: capacity should be UNCHANGED

    lActualCapacity = lConnectionBuffer_3.GetCapacity();
    NL_TEST_ASSERT(inSuite, lActualCapacity == kCapacity);

    lHead = lConnectionBuffer_3.GetHead();
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lComparison = memcmp(lHead, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);

    // 1.2.2: Test reserve at greater than current capacity

    lStatus = lConnectionBuffer_3.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lHead = lConnectionBuffer_3.Put(lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lSize = lConnectionBuffer_3.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    lReserveCapacity = kCapacity << 1;

    lStatus = lConnectionBuffer_3.Reserve(lReserveCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_3.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == kTestDataLength);

    lActualCapacity = lConnectionBuffer_3.GetCapacity();
    NL_TEST_ASSERT(inSuite, lActualCapacity == lReserveCapacity);

    lHead = lConnectionBuffer_3.GetHead();
    NL_TEST_ASSERT(inSuite, lHead != nullptr);

    lComparison = memcmp(lHead, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lComparison == 0);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    TestDestroy(inSuite, inContext);
    TestPut(inSuite, inContext);
    TestGet(inSuite, inContext);
    TestSetSize(inSuite, inContext);
    TestFlush(inSuite, inContext);
    TestReserve(inSuite, inContext);
}

static void TestUtilities(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    constexpr size_t   kCapacity       = 257;
    const char * const kTestData       = "This is a test.";
    const size_t       kTestDataLength = strlen(kTestData);
    ConnectionBuffer   lConnectionBuffer_1;
    ConnectionBuffer   lConnectionBuffer_2;
    ConnectionBuffer   lConnectionBuffer_3;
    ConnectionBuffer   lConnectionBuffer_4;
    const uint8_t *    lOurData;
    size_t             lOurSize;
    uint8_t            lBackingBuffer[kCapacity];
    size_t             lSize;
    size_t             lCapacity;
    Status             lStatus;


    lOurData = reinterpret_cast<const uint8_t *>(kTestData);
    lOurSize = kTestDataLength;

    // 1.1: Test get on a caller-owned backing store

    // 1.1.1: Test put with insufficient head room

    lStatus = lConnectionBuffer_1.Init(&lBackingBuffer[0], kTestDataLength >> 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = Utilities::Put(lConnectionBuffer_1, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lStatus == kError_BufferNotOwned);

    // 1.1.2: Test put with sufficient head room

    lStatus = lConnectionBuffer_2.Init(&lBackingBuffer[0], kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = Utilities::Put(lConnectionBuffer_2, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_2.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lCapacity = lConnectionBuffer_2.GetCapacity();
    NL_TEST_ASSERT(inSuite, lCapacity == kCapacity);

    // 1.2: Test get on a buffer-owned backing store

    // 1.2.1: Test put with insufficient head room

    lStatus = lConnectionBuffer_3.Init(nullptr, kTestDataLength >> 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = Utilities::Put(lConnectionBuffer_3, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_3.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    // The requested size is 15, twice that rounded up to the capacity
    // of 7 is 35, and the next power of two up from that is 64.

    lCapacity = lConnectionBuffer_3.GetCapacity();
    NL_TEST_ASSERT(inSuite, lCapacity == 64);

    // 1.2.2: Test put with sufficient head room

    lStatus = lConnectionBuffer_4.Init(nullptr, kCapacity);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = Utilities::Put(lConnectionBuffer_4, lOurData, lOurSize);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSize = lConnectionBuffer_4.GetSize();
    NL_TEST_ASSERT(inSuite, lSize == lOurSize);

    lCapacity = lConnectionBuffer_4.GetCapacity();
    NL_TEST_ASSERT(inSuite, lCapacity == kCapacity);
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Mutation",       TestMutation),
    NL_TEST_DEF("Utilities",      TestUtilities),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Connection Buffer",
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
