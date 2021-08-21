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
 *      This file defines a base object for all server-side HLX
 *      controllers.
 *
 */

#ifndef OPENHLXSERVERCONTROLLERBASIS_HPP
#define OPENHLXSERVERCONTROLLERBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/CommandManagerDelegate.hpp>
#include <OpenHLX/Server/CommandRequestBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    A base object for all server-side HLX controllers.
 *
 *  @ingroup server
 *
 */
class ControllerBasis
{
public:
    virtual ~ControllerBasis(void);

    virtual Common::Status Init(CommandManager &aCommandManager);

protected:
    ControllerBasis(void);

    /**
     *  @brief
     *    A base object for registering server-side handlers for
     *    command requests.
     *
     *  @ingroup server
     *
     */
    struct RequestHandlerBasis
    {
        /**
         *  A reference to the expected server command request
         *  regular expression pattern that, when matched, should
         *  trigger invocation of the associated handler.
         *
         */
        Command::RequestBasis &                mRequest;
        /**
         *  The handler to invoke when the request pattern is matched
         *  on receipt of an unsolicited command request.
         *
         */
        CommandManager::OnRequestReceivedFunc  mOnRequestReceivedHandler;
    };

    Common::Status DoRequestHandlers(const RequestHandlerBasis *aFirst,
                                     const RequestHandlerBasis *aLast,
                                     void *aContext,
                                     const bool &aRegister);

    Common::Status SendResponse(ConnectionBasis &aConnection,
                                Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) const;

    Common::Status SendErrorResponse(ConnectionBasis &aConnection) const;
    Common::Status SendErrorResponse(ConnectionBasis &aConnection,
                                     Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

private:
    CommandManager *           mCommandManager;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERCONTROLLERBASIS_HPP
