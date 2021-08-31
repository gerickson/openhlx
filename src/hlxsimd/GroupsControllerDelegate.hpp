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
 *      This file defines a delegate interface for the server
 *      group controller object.
 *
 */

#ifndef OPENHLXSIMULATORGROUPSCONTROLLERDELEGATE_HPP
#define OPENHLXSIMULATORGROUPSCONTROLLERDELEGATE_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>

#include "ObjectControllerBasisDelegate.hpp"


namespace HLX
{

namespace Simulator
{

class GroupsController;
class ConnectionBasis;

/**
 *  @brief
 *    This file defines a delegate interface for the server group
 *    controller object.
 *
 *  This delegate interface allows an interested delegate to receive
 *  notifications regarding group state changes that require actions
 *  to be taken elsewhere, including:
 *
 *    - Adjusting or setting the group volume level property.
 *    - Setting or toggling the group volume mute property.
 *    - Setting the group source property.
 *
 *  @ingroup server
 *  @ingroup group
 */
class GroupsControllerDelegate
{
public:
    GroupsControllerDelegate(void) = default;
    virtual ~GroupsControllerDelegate(void) = default;

    virtual Common::Status ShouldAdjustVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment) = 0;
    virtual Common::Status ShouldSetMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute) = 0;
    virtual Common::Status ShouldSetSource(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier) = 0;
    virtual Common::Status ShouldSetVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume) = 0;
    virtual Common::Status ShouldToggleMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel) = 0;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATORGROUPSCONTROLLERDELEGATE_HPP
