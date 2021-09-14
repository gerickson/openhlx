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
 *      This file defines a derivable object for realizing a HLX
 *      equalizer presets controller, in a server.
 *
 */

#ifndef OPENHLXSERVEREQUALIZERPRESETSCONTROLLERBASIS_HPP
#define OPENHLXSERVEREQUALIZERPRESETSCONTROLLERBASIS_HPP

#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/EqualizerPresetsModel.hpp>
#include <OpenHLX/Server/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A derivable object for realizing a HLX equalizer presets
 *    controller, in a server.
 *
 *  @ingroup server
 *  @ingroup equalizer-preset
 *
 */
class EqualizerPresetsControllerBasis :
    public Server::ObjectControllerBasis
{
public:
    virtual ~EqualizerPresetsControllerBasis(void);

protected:
    EqualizerPresetsControllerBasis(Model::EqualizerPresetsModel &aEqualizerPresetsModel,
                                    const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetsMax);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

private:
    Model::EqualizerPresetsModel &                           mEqualizerPresetsModel;
    const Model::EqualizerPresetModel::IdentifierType &      mEqualizerPresetsMax;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status HandleQueryReceived(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                       Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

    static Common::Status HandleBandResponse(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier,
                                             const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                             const Model::EqualizerBandModel::LevelType &aBandLevel,
                                             Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

protected:
    static Server::Command::EqualizerPresets::DecreaseBandRequest  kDecreaseBandRequest;
    static Server::Command::EqualizerPresets::IncreaseBandRequest  kIncreaseBandRequest;
    static Server::Command::EqualizerPresets::QueryRequest         kQueryRequest;
    static Server::Command::EqualizerPresets::SetBandRequest       kSetBandRequest;
    static Server::Command::EqualizerPresets::SetNameRequest       kSetNameRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVEREQUALIZERPRESETSCONTROLLERBASIS_HPP
