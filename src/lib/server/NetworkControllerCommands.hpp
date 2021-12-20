/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines objects for HLX server network data model
 *      commands and their constituent requests and responses.
 *
 */

#ifndef OPENHLXSERVERNETWORKCONTROLLERCOMMANDS_HPP
#define OPENHLXSERVERNETWORKCONTROLLERCOMMANDS_HPP

#include <OpenHLX/Common/CommandNetworkBufferBases.hpp>
#include <OpenHLX/Common/CommandNetworkRegularExpressionBases.hpp>
#include <OpenHLX/Common/Errors.hpp>

#include "CommandQueryResponseBasis.hpp"
#include "CommandRequestBasis.hpp"
#include "CommandResponseBasis.hpp"


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Network
{

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server network data model query command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryRequest :
    public RequestBasis,
    public Common::Command::Network::QueryRegularExpressionBasis
{
public:
    QueryRequest(void) = default;
    virtual ~QueryRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server network data model query command
 *    response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class QueryResponse :
    public QueryResponseBasis
{
public:
    QueryResponse(void) = default;
    virtual ~QueryResponse(void) = default;

    // Allow both the base and derived class initializers

    using QueryResponseBasis::Init;

    Common::Status Init(void);
};

// MARK: Network Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    A object for a HLX server Ethernet network interface DHCPv4
 *    enabled data model property mutation command request regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class SetDHCPv4EnabledRequest :
    public RequestBasis,
    public Common::Command::Network::DHCPv4EnabledRegularExpressionBasis
{
public:
    SetDHCPv4EnabledRequest(void) = default;
    virtual ~SetDHCPv4EnabledRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server Ethernet network interface DHCPv4
 *    enabled data model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class DHCPv4EnabledResponse :
    public ResponseBasis,
    public Common::Command::Network::DHCPv4EnabledBufferBasis
{
public:
    DHCPv4EnabledResponse(void) = default;
    virtual ~DHCPv4EnabledResponse(void) = default;

    Common::Status Init(const Model::NetworkModel::EnabledType &aEnabled);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server Ethernet network interface EUI-48
 *    address data model property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class EthernetEUI48Response :
    public ResponseBasis,
    public Common::Command::Network::EthernetEUI48BufferBasis
{
public:
    EthernetEUI48Response(void) = default;
    virtual ~EthernetEUI48Response(void) = default;

    Common::Status Init(const Model::NetworkModel::EthernetEUI48Type &aEthernetEUI48);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server Ethernet network interface Control4
 *    SDDP enabled data model property mutation command request
 *    regular expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class SetSDDPEnabledRequest :
    public RequestBasis,
    public Common::Command::Network::SDDPEnabledRegularExpressionBasis
{
public:
    SetSDDPEnabledRequest(void) = default;
    virtual ~SetSDDPEnabledRequest(void) = default;

    Common::Status Init(void);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A object for a HLX server Ethernet network interface Control4
 *    SDDP enabled data model property mutation command response
 *    buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup network
 *
 */
class SDDPEnabledResponse :
    public ResponseBasis,
    public Common::Command::Network::SDDPEnabledBufferBasis
{
public:
    SDDPEnabledResponse(void) = default;
    virtual ~SDDPEnabledResponse(void) = default;

    Common::Status Init(const Model::NetworkModel::EnabledType &aEnabled);

private:
    // Explicitly hide base class initializers

    using ResponseBasis::Init;
};

}; // namespace Network

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERNETWORKCONTROLLERCOMMANDS_HPP
