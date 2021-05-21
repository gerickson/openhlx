/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file implements a collection object for managing HLX object
 *      identifiers.
 *
 */

#include "IdentifiersCollection.hpp"

#include <algorithm>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
IdentifiersCollection :: IdentifiersCollection(void) :
    mInitialized(false),
    mIdentifiers()
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the collection with an empty set of identifiers.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *
 */
Status
IdentifiersCollection :: Init(void)
{
    const IdentifierType *  lIdentifiers = nullptr;
    const size_t            lCount = 0;

    return (Init(lIdentifiers, lCount));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the collection with the specified identifiers.
 *
 *  @param[in]  aIdentifiers  An iterator to an immutable linear array
 *                            (that is, a standard C-style array or
 *                            adapter thereto) of identifiers to
 *                            initialize the collection with whose
 *                            size is represented by @a aCount.
 *  @param[in]  aCount        A immutable reference to the number of
 *                            identifiers in @a aIdentifiers.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifiers are already
 *                                    associated with the collection.
 */
Status
IdentifiersCollection :: Init(const IdentifierType *aIdentifiers, const size_t &aCount)
{
    Status  lRetval;

    mIdentifiers.clear();

    lRetval = AddIdentifiersPrivate(*this, aIdentifiers, aCount);
    nlREQUIRE_SUCCESS(lRetval, done);

    mInitialized = true;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified identifiers collection.
 *
 *  @param[in]  aIdentifiersCollection  An immutable reference to the
 *                                      identifiers collection to
 *                                      initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IdentifiersCollection :: Init(const IdentifiersCollection &aIdentifiersCollection)
{
    Status  lRetval = ((!aIdentifiersCollection.mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        *this = aIdentifiersCollection;
    }

    mInitialized = true;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified identifiers collection to this
 *  one.
 *
 *  @param[in]  aIdentifiersCollection  An immutable reference to the
 *                                      identifiers collection to
 *                                      assign (copy) to this one.
 *
 *  @returns
 *    A reference to this identifiers collection after the assignment
 *    (copy) is complete.
 *
 */
IdentifiersCollection &
IdentifiersCollection :: operator =(const IdentifiersCollection &aIdentifiersCollection)
{
    mInitialized = aIdentifiersCollection.mInitialized;
    mIdentifiers = aIdentifiersCollection.mIdentifiers;

    return (*this);
}

/**
 *  @brief
 *    Determine whether or not the specified identifier is a
 *    member of the collection.
 *
 *  This determines whether or not the specified identifier is a
 *  member of the collection.
 *
 *  @param[in]  aIdentifier  An immutable reference to the
 *                           identifer to check for collection
 *                           membership.
 *
 *  @returns
 *    True if the identifier is a member of the collection; otherwise,
 *    false.
 *
 *  @sa AddIdentifier
 *  @sa ClearIdentifiers
 *  @sa RemoveIdentifier
 *
 */
bool
IdentifiersCollection :: ContainsIdentifier(const IdentifierType &aIdentifier) const
{
    Identifiers::const_iterator  lCurrent = mIdentifiers.begin();
    Identifiers::const_iterator  lEnd = mIdentifiers.end();
    Identifiers::const_iterator  lResult;
    bool                         lRetval;

    nlEXPECT_ACTION(mInitialized, done, lRetval = false);

    lResult = std::find(lCurrent, lEnd, aIdentifier);

    lRetval = (lResult != lEnd);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the number of identifiers associated with the collection.
 *
 *  @note
 *    This may be useful to help dynamically-size caller-allocated
 *    storage for the actual identifiers.
 *
 *  @param[out]  aCount       The number of identifiers associated
 *                            with the collection, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *
 */
Status
IdentifiersCollection :: GetCount(size_t &aCount) const
{
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        aCount = mIdentifiers.size();
    }

    return (lRetval);
}

/**
 *  @brief
 *    Get the identifiers from collection.
 *
 *  @param[in,out]  aIdentifiers  An iterator to a mutable linear
 *                                array (that is a standard C-style
 *                                array or adapter thereto) for
 *                                identifiers in the collection whose
 *                                size is represented by @a aCount. On
 *                                success, this will contain the
 *                                minmum of @a aCount and the actual
 *                                number of identifiers in the
 *                                collection.
 *  @param[in,out]  aCount        A mutable reference to the number
 *                                of elements @a aIdentifiers can
 *                                support. On success, this will
 *                                contain the minmum of @a aCount and
 *                                the actual number of identifiers in
 *                                the collection.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *
 */
Status
IdentifiersCollection :: GetIdentifiers(IdentifierType *aIdentifiers, size_t &aCount) const
{
    Identifiers::const_iterator lSourceCurrent = mIdentifiers.begin();
    Identifiers::const_iterator lSourceEnd = mIdentifiers.end();
    IdentifierType *            lDestinationCurrent = aIdentifiers;
    IdentifierType *            lDestinationEnd = lDestinationCurrent + aCount;
    size_t                      lCount = std::min(aCount, mIdentifiers.size());
    Status                      lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);


    if (lRetval == kStatus_Success)
    {
        aCount = lCount;

        while ((lSourceCurrent != lSourceEnd) && (lDestinationCurrent != lDestinationEnd) && (lCount))
        {
            *lDestinationCurrent++ = *lSourceCurrent++;

            lCount--;
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate an identifier with the collection.
 *
 *  @param[in]       aIdentifier  An immutable reference to the
 *                                identifier to associate with the
 *                                collection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifier is already
 *                                    associated with the collection.
 *  @retval  kError_NotInitialized    If the identifiers have not
 *                                    been initialized with a known
 *                                    value(s).
 *
 */
Status
IdentifiersCollection :: AddIdentifier(const IdentifierType &aIdentifier)
{
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        lRetval = AddIdentifierPrivate(aIdentifier);
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to remove (disassociate) an identifier from the
 *    collection.
 *
 *  @param[in]       aIdentifier  An immutable reference to the
 *                                identifier to remove (disassociate)
 *                                from the collection.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not
 *                                  been initialized with a known
 *                                  value(s).
 *  @retval  -ENOENT                The identifier is not
 *                                  associated with the collection.
 *
 */
Status
IdentifiersCollection :: RemoveIdentifier(const IdentifierType &aIdentifier)
{
    size_t  lResult;
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        lResult = mIdentifiers.erase(aIdentifier);

        lRetval = (lResult == 1) ? kStatus_Success : -ENOENT;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate multiple identifiers with the collection,
 *    removing any other identifiers.
 *
 *  This attempts to associate the specified multiple identifiers with
 *  the collection, removing any identifiers, if successful.
 *
 *  @param[in]  aIdentifiers  An iterator to an immutable linear array
 *                            (that is, a standard C-style array or
 *                            adapter thereto) of identifiers to
 *                            associate with the collection whose size
 *                            is represented by @a aCount.
 *  @param[in]  aCount        A immutable reference to the number of
 *                            identifiers in @a aIdentifiers.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifiers and @b only
 *                                    those identifiers are already in
 *                                    the collection. If the
 *                                    requested identifiers are some
 *                                    of many already in the
 *                                    collection, then the return
 *                                    status will be kStatus_Success,
 *                                    if successful.
 *  @retval  kError_NotInitialized    If the identifiers have not been
 *                                    initialized with a known
 *                                    value(s).
 *
 *  @sa AddIdentifiers
 *
 */
Status
IdentifiersCollection :: SetIdentifiers(const IdentifierType *aIdentifiers, const size_t &aCount)
{
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        lRetval = SetIdentifiersPrivate(aIdentifiers, aCount);
    }

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate identifiers with the collection, removing
 *    any other identifiers.
 *
 *  This attempts to associate the specified multiple identifiers with
 *  the collection, removing any other identifier associations, if
 *  successful.
 *
 *  @param[in]  aIdentifiersCollection  An immutable reference to a
 *                                      container collection of
 *                                      identifiers to set for this
 *                                      collection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifiers and @b only
 *                                    those identifiers are already in
 *                                    the collection. If the
 *                                    requested identifiers are some
 *                                    of many already in the
 *                                    collection, then the return
 *                                    status will be kStatus_Success,
 *                                    if successful.
 *  @retval  kError_NotInitialized    If the identifiers have
 *                                    not been initialized with a
 *                                    known value(s).
 *
 *  @sa AddIdentifiers
 *
 */
Status
IdentifiersCollection :: SetIdentifiers(const IdentifiersCollection &aIdentifiersCollection)
{
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        if ((this == &aIdentifiersCollection) || (*this == aIdentifiersCollection))
        {
            lRetval = kStatus_ValueAlreadySet;
        }
        else
        {
            *this = aIdentifiersCollection;
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    Clear (remove) all identifiers associated with the collection.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the identifiers have not been
 *                                  initialized with a known value(s).
 *
 */
Status
IdentifiersCollection :: ClearIdentifiers(void)
{
    Status  lRetval = ((!mInitialized) ? kError_NotInitialized : kStatus_Success);

    if (lRetval == kStatus_Success)
    {
        lRetval = Init();
    }

    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided identifiers collection against this one
 *  to determine if they are equal to one another.
 *
 *  @param[in]  aIdentifiersCollection  An immutable reference to the
 *                                      identifiers collection to
 *                                      compare for equality.
 *
 *  @returns
 *    True if this identifiers collection is equal to the specified
 *    one; otherwise, false.
 *
 */
bool
IdentifiersCollection :: operator ==(const IdentifiersCollection &aIdentifiersCollection) const
{
    return ((mInitialized == aIdentifiersCollection.mInitialized) &&
            (mIdentifiers == aIdentifiersCollection.mIdentifiers       ));
}

/**
 *  @brief
 *    Attempt to associate an identifier with the collection.
 *
 *  @param[in]       aIdentifier  An immutable reference to the
 *                                identifier to associate with the
 *                                collection.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifier is already
 *                                    associated with the collection.
 *
 */
Status
IdentifiersCollection :: AddIdentifierPrivate(const IdentifierType &aIdentifier)
{
    std::pair<Identifiers::iterator, bool>  lResult;
    Status                                  lRetval;

    lResult = mIdentifiers.insert(aIdentifier);

    lRetval = (lResult.second) ? kStatus_Success : kStatus_ValueAlreadySet;

    return (lRetval);
}

/**
 *  @brief
 *    Attempt to associate identifiers with the specified
 *    identifiers collection.
 *
 *  @param[in,out]  aIdentifiersCollection   A mutable reference to the
 *                                           identifiers collection to
 *                                           add the identifiers to.
 *  @param[in]      aIdentifiers             An iterator to an immutable
 *                                           linear array (that is a
 *                                           standard C-style array or
 *                                           adapter thereto) of
 *                                           identifiers to add to the
 *                                           collection whose size is
 *                                           represented by @a aCount.
 *  @param[in]      aCount                   A immutable reference to
 *                                           the number of identifiers
 *                                           in @a aIdentifiers.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the identifiers are already
 *                                    associated with the collection.
 *
 */
Status
IdentifiersCollection :: AddIdentifiersPrivate(IdentifiersCollection &aIdentifiersCollection, const IdentifierType *aIdentifiers, const size_t &aCount)
{
    const IdentifierType *  lCurrent = aIdentifiers;
    const IdentifierType *  lEnd     = lCurrent + aCount;
    Status                  lRetval  = kStatus_Success;

    while (lCurrent != lEnd)
    {
        lRetval = aIdentifiersCollection.AddIdentifierPrivate(*lCurrent);
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

Status
IdentifiersCollection :: SetIdentifiersPrivate(const IdentifierType *aIdentifiers, const size_t &aCount)
{
    IdentifiersCollection   lTemporaryCollection;
    Status                  lRetval;
    bool                    lAreEqual;

    // Create a temporary identifiers collection, add the specified
    // identifiers to the temporary collection, check for equality
    // with the current collection, swap the current and temporary
    // collections, and set the return status accordingly.

    // Initialize the temporary identifiers collection.

    lRetval = lTemporaryCollection.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    // Add the specified identifiers to the temporary collection.

    lRetval = AddIdentifiersPrivate(lTemporaryCollection, aIdentifiers, aCount);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Check for equality between the current and temporary collection.

    lAreEqual = (*this == lTemporaryCollection);

    // Copy the temporary collection to this one.

    *this = lTemporaryCollection;

    // Set the return status according to the equality test.

    lRetval = ((lAreEqual) ? kStatus_ValueAlreadySet : kStatus_Success);

 done:
    return (lRetval);
}

}; // namespace Model

}; // namespace HLX
