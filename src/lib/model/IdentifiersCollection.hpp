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
 *      This file defines a collection object for managing HLX object
 *      identifiers.
 *
 */

#ifndef OPENHLXMMODELIDENTIFIERSCOLLECTION_HPP
#define OPENHLXMMODELIDENTIFIERSCOLLECTION_HPP

#include <set>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX object identifiers.
 *
 *  This may be used for managing a collection of HLX object
 *  identifiers such as source usage or zone membership in a HLX
 *  group.
 *
 *  @ingroup model
 *
 */
class IdentifiersCollection
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef IdentifierModel::IdentifierType IdentifierType;

public:
    IdentifiersCollection(void);
    virtual ~IdentifiersCollection(void) = default;

    Common::Status Init(void);
    Common::Status Init(const IdentifierType *aIdentifiers, const size_t &aCount);
    Common::Status Init(const IdentifiersCollection &aIdentifiersCollection);

    IdentifiersCollection &operator =(const IdentifiersCollection &aIdentifiersCollection);

    bool   ContainsIdentifier(const IdentifierType &aIdentifier) const;

    Common::Status GetCount(size_t &aCount) const;
    Common::Status GetIdentifiers(IdentifierType *aIdentifiers, size_t &aCount) const;

    Common::Status AddIdentifier(const IdentifierType &aIdentifier);
    Common::Status RemoveIdentifier(const IdentifierType &aIdentifier);
    Common::Status SetIdentifiers(const IdentifierType *aIdentifiers, const size_t &aCount);
    Common::Status SetIdentifiers(const IdentifiersCollection &aIdentifiersCollection);
    Common::Status ClearIdentifiers(void);

    bool operator ==(const IdentifiersCollection &aIdentifiersCollection) const;

private:
    Common::Status AddIdentifierPrivate(const IdentifierType &aIdentifier);
    Common::Status SetIdentifiersPrivate(const IdentifierType *aIdentifiers, const size_t &aCount);

    static Common::Status AddIdentifiersPrivate(IdentifiersCollection &aIdentifiersCollection, const IdentifierType *aIdentifiers, const size_t &aCount);

private:
    typedef std::set<IdentifierType>  Identifiers;

    bool                 mInitialized;
    Identifiers          mIdentifiers;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELIDENTIFIERSCOLLECTION_HPP
