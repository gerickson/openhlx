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
 *      This file defines a base, derivable object for managing
 *      peer-to-peer connections between a HLX client and server.
 *
 */

#ifndef OPENHLXCOMMONCONNECTIONMANAGERBASIS_HPP
#define OPENHLXCOMMONCONNECTIONMANAGERBASIS_HPP

#include <string>
#include <vector>

#include <stdint.h>

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/IPAddress.hpp>


namespace HLX
{

namespace Common
{

class ConnectionBasis;
class ConnectionManagerApplicationDataDelegate;
class RunLoopParameters;

/**
 *  @brief
 *    A base, derivable object for managing peer-to-peer connections
 *    between a HLX client and server.
 *
 *  @ingroup common
 *
 */
class ConnectionManagerBasis
{
public:
    /**
     *  Possible role(s) to act in. Use with #Roles.
     *
     *  @sa #Roles
     *
     */
    enum Role
    {
        kRoleNone   = 0x00, //!< Act in no role.
        kRoleClient = 0x01, //!< Act in a client or initiator role.
        kRoleServer = 0x02  //!< Act in a server or responder role.
    };

    /**
     *  Type for indicating which role to act in.
     *
     *  This is particularly use for disambiguating common delegation
     *  methods.
     *
     *  @sa #Role
     *
     */
    typedef uint8_t Roles;

    /**
     *  Possible IP address versions to use when resolving host names
     *  to IP addresses. Use with #Versions.
     *
     *  @sa #Versions
     *
     */
    enum Version
    {
        kIPv4 = 0x01, //!< Use IPv4 addresses
        kIPv6 = 0x02  //!< Use IPv6 addresses
    };

    /**
     *  Type for indicating which IP address versions to use when
     *  resolving host names to IP addresses.
     *
     *  @sa #Version
     *
     */
    typedef uint8_t Versions;

public:
    virtual ~ConnectionManagerBasis(void) = default;

    Common::Status Init(const Roles &aRoles,
                        const Common::RunLoopParameters &aRunLoopParameters);

    const Roles & GetRoles(void) const;

    virtual bool SupportsScheme(CFStringRef aSchemeRef) const = 0;

    ConnectionManagerApplicationDataDelegate *GetApplicationDataDelegate(void) const;
    Common::Status SetApplicationDataDelegate(ConnectionManagerApplicationDataDelegate *aDelegate);

protected:
    ConnectionManagerBasis(void);

    /**
     *  A collection of IP addresses.
     *
     */
    typedef std::vector<Common::IPAddress> IPAddresses;

    Common::Status Resolve(const char *aMaybeHost, const Versions &aVersions, IPAddresses &aOutIPAddresses);

    static Status ParseHostOrHostAndPort(const char *aMaybeHost,
                                         std::string &aOutHost,
                                         int32_t &aOutPort);
    static Status ParseURL(const char *aMaybeURL, CFURLRef &aOutURL);

    /**
     *  @brief
     *    Trigger delegation that a host name will resolve.
     *
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that will resolve.
     *
     */
    virtual void OnWillResolve(const char *aHost) = 0;

    /**
     *  @brief
     *    Trigger delegation that a host name is resolving.
     *
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that is resolving.
     *
     */
    virtual void OnIsResolving(const char *aHost) = 0;

    /**
     *  @brief
     *    Trigger delegation that a host name has resolved to an IP
     *    address.
     *
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did resolve.
     *  @param[in]  aIPAddress          An immutable reference to an IP
     *                                  address that the host name
     *                                  resolved to.
     *
     */
    virtual void OnDidResolve(const char *aHost, const Common::IPAddress &aIPAddress) = 0;

    /**
     *  @brief
     *    Trigger delegation that a host name did not resolve.
     *
     *  @param[in]  aHost               A pointer to a null-terminated C
     *                                  string containing the host
     *                                  name that did not resolve.
     *  @param[in]  aError              An immutable reference to the error
     *                                  associated with the failed
     *                                  resolution.
     *
     */
    virtual void OnDidNotResolve(const char *aHost, const Common::Error &aError) = 0;

private:
    Roles                                       mRoles;
    ConnectionManagerApplicationDataDelegate *  mApplicationDataDelegate;
};

namespace Utilities
{

extern ConnectionManagerBasis::Versions GetVersions(const bool &aUseIPv6, const bool &aUseIPv4);

}; // namespace Utilities

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCONNECTIONMANAGERBASIS_HPP
