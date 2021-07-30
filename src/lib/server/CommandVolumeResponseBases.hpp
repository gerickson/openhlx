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
 *      This file defines derivable objects for HLX server volume
 *      data model properties mutation command response buffers.
 *
 */

#ifndef HLXSERVERCOMMANDVOLUMERESPONSEBASIS_HPP
#define HLXSERVERCOMMANDVOLUMERESPONSEBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/CommandVolumeBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>
#include <OpenHLX/Server/CommandResponseBasis.hpp>


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    A derivable object for a HLX server volume level data model set
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeResponseBasis :
    public ResponseBasis,
    public Common::Command::VolumeBufferBasis
{
protected:
    VolumeResponseBasis(void) = default;
    virtual ~VolumeResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const LevelType &aVolume);
};

/**
 *  @brief
 *    A derivable object for a HLX server all-objects volume level
 *    data model set property mutation command request regular
 *    expression.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeAllResponseBasis :
    public ResponseBasis,
    public Common::Command::VolumeAllBufferBasis
{
protected:
    VolumeAllResponseBasis(void) = default;
    virtual ~VolumeAllResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const LevelType &aVolume);
};

/**
 *  @brief
 *    A derivable object for a HLX server volume fixed/locked data
 *    model set property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeFixedResponseBasis :
    public ResponseBasis,
    public Common::Command::VolumeFixedBufferBasis
{
protected:
    VolumeFixedResponseBasis(void) = default;
    virtual ~VolumeFixedResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const FixedType &aVolumeFixed);
};

/**
 *  @brief
 *    A derivable object for a HLX server volume mute data model set
 *    property mutation command response buffer.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteResponseBasis :
    public ResponseBasis,
    public Common::Command::VolumeMuteBufferBasis
{
protected:
    VolumeMuteResponseBasis(void) = default;
    virtual ~VolumeMuteResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const MuteType &aMute);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDVOLUMERESPONSEBASIS_HPP
