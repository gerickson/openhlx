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
 *      This file defines base and derived objects for composing HLX
 *      commands that observe or set volume data model properties.
 *
 */

#ifndef HLXCOMMONCOMMANDVOLUMEBUFFERBASIS_HPP
#define HLXCOMMONCOMMANDVOLUMEBUFFERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

#include <CommandPropertyBufferBases.hpp>
#include <OpenHLX/Common/CommandBuffer.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/VolumeModel.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or mutate
 *    the volume data model level property for another collection
 *    object, such as a group or zone.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeBufferBasis :
    public PropertySetBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a LevelType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::LevelType           LevelType;

protected:
    VolumeBufferBasis(void) = default;
    ~VolumeBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const LevelType &aLevel);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or
 *    mutate the volume data model level property for all collection
 *    objects.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeAllBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a LevelType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::LevelType           LevelType;

protected:
    VolumeAllBufferBasis(void) = default;
    ~VolumeAllBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const LevelType &aLevel);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or
 *    mutate the volume data model fixed/locked property for another
 *    collection object, such as a group or zone.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeFixedBufferBasis :
    public PropertySetBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a FixedType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::FixedType           FixedType;

protected:
    VolumeFixedBufferBasis(void) = default;
    ~VolumeFixedBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const FixedType &aFixed);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or
 *    mutate the volume data model mute property for another
 *    collection object, such as a group or zone.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *
 */
class VolumeMuteBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a MuteType from the
     *  volume model.
     *
     */
    typedef Model::VolumeModel::MuteType            MuteType;

protected:
    VolumeMuteBufferBasis(void) = default;
    ~VolumeMuteBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const Model::IdentifierModel::IdentifierType &aIdentifier, const MuteType &aMute);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDVOLUMEBUFFERBASIS_HPP
