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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP
#define OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/GroupsControllerCommands.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup group
 *
 */
class GroupsControllerBasis
{
public:
    virtual ~GroupsControllerBasis(void);

protected:
    GroupsControllerBasis(void);

    Common::Status Init(void);

private:
    Common::Status ResponseInit(void);

protected:
    size_t                                          mGroupsDidRefreshCount;

protected:
    static Command::Groups::SetMuteResponse         kSetMuteResponse;
    static Command::Groups::NameResponse            kNameResponse;
    static Command::Groups::QueryResponse           kQueryResponse;
    static Command::Groups::SourceResponse          kSourceResponse;
    static Command::Groups::SetVolumeResponse       kSetVolumeResponse;
    static Command::Groups::ZoneResponse            kZoneResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTGROUPSCONTROLLERBASIS_HPP
