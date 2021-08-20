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

#ifndef OPENHLXCLIENTSOURCESCONTROLLER_HPP
#define OPENHLXCLIENTSOURCESCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

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
    public Common::SourcesControllerBasis,
    public Client::SourcesControllerBasis
{
public:
    SourcesController(void);
    virtual ~SourcesController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status GetSource(const IdentifierType &aIdentifier, const Model::SourceModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aSourceIdentifier) const;

    // Mutator Methods

    Common::Status SetName(const IdentifierType &aSourceIdentifier, const char *aName);
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTSOURCESCONTROLLER_HPP
