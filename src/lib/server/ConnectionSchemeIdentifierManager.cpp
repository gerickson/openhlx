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
 *      This file defines an object for...
 *
 */

#include "ConnectionSchemeIdentifierManager.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;
using namespace std;


namespace HLX
{

namespace Server
{

static constexpr ConnectionSchemeIdentifierManager::IdentifierType kFirstIdentifier = 1;

// MARK: Observers

bool
ConnectionSchemeIdentifierManager :: IsSchemeIdentifierClaimed(const char *aScheme, const IdentifierType &aIdentifier) const
{
    return (IsSchemeIdentifierClaimed(string(aScheme), aIdentifier));
}

bool
ConnectionSchemeIdentifierManager :: IsSchemeIdentifierClaimed(const std::string &aScheme, const IdentifierType &aIdentifier) const
{
    SchemeIdentifierMap::const_iterator  lIdentifiers;
    IdentifierMap::const_iterator        lIdentifier;
    bool                                 lRetval = false;


    Log::Debug().Write("%s: aScheme '%s' aIdentifier %zu\n",
                       __func__, aScheme.c_str(), aIdentifier);

    nlREQUIRE(aIdentifier != kInvalidIdentifier, done);
    nlREQUIRE(!aScheme.empty(), done);

    // Attempt to find the identifier map for the specified scheme.

    lIdentifiers = mSchemeIdentifierMap.find(aScheme);
    nlEXPECT(lIdentifiers != mSchemeIdentifierMap.end(), done);

    // Attempt to find the identifer in the scheme-specific identifier
    // map.

    lIdentifier = lIdentifiers->second.mIdentifierMap.find(aIdentifier);

    lRetval = (lIdentifier != lIdentifiers->second.mIdentifierMap.end());

 done:
    Log::Debug().Write("%s: lRetval %u\n", __func__, lRetval);

    return (lRetval);
}

// MARK: Mutators

ConnectionSchemeIdentifierManager::IdentifierType
ConnectionSchemeIdentifierManager :: ClaimSchemeIdentifier(const std::string &aScheme)
{
    SchemeIdentifierMap::iterator  lIdentifiers;
    IdentifierType                 lRetval = kInvalidIdentifier;


    Log::Debug().Write("%s: aScheme '%s'\n", __func__, aScheme.c_str());

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
        Log::Debug().Write("%s: First time seeing scheme '%s'\n", __func__, aScheme.c_str());
        constexpr IdentifierType  lIdentifier = kFirstIdentifier;
        const auto                lResult     = mSchemeIdentifierMap[aScheme].mIdentifierMap.insert(lIdentifier);

        if (lResult.second)
        {
            mSchemeIdentifierMap[aScheme].mNextIdentifier = lIdentifier + 1;

            lRetval = lIdentifier;
        }
    }
    else
    {
        Log::Debug().Write("%s: We've seen scheme '%s' before!\n", __func__, aScheme.c_str());
        const IdentifierType  lIdentifier = lIdentifiers->second.mNextIdentifier;
        const auto            lResult     = lIdentifiers->second.mIdentifierMap.insert(lIdentifier);

        if (lResult.second)
        {
            lIdentifiers->second.mNextIdentifier = lIdentifier + 1;

            lRetval = lIdentifier;

            while (lIdentifiers->second.mIdentifierMap.find(lIdentifiers->second.mNextIdentifier) != lIdentifiers->second.mIdentifierMap.end())
            {
                lIdentifiers->second.mNextIdentifier++;
            }
        }

        Log::Debug().Write("%s: the next identifier is %zu\n", __func__, lIdentifiers->second.mNextIdentifier);
    }

    Log::Debug().Write("%s: lRetval %zu\n", __func__, lRetval);

 done:
    return (lRetval);
}

bool
ConnectionSchemeIdentifierManager :: ReleaseSchemeIdentifier(const std::string &aScheme, const IdentifierType &aIdentifier)
{
    SchemeIdentifierMap::iterator  lIdentifiers;
    IdentifierMap::const_iterator  lIdentifier;
    bool                           lRetval = false;


    Log::Debug().Write("%s: aScheme '%s' aIdentifier %zu\n",
                       __func__, aScheme.c_str(), aIdentifier);

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

    Log::Debug().Write("%s: found identifier %zu\n", __func__, *lIdentifier);
    Log::Debug().Write("%s: next identifier is %zu\n", __func__, lIdentifiers->second.mNextIdentifier);

    // Remove the identifier

    lIdentifiers->second.mIdentifierMap.erase(lIdentifier);

    // If an identifer was just released that is less that the present
    // next identifier, update the next identifier with the one just
    // released.

    if (aIdentifier < lIdentifiers->second.mNextIdentifier)
    {
        lIdentifiers->second.mNextIdentifier = aIdentifier;

        printf("%s: next identifier is now %zu\n", __func__, lIdentifiers->second.mNextIdentifier);
    }

    Log::Debug().Write("%s: lRetval %u\n", __func__, lRetval);

 done:
    return (lRetval);
}

}; // namespace Server

}; // namespace HLX
