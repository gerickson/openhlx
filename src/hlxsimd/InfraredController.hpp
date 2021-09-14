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

#ifndef OPENHLXSIMULATORINFRAREDCONTROLLER_HPP
#define OPENHLXSIMULATORINFRAREDCONTROLLER_HPP

#include <OpenHLX/Common/InfraredControllerBasis.hpp>
#include <OpenHLX/Server/InfraredControllerBasis.hpp>
#include <OpenHLX/Server/InfraredControllerCommands.hpp>

#include "ObjectControllerBasis.hpp"


namespace HLX
{

namespace Simulator
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
    public Common::InfraredControllerBasis,
    public Server::InfraredControllerBasis,
    public Simulator::ObjectControllerBasis
{
public:
    InfraredController(void);
    virtual ~InfraredController(void);

    // Initializer(s)

    Common::Status Init(Server::CommandManager &aCommandManager) final;

    // Configuration Management Methods

    Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary) final;
    void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const final;
    void ResetToDefaultConfiguration(void) final;
    void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary) final;

    // Command Request Handler Trampolines

    static void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status DoRequestHandlers(const bool &aRegister);



    // Command Completion Handlers

    void QueryRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SetDisabledRequestReceivedHandler(Server::ConnectionBasis &aConnection, const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORINFRAREDCONTROLLER_HPP
