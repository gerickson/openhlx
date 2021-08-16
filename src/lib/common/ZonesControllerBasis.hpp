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
 *      This file defines a base object for realizing a HLX zones
 *      controller, either in a client or server context.
 *
 */

#ifndef OPENHLXCOMMONZONESCONTROLLERBASIS_HPP
#define OPENHLXCOMMONZONESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX zones controller, either in a
 *    client or server context.
 *
 *  @ingroup common
 *
 */
class ZonesControllerBasis
{
public:
    /**
     *  A locally-scoped convenience type for a zone identifier.
     *
     */
    typedef Model::ZoneModel::IdentifierType IdentifierType;

public:
    virtual ~ZonesControllerBasis(void) = default;

    // Observer Methods

    static Common::Status GetZonesMax(size_t &aZones);
    static size_t         GetZonesMax(void);

    static bool           IsValidIdentifier(const IdentifierType &aZoneIdentifier);
    static Common::Status ValidateIdentifier(const IdentifierType &aZoneIdentifier);

protected:
    ZonesControllerBasis(void) = default;

    // Initializer(s)

    Common::Status Init(void);

protected:
    Model::ZonesModel            mZones;

protected:
    static const IdentifierType  kZonesMax;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONZONESCONTROLLERBASIS_HPP
