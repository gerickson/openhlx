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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a HLX infrared remote control
 *      interface.
 *
 */

#include "InfraredController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/InfraredControllerCommands.hpp>
#include <OpenHLX/Client/InfraredStateChangeNotifications.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
InfraredController :: InfraredController(void) :
    Common::InfraredControllerBasis(),
    Client::InfraredControllerBasis(Common::InfraredControllerBasis::mInfraredModel)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
InfraredController :: ~InfraredController(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
InfraredController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::InfraredControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::InfraredControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Disable Mutator Commands

/**
 *  @brief
 *    Set the infrared disabled state to the specified state.
 *
 *  This attempts to set the HLX infrared receiver disabled state to
 *  the specified state.
 *
 *  @param[in]  aDisabled  An immutable reference to the disabled
 *                         state to set.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 */
Status
InfraredController :: SetDisabled(const InfraredModel::DisabledType &aDisabled)
{
    Command::ExchangeBasis::MutableCountedPointer  lCommand;
    Status                                         lRetval = kStatus_Success;


    lCommand.reset(new Command::Infrared::SetDisabled());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::Infrared::SetDisabled>(lCommand)->Init(aDisabled);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::InfraredControllerBasis::SetDisabledCompleteHandler,
                          Client::InfraredControllerBasis::CommandErrorHandler,
                          static_cast<Client::InfraredControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Command Completion Handlers


}; // namespace Client

}; // namespace HLX
