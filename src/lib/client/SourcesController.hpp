/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      observation and mutation of a collection of HLX sources.
 *
 */

#ifndef HLXCLIENTSOURCESCONTROLLER_HPP
#define HLXCLIENTSOURCESCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/ControllerBasis.hpp>
#include <OpenHLX/Client/SourcesControllerBasis.hpp>
#include <OpenHLX/Client/SourcesControllerCommands.hpp>
#include <OpenHLX/Common/SourcesControllerBasis.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX sources.
 *
 *  @ingroup client
 *  @ingroup source
 *
 */
class SourcesController :
    public Client::ControllerBasis,
    public Common::SourcesControllerBasis,
    public Client::SourcesControllerBasis
{
public:
    SourcesController(void);
    virtual ~SourcesController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status GetSource(const IdentifierType &aIdentifier, const Model::SourceModel *&aModel) const;

    Common::Status GetSourcesMax(size_t &aSources) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aSourceIdentifier) const;

    // Mutator Methods

    Common::Status SetName(const IdentifierType &aSourceIdentifier, const char *aName);

    // Command Completion Handler Trampolines

    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoNotificationHandlers(bool aRegister);

    // Command Completion Handlers

    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTSOURCESCONTROLLER_HPP
