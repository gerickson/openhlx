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
 *      This file defines a collection object for managing HLX zone
 *      objects.
 *
 */

#ifndef OPENHLXMMODELZONESMODEL_HPP
#define OPENHLXMMODELZONESMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX zone objects.
 *
 *  @ingroup model
 *
 */
class ZonesModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  zone model.
     *
     */
    typedef ZoneModel::IdentifierType IdentifierType;

public:
    ZonesModel(void);
    ZonesModel(const ZonesModel &aZonesModel);
    virtual ~ZonesModel(void) = default;

    Common::Status Init(const IdentifierType &aZonesMax);
    Common::Status Init(const ZonesModel &aZonesModel);

    ZonesModel &operator =(const ZonesModel &aZonesModel);

    Common::Status GetZone(const IdentifierType &aZoneIdentifier, ZoneModel *&aZoneModel);
    Common::Status GetZone(const IdentifierType &aZoneIdentifier, const ZoneModel *&aZoneModel) const;
    Common::Status GetZone(const char *aName, const ZoneModel *&aZoneModel) const;

    Common::Status SetZone(const IdentifierType &aZoneIdentifier, const ZoneModel &aZoneModel);

    bool operator ==(const ZonesModel &aZonesModel) const;

private:
    Common::Status ValidateIdentifier(const IdentifierType &aZoneIdentifier) const;

private:
    typedef std::map<IdentifierType, ZoneModel> Zones;

    IdentifierType  mZonesMax;
    Zones           mZones;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELZONESMODEL_HPP
