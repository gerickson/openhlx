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
 *      This file defines base, common (that is, independent of
 *      requestor or responder) command regular expression objects for
 *      HLX Ethernet network interface commands.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDNETWORKREGULAREXPRESSIONBASES_HPP
#define OPENHLXCOMMONCOMMANDNETWORKREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace Network
{

/**
 *  @brief
 *    Base regular expression object for HLX Ethernet network
 *    interface DHCPv4 enabled state property.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX Ethernet network
 *  interface DHCPv4 enabled state property.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup network
 *
 */
class DHCPv4EnabledRegularExpressionBasis
{
protected:
    DHCPv4EnabledRegularExpressionBasis(void) = default;
    virtual ~DHCPv4EnabledRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX Ethernet network
 *    interface MAC address property.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX Ethernet network
 *  interface MAC address property.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup network
 *
 */
class EthernetAddressRegularExpressionBasis
{
protected:
    EthernetAddressRegularExpressionBasis(void) = default;
    virtual ~EthernetAddressRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX Ethernet network
 *    interface query command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX Ethernet network
 *  interface query command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryRegularExpressionBasis
{
protected:
    QueryRegularExpressionBasis(void) = default;
    virtual ~QueryRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX Ethernet network
 *    interface Control4 SDDP enabled state property.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX Ethernet network
 *  interface Control4 SDDP enabled state property.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup network
 *
 */
class SDDPEnabledRegularExpressionBasis
{
protected:
    SDDPEnabledRegularExpressionBasis(void) = default;
    virtual ~SDDPEnabledRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

}; // namespace Network

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDNETWORKREGULAREXPRESSIONBASES_HPP
