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

#ifndef HLXSERVERSOURCESCONTROLLER_HPP
#define HLXSERVERSOURCESCONTROLLER_HPP

#include <ContainerControllerBasis.hpp>
#include <ControllerBasis.hpp>
#include <SourcesControllerBasis.hpp>
#include <SourcesControllerCommands.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>


namespace HLX
{

namespace Server
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
    public ControllerBasis,
    public ContainerControllerBasis,
    public Common::SourcesControllerBasis
{
public:
    SourcesController(void);
    virtual ~SourcesController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void SetNameRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status RequestInit(void);
    Common::Status DoRequestHandlers(const bool &aRegister);

    Common::Status ElementLoadFromBackupConfiguration(CFDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier) final;
    Common::Status ElementSaveToBackupConfiguration(CFMutableDictionaryRef aSourcesDictionary, const IdentifierType &aSourceIdentifier) const final;
    Common::Status HandleQueryReceived(const IdentifierType &aSourceIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    // Command Completion Handlers

    void SetNameRequestReceivedHandler(ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    Model::SourcesModel                      mSources;

private:
    static Command::Sources::SetNameRequest  kSetNameRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERSOURCESCONTROLLER_HPP
