/*
 *    Copyright (c) 2022 Grant Erickson
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
 *      HLX::Client::Command::Network::* classes.
 *
 */

#include <utility>

#include <errno.h>
#include <string.h>

#include <nlunit-test.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/NetworkControllerCommands.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>

using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Client::Command;
using namespace HLX::Client::Command::Network;
using namespace HLX::Utilities;
using namespace Nuovations;


struct TestCase
{
    const char * const mResponseString;
    bool               mMatchExpected;
};

static TestCase sDHCPv4EnabledResponseVector[] = {
    { "(DHCP0)",  true  },
    { "(DHCP1)",  true  },

    { "(DHCP-1)", false },
    { "(DHCP2)",  false },
    { "[DHCP0]",  false },
    { "[DHCP1]",  false },
    { "[DHCP-1]", false },
    { "[DHCP2]",  false }
};

static TestCase sEthernetEUI48ResponseVector[] = {
    { "(MAC00-00-00-00-00-00)", true  },
    { "(MAC00-50-C2-D8-24-71)", true  },
    { "(MACA0-99-9B-18-05-DB)", true  },
    { "(MACFF-FF-FF-FF-FF-FF)", true  },

    { "(00-00-00-00-00-00)",    false },
    { "(FF-FF-FF-FF-FF-FF)",    false },
    { "[MAC00-00-00-00-00-00]", false },
    { "[MACFF-FF-FF-FF-FF-FF]", false },
};

static TestCase sIPDefaultRouterAddressResponseVector[] = {
    // IPv4 Test Cases

    // Positive IPv4 Test Cases

    { "(GW0.0.0.0)",         true  },
    { "(GW127.0.0.1)",       true  },
    { "(GW1.2.3.4)",         true  },
    { "(GW1.102.103.104)",   true  },
    { "(GW8.8.8.8)",         true  },
    { "(GW192.168.1.72)",    true  },
    { "(GW255.255.255.0)",   true  },
    { "(GW224.0.0.0)",       true  },
    { "(GW224.0.0.1)",       true  },
    { "(GW224.0.0.128)",     true  },
    { "(GW224.0.0.254)",     true  },
    { "(GW224.0.0.255)",     true  },
    { "(GW224.0.1.0)",       true  },
    { "(GW224.0.1.1)",       true  },
    { "(GW224.0.1.128)",     true  },
    { "(GW224.0.1.254)",     true  },
    { "(GW224.0.1.255)",     true  },
    { "(GW224.0.2.0)",       true  },
    { "(GW224.0.2.1)",       true  },
    { "(GW224.0.129.0)",     true  },
    { "(GW224.0.255.254)",   true  },
    { "(GW224.0.255.255)",   true  },
    { "(GW224.3.0.0)",       true  },
    { "(GW224.3.0.1)",       true  },
    { "(GW224.4.0.0)",       true  },
    { "(GW224.4.255.254)",   true  },
    { "(GW224.4.255.255)",   true  },
    { "(GW232.0.0.0)",       true  },
    { "(GW232.0.0.1)",       true  },
    { "(GW232.128.0.0)",     true  },
    { "(GW232.255.255.254)", true  },
    { "(GW232.255.255.255)", true  },
    { "(GW233.0.0.0)",       true  },
    { "(GW233.0.0.1)",       true  },
    { "(GW233.126.0.0)",     true  },
    { "(GW233.251.255.254)", true  },
    { "(GW233.251.255.255)", true  },
    { "(GW233.252.0.0)",     true  },
    { "(GW233.252.0.1)",     true  },
    { "(GW233.254.0.0)",     true  },
    { "(GW233.255.255.254)", true  },
    { "(GW233.255.255.255)", true  },
    { "(GW234.0.0.0)",       true  },
    { "(GW234.0.0.1)",       true  },
    { "(GW234.128.0.0)",     true  },
    { "(GW234.255.255.254)", true  },
    { "(GW234.255.255.255)", true  },
    { "(GW239.0.0.0)",       true  },
    { "(GW239.0.0.1)",       true  },
    { "(GW239.128.0.0)",     true  },
    { "(GW239.255.255.254)", true  },
    { "(GW239.255.255.255)", true  },
    { "(GW255.255.255.255)", true  },

    // Negative IPv4 Test Cases

    { "[GW0.0.0.0]",         false },
    { "[GW127.0.0.1]",       false },
    { "[GW255.255.255.0]",   false },
    { "(GW256.300.987.1)",   false },

    // IPv6 Test Cases

    // Positive IPv6 Test Cases

    { "(GW::)",                                           true  },
    { "(GW0000:0000:0000:0000:0000:0000:0000:0000)",      true  },
    { "(GW::1)",                                          true  },
    { "(GW0000:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(GW0000:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(GWfe80::8edc:d4ff:fe3a:ebfb)",                    true  },
    { "(GWfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb)",      true  },
    { "(GWff01::1)",                                      true  },
    { "(GWff01:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(GWff01:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(GWfd00:0:1:1::1)",                                true  },
    { "(GWfd00:0000:1:1::1)",                             true  },
    { "(GWfd00:0000:0001:1::1)",                          true  },
    { "(GWfd00:0000:0001:0001::1)",                       true  },
    { "(GWfd00:0000:0001:0001:0000:0000:0000:1)",         true  },
    { "(GWfd00:0000:0001:0001:0000:0000:0000:0001)",      true  },
    { "(GWfd12:3456:1:abcd:abcd:ef00:fedc:ba09)",         true  },
    { "(GWfdff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(GWff01::1)",                                      true  },
    { "(GWff02::1)",                                      true  },
    { "(GWff03::1)",                                      true  },
    { "(GWff04::1)",                                      true  },
    { "(GWff05::1)",                                      true  },
    { "(GWff08::1)",                                      true  },
    { "(GWff0e::1)",                                      true  },
    { "(GWff01::2)",                                      true  },
    { "(GWff02::2)",                                      true  },
    { "(GWff03::2)",                                      true  },
    { "(GWff04::2)",                                      true  },
    { "(GWff05::2)",                                      true  },
    { "(GWff08::2)",                                      true  },
    { "(GWff0e::2)",                                      true  },
    { "(GWff11::1)",                                      true  },
    { "(GWff12::1)",                                      true  },
    { "(GWff13::1)",                                      true  },
    { "(GWff14::1)",                                      true  },
    { "(GWff15::1)",                                      true  },
    { "(GWff18::1)",                                      true  },
    { "(GWff1e::1)",                                      true  },
    { "(GWff11:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff12:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff13:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff14:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff15:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff18:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff1e:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(GWff31:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff32:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff33:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff34:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff35:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff38:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff3e:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(GWff31:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff32:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff33:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff34:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff35:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff38:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff3e:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(GWff31:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff32:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff33:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff34:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff35:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff38:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff3e:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(GWff31:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff32:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff33:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff34:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff35:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff38:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWff3e:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(GWffff::)",                                       true  },
    { "(GWffff:ffff::)",                                  true  },
    { "(GWffff:ffff:ffff::)",                             true  },
    { "(GWffff:ffff:ffff:ffff::)",                        true  },
    { "(GWffff:ffff:ffff:ffff:ffff::)",                   true  },
    { "(GWffff:ffff:ffff:ffff:ffff:ffff::)",              true  },
    { "(GWffff:ffff:ffff:ffff:ffff:ffff:ffff::)",         true  },
    { "(GWffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(GW2620:1:10e7:400:e83f:b28f:9c3a:1941)",          true  },
    { "(GW2620:0001:10e7:400:e83f:b28f:9c3a:1941)",       true  },
    { "(GW2620:0001:10e7:0400:e83f:b28f:9c3a:1941)",      true  },

    // IPv6 Dual Address Positive Test Cases

    { "(GW::11.22.33.44)",                                 true  },
    { "(GW0000:0000:0000:0000:0000:0000:11.22.33.44)",     true  },
    { "(GW::1234:5678:1.2.3.4)",                           true  },
    { "(GW0000:0000:0000:0000:1234:5678:1.2.3.4)",         true  },
    { "(GW::1234:5678:91.123.4.56)",                       true  },
    { "(GW0000:0000:0000:0000:1234:5678:91.123.4.56)",     true  },
    { "(GW2001:db8:3333:4444:5555:6666:1.2.3.4)",          true  },
    { "(GW2001:0db8:3333:4444:5555:6666:1.2.3.4)",         true  },
    { "(GW2001:db8::123.123.123.123)",                     true  },
    { "(GW2001:0db8:0000:0000:0000:0000:123.123.123.123)", true  },
    { "(GW2001:db8::1234:5678:5.6.7.8)",                   true  },
    { "(GW2001:0db8:0000:0000:1234:5678:5.6.7.8)",         true  },

    // Negative IPv6 Test Cases

    { "[GW::]",                                            false },
    { "[GW::1]",                                           false },
    { "[GWfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb]",       false },

    { "(GWfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb:54ce)",  false },

    { "(GWge80::1)",                                       false },

    // IP6 Dual Address Negative Test Cases

    { "(GW::256.300.987.1)",                               false },
    { "(GW::1234:5678:256.300.987.1)",                     false },
    { "(GW::1234:5678:256.300.987.1)",                     false },
    { "(GW2001:db8:3333:4444:5555:6666:256.300.987.1)",    false },
    { "(GW2001:db8::256.300.987.1)",                       false },
    { "(GW2001:db8::256.300.987.1)",                       false },
};

static TestCase sIPHostAddressResponseVector[] = {
    // IPv4 Test Cases

    // Positive IPv4 Test Cases

    { "(IP0.0.0.0)",         true  },
    { "(IP127.0.0.1)",       true  },
    { "(IP1.2.3.4)",         true  },
    { "(IP1.102.103.104)",   true  },
    { "(IP8.8.8.8)",         true  },
    { "(IP192.168.1.72)",    true  },
    { "(IP255.255.255.0)",   true  },
    { "(IP224.0.0.0)",       true  },
    { "(IP224.0.0.1)",       true  },
    { "(IP224.0.0.128)",     true  },
    { "(IP224.0.0.254)",     true  },
    { "(IP224.0.0.255)",     true  },
    { "(IP224.0.1.0)",       true  },
    { "(IP224.0.1.1)",       true  },
    { "(IP224.0.1.128)",     true  },
    { "(IP224.0.1.254)",     true  },
    { "(IP224.0.1.255)",     true  },
    { "(IP224.0.2.0)",       true  },
    { "(IP224.0.2.1)",       true  },
    { "(IP224.0.129.0)",     true  },
    { "(IP224.0.255.254)",   true  },
    { "(IP224.0.255.255)",   true  },
    { "(IP224.3.0.0)",       true  },
    { "(IP224.3.0.1)",       true  },
    { "(IP224.4.0.0)",       true  },
    { "(IP224.4.255.254)",   true  },
    { "(IP224.4.255.255)",   true  },
    { "(IP232.0.0.0)",       true  },
    { "(IP232.0.0.1)",       true  },
    { "(IP232.128.0.0)",     true  },
    { "(IP232.255.255.254)", true  },
    { "(IP232.255.255.255)", true  },
    { "(IP233.0.0.0)",       true  },
    { "(IP233.0.0.1)",       true  },
    { "(IP233.126.0.0)",     true  },
    { "(IP233.251.255.254)", true  },
    { "(IP233.251.255.255)", true  },
    { "(IP233.252.0.0)",     true  },
    { "(IP233.252.0.1)",     true  },
    { "(IP233.254.0.0)",     true  },
    { "(IP233.255.255.254)", true  },
    { "(IP233.255.255.255)", true  },
    { "(IP234.0.0.0)",       true  },
    { "(IP234.0.0.1)",       true  },
    { "(IP234.128.0.0)",     true  },
    { "(IP234.255.255.254)", true  },
    { "(IP234.255.255.255)", true  },
    { "(IP239.0.0.0)",       true  },
    { "(IP239.0.0.1)",       true  },
    { "(IP239.128.0.0)",     true  },
    { "(IP239.255.255.254)", true  },
    { "(IP239.255.255.255)", true  },
    { "(IP255.255.255.255)", true  },

    // Negative IPv4 Test Cases

    { "[IP0.0.0.0]",         false },
    { "[IP127.0.0.1]",       false },
    { "[IP255.255.255.0]",   false },
    { "(IP256.300.987.1)",   false },

    // IPv6 Test Cases

    // Positive IPv6 Test Cases

    { "(IP::)",                                           true  },
    { "(IP0000:0000:0000:0000:0000:0000:0000:0000)",      true  },
    { "(IP::1)",                                          true  },
    { "(IP0000:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(IP0000:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(IPfe80::8edc:d4ff:fe3a:ebfb)",                    true  },
    { "(IPfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb)",      true  },
    { "(IPff01::1)",                                      true  },
    { "(IPff01:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(IPff01:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(IPfd00:0:1:1::1)",                                true  },
    { "(IPfd00:0000:1:1::1)",                             true  },
    { "(IPfd00:0000:0001:1::1)",                          true  },
    { "(IPfd00:0000:0001:0001::1)",                       true  },
    { "(IPfd00:0000:0001:0001:0000:0000:0000:1)",         true  },
    { "(IPfd00:0000:0001:0001:0000:0000:0000:0001)",      true  },
    { "(IPfd12:3456:1:abcd:abcd:ef00:fedc:ba09)",         true  },
    { "(IPfdff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(IPff01::1)",                                      true  },
    { "(IPff02::1)",                                      true  },
    { "(IPff03::1)",                                      true  },
    { "(IPff04::1)",                                      true  },
    { "(IPff05::1)",                                      true  },
    { "(IPff08::1)",                                      true  },
    { "(IPff0e::1)",                                      true  },
    { "(IPff01::2)",                                      true  },
    { "(IPff02::2)",                                      true  },
    { "(IPff03::2)",                                      true  },
    { "(IPff04::2)",                                      true  },
    { "(IPff05::2)",                                      true  },
    { "(IPff08::2)",                                      true  },
    { "(IPff0e::2)",                                      true  },
    { "(IPff11::1)",                                      true  },
    { "(IPff12::1)",                                      true  },
    { "(IPff13::1)",                                      true  },
    { "(IPff14::1)",                                      true  },
    { "(IPff15::1)",                                      true  },
    { "(IPff18::1)",                                      true  },
    { "(IPff1e::1)",                                      true  },
    { "(IPff11:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff12:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff13:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff14:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff15:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff18:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff1e:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(IPff31:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff32:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff33:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff34:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff35:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff38:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff3e:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(IPff31:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff32:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff33:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff34:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff35:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff38:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff3e:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(IPff31:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff32:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff33:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff34:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff35:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff38:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff3e:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(IPff31:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff32:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff33:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff34:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff35:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff38:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPff3e:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(IPffff::)",                                       true  },
    { "(IPffff:ffff::)",                                  true  },
    { "(IPffff:ffff:ffff::)",                             true  },
    { "(IPffff:ffff:ffff:ffff::)",                        true  },
    { "(IPffff:ffff:ffff:ffff:ffff::)",                   true  },
    { "(IPffff:ffff:ffff:ffff:ffff:ffff::)",              true  },
    { "(IPffff:ffff:ffff:ffff:ffff:ffff:ffff::)",         true  },
    { "(IPffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(IP2620:1:10e7:400:e83f:b28f:9c3a:1941)",          true  },
    { "(IP2620:0001:10e7:400:e83f:b28f:9c3a:1941)",       true  },
    { "(IP2620:0001:10e7:0400:e83f:b28f:9c3a:1941)",      true  },

    // IPv6 Dual Address Positive Test Cases

    { "(IP::11.22.33.44)",                                 true  },
    { "(IP0000:0000:0000:0000:0000:0000:11.22.33.44)",     true  },
    { "(IP::1234:5678:1.2.3.4)",                           true  },
    { "(IP0000:0000:0000:0000:1234:5678:1.2.3.4)",         true  },
    { "(IP::1234:5678:91.123.4.56)",                       true  },
    { "(IP0000:0000:0000:0000:1234:5678:91.123.4.56)",     true  },
    { "(IP2001:db8:3333:4444:5555:6666:1.2.3.4)",          true  },
    { "(IP2001:0db8:3333:4444:5555:6666:1.2.3.4)",         true  },
    { "(IP2001:db8::123.123.123.123)",                     true  },
    { "(IP2001:0db8:0000:0000:0000:0000:123.123.123.123)", true  },
    { "(IP2001:db8::1234:5678:5.6.7.8)",                   true  },
    { "(IP2001:0db8:0000:0000:1234:5678:5.6.7.8)",         true  },

    // Negative IPv6 Test Cases

    { "[IP::]",                                            false },
    { "[IP::1]",                                           false },
    { "[IPfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb]",       false },

    { "(IPfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb:54ce)",  false },

    { "(IPge80::1)",                                       false },

    // IP6 Dual Address Negative Test Cases

    { "(IP::256.300.987.1)",                               false },
    { "(IP::1234:5678:256.300.987.1)",                     false },
    { "(IP::1234:5678:256.300.987.1)",                     false },
    { "(IP2001:db8:3333:4444:5555:6666:256.300.987.1)",    false },
    { "(IP2001:db8::256.300.987.1)",                       false },
    { "(IP2001:db8::256.300.987.1)",                       false },
};

static TestCase sIPNetmaskResponseVector[] = {
    // IPv4 Test Cases

    // Positive IPv4 Test Cases

    { "(NM0.0.0.0)",         true  },
    { "(NM127.0.0.1)",       true  },
    { "(NM1.2.3.4)",         true  },
    { "(NM1.102.103.104)",   true  },
    { "(NM8.8.8.8)",         true  },
    { "(NM192.168.1.72)",    true  },
    { "(NM255.255.255.0)",   true  },
    { "(NM224.0.0.0)",       true  },
    { "(NM224.0.0.1)",       true  },
    { "(NM224.0.0.128)",     true  },
    { "(NM224.0.0.254)",     true  },
    { "(NM224.0.0.255)",     true  },
    { "(NM224.0.1.0)",       true  },
    { "(NM224.0.1.1)",       true  },
    { "(NM224.0.1.128)",     true  },
    { "(NM224.0.1.254)",     true  },
    { "(NM224.0.1.255)",     true  },
    { "(NM224.0.2.0)",       true  },
    { "(NM224.0.2.1)",       true  },
    { "(NM224.0.129.0)",     true  },
    { "(NM224.0.255.254)",   true  },
    { "(NM224.0.255.255)",   true  },
    { "(NM224.3.0.0)",       true  },
    { "(NM224.3.0.1)",       true  },
    { "(NM224.4.0.0)",       true  },
    { "(NM224.4.255.254)",   true  },
    { "(NM224.4.255.255)",   true  },
    { "(NM232.0.0.0)",       true  },
    { "(NM232.0.0.1)",       true  },
    { "(NM232.128.0.0)",     true  },
    { "(NM232.255.255.254)", true  },
    { "(NM232.255.255.255)", true  },
    { "(NM233.0.0.0)",       true  },
    { "(NM233.0.0.1)",       true  },
    { "(NM233.126.0.0)",     true  },
    { "(NM233.251.255.254)", true  },
    { "(NM233.251.255.255)", true  },
    { "(NM233.252.0.0)",     true  },
    { "(NM233.252.0.1)",     true  },
    { "(NM233.254.0.0)",     true  },
    { "(NM233.255.255.254)", true  },
    { "(NM233.255.255.255)", true  },
    { "(NM234.0.0.0)",       true  },
    { "(NM234.0.0.1)",       true  },
    { "(NM234.128.0.0)",     true  },
    { "(NM234.255.255.254)", true  },
    { "(NM234.255.255.255)", true  },
    { "(NM239.0.0.0)",       true  },
    { "(NM239.0.0.1)",       true  },
    { "(NM239.128.0.0)",     true  },
    { "(NM239.255.255.254)", true  },
    { "(NM239.255.255.255)", true  },
    { "(NM255.255.255.255)", true  },

    // Negative IPv4 Test Cases

    { "[NM0.0.0.0]",         false },
    { "[NM127.0.0.1]",       false },
    { "[NM255.255.255.0]",   false },
    { "(NM256.300.987.1)",   false },

    // IPv6 Test Cases

    // Positive IPv6 Test Cases

    { "(NM::)",                                           true  },
    { "(NM0000:0000:0000:0000:0000:0000:0000:0000)",      true  },
    { "(NM::1)",                                          true  },
    { "(NM0000:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(NM0000:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(NMfe80::8edc:d4ff:fe3a:ebfb)",                    true  },
    { "(NMfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb)",      true  },
    { "(NMff01::1)",                                      true  },
    { "(NMff01:0000:0000:0000:0000:0000:0000:1)",         true  },
    { "(NMff01:0000:0000:0000:0000:0000:0000:0001)",      true  },
    { "(NMfd00:0:1:1::1)",                                true  },
    { "(NMfd00:0000:1:1::1)",                             true  },
    { "(NMfd00:0000:0001:1::1)",                          true  },
    { "(NMfd00:0000:0001:0001::1)",                       true  },
    { "(NMfd00:0000:0001:0001:0000:0000:0000:1)",         true  },
    { "(NMfd00:0000:0001:0001:0000:0000:0000:0001)",      true  },
    { "(NMfd12:3456:1:abcd:abcd:ef00:fedc:ba09)",         true  },
    { "(NMfdff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(NMff01::1)",                                      true  },
    { "(NMff02::1)",                                      true  },
    { "(NMff03::1)",                                      true  },
    { "(NMff04::1)",                                      true  },
    { "(NMff05::1)",                                      true  },
    { "(NMff08::1)",                                      true  },
    { "(NMff0e::1)",                                      true  },
    { "(NMff01::2)",                                      true  },
    { "(NMff02::2)",                                      true  },
    { "(NMff03::2)",                                      true  },
    { "(NMff04::2)",                                      true  },
    { "(NMff05::2)",                                      true  },
    { "(NMff08::2)",                                      true  },
    { "(NMff0e::2)",                                      true  },
    { "(NMff11::1)",                                      true  },
    { "(NMff12::1)",                                      true  },
    { "(NMff13::1)",                                      true  },
    { "(NMff14::1)",                                      true  },
    { "(NMff15::1)",                                      true  },
    { "(NMff18::1)",                                      true  },
    { "(NMff1e::1)",                                      true  },
    { "(NMff11:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff12:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff13:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff14:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff15:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff18:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff1e:d5d6:2ba2:7847:6452:587a:c955:b5a)",       true  },
    { "(NMff31:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff32:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff33:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff34:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff35:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff38:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff3e:38:373a:cba4:d2ad:8d00:1:1)",              true  },
    { "(NMff31:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff32:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff33:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff34:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff35:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff38:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff3e:38:373a:cba4:d2ad:8d00:afff:5258)",        true  },
    { "(NMff31:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff32:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff33:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff34:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff35:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff38:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff3e:40:6664:3dfb:afa4:385b:1:1)",              true  },
    { "(NMff31:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff32:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff33:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff34:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff35:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff38:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMff3e:40:6664:3dfb:afa4:385b:afff:5258)",        true  },
    { "(NMffff::)",                                       true  },
    { "(NMffff:ffff::)",                                  true  },
    { "(NMffff:ffff:ffff::)",                             true  },
    { "(NMffff:ffff:ffff:ffff::)",                        true  },
    { "(NMffff:ffff:ffff:ffff:ffff::)",                   true  },
    { "(NMffff:ffff:ffff:ffff:ffff:ffff::)",              true  },
    { "(NMffff:ffff:ffff:ffff:ffff:ffff:ffff::)",         true  },
    { "(NMffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)",      true  },
    { "(NM2620:1:10e7:400:e83f:b28f:9c3a:1941)",          true  },
    { "(NM2620:0001:10e7:400:e83f:b28f:9c3a:1941)",       true  },
    { "(NM2620:0001:10e7:0400:e83f:b28f:9c3a:1941)",      true  },

    // IPv6 Dual Address Positive Test Cases

    { "(NM::11.22.33.44)",                                 true  },
    { "(NM0000:0000:0000:0000:0000:0000:11.22.33.44)",     true  },
    { "(NM::1234:5678:1.2.3.4)",                           true  },
    { "(NM0000:0000:0000:0000:1234:5678:1.2.3.4)",         true  },
    { "(NM::1234:5678:91.123.4.56)",                       true  },
    { "(NM0000:0000:0000:0000:1234:5678:91.123.4.56)",     true  },
    { "(NM2001:db8:3333:4444:5555:6666:1.2.3.4)",          true  },
    { "(NM2001:0db8:3333:4444:5555:6666:1.2.3.4)",         true  },
    { "(NM2001:db8::123.123.123.123)",                     true  },
    { "(NM2001:0db8:0000:0000:0000:0000:123.123.123.123)", true  },
    { "(NM2001:db8::1234:5678:5.6.7.8)",                   true  },
    { "(NM2001:0db8:0000:0000:1234:5678:5.6.7.8)",         true  },

    // Negative IPv6 Test Cases

    { "[NM::]",                                            false },
    { "[NM::1]",                                           false },
    { "[NMfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb]",       false },

    { "(NMfe80:0000:0000:0000:8edc:d4ff:fe3a:ebfb:54ce)",  false },

    { "(NMge80::1)",                                       false },

    // IP6 Dual Address Negative Test Cases

    { "(NM::256.300.987.1)",                               false },
    { "(NM::1234:5678:256.300.987.1)",                     false },
    { "(NM::1234:5678:256.300.987.1)",                     false },
    { "(NM2001:db8:3333:4444:5555:6666:256.300.987.1)",    false },
    { "(NM2001:db8::256.300.987.1)",                       false },
    { "(NM2001:db8::256.300.987.1)",                       false },
};

static TestCase sQueryResponseVector[] = {
    { "(QE)",  true  },

    { "(QF)",  false },
    { "(QD)",  false },
    { "[QE]",  false }
};

static TestCase sSDDPEnabledResponseVector[] = {
    { "(SDDP0)",  true  },
    { "(SDDP1)",  true  },

    { "(SDDP-1)", false },
    { "(SDDP2)",  false },
    { "[SDDP0]",  false },
    { "[SDDP1]",  false },
    { "[SDDP-1]", false },
    { "[SDDP2]",  false }
};

static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    DHCPv4EnabledResponse           lDHCPv4EnabledResponse;
    EthernetEUI48Response           lEthernetEUI48Response;
    IPDefaultRouterAddressResponse  lIPDefaultRouterAddressResponse;
    IPHostAddressResponse           lIPHostAddressResponse;
    IPNetmaskResponse               lIPNetmaskResponse;
    QueryRequest                    lQueryRequest;
    QueryResponse                   lQueryResponse;
    Query                           lQuery;
    SDDPEnabledResponse             lSDDPEnabledResponse;
}

static void TestInitialization(nlTestSuite *inSuite,
                               void *inContext __attribute__((unused)))
{
    DHCPv4EnabledResponse           lDHCPv4EnabledResponse;
    EthernetEUI48Response           lEthernetEUI48Response;
    IPDefaultRouterAddressResponse  lIPDefaultRouterAddressResponse;
    IPHostAddressResponse           lIPHostAddressResponse;
    IPNetmaskResponse               lIPNetmaskResponse;
    QueryRequest                    lQueryRequest;
    QueryResponse                   lQueryResponse;
    Query                           lQuery;
    SDDPEnabledResponse             lSDDPEnabledResponse;
    Status                          lStatus;


    lStatus = lDHCPv4EnabledResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lEthernetEUI48Response.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIPDefaultRouterAddressResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIPHostAddressResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lIPNetmaskResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lQueryRequest.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lQueryResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lQuery.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSDDPEnabledResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestResponse(nlTestSuite *inSuite,
                         ResponseBasis &aResponse,
                         const TestCase *aTestCase,
                         const size_t &aTestCaseCount)
{
    bool   lDidMatch;
    Status lStatus;

    for (size_t i = 0; i < aTestCaseCount; i++)
    {
        const RegularExpression &    lRegularExpression    = aResponse.GetRegularExpression();
        RegularExpression::Matches & lMatches              = aResponse.GetMatches();
        const char *                 lResponseString       = aTestCase[i].mResponseString;
        const size_t                 lResponseStringLength = strlen(lResponseString);
        const bool &                 lShouldMatch          = aTestCase[i].mMatchExpected;

        // Test with null-termination.

        lStatus = lRegularExpression.Match(lResponseString, lMatches);

        lDidMatch = (lStatus == 0);
        NL_TEST_ASSERT(inSuite, lDidMatch == lShouldMatch);

        if (lDidMatch != lShouldMatch)
        {
            Log::Error().Write("FAILURE: Tried matching '%s' and did%s expect it to match.\n", lResponseString, (lShouldMatch ? "" : " NOT"));
        }
        else
        {
            // Test without null-termination

            lStatus = lRegularExpression.Match(lResponseString, lResponseStringLength, lMatches);

            lDidMatch = (lStatus == 0);
            NL_TEST_ASSERT(inSuite, lDidMatch == lShouldMatch);

            if (lDidMatch != lShouldMatch)
            {
                Log::Error().Write("FAILURE: Tried matching '%s' and did%s expect it to match.\n", lResponseString, (lShouldMatch ? "" : " NOT"));
            }
        }
    }
}

static void TestResponses(nlTestSuite *inSuite,
                          void *inContext __attribute__((unused)))
{
    DHCPv4EnabledResponse           lDHCPv4EnabledResponse;
    EthernetEUI48Response           lEthernetEUI48Response;
    IPDefaultRouterAddressResponse  lIPDefaultRouterAddressResponse;
    IPHostAddressResponse           lIPHostAddressResponse;
    IPNetmaskResponse               lIPNetmaskResponse;
    QueryResponse                   lQueryResponse;
    SDDPEnabledResponse             lSDDPEnabledResponse;
    Status                          lStatus;

    lStatus = lDHCPv4EnabledResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lDHCPv4EnabledResponse, sDHCPv4EnabledResponseVector, ElementsOf(sDHCPv4EnabledResponseVector));

    lStatus = lEthernetEUI48Response.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lEthernetEUI48Response, sEthernetEUI48ResponseVector, ElementsOf(sEthernetEUI48ResponseVector));

    lStatus = lIPDefaultRouterAddressResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lIPDefaultRouterAddressResponse, sIPDefaultRouterAddressResponseVector, ElementsOf(sIPDefaultRouterAddressResponseVector));

    lStatus = lIPHostAddressResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lIPHostAddressResponse, sIPHostAddressResponseVector, ElementsOf(sIPHostAddressResponseVector));

    lStatus = lIPNetmaskResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lIPNetmaskResponse, sIPNetmaskResponseVector, ElementsOf(sIPNetmaskResponseVector));

    lStatus = lQueryResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lQueryResponse, sQueryResponseVector, ElementsOf(sQueryResponseVector));

    lStatus = lSDDPEnabledResponse.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestResponse(inSuite, lSDDPEnabledResponse, sSDDPEnabledResponseVector, ElementsOf(sSDDPEnabledResponseVector));
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Responses",      TestResponses),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Network Controller Commands",
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
