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

#ifndef OPENHLXSERVERGROUPSCONTROLLERBASIS_HPP
#define OPENHLXSERVERGROUPSCONTROLLERBASIS_HPP

#include <OpenHLX/Server/GroupsControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup groups
 *
 */
class GroupsControllerBasis
{
public:
    virtual ~GroupsControllerBasis(void);

protected:
    GroupsControllerBasis(void);

    Common::Status Init(void);

protected:
    Common::Status RequestInit(void);

protected:
    static Server::Command::Groups::AddZoneRequest         kAddZoneRequest;
    static Server::Command::Groups::ClearZonesRequest      kClearZonesRequest;
    static Server::Command::Groups::DecreaseVolumeRequest  kDecreaseVolumeRequest;
    static Server::Command::Groups::IncreaseVolumeRequest  kIncreaseVolumeRequest;
    static Server::Command::Groups::MuteRequest            kMuteRequest;
    static Server::Command::Groups::QueryRequest           kQueryRequest;
    static Server::Command::Groups::RemoveZoneRequest      kRemoveZoneRequest;
    static Server::Command::Groups::SetNameRequest         kSetNameRequest;
    static Server::Command::Groups::SetSourceRequest       kSetSourceRequest;
    static Server::Command::Groups::SetVolumeRequest       kSetVolumeRequest;
    static Server::Command::Groups::ToggleMuteRequest      kToggleMuteRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERGROUPSCONTROLLERBASIS_HPP
