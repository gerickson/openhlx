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
 *      This file defines an object for managing the server-side
 *      observation and mutation of a collection of HLX equalizer
 *      presets.
 *
 */

#ifndef OPENHLXSIMULATOREQUALIZERPRESETSCONTROLLER_HPP
#define OPENHLXSIMULATOREQUALIZERPRESETSCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Common/EqualizerPresetsControllerBasis.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/EqualizerPresetsModel.hpp>
#include <OpenHLX/Server/EqualizerPresetsControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a collection of HLX equalizer presets.
 *
 *  @ingroup server
 *  @ingroup equalizer-preset
 *
 */
class EqualizerPresetsController :
    public Simulator::ControllerBasis,
    public Server::ContainerControllerBasis,
    public Common::EqualizerPresetsControllerBasis
{
public:
    EqualizerPresetsController(void);
    virtual ~EqualizerPresetsController(void);

    Common::Status Init(Server::CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status EqualizerPresetEqualizerLoadFromBackupConfiguration(CFDictionaryRef aEqualizerPresetDictionary, Model::EqualizerPresetModel &aEqualizerPresetModel);
    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aSourcesDictionary, const IdentifierType &aEqualizerPresetIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aEqualizerPresetsDictionary, const IdentifierType &aEqualizerPresetIdentifier) const final;
    static Common::Status EqualizerPresetEqualizerSaveToBackupConfiguration(CFMutableDictionaryRef aEqualizerPresetDictionary, const Model::EqualizerPresetModel &aEqualizerPresetModel);

    Common::Status GetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, Model::EqualizerBandModel *&aEqualizerBandModel);

    Common::Status HandleQueryReceived(const IdentifierType &aEqualizerPresetIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    Common::Status HandleAdjustBandReceived(Server::ConnectionBasis &aConnection, const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment);
    Common::Status HandleAdjustBandReceived(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandAdjustment, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    Common::Status HandleSetBandReceived(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    static Common::Status HandleBandResponse(const IdentifierType &aEqualizerPresetIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aBandLevel, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    // Command Completion Handlers

    void DecreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void IncreaseBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetBandRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::EqualizerPresetsModel                           mEqualizerPresets;

private:
    static Server::Command::EqualizerPresets::DecreaseBandRequest  kDecreaseBandRequest;
    static Server::Command::EqualizerPresets::IncreaseBandRequest  kIncreaseBandRequest;
    static Server::Command::EqualizerPresets::QueryRequest         kQueryRequest;
    static Server::Command::EqualizerPresets::SetBandRequest       kSetBandRequest;
    static Server::Command::EqualizerPresets::SetNameRequest       kSetNameRequest;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATOREQUALIZERPRESETSCONTROLLER_HPP
