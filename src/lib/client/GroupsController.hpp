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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a collection of HLX groups.
 *
 */

#ifndef OPENHLXCLIENTGROUPSCONTROLLER_HPP
#define OPENHLXCLIENTGROUPSCONTROLLER_HPP

#include <stddef.h>
#include <stdint.h>

#include <OpenHLX/Client/GroupsControllerBasis.hpp>
#include <OpenHLX/Client/GroupsControllerCommands.hpp>
#include <OpenHLX/Common/GroupsControllerBasis.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a collection of HLX groups.
 *
 *  @ingroup client
 *  @ingroup group
 *
 */
class GroupsController :
    public Common::GroupsControllerBasis,
    public Client::GroupsControllerBasis
{
public:
    GroupsController(void);
    virtual ~GroupsController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status LookupIdentifier(const char *aName, IdentifierType &aGroupIdentifier) const;

    // Mutator Methods

    Common::Status ClearMute(const IdentifierType &aGroupIdentifier);
    Common::Status SetMute(const IdentifierType &aGroupIdentifier);
    Common::Status SetMute(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::MuteType &aMute);
    Common::Status ToggleMute(const IdentifierType &aGroupIdentifier);

    Common::Status SetName(const IdentifierType &aGroupIdentifier, const char *aName);

    Common::Status SetSource(const IdentifierType &aGroupIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);

    Common::Status SetVolume(const IdentifierType &aGroupIdentifier, const Model::VolumeModel::LevelType &aVolume);
    Common::Status IncreaseVolume(const IdentifierType &aGroupIdentifier);
    Common::Status DecreaseVolume(const IdentifierType &aGroupIdentifier);

    Common::Status AddZone(const IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
    Common::Status RemoveZone(const IdentifierType &aGroupIdentifier, const Model::ZoneModel::IdentifierType &aZoneIdentifier);
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTGROUPSCONTROLLER_HPP
