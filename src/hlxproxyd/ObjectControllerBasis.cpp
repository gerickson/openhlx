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

#include "ObjectControllerBasis.hpp"

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
        Server::ConnectionBasis *  mClientConnection;
        const uint8_t *            mRequestBuffer;
        size_t                     mRequestSize;
        RegularExpression::Matches mServerMatches;
        Client::CommandManager::OnCommandCompleteFunc mOnCommandCompleteHandler;
        Client::CommandManager::OnCommandErrorFunc    mOnCommandErrorHandler;
        Server::CommandManager::OnRequestReceivedFunc mOnRequestReceivedHandler;
        void *                                        mTheirClientContext;
        void *                                        mTheirServerContext;
        void *                                        mOurContext;
    };
}

ObjectControllerBasis :: ObjectControllerBasis(void) :
    mClientCommandManager(nullptr),
    mServerCommandManager(nullptr),
    mTimeout()
{
    return;
}

ObjectControllerBasis :: ~ObjectControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

Status
ObjectControllerBasis :: Init(Client::CommandManager &aClientCommandManager, Server::CommandManager &aServerCommandManager, const Common::Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    mClientCommandManager = &aClientCommandManager;
    mServerCommandManager = &aServerCommandManager;
    mTimeout              = aTimeout;

    return (lRetval);
}

// MARK: Configuration Management Methods

Status
ObjectControllerBasis :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Status lRetval = kStatus_Success;

    (void)aConnection;
    (void)aBuffer;

    return (lRetval);
}

// MARK: Command Proxying Methods

Status
ObjectControllerBasis :: ProxyMutationCommand(Server::ConnectionBasis &aClientConnection,
                                        const uint8_t *aRequestBuffer,
                                        const size_t &aRequestSize,
                                        const Common::RegularExpression::Matches &aServerMatches,
                                        const Client::Command::ResponseBasis &aExpectedResponse,
                                        Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                        Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                        void *aContext)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    std::unique_ptr<Detail::ProxyContext>                 lProxyContext;
    Status                                                lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aRequestBuffer != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aRequestSize > 0, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandCompleteHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandErrorHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aContext != nullptr, done, lRetval = -EINVAL);

    lProxyContext.reset(new Detail::ProxyContext());
    nlREQUIRE_ACTION(lProxyContext, done, lRetval = -ENOMEM);

    lProxyContext->mClientConnection         = &aClientConnection;
    lProxyContext->mRequestBuffer            = aRequestBuffer;
    lProxyContext->mRequestSize              = aRequestSize;
    lProxyContext->mServerMatches            = aServerMatches;
    lProxyContext->mOnCommandCompleteHandler = aOnCommandCompleteHandler;
    lProxyContext->mOnCommandErrorHandler    = aOnCommandErrorHandler;
    lProxyContext->mOnRequestReceivedHandler = nullptr;
    lProxyContext->mTheirClientContext       = aContext;
    lProxyContext->mTheirServerContext       = nullptr;
    lProxyContext->mOurContext               = this;

    lCommand.reset(new Proxy::Command::Proxy());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Proxy::Command::Proxy>(lCommand)->Init(aRequestBuffer, aRequestSize, aExpectedResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientCommandManager->SendCommand(lCommand,
                                                 mTimeout,
                                                 ObjectControllerBasis::ProxyMutationCompleteHandler,
                                                 ObjectControllerBasis::ProxyErrorHandler,
                                                 lProxyContext.release());
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
ObjectControllerBasis :: ProxyObservationCommand(Server::ConnectionBasis &aClientConnection,
                                           const uint8_t *aRequestBuffer,
                                           const size_t &aRequestSize,
                                           const Common::RegularExpression::Matches &aServerMatches,
                                           const Client::Command::ResponseBasis &aExpectedResponse,
                                           Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                           Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                           Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                           void *aClientContext,
                                           void *aServerContext)
{
    Client::Command::ExchangeBasis::MutableCountedPointer lCommand;
    std::unique_ptr<Detail::ProxyContext>                 lProxyContext;
    Status                                                lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aRequestBuffer != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aRequestSize > 0, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandCompleteHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnCommandErrorHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aOnRequestReceivedHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aClientContext != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aServerContext != nullptr, done, lRetval = -EINVAL);

    lProxyContext.reset(new Detail::ProxyContext());
    nlREQUIRE_ACTION(lProxyContext, done, lRetval = -ENOMEM);

    lProxyContext->mClientConnection         = &aClientConnection;
    lProxyContext->mRequestBuffer            = aRequestBuffer;
    lProxyContext->mRequestSize              = aRequestSize;
    lProxyContext->mServerMatches            = aServerMatches;
    lProxyContext->mOnCommandCompleteHandler = aOnCommandCompleteHandler;
    lProxyContext->mOnCommandErrorHandler    = aOnCommandErrorHandler;
    lProxyContext->mOnRequestReceivedHandler = aOnRequestReceivedHandler;
    lProxyContext->mTheirClientContext       = aClientContext;
    lProxyContext->mTheirServerContext       = aServerContext;
    lProxyContext->mOurContext               = this;

    lCommand.reset(new Proxy::Command::Proxy());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Proxy::Command::Proxy>(lCommand)->Init(aRequestBuffer, aRequestSize, aExpectedResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mClientCommandManager->SendCommand(lCommand,
                                                 mTimeout,
                                                 ObjectControllerBasis::ProxyObservationCompleteHandler,
                                                 ObjectControllerBasis::ProxyErrorHandler,
                                                 lProxyContext.release());
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Notification Proxy Methods

Status
ObjectControllerBasis :: ProxyNotification(const uint8_t *aNotificationBuffer,
                                     const size_t &aNotificationSize,
                                     const Common::RegularExpression::Matches &aNotificationMatches,
                                     Client::CommandManager::OnNotificationReceivedFunc  aOnNotificationReceivedHandler,
                                     void *aClientContext)
{
    ConnectionBuffer::MutableCountedPointer  lResponseBuffer;
    uint8_t *                                lResult;
    Status                                   lRetval;


    aOnNotificationReceivedHandler(aNotificationBuffer,
                                   aNotificationSize,
                                   aNotificationMatches,
                                   aClientContext);

    // Allocate a buffer and put the notification contents into it and
    // send it to all subscribed clients.

    lResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lResponseBuffer, done, lRetval = -ENOMEM);

    lRetval = lResponseBuffer->Init(const_cast<uint8_t *>(aNotificationBuffer), aNotificationSize);
    nlREQUIRE_SUCCESS(lRetval, done);

    lResult = lResponseBuffer->Put(aNotificationSize);
    nlREQUIRE_ACTION(lResult != nullptr, done, lRetval = -ENOSPC);

    lRetval = mServerCommandManager->SendResponse(lResponseBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Command Proxy Handlers

void
ObjectControllerBasis :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                     const Common::Error &aClientError,
                                     Server::ConnectionBasis &aClientConnection,
                                     Client::CommandManager::OnCommandErrorFunc aOnCommandErrorHandler,
                                     void * aContext)
{
    Status lStatus;

    aOnCommandErrorHandler(aClientExchange, aClientError, aContext);

    lStatus = mServerCommandManager->SendErrorResponse(aClientConnection);
    nlVERIFY_SUCCESS(lStatus);
}

void
ObjectControllerBasis :: ProxyObservationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                                   const Common::RegularExpression::Matches &aClientMatches,
                                                   Server::ConnectionBasis &aClientConnection,
                                                   const uint8_t *aRequestBuffer,
                                                   const size_t &aRequestSize,
                                                   const Common::RegularExpression::Matches &aServerMatches,
                                                   Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                                   Server::CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler,
                                                   void * aClientContext,
                                                   void * aServerContext)
{
    aOnCommandCompleteHandler(aClientExchange,
                              aClientMatches,
                              aClientContext);

    aOnRequestReceivedHandler(aClientConnection,
                              aRequestBuffer,
                              aRequestSize,
                              aServerMatches,
                              aServerContext);
}

void
ObjectControllerBasis :: ProxyMutationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                                const Common::RegularExpression::Matches &aClientMatches,
                                                Server::ConnectionBasis &aClientConnection,
                                                const uint8_t *aRequestBuffer,
                                                const size_t &aRequestSize,
                                                const Common::RegularExpression::Matches &aServerMatches,
                                                Client::CommandManager::OnCommandCompleteFunc aOnCommandCompleteHandler,
                                                void * aContext)
{
    const Client::Command::ResponseBasis *  lServerResponse           = aClientExchange->GetResponse();
    const uint8_t *                         lServerResponseBuffer     = lServerResponse->GetBuffer()->GetHead();
    const size_t                            lServerResponseBufferSize = lServerResponse->GetBuffer()->GetSize();
    ConnectionBuffer::MutableCountedPointer lProxyResponseBuffer;
    uint8_t *                               lResult;
    Status                                  lStatus;

    (void)aRequestBuffer;
    (void)aRequestSize;
    (void)aServerMatches;

    aOnCommandCompleteHandler(aClientExchange,
                              aClientMatches,
                              aContext);

    lProxyResponseBuffer.reset(new ConnectionBuffer);
    nlREQUIRE_ACTION(lProxyResponseBuffer, done, lStatus = -ENOMEM);

    lStatus = lProxyResponseBuffer->Init(const_cast<uint8_t *>(lServerResponseBuffer),
                                         lServerResponseBufferSize);
    nlREQUIRE_SUCCESS(lStatus, done);

    lResult = lProxyResponseBuffer->Put(lServerResponseBufferSize);
    nlREQUIRE_ACTION(lResult != nullptr, done, lStatus = -ENOSPC);

 done:
    lStatus = mServerCommandManager->SendResponse(aClientConnection, lProxyResponseBuffer);
    nlREQUIRE_SUCCESS(lStatus, exit);

 exit:
    if (lStatus < kStatus_Success)
    {
        lStatus = mServerCommandManager->SendErrorResponse(aClientConnection);
        nlVERIFY_SUCCESS(lStatus);
    }

    return;
}

// MARK: Proxy Handler Trampolines

/* static */ void
ObjectControllerBasis :: ProxyErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                     const Common::Error &aClientError,
                                     void *aContext)
{
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ObjectControllerBasis *lController = static_cast<ObjectControllerBasis *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyErrorHandler(aClientExchange,
                                           aClientError,
                                           *lContext->mClientConnection,
                                           lContext->mOnCommandErrorHandler,
                                           lContext->mTheirClientContext);
        }

        delete lContext;
    }
}

/* static */ void
ObjectControllerBasis :: ProxyObservationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                                   const Common::RegularExpression::Matches &aClientMatches,
                                                   void *aContext)
{
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ObjectControllerBasis *lController = static_cast<ObjectControllerBasis *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyObservationCompleteHandler(aClientExchange,
                                                         aClientMatches,
                                                         *lContext->mClientConnection,
                                                         lContext->mRequestBuffer,
                                                         lContext->mRequestSize,
                                                         lContext->mServerMatches,
                                                         lContext->mOnCommandCompleteHandler,
                                                         lContext->mOnRequestReceivedHandler,
                                                         lContext->mTheirClientContext,
                                                         lContext->mTheirServerContext);
        }

        delete lContext;
    }
}

/* static */ void
ObjectControllerBasis :: ProxyMutationCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aClientExchange,
                                                const Common::RegularExpression::Matches &aClientMatches,
                                                void *aContext)
{
    Detail::ProxyContext *lContext = static_cast<Detail::ProxyContext *>(aContext);

    if (lContext != nullptr)
    {
        ObjectControllerBasis *lController = static_cast<ObjectControllerBasis *>(lContext->mOurContext);

        if (lController != nullptr)
        {
            lController->ProxyMutationCompleteHandler(aClientExchange,
                                                      aClientMatches,
                                                      *lContext->mClientConnection,
                                                      lContext->mRequestBuffer,
                                                      lContext->mRequestSize,
                                                      lContext->mServerMatches,
                                                      lContext->mOnCommandCompleteHandler,
                                                      lContext->mTheirClientContext);
        }

        delete lContext;
    }
}

}; // namespace Proxy

}; // namespace HLX
