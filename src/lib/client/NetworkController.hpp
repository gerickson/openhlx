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
 *      observation and mutation of a HLX Ethernet network interface.
 *
 */

#ifndef HLXCLIENTNETWORKCONTROLLER_HPP
#define HLXCLIENTNETWORKCONTROLLER_HPP

#include <OpenHLX/Client/ControllerBasis.hpp>
#include <OpenHLX/Client/NetworkControllerBasis.hpp>
#include <OpenHLX/Client/NetworkControllerCommands.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a HLX Ethernet network interface.
 *
 *  @ingroup client
 *  @ingroup network
 *
 */
class NetworkController :
    public Client::ControllerBasis,
    public Client::NetworkControllerBasis
{
public:
    NetworkController(void);
    virtual ~NetworkController(void);

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);
};

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTNETWORKCONTROLLER_HPP
