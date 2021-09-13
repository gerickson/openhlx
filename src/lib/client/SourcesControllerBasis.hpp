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
 *      sources controller, in a client.
 *
 */

#ifndef OPENHLXCLIENTSOURCESCONTROLLERBASIS_HPP
#define OPENHLXCLIENTSOURCESCONTROLLERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/ObjectControllerBasis.hpp>
#include <OpenHLX/Client/SourcesControllerCommands.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup source
 *
 */
class SourcesControllerBasis :
    public Client::ObjectControllerBasis
{
public:
    virtual ~SourcesControllerBasis(void);

protected:
    /**
     *  @brief
     *    This is a class constructor.
     *
     *  This constructs the sources controller with the specified sources
     *  collection model and the maximum number of allowed sources.
     *
     *  @param[in]  aSourcesModel  A mutable reference to the sources
     *                             collection model to construct the
     *                             controller with. This is retained by a
     *                             weak pointer reference and,
     *                             consequently, must remain in scope for
     *                             the lifetime of the controller.
     *  @param[in]  aSourcesMax    An immutable reference to the maximum
     *                             number of allowed sources managed by
     *                             the controller.
     *
     */
    SourcesControllerBasis(Model::SourcesModel &aSourcesModel,
                           const Model::SourceModel::IdentifierType &aSourcesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Command Completion Handler Trampolines

    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

protected:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);

private:
    // Command Completion Handlers

    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status ResponseInit(void);

private:
    Model::SourcesModel &                      mSourcesModel;
    const Model::SourceModel::IdentifierType & mSourcesMax;

protected:
    static Command::Sources::NameResponse    kNameResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTSOURCESCONTROLLERBASIS_HPP
