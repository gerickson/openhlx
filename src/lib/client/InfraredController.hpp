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
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 */

#ifndef HLXCLIENTINFRAREDCONTROLLER_HPP
#define HLXCLIENTINFRAREDCONTROLLER_HPP

#include <ControllerBasis.hpp>
#include <InfraredControllerCommands.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>

namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the server-side
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 *  @ingroup client
 *  @ingroup infrared
 *
 */
class InfraredController :
    public ControllerBasis
{
public:
    InfraredController(void);
    ~InfraredController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);

    // Mutator Methods

    Common::Status SetDisabled(const Model::InfraredModel::DisabledType &aDisabled);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetDisabledCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void DisabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status ResponseInit(void);
    Common::Status DoNotificationHandlers(bool aRegister);

    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetDisabledCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void BrightnessNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void DisabledNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::InfraredModel                            mInfraredModel;

private:
    static Command::Infrared::DisabledResponse      kDisabledResponse;
    static Command::Infrared::QueryResponse         kQueryResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTINFRAREDCONTROLLER_HPP
