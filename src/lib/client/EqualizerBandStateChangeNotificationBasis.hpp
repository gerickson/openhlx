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
 *      This file defines a base object for a HLX client object data
 *      model equalizer band level property state change notification
 *      (SCN).
 *
 */

#ifndef HLXCLIENTEQUALIZERBANDSTATECHANGENOTIFICATIONBASIS_HPP
#define HLXCLIENTEQUALIZERBANDSTATECHANGENOTIFICATIONBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A base object for a HLX client object data model equalizer band
 *    level property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  equalizer band model.
     *
     */
    typedef Model::EqualizerBandModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a LevelType from the
     *  equalizer band model.
     *
     */
    typedef Model::EqualizerBandModel::LevelType    LevelType;

public:
    virtual ~EqualizerBandNotificationBasis(void) = default;

    IdentifierType GetBand(void) const;
    LevelType      GetLevel(void) const;

protected:
    EqualizerBandNotificationBasis(void);

    Common::Status Init(const IdentifierType &aBandIdentifier, const LevelType &aBandLevel);

private:
    IdentifierType mBand;
    LevelType      mLevel;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTEQUALIZERBANDSTATECHANGENOTIFICATIONBASIS_HPP
