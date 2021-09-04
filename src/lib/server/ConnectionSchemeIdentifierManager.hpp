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
 *      This file defines an object for vending and managing
 *      connection scheme (for example, 'telnet') identifiers.
 *
 */

#ifndef OPENHLXSERVERCONNECTIONSCHEMEIDENTIFIERMANAGER_HPP
#define OPENHLXSERVERCONNECTIONSCHEMEIDENTIFIERMANAGER_HPP


#include <limits>
#include <map>
#include <set>
#include <string>

#include <OpenHLX/Server/ConnectionBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for for vending and managing connection scheme (for
 *    example, 'telnet') identifiers.
 *
 *  This HLX protocol has a server-to-client connection confirmation
 *  of the form '<scheme>_client_<identifier>: connected' that occurs
 *  after the TCP connection but before the server accepts any command
 *  requests from the client. This object vends and manages those
 *  identifiers that connections use to format and send such
 *  confirmations.
 *
 */
class ConnectionSchemeIdentifierManager
{
public:
    typedef ConnectionBasis::IdentifierType IdentifierType;

public:
    static constexpr IdentifierType kInvalidIdentifier = std::numeric_limits<IdentifierType>::min();

public:
    // Con/destructor

    ConnectionSchemeIdentifierManager(void) = default;
    ~ConnectionSchemeIdentifierManager(void) = default;

    // Observers

    bool IsSchemeIdentifierClaimed(const std::string &aScheme, const IdentifierType &aIdentifier) const;

    // Mutators

    IdentifierType ClaimSchemeIdentifier(const std::string &aScheme);
    bool ReleaseSchemeIdentifier(const std::string &aScheme, const IdentifierType &aIdentifier);

private:
    typedef std::set<IdentifierType> IdentifierMap;

    struct IdentifierMapState
    {
        IdentifierMap   mIdentifierMap;
        IdentifierType  mNextIdentifier;
    };

    typedef std::map<std::string, IdentifierMapState> SchemeIdentifierMap;

    SchemeIdentifierMap mSchemeIdentifierMap;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONNECTIONSCHEMEIDENTIFIERMANAGER_HPP
