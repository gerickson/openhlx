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
 *      observation and mutation of a collection of HLX sources.
 *
 */

#ifndef OPENHLXSIMULATORSOURCESCONTROLLER_HPP
#define OPENHLXSIMULATORSOURCESCONTROLLER_HPP

#include <OpenHLX/Common/SourcesControllerBasis.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>
#include <OpenHLX/Server/SourcesControllerCommands.hpp>

#include "ContainerControllerBasis.hpp"
#include "ControllerBasis.hpp"


namespace HLX
{

namespace Simulator
{

/**
 *  @brief
 *    An object for managing the server-side observation and mutation
 *    of a collection of HLX sources.
 *
 *  @ingroup server
 *  @ingroup source
 *
 */
class SourcesController :
    public Simulator::ControllerBasis,
    public Server::ContainerControllerBasis,
    public Common::SourcesControllerBasis
{
public:
    SourcesController(void);
    virtual ~SourcesController(void);

    Common::Status Init(Server::CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier) const final;
    Common::Status HandleQueryReceived(const IdentifierType &aSourceIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    // Command Completion Handlers

    void SetNameRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::SourcesModel                      mSources;

private:
    static Server::Command::Sources::SetNameRequest  kSetNameRequest;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORSOURCESCONTROLLER_HPP
