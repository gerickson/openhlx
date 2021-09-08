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
 *      This file implements an object for vending and managing
 *      connection scheme (for example, 'telnet') identifiers.
 *
 */

#include "ConnectionSchemeIdentifierManager.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace std;


namespace HLX
{

namespace Server
{

static constexpr ConnectionSchemeIdentifierManager::IdentifierType kFirstIdentifier = 1;

// MARK: Observers

/**
 *  @brief
 *    Determine whether an identifier associated with a connection
 *    scheme is claimed.
 *
 *  @param[in]  aScheme      An immutable reference to the connection
 *                           scheme (for example, 'telnet') associated
 *                           with the identifier, @a aIdentifier.
 *  @param[in]  aIdentifier  An immutable reference to the identifier
 *                           to check.
 *
 *  @returns
 *    True if the identifier is claimed for the specified scheme;
 *    otherwise, false.
 *
 *  @sa ClaimSchemeIdentifier
 *
 */
bool
ConnectionSchemeIdentifierManager :: IsSchemeIdentifierClaimed(const std::string &aScheme, const IdentifierType &aIdentifier) const
{
    SchemeIdentifierMap::const_iterator  lIdentifiers;
    IdentifierMap::const_iterator        lIdentifier;
    bool                                 lRetval = false;


    nlREQUIRE(aIdentifier != kInvalidIdentifier, done);
    nlREQUIRE(!aScheme.empty(), done);

    // Attempt to find the identifier map for the specified scheme.

    lIdentifiers = mSchemeIdentifierMap.find(aScheme);
    nlEXPECT(lIdentifiers != mSchemeIdentifierMap.end(), done);

    // Attempt to find the identifer in the scheme-specific identifier
    // map.

    lIdentifier = lIdentifiers->second.mIdentifierMap.find(aIdentifier);

    // If we have a valid iterator result, it was found.

    lRetval = (lIdentifier != lIdentifiers->second.mIdentifierMap.end());

 done:
    return (lRetval);
}

// MARK: Mutators

/**
 *  @brief
 *    Request and claim a connection identifier associated with a
 *    connection scheme.
 *
 *  @param[in]  aScheme      An immutable reference to the connection
 *                           scheme (for example, 'telnet') for which
 *                           an identifier is being requested and
 *                           claimed.
 *
 *  @returns
 *    A connection identifier associated with the specified scheme on
 *    success; otherwise, #kInvalidIdentifier.
 *
 *  @sa IsSchemeIdentifierClaimed
 *  @sa ReleaseSchemeIdentifier
 *
 */
ConnectionSchemeIdentifierManager::IdentifierType
ConnectionSchemeIdentifierManager :: ClaimSchemeIdentifier(const std::string &aScheme)
{
    SchemeIdentifierMap::iterator  lIdentifiers;
    IdentifierMapState *           lIdentifierMapState;
    IdentifierType                 lRetval = kInvalidIdentifier;


    nlREQUIRE(!aScheme.empty(), done);

    lIdentifiers = mSchemeIdentifierMap.find(aScheme);

    // If the scheme could not be found, this is the first time an
    // identifier has been requested for this scheme and we start with
    // the first identifier.
    //
    // Otherwise, we have satisfied a prior claim for this scheme and
    // can simply assign the next identifier.

    if (lIdentifiers == mSchemeIdentifierMap.end())
    {
        lIdentifierMapState = &mSchemeIdentifierMap[aScheme];

        lRetval = kFirstIdentifier;
    }
    else
    {
        lIdentifierMapState = &lIdentifiers->second;

        lRetval = lIdentifiers->second.mNextIdentifier;
    }

    {
        const auto lResult = lIdentifierMapState->mIdentifierMap.insert(lRetval);

        if (lResult.second)
        {
            lIdentifierMapState->mNextIdentifier = lRetval + 1;

            while (lIdentifierMapState->mIdentifierMap.find(lIdentifierMapState->mNextIdentifier) != lIdentifierMapState->mIdentifierMap.end())
            {
                lIdentifierMapState->mNextIdentifier++;
            }
        }
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Relinquish a connection identifier associated with a connection
 *    scheme.
 *
 *  @param[in]  aScheme      An immutable reference to the connection
 *                           scheme (for example, 'telnet') for which
 *                           the identifier is being relinquished.
 *  @param[in]  aIdentifier  An immutable reference to the identifier
 *                           to relinquish.
 *
 *  @returns
 *    True if the identifier was successfully relinquished; otherwise,
 *    false.
 *
 *  @sa IsSchemeIdentifierClaimed
 *  @sa ClaimSchemeIdentifier
 *
 */
bool
ConnectionSchemeIdentifierManager :: ReleaseSchemeIdentifier(const std::string &aScheme, const IdentifierType &aIdentifier)
{
    SchemeIdentifierMap::iterator  lIdentifiers;
    IdentifierMap::const_iterator  lIdentifier;
    bool                           lRetval = false;


    nlREQUIRE(aIdentifier != kInvalidIdentifier, done);
    nlREQUIRE(!aScheme.empty(), done);

    // Attempt to find the identifier map for the specified scheme.

    lIdentifiers = mSchemeIdentifierMap.find(aScheme);
    nlREQUIRE(lIdentifiers != mSchemeIdentifierMap.end(), done);

    // Attempt to find the identifer in the scheme-specific identifier
    // map.

    lIdentifier = lIdentifiers->second.mIdentifierMap.find(aIdentifier);

    lRetval = (lIdentifier != lIdentifiers->second.mIdentifierMap.end());
    nlREQUIRE(lRetval == true, done);

    // Remove the identifier

    lIdentifiers->second.mIdentifierMap.erase(lIdentifier);

    // If an identifer was just released that is less that the present
    // next identifier, update the next identifier with the one just
    // released.

    if (aIdentifier < lIdentifiers->second.mNextIdentifier)
    {
        lIdentifiers->second.mNextIdentifier = aIdentifier;
    }

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
