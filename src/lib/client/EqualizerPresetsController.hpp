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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a collection of HLX equalizer
 *      presets.
 *
 */

#ifndef OPENHLXCLIENTEQUALIZERPRESETSCONTROLLER_HPP
#define OPENHLXCLIENTEQUALIZERPRESETSCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/EqualizerPresetsControllerBasis.hpp>
#include <OpenHLX/Client/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Common/EqualizerPresetsControllerBasis.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/EqualizerPresetsModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX equalizer presets.
 *
 *  @ingroup client
 *  @ingroup equalizer-preset
 *
 */
class EqualizerPresetsController :
    public Common::EqualizerPresetsControllerBasis,
    public Client::EqualizerPresetsControllerBasis
{
public:
    EqualizerPresetsController(void);
    virtual ~EqualizerPresetsController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status GetEqualizerPreset(const IdentifierType &aIdentifier, const Model::EqualizerPresetModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aEqualizerPresetIdentifier) const;

    // Mutator Methods

    Common::Status SetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aLevel);
    Common::Status IncreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
    Common::Status DecreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);

    Common::Status SetName(const IdentifierType &aEqualizerPresetIdentifier, const char *aName);

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTEQUALIZERPRESETSCONTROLLER_HPP
