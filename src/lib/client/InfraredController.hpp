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

#ifndef OPENHLXCLIENTINFRAREDCONTROLLER_HPP
#define OPENHLXCLIENTINFRAREDCONTROLLER_HPP

#include <OpenHLX/Common/InfraredControllerBasis.hpp>
#include <OpenHLX/Client/InfraredControllerBasis.hpp>
#include <OpenHLX/Client/InfraredControllerCommands.hpp>
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
    public Common::InfraredControllerBasis,
    public Client::InfraredControllerBasis
{
public:
    InfraredController(void);
    virtual ~InfraredController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Mutator Methods

    Common::Status SetDisabled(const Model::InfraredModel::DisabledType &aDisabled);

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTINFRAREDCONTROLLER_HPP
