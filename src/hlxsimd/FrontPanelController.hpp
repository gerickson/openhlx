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
 *      observation and mutation of a HLX physical front panel.
 *
 */

#ifndef OPENHLXSIMULATORFRONTPANELCONTROLLER_HPP
#define OPENHLXSIMULATORFRONTPANELCONTROLLER_HPP

#include <OpenHLX/Model/FrontPanelModel.hpp>
#include <OpenHLX/Server/FrontPanelControllerCommands.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a HLX physical front panel.
 *
 *  @ingroup simulator
 *  @ingroup front-panel
 *
 */
class FrontPanelController :
    public Simulator::ControllerBasis
{
public:
    FrontPanelController(void);
    ~FrontPanelController(void);

    Common::Status Init(Server::CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    void QueryHandler(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    static Common::Status HandleBrightnessResponse(const Model::FrontPanelModel::BrightnessType &aBrightness, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleLockedResponse(const Model::FrontPanelModel::LockedType &aLocked, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

    // Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetBrightnessRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetLockedRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::FrontPanelModel                            mFrontPanelModel;

private:
    static Server::Command::FrontPanel::QueryRequest          kQueryRequest;
    static Server::Command::FrontPanel::SetBrightnessRequest  kSetBrightnessRequest;
    static Server::Command::FrontPanel::SetLockedRequest      kSetLockedRequest;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORFRONTPANELCONTROLLER_HPP
