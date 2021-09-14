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
 *      This file defines derivable objects for HLX client volume data
 *      model properties mutation command request buffers.
 *
 */

#ifndef OPENHLXCLIENTCOMMANDVOLUMEREQUESTBASIS_HPP
#define OPENHLXCLIENTCOMMANDVOLUMEREQUESTBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Client/CommandPropertyRequestBases.hpp>
#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandVolumeBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    A derivable object for a HLX client volume level data model
 *    property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeRequestBasis :
    public PropertyRequestBasis
{
protected:
    VolumeRequestBasis(void) = default;
    virtual ~VolumeRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

/**
 *  @brief
 *    A derivable object for a HLX client volume level increase data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeIncreaseRequestBasis :
    public VolumeRequestBasis
{
protected:
    VolumeIncreaseRequestBasis(void) = default;
    virtual ~VolumeIncreaseRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client volume level decrease data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeDecreaseRequestBasis :
    public VolumeRequestBasis
{
protected:
    VolumeDecreaseRequestBasis(void) = default;
    virtual ~VolumeDecreaseRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client volume level data model
 *    set property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeSetRequestBasis :
    public RequestBasis,
    public Common::Command::VolumeBufferBasis
{
protected:
    VolumeSetRequestBasis(void) = default;
    virtual ~VolumeSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const LevelType &aLevel);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client volume fixed/locked data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeFixedRequestBasis :
    public RequestBasis,
    public Common::Command::VolumeFixedBufferBasis
{
protected:
    VolumeFixedRequestBasis(void) = default;
    virtual ~VolumeFixedRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const FixedType &aFixed);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client volume mute data model
 *    property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteRequestBasis :
    public Command::RequestBasis,
    public Common::Command::VolumeMuteBufferBasis
{
protected:
    VolumeMuteRequestBasis(void) = default;
    virtual ~VolumeMuteRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client volume mute set/assert data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteSetRequestBasis :
    public VolumeMuteRequestBasis
{
protected:
    VolumeMuteSetRequestBasis(void) = default;
    virtual ~VolumeMuteSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client volume mute clear/deassert
 *    data model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteClearRequestBasis :
    public VolumeMuteRequestBasis
{
protected:
    VolumeMuteClearRequestBasis(void) = default;
    virtual ~VolumeMuteClearRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client volume mute toggle data
 *    model property mutation command request buffer.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteToggleRequestBasis :
    public VolumeMuteRequestBasis
{
protected:
    VolumeMuteToggleRequestBasis(void) = default;
    virtual ~VolumeMuteToggleRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const IdentifierType &aIdentifier);
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCOMMANDVOLUMEREQUESTBASIS_HPP
