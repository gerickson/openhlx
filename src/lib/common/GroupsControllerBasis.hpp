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
 *      This file defines a base object for realizing a HLX groups
 *      controller, either in a client or server context.
 *
 */

#ifndef HLXCOMMONGROUPSCONTROLLERBASIS_HPP
#define HLXCOMMONGROUPSCONTROLLERBASIS_HPP

#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Model/GroupsModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX groups controller, either in a
 *    client or server context.
 *
 *  @ingroup common
 *
 */
class GroupsControllerBasis
{
public:
    /**
     *  A locally-scoped convenience type for a group identifier.
     *
     */
    typedef Model::GroupModel::IdentifierType IdentifierType;

public:
    virtual ~GroupsControllerBasis(void) = default;

    // Observer Methods

    static bool           IsValidIdentifier(const IdentifierType &aGroupIdentifier);
    static Common::Status ValidateIdentifier(const IdentifierType &aGroupIdentifier);

protected:
    GroupsControllerBasis(void) = default;

protected:
    Model::GroupsModel           mGroups;

protected:
    static const IdentifierType  kGroupsMax;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONGROUPSCONTROLLERBASIS_HPP
