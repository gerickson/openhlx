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
 *      This file defines a collection object for managing HLX group
 *      objects.
 *
 */

#ifndef OPENHLXMMODELGROUPSMODEL_HPP
#define OPENHLXMMODELGROUPSMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/GroupModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX group objects.
 *
 *  @ingroup model
 *
 */
class GroupsModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  group model.
     *
     */
    typedef GroupModel::IdentifierType IdentifierType;

public:
    GroupsModel(void);
    GroupsModel(const GroupsModel &aGroupsModel);
    virtual ~GroupsModel(void) = default;

    Common::Status Init(const IdentifierType &aGroupsMax);
    Common::Status Init(const GroupsModel &aGroupsModel);

    GroupsModel &operator =(const GroupsModel &aGroupsModel);

    Common::Status GetGroup(const IdentifierType &aGroupIdentifier, GroupModel *&aGroupModel);
    Common::Status GetGroup(const IdentifierType &aGroupIdentifier, const GroupModel *&aGroupModel) const;
    Common::Status GetGroup(const char *aName, const GroupModel *&aGroupModel) const;

    Common::Status SetGroup(const IdentifierType &aGroupIdentifier, const GroupModel &aGroupModel);

    bool operator ==(const GroupsModel &aGroupsModel) const;

private:
    Common::Status ValidateIdentifier(const IdentifierType &aGroupIdentifier) const;

private:
    typedef std::map<IdentifierType, GroupModel> Groups;

    IdentifierType  mGroupsMax;
    Groups          mGroups;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELGROUPSMODEL_HPP
