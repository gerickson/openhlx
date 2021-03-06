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
 *      This file implements a base object for....
 *
 */

#include "ObjectControllerBasis.hpp"

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Server/CommandManager.hpp>
#include <OpenHLX/Server/ConnectionManager.hpp>
#include <OpenHLX/Utilities/Assert.hpp>

#include "ObjectControllerBasisDelegate.hpp"


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

ObjectControllerBasis :: ObjectControllerBasis(void) :
    mDelegate(nullptr)
{
    return;
}

ObjectControllerBasis :: ~ObjectControllerBasis(void)
{
    return;
}

ObjectControllerBasisDelegate *
ObjectControllerBasis :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
ObjectControllerBasis :: SetDelegate(ObjectControllerBasisDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(aDelegate != mDelegate, done, lRetval = kStatus_ValueAlreadySet);

    mDelegate = aDelegate;

 done:
    return (lRetval);
}

void
ObjectControllerBasis :: OnConfigurationIsDirty(void)
{
    if (mDelegate != nullptr)
    {
        mDelegate->ControllerConfigurationIsDirty(*this);
    }
}

// MARK: Configuration Management Methods

Status
ObjectControllerBasis :: LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

 done:
    return (lRetval);
}

void
ObjectControllerBasis :: QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    (void)aConnection;
    (void)aBuffer;

    return;
}

void
ObjectControllerBasis :: ResetToDefaultConfiguration(void)
{
    return;
}

void
ObjectControllerBasis :: SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary)
{
    (void)aBackupDictionary;

    return;
}

}; // namespace Simulator

}; // namespace HLX
