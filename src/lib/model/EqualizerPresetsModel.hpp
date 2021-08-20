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
 *      This file defines a collection object for managing HLX
 *      equalizer preset objects.
 *
 */

#ifndef OPENHLXMMODELEQUALIZERPRESETSMODEL_HPP
#define OPENHLXMMODELEQUALIZERPRESETSMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX equalizer preset objects.
 *
 *  @ingroup model
 *
 */
class EqualizerPresetsModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  equalizer preset model.
     *
     */
    typedef EqualizerPresetModel::IdentifierType IdentifierType;

public:
    EqualizerPresetsModel(void) = default;
    EqualizerPresetsModel(const EqualizerPresetsModel &aEqualizerPresetsModel);
    virtual ~EqualizerPresetsModel(void) = default;

    Common::Status Init(const IdentifierType &aEqualizerPresetsMax);
    Common::Status Init(const EqualizerPresetsModel &aEqualizerPresetsModel);

    EqualizerPresetsModel &operator =(const EqualizerPresetsModel &aEqualizerPresetsModel);

    Common::Status GetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, EqualizerPresetModel *&aEqualizerPresetModel);
    Common::Status GetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerPresetModel *&aEqualizerPresetModel) const;
    Common::Status GetEqualizerPreset(const char *aName, const EqualizerPresetModel *&aEqualizerPresetModel) const;

    Common::Status SetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerPresetModel &aEqualizerPresetModel);

    bool operator ==(const EqualizerPresetsModel &aEqualizerPresetsModel) const;

private:
    Common::Status ValidateIdentifier(const IdentifierType &aEqualizerPresetIdentifier) const;

private:
    typedef std::map<IdentifierType, EqualizerPresetModel> EqualizerPresets;

    IdentifierType    mEqualizerPresetsMax;
    EqualizerPresets  mEqualizerPresets;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELEQUALIZERPRESETSMODEL_HPP
