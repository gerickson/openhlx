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
 *      This file implements a base object for....
 *
 */

#include "ControllerBasis.hpp"

#include <memory>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>

#include "ProxyCommand.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Proxy
{

namespace Detail
{
    // XXX - Need to figure out how to make the lifetime of this
    // persist across multiple, potentially-failed proxy requests and
    // how to limit the number of proxy requests since infinite loops
    // may be introduced.

    struct ProxyContext
    {
        Server::ConnectionBasis *  mConnection;
        const uint8_t *            mBuffer;
        size_t                     mSize;
        RegularExpression::Matches mMatches;
        Client::CommandManager::OnCommandCompleteFunc mOnCommandCompleteHandler;
        Client::CommandManager::OnCommandErrorFunc    mOnCommandErrorHandler;
        Server::CommandManager::OnRequestReceivedFunc mOnRequestReceivedHandler;
        void *                                        mTheirContext;
        void *                                        mOurContext;
    };
}

ControllerBasis :: ControllerBasis(void) :
    Client::ControllerBasis(),
    Server::ControllerBasis()
{
    return;
}

ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

Status
ControllerBasis :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = Init(aClientCommandManager, aServerCommandManager, kTimeoutDefault);

    return (lRetval);
}

Status
ControllerBasis :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = Client::ControllerBasis::Init(aClientCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Server::ControllerBasis::Init(aServerCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Configuration Management Methods

Status
ControllerBasis :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    (void)aConnection;
    (void)aBuffer;

    return (lRetval);
}

Status
ControllerBasis :: ProxyObservationCommand(Server::ConnectionBasis &aConnection,
                                           const uint8_t *aBuffer,
                                           const size_t &aSize,
                                           const Common::RegularExpression::Matches &aMatches,
                                           const Client::Command::ResponseBasis &aResponse,
                                           Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                           Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                           Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                           void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    std::unique_ptr<Detail::ProxyContext>                 lProxyContext;
    Status                                                lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBuffer != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aSize > 0, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandCompleteHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandErrorHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnRequestReceivedHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aContext != nullptr, done, lRetval = -EINVAL);

    lProxyContext.reset(new Detail::ProxyContext());
    nlREQUIRE_ACTION(lProxyContext, done, lRetval = -ENOMEM);

    lProxyContext->mConnection               = &aConnection;
    lProxyContext->mBuffer                   = aBuffer;
    lProxyContext->mSize                     = aSize;
    lProxyContext->mMatches                  = aMatches;
    lProxyContext->mOnCommandCompleteHandler = aOnCommandCompleteHandler;
    lProxyContext->mOnCommandErrorHandler    = aOnCommandErrorHandler;
    lProxyContext->mOnRequestReceivedHandler = aOnRequestReceivedHandler;
    lProxyContext->mTheirContext             = aContext;
    lProxyContext->mOurContext               = this;

    lCommand.reset(new Proxy::Command::Proxy());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Proxy::Command::Proxy>(lCommand)->Init(aBuffer, aSize, aResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          ControllerBasis::ProxyCompleteHandler,
                          ControllerBasis::ProxyErrorHandler,
                          lProxyContext.release());
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Proxy Handlers

void
ControllerBasis :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                     const Common::Error &aError,
                                     Server::ConnectionBasis &aConnection,
                                     Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                     void * aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lStatus;

    aOnCommandErrorHandler(aExchange, aError, aContext);

    lStatus = SendErrorResponse(aConnection);
    nlVERIFY_SUCCESS(lStatus);
}

void
ControllerBasis :: ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange,
                                        const Common::RegularExpression::Matches &aClientMatches,
                                        Server::ConnectionBasis &aConnection,
                                        const uint8_t *aBuffer,
                                        const size_t &aSize,
                                        const Common::RegularExpression::Matches &aServerMatches,
                                        Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                        Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                        void * aContext)
{
    DeclareScopedFunctionTracer(lTracer);

    aOnCommandCompleteHandler(aExchange, aClientMatches, aContext);

    aOnRequestReceivedHandler(aConnection, aBuffer, aSize, aServerMatches, aContext);
}

// MARK: Proxy Handler Trampolines

/* static */ void
ControllerBasis :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ControllerBasis *lController = static_cast<ControllerBasis *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyErrorHandler(aExchange,
                                           aError,
                                           *lContext->mConnection,
                                           lContext->mOnCommandErrorHandler,
                                           lContext->mTheirContext);
        }

        delete lContext;
    }
}

/* static */ void
ControllerBasis :: ProxyCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext)
{
    DeclareScopedFunctionTracer(lTracer);
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ControllerBasis *lController = static_cast<ControllerBasis *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyCompleteHandler(aExchange,
                                              aMatches,
                                              *lContext->mConnection,
                                              lContext->mBuffer,
                                              lContext->mSize,
                                              lContext->mMatches,
                                              lContext->mOnCommandCompleteHandler,
                                              lContext->mOnRequestReceivedHandler,
                                              lContext->mTheirContext);

        }

        delete lContext;
    }
}

}; // namespace Proxy

}; // namespace HLX
