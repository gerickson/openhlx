/*
 *    Copyright (c) 2018-2022 Grant Erickson
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
 *      This file implements base, common (that is, independent of
 *      requestor or responder) command regular expression objects for
 *      HLX Ethernet network interface commands.
 *
 */

#include "CommandNetworkRegularExpressionBases.hpp"

#include <OpenHLX/Common/CommandRegularExpressionBasis.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Network
{

#define kDHCPPropertyRegExp                   "DHCP"
#define kEUI48PropertyRegExp                  "MAC"
#define kIPDefaultRouterAddressPropertyRegExp "GW"
#define kIPHostAddressPropertyRegExp          "IP"
#define kIPNetmaskPropertyRegExp              "NM"
#define kControl4SDDPPropertyRegExp           "SDDP"

#define kBooleanRegExp          "([01])"
#define kColonRegExp            ":"
#define kDecOctetRegExp         "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
#define kDoubleColonRegExp      kColonRegExp kColonRegExp
#define kEUI48RegExp            "(([[:xdigit:]]{2}-){5}[[:xdigit:]]{2})"
#define kFullStopRegExp         "\\."
#define kIPv4AddressRegExp      "((" kDecOctetRegExp kFullStopRegExp "){3}" kDecOctetRegExp ")"
#define kIPv6AddressH16RegExp   "[[:xdigit:]]{1,4}"
#define kIPv6AddressLS32RegExp  "(" kIPv6AddressH16RegExp kColonRegExp kIPv6AddressH16RegExp "|" kIPv4AddressRegExp ")"
#define kIPv6AddressRegExp      \
    "("                                                                                                                                                   \
        "("                                                                                                "(" kIPv6AddressH16RegExp kColonRegExp "){6}|" \
                                                                                        kDoubleColonRegExp "(" kIPv6AddressH16RegExp kColonRegExp "){5}|" \
            "("  kIPv6AddressH16RegExp ")?"                                             kDoubleColonRegExp "(" kIPv6AddressH16RegExp kColonRegExp "){4}|" \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,1}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp "(" kIPv6AddressH16RegExp kColonRegExp "){3}|" \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,2}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp "(" kIPv6AddressH16RegExp kColonRegExp "){2}|" \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,3}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp "(" kIPv6AddressH16RegExp kColonRegExp ")   |" \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,4}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp                                                \
        ")(" kIPv6AddressLS32RegExp ")|"                                                                                                                  \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,5}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp "(" kIPv6AddressH16RegExp ")|"                 \
            "((" kIPv6AddressH16RegExp kColonRegExp "){0,6}" kIPv6AddressH16RegExp ")?" kDoubleColonRegExp                                                \
    ")"
#define kIPAddressRegExp              "(" kIPv4AddressRegExp "|" kIPv6AddressRegExp ")"

/**
 *  The Ethernet network interface DHCPv4 enabled state regular
 *  expression pattern string.
 *
 */
const char * const DHCPv4EnabledRegularExpressionBasis::kRegexp          = kDHCPPropertyRegExp kBooleanRegExp;

/**
 *  The Ethernet network interface EUI-48 address regular expression
 *  pattern string.
 *
 */
const char * const EthernetEUI48RegularExpressionBasis::kRegexp          = kEUI48PropertyRegExp kEUI48RegExp;

/**
 *  The Ethernet network interface default router IP address regular
 *  expression pattern string.
 *
 */
const char * const IPDefaultRouterAddressRegularExpressionBasis::kRegexp = kIPDefaultRouterAddressPropertyRegExp kIPAddressRegExp;

/**
 *  The Ethernet network interface host address IP regular expression
 *  pattern string.
 *
 */
const char * const IPHostAddressRegularExpressionBasis::kRegexp          = kIPHostAddressPropertyRegExp kIPAddressRegExp;

/**
 *  The Ethernet network interface IP netmask regular expression
 *  pattern string.
 *
 */
const char * const IPNetmaskRegularExpressionBasis::kRegexp              = kIPNetmaskPropertyRegExp kIPAddressRegExp;

/**
 *  The Ethernet network interface query command regular expression
 *  pattern string.
 *
 */
const char * const QueryRegularExpressionBasis::kRegexp                  = "QE";

/**
 *  The Ethernet network interface Control4 SDDP enabled state regular
 *  expression pattern string.
 *
 */
const char * const SDDPEnabledRegularExpressionBasis::kRegexp            = kControl4SDDPPropertyRegExp kBooleanRegExp;

/**
 *  The Ethernet network interface DHCPv4 enabled state regular
 *  expression pattern expected substring matches.
 *
 */
const size_t DHCPv4EnabledRegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The Ethernet network interface EUI-48 address regular expression
 *  pattern expected substring matches.
 *
 */
const size_t EthernetEUI48RegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The Ethernet network interface default router IP address regular
 *  expression pattern expected substring matches.
 *
 */
const size_t IPDefaultRouterAddressRegularExpressionBasis::kExpectedMatches = 2;

/**
 *  The Ethernet network interface host IP address regular expression
 *  pattern expected substring matches.
 *
 */
const size_t IPHostAddressRegularExpressionBasis::kExpectedMatches          = 2;

/**
 *  The Ethernet network interface IP netmask regular expression
 *  pattern expected substring matches.
 *
 */
const size_t IPNetmaskRegularExpressionBasis::kExpectedMatches              = 2;

/**
 *  The Ethernet network interface query command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t QueryRegularExpressionBasis::kExpectedMatches                  = 1;

/**
 *  The Ethernet network interface Control4 SDDP enabled state regular
 *  expression pattern expected substring matches.
 *
 */
const size_t SDDPEnabledRegularExpressionBasis::kExpectedMatches            = 2;

/**
 *  @brief
 *    This initializes the Ethernet network interface DHCPv4 enabled
 *    state property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      DHCPv4 enabled property
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DHCPv4EnabledRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface EUI-48 address
 *    property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      EUI-48 address property
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EthernetEUI48RegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface default router
 *    IP address property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      default router IP address
 *                                      property regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPDefaultRouterAddressRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface host IP address
 *    property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      host IP address property
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPHostAddressRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface IP netmask
 *    property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface IP
 *                                      netmask property regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IPNetmaskRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface query command
 *    regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      query command regular
 *                                      expression to initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

/**
 *  @brief
 *    This initializes the Ethernet network interface Control4 SDDP
 *    enabled state property regular expression.
 *
 *  @param[in,out]  aRegularExpression  A mutable reference to the
 *                                      Ethernet network interface
 *                                      Control4 SDDP enabled property
 *                                      regular expression to
 *                                      initialize.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SDDPEnabledRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
{
    return (aRegularExpression.Init(kRegexp, kExpectedMatches));
}

}; // namespace Network

}; // namespace Command

}; // namespace Common

}; // namespace HLX
