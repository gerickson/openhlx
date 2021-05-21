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
 *      This file implements a unit test for HLX::Common::SocketAddress.
 *
 */

#include <utility>

#include <errno.h>
#include <string.h>

#include <arpa/inet.h>

#include <nlunit-test.h>

#include <OpenHLX/Common/SocketAddress.hpp>


using namespace HLX;
using namespace HLX::Common;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    SocketAddress lSocketAddress;

    (void)lSocketAddress;
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SocketAddress  lSocketAddress_1;
    SocketAddress  lSocketAddress_2;
    SocketAddress  lSocketAddress_3;
    SocketAddress  lSocketAddress_4;
    SocketAddress  lSocketAddress_5;
    SocketAddress  lSocketAddress_6;
    int            lStatus;
    bool           lAreEqual;


    // Socket addresses are plain old data (POD), so there exists no
    // constructor and no predicatable initialization for them. Start
    // everything off at a known state.

    memset(&lSocketAddress_1, 0, sizeof (lSocketAddress_1));
    memset(&lSocketAddress_2, 0, sizeof (lSocketAddress_2));
    memset(&lSocketAddress_3, 0, sizeof (lSocketAddress_3));
    memset(&lSocketAddress_4, 0, sizeof (lSocketAddress_4));
    memset(&lSocketAddress_5, 0, sizeof (lSocketAddress_5));

    lSocketAddress_3.uSocketAddress.sa_family = AF_INET;
    lStatus = inet_pton(AF_INET,
                        "127.0.0.1",
                        &lSocketAddress_3.uSocketAddressIPv4.sin_addr);
    NL_TEST_ASSERT(inSuite, lStatus == true);

    lSocketAddress_4.uSocketAddress.sa_family = AF_INET;
    lStatus = inet_pton(AF_INET,
                        "192.168.1.1",
                        &lSocketAddress_4.uSocketAddressIPv4.sin_addr);

    lSocketAddress_5.uSocketAddress.sa_family = AF_INET6;
    lStatus = inet_pton(AF_INET6,
                        "::1",
                        &lSocketAddress_5.uSocketAddressIPv6.sin6_addr);

    lSocketAddress_6.uSocketAddress.sa_family = AF_INET6;
    lStatus = inet_pton(AF_INET6,
                        "2601:647:4901:5dc0:419:95e6:a382:2a2f",
                        &lSocketAddress_6.uSocketAddressIPv6.sin6_addr);

    // 1: Test that any arbitrary socket address is equal to itself,
    //    initialized or not.

    lAreEqual = (lSocketAddress_1 == lSocketAddress_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lAreEqual = (lSocketAddress_3 == lSocketAddress_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // 2: Test that two uninitialized socket addresses are equal.

    lAreEqual = (lSocketAddress_1 == lSocketAddress_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // 3: Test that an initialized and an uninitialized socket address
    //    are NOT equal.

    lAreEqual = (lSocketAddress_2 == lSocketAddress_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lSocketAddress_4 == lSocketAddress_1);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // 4: Test that two initialized socket addresses with the
    //    same family but different IP addresses are NOT equal.

    lAreEqual = (lSocketAddress_3 == lSocketAddress_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lSocketAddress_5 == lSocketAddress_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // 5: Test that two initialized socket addresses with the same
    //    family and IP address are equal.

    lAreEqual = (lSocketAddress_3 == lSocketAddress_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lAreEqual = (lSocketAddress_4 == lSocketAddress_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lAreEqual = (lSocketAddress_5 == lSocketAddress_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    lAreEqual = (lSocketAddress_6 == lSocketAddress_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // 6: Test that two initialized socket addresses with different
    //    families are not equal.

    lAreEqual = (lSocketAddress_3 == lSocketAddress_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lSocketAddress_3 == lSocketAddress_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lSocketAddress_4 == lSocketAddress_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    lAreEqual = (lSocketAddress_4 == lSocketAddress_6);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Equality",       TestEquality),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Socket Address",
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
