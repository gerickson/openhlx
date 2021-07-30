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
 *      This file defines an object for....
 *
 */

#ifndef OPENHLXPROXYZONESCONTROLLER_HPP
#define OPENHLXPROXYZONESCONTROLLER_HPP
#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/ZonesControllerBasis.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup proxy
 *  @ingroup zone
 *
 */
class ZonesController :
    public ControllerBasis,
    public Common::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    Common::Status Init(Client::CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) /* final */;

    // Server-facing Client Observer Methods

    Common::Status Query(void);
    Common::Status Query(const IdentifierType &aZoneIdentifier);

    Common::Status GetZonesMax(size_t &aZones) const;

    Common::Status GetZone(const IdentifierType &aIdentifier, const Model::ZoneModel *&aModel) const;

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aZoneIdentifier) const;

    // Server-facing Client Mutator Methods

    Common::Status SetVolume(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aLevel);
    Common::Status IncreaseVolume(const IdentifierType &aZoneIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aZoneIdentifier);


    // Server-facing Client Command Completion Handler Trampolines

    static void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Server-facing Client Notification Handler Trampolines

    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

private:
    Common::Status ResponseInit(void);
    Common::Status DoNotificationHandlers(const bool &aRegister);

    // Server-facing Client Command Completion Handlers

    void QueryCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Client::Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Server-facing Client Notification Handlers

    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    void HandleVolumeChange(const IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);

private:
    size_t                                            mZonesDidRefreshCount;
    Model::ZonesModel                                 mZones;

private:
    static Client::Command::Zones::VolumeResponse     kVolumeResponse;
};

}; // namespace Proxy

}; // namespace HLX

#endif // HLXPROXYZONESCONTROLLER_HPP
