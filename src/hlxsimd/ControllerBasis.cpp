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
 *      This file implements a base object for all server-side HLX
 *      controllers.
 *
 */

#include "ControllerBasis.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

#include <CommandManager.hpp>
#include <ControllerBasisDelegate.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Server
{

ControllerBasis :: ControllerBasis(void) :
    mDelegate(nullptr),
    mCommandManager(nullptr)
{
    return;
}

ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

Status ControllerBasis :: Init(CommandManager &aCommandManager)
{
    Status lRetval = kStatus_Success;


    lRetval = Init(aCommandManager, kTimeoutDefault);

    return (lRetval);
}

Status ControllerBasis :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;


    (void)aTimeout;

    mCommandManager = &aCommandManager;

    return (lRetval);
}

Status ControllerBasis :: DoRequestHandlers(const RequestHandlerBasis *aFirstRequestHandler, const RequestHandlerBasis *aLastRequestHandler, const bool &aRegister)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr,      done, lRetval = kError_NotInitialized);
    nlREQUIRE_ACTION(aFirstRequestHandler != nullptr, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aLastRequestHandler  != nullptr, done, lRetval = -EINVAL);

    while (aFirstRequestHandler != aLastRequestHandler)
    {
        if (aRegister)
        {
            lRetval = mCommandManager->RegisterRequestHandler(aFirstRequestHandler->mRequest, this, aFirstRequestHandler->mOnRequestReceivedHandler);
            nlREQUIRE_SUCCESS(lRetval, done);
        }
        else
        {
            lRetval = mCommandManager->UnregisterRequestHandler(aFirstRequestHandler->mRequest, this);
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        ++aFirstRequestHandler;
    }

 done:
    return (lRetval);
}

ControllerBasisDelegate * ControllerBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

Status ControllerBasis :: SetDelegate(ControllerBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

Status ControllerBasis :: SendResponse(ConnectionBasis &aConnection, ConnectionBuffer::ImmutableCountedPointer aBuffer) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendErrorResponse(aConnection);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status ControllerBasis :: SendErrorResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(mCommandManager != nullptr, done, lRetval = kError_NotInitialized);

    lRetval = mCommandManager->SendErrorResponse(aConnection, aBuffer);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

void ControllerBasis :: OnConfigurationIsDirty(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ControllerConfigurationIsDirty(*this);
    }
}

// MARK: Configuration Management Methods

Status ControllerBasis :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

 done:
    return (lRetval);
}

void ControllerBasis :: QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;
    (void)aBuffer;

    return;
}

void ControllerBasis :: ResetToDefaultConfiguration(void)
{
    return;
}

void ControllerBasis :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    (void)aBackupDictionary;

    return;
}

}; // namespace Server

}; // namespace HLX
