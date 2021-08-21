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

#ifndef OPENHLXSERVERSOURCESCONTROLLERBASIS_HPP
#define OPENHLXSERVERSOURCESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>
#include <OpenHLX/Server/ControllerBasis.hpp>
#include <OpenHLX/Server/SourcesControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup sources
 *
 */
class SourcesControllerBasis :
    public Server::ControllerBasis
{
public:
    virtual ~SourcesControllerBasis(void);

protected:
    SourcesControllerBasis(Model::SourcesModel &aSourcesModel,
                           const Model::SourceModel::IdentifierType &aSourcesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

private:
    Model::SourcesModel &                      mSourcesModel;
    const Model::SourceModel::IdentifierType & mSourcesMax;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    Common::Status HandleQueryReceived(const Model::SourceModel::IdentifierType &aSourceIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

protected:
    // Observation (Query) Command Request Class (Static) Handlers

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

protected:
    static Server::Command::Sources::SetNameRequest  kSetNameRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERSOURCESCONTROLLERBASIS_HPP
