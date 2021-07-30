/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      commands that observe or set HLX DSP equalizer data model
 *      properties.
 *
 */

#ifndef HLXCOMMONCOMMANDEQUALIZERBUFFERBASES_HPP
#define HLXCOMMONCOMMANDEQUALIZERBUFFERBASES_HPP

#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerBandsModel.hpp>
#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/CrossoverModel.hpp>
#include <OpenHLX/Model/SoundModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or mutate
 *    the equalizer band data model.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerBandBufferBasis :
    public PropertyBufferBasis
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
     *  equalizer band model.
     *
     */
    typedef Model::EqualizerBandModel::LevelType    LevelType;

protected:
    EqualizerBandBufferBasis(void) = default;
    ~EqualizerBandBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aEqualizerIdentifier, const IdentifierType &aEqualizerBandIdentifier, const LevelType &aLevel);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aEqualizerIdentifier, const IdentifierType &aEqualizerBandIdentifier, const char *aOperation);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate an
 *    equalizer crossover frequency data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerCrossoverBufferBasis :
    public PropertySetBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a FrequencyType from the
     *  crossover model.
     *
     */
    typedef Model::CrossoverModel::FrequencyType    FrequencyType;

    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

protected:
    EqualizerCrossoverBufferBasis(void) = default;
    virtual ~EqualizerCrossoverBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const char *aOperation, const FrequencyType &Frequency);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate the
 *    equalizer highpass crossover frequency data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerHighpassCrossoverBufferBasis :
    public EqualizerCrossoverBufferBasis
{
protected:
    EqualizerHighpassCrossoverBufferBasis(void) = default;
    virtual ~EqualizerHighpassCrossoverBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const FrequencyType &Frequency);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate the
 *    equalizer lowpass crossover frequency data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerLowpassCrossoverBufferBasis :
    public EqualizerCrossoverBufferBasis
{
protected:
    EqualizerLowpassCrossoverBufferBasis(void) = default;
    virtual ~EqualizerLowpassCrossoverBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const FrequencyType &Frequency);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate the
 *    equalizer preset data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerPresetBufferBasis :
    public PropertySetBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

protected:
    EqualizerPresetBufferBasis(void) = default;
    virtual ~EqualizerPresetBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const IdentifierType &aEqualizerPresetIdentifier);
};

/**
 *  @brief
 *    A derived object for composing HLX commands that mutate the
 *    equalizer sound mode data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class EqualizerSoundModeBufferBasis :
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
     *  Convenience type redeclaring @a SoundMode from the
     *  sound model.
     *
     */
    typedef Model::SoundModel::SoundMode            SoundMode;

protected:
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aObjectIdentifier, const SoundMode &aSoundMode);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDEQUALIZERBUFFERBASES_HPP
