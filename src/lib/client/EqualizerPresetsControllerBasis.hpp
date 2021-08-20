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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXCLIENTEQUALIZERPRESETSCONTROLLERBASIS_HPP
#define OPENHLXCLIENTEQUALIZERPRESETSCONTROLLERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/ControllerBasis.hpp>
#include <OpenHLX/Client/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/EqualizerPresetsModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup equalizer-preset
 *
 */
class EqualizerPresetsControllerBasis :
    public Client::ControllerBasis
{
public:
    virtual ~EqualizerPresetsControllerBasis(void);

protected:
    EqualizerPresetsControllerBasis(Model::EqualizerPresetsModel &aEqualizerPresetsModel,
                                    const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetsMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const Model::EqualizerPresetModel::IdentifierType &aEqualizerPresetIdentifier);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);
    Common::Status ResponseInit(void);

protected:
    size_t                                                   mEqualizerPresetsDidRefreshCount;

private:
    Model::EqualizerPresetsModel &                           mEqualizerPresetsModel;
    const Model::EqualizerPresetModel::IdentifierType &      mEqualizerPresetsMax;

protected:
    static Command::EqualizerPresets::EqualizerBandResponse  kEqualizerBandResponse;
    static Command::EqualizerPresets::NameResponse           kNameResponse;
    static Command::EqualizerPresets::QueryResponse          kQueryResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTEQUALIZERPRESETSCONTROLLERBASIS_HPP
