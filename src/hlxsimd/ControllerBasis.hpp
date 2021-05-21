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

#ifndef HLXSERVERCONTROLLERBASIS_HPP
#define HLXSERVERCONTROLLERBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include <CommandRequestBasis.hpp>
#include <CommandManager.hpp>
#include <CommandManagerDelegate.hpp>


namespace HLX
{

namespace Server
{

class ControllerBasisDelegate;

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
    virtual Common::Status Init(CommandManager &aCommandManager);
    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status SetDelegate(ControllerBasisDelegate *aDelegate);
    ControllerBasisDelegate *GetDelegate(void) const;

    Common::Status SendResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::ImmutableCountedPointer aBuffer) const;

    Common::Status SendErrorResponse(ConnectionBasis &aConnection) const;
    Common::Status SendErrorResponse(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

    // Configuration Management Methods

    virtual Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary);
    virtual void QueryCurrentConfiguration(ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    virtual void ResetToDefaultConfiguration(void);
    virtual void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary);

    void OnConfigurationIsDirty(void);

protected:
    ControllerBasis(void);
    ~ControllerBasis(void);

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
        Command::RequestBasis &                mRequest;
        CommandManager::OnRequestReceivedFunc  mOnRequestReceivedHandler;
    };

    Common::Status DoRequestHandlers(const RequestHandlerBasis *aFirst, const RequestHandlerBasis *aLast, const bool &aRegister);

private:
    ControllerBasisDelegate *  mDelegate;
    CommandManager *           mCommandManager;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONTROLLERBASIS_HPP
