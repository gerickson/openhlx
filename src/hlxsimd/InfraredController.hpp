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
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 */

#ifndef HLXSERVERINFRAREDCONTROLLER_HPP
#define HLXSERVERINFRAREDCONTROLLER_HPP

#include <ControllerBasis.hpp>
#include <InfraredControllerCommands.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>

namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for managing the server-side
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 *  @ingroup server
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

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetDisabledRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    void QueryHandler(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    static Common::Status HandleDisabledResponse(const Model::InfraredModel::DisabledType &aDisabled, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    // Command Completion Handlers

    void QueryRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetDisabledRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::InfraredModel                            mInfraredModel;

private:
    static Command::Infrared::QueryRequest          kQueryRequest;
    static Command::Infrared::SetDisabledRequest    kSetDisabledRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERINFRAREDCONTROLLER_HPP
