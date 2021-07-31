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

#ifndef HLXSIMULATORCONTROLLERBASIS_HPP
#define HLXSIMULATORCONTROLLERBASIS_HPP

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Server/ControllerBasis.hpp>


namespace HLX
{

namespace Simulator
{

class ControllerBasisDelegate;

/**
 *  @brief
 *    A base object for....
 *
 *  @ingroup simulator
 *
 */
class ControllerBasis :
    public Server::ControllerBasis
{
public:
    Common::Status SetDelegate(ControllerBasisDelegate *aDelegate);
    ControllerBasisDelegate *GetDelegate(void) const;

    // Configuration Management Methods

    virtual Common::Status LoadFromBackupConfiguration(CFDictionaryRef aBackupDictionary);
    virtual void QueryCurrentConfiguration(Server::ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;
    virtual void ResetToDefaultConfiguration(void);
    virtual void SaveToBackupConfiguration(CFMutableDictionaryRef aBackupDictionary);

    void OnConfigurationIsDirty(void);

protected:
    ControllerBasis(void);
    virtual ~ControllerBasis(void);

private:
    ControllerBasisDelegate *  mDelegate;
};

}; // namespace Simulator

}; // namespace HLX

#endif // HLXSIMULATORCONTROLLERBASIS_HPP
