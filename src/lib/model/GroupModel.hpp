/*
 *    Copyright (c) 2019-2024 Grant Erickson
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
 *      This file defines an object for managing a HLX group data
 *      model.
 *
 */

#ifndef OPENHLXMMODELGROUPMODEL_HPP
#define OPENHLXMMODELGROUPMODEL_HPP

#include <set>
#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifiersCollection.hpp>
#include <OpenHLX/Model/OutputModelBasis.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX group data model.
 *
 *  @ingroup model
 *  @ingroup group
 *
 */
class GroupModel :
    public OutputModelBasis
{
public:
    /**
     *  Type for a collection of group source (input) identifiers.
     *
     */
    typedef IdentifiersCollection           Sources;

public:
    GroupModel(void) = default;
    virtual ~GroupModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const char *aName, const IdentifierType &aIdentifier);
    Common::Status Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier);
    Common::Status Init(const std::string &aName, const IdentifierType &aIdentifier);
    Common::Status Init(const GroupModel &aGroupModel);

    GroupModel &operator =(const GroupModel &aGroupModel);

    bool   ContainsZone(const ZoneModel::IdentifierType &aZoneIdentifier) const;

    Common::Status GetSources(size_t &aCount) const;
    Common::Status GetSources(SourceModel::IdentifierType *aSourceIdentifiers, size_t &aCount) const;
    Common::Status GetSources(Sources &aSourceIdentifiers) const;
    Common::Status GetZones(size_t &aCount) const;
    Common::Status GetZones(ZoneModel::IdentifierType *aZoneIdentifiers, size_t &aCount) const;

    Common::Status AddSource(const SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status AddZone(const ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status ClearZones(void);
    Common::Status RemoveSource(const SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status RemoveZone(const ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status SetSource(const SourceModel::IdentifierType &aSourceIdentifier);
    Common::Status SetSources(const SourceModel::IdentifierType *aSourceIdentifiers, const size_t &aCount);
    Common::Status SetSources(const Sources &aSourceIdentifiers);

    bool operator ==(const GroupModel &aGroupModel) const;

private:
    static Common::Status GetIdentifiers(const IdentifiersCollection &aCollection, size_t &aCount);
    static Common::Status GetIdentifiers(const IdentifiersCollection &aCollection, IdentifiersCollection::IdentifierType *aIdentifiers, size_t &aCount);

    static Common::Status AddIdentifier(IdentifiersCollection &aCollection, const IdentifiersCollection::IdentifierType &aIdentifier);
    static Common::Status RemoveIdentifier(IdentifiersCollection &aCollection, const IdentifiersCollection::IdentifierType &aIdentifier);

private:
    /**
     *  Type for a collection of group zone identifiers.
     *
     */
    typedef IdentifiersCollection  Zones;

    Sources          mSources;
    Zones            mZones;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELGROUPMODEL_HPP
