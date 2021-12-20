/*
 *    Copyright (c) 2018-2021 Grant Erickson
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

/**
 *  The Ethernet network interface DHCPv4 enabled state regular
 *  expression pattern string.
 *
 */
const char * const DHCPv4EnabledRegularExpressionBasis::kRegexp   = "DHCP([01])";

/**
 *  The Ethernet network interface EUI-48 address regular expression
 *  pattern string.
 *
 */
const char * const EthernetAddressRegularExpressionBasis::kRegexp = "MAC(([[:xdigit:]]{2}-){5}[[:xdigit:]]{2})";

/**
 *  The Ethernet network interface query command regular expression
 *  pattern string.
 *
 */
const char * const QueryRegularExpressionBasis::kRegexp           = "QE";

/**
 *  The Ethernet network interface Control4 SDDP enabled state regular
 *  expression pattern string.
 *
 */
const char * const SDDPEnabledRegularExpressionBasis::kRegexp     = "SDDP([01])";

/**
 *  The Ethernet network interface DHCPv4 enabled state regular
 *  expression pattern expected substring matches.
 *
 */
const size_t DHCPv4EnabledRegularExpressionBasis::kExpectedMatches   = 2;

/**
 *  The Ethernet network interface EUI-48 address regular expression
 *  pattern expected substring matches.
 *
 */
const size_t EthernetAddressRegularExpressionBasis::kExpectedMatches = 2;

/**
 *  The Ethernet network interface query command regular expression
 *  pattern expected substring matches.
 *
 */
const size_t QueryRegularExpressionBasis::kExpectedMatches           = 1;

/**
 *  The Ethernet network interface Control4 SDDP enabled state regular
 *  expression pattern expected substring matches.
 *
 */
const size_t SDDPEnabledRegularExpressionBasis::kExpectedMatches     = 2;

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
EthernetAddressRegularExpressionBasis :: Init(RegularExpressionBasis &aRegularExpression)
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
