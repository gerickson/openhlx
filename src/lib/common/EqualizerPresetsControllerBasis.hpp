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
 *      This file defines a base object for realizing a HLX equalizer
 *      presets controller, either in a client or server context.
 *
 */

#ifndef HLXCOMMONEQUALIZERPRESETSCONTROLLERBASIS_HPP
#define HLXCOMMONEQUALIZERPRESETSCONTROLLERBASIS_HPP

#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Model/EqualizerPresetsModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX equalizer presets controller,
 *    either in a client or server context.
 *
 *  @ingroup common
 *
 */
class EqualizerPresetsControllerBasis
{
public:
    /**
     *  A locally-scoped convenience type for an equalizer preset
     *  identifier.
     *
     */
    typedef Model::EqualizerPresetModel::IdentifierType IdentifierType;

public:
    virtual ~EqualizerPresetsControllerBasis(void) = default;

    // Observer Methods

    static bool           IsValidIdentifier(const IdentifierType &aEqualizerPresetIdentifier);
    static Common::Status ValidateIdentifier(const IdentifierType &aEqualizerPresetIdentifier);

protected:
    EqualizerPresetsControllerBasis(void) = default;

protected:
    Model::EqualizerPresetsModel mEqualizerPresets;

protected:
    static const IdentifierType  kEqualizerPresetsMax;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONEQUALIZERPRESETSCONTROLLERBASIS_HPP
