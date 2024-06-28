/*
 *    Copyright (c) 2024 Grant Erickson
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
 *      This file defines an object for managing a HLX output data model.
 *
 */

#ifndef OPENHLXMMODELOUTPUTMODELBASIS_HPP
#define OPENHLXMMODELOUTPUTMODELBASIS_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/NameModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX output data model.
 *
 *  @ingroup model
 *  @ingroup output
 *
 */
class OutputModelBasis
{
public:
    static const size_t kNameLengthMax;

    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef IdentifierModel::IdentifierType IdentifierType;
    /**
     *  Convenience type redeclaring @a MuteType from the
     *  volume model.
     *
     */
    typedef VolumeModel::MuteType           MuteType;

    /**
     *  Convenience type redeclaring @a LevelType from the
     *  volume model.
     *
     */
    typedef VolumeModel::LevelType          LevelType;

public:
    // Destruction

    virtual ~OutputModelBasis(void) = default;

protected:
    // Construction

    OutputModelBasis(void) = default;

    // Initialization

    Common::Status Init(void);
    Common::Status Init(const char *aName, const IdentifierType &aIdentifier);
    Common::Status Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier);
    Common::Status Init(const std::string &aName, const IdentifierType &aIdentifier);
    Common::Status Init(const OutputModelBasis &aOutputModelBasis);

    OutputModelBasis &operator =(const OutputModelBasis &aOutputModelBasis);

public:
    // Observation

    Common::Status GetIdentifier(IdentifierType &aIdentifier) const;
    Common::Status GetName(const char *&aName) const;
    Common::Status GetMute(MuteType &aMute) const;
    Common::Status GetVolume(LevelType &aVolume) const;

    // Mutation

    Common::Status DecreaseVolume(LevelType &aOutLevel);
    Common::Status IncreaseVolume(LevelType &aOutLevel);
    Common::Status SetIdentifier(const IdentifierType &aIdentifier);
    Common::Status SetName(const char *aName);
    Common::Status SetName(const char *aName, const size_t &aNameLength);
    Common::Status SetMute(const MuteType &aMute);
    Common::Status SetVolume(const LevelType &aVolume);
    Common::Status ToggleMute(MuteType &aOutMute);

    // Equality

    bool operator ==(const OutputModelBasis &aOutputModelBasis) const;

private:
    // We can: 1) make all three data members protected just so that
    // the zone model can access the volume model for the fixed volume
    // methods, 2) make none protected but make the zone model a
    // friend, or 3) move fixed volume out of the volume model and
    // into the zone model. Pursuing (2) seems the most reasonable
    // trade-off between breaking encapsulation and keeping like
    // properties and methods together.

    friend class ZoneModel;

    IdentifierModel  mIdentifier;
    NameModel        mName;
    VolumeModel      mVolume;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELOUTPUTMODELBASIS_HPP
