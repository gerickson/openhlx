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
 *      equalizer band objects.
 *
 */

#ifndef OPENHLXMMODELEQUALIZERBANDSMODEL_HPP
#define OPENHLXMMODELEQUALIZERBANDSMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Model/EqualizerBandModel.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX equalizer band objects.
 *
 *  @ingroup model
 *
 */
class EqualizerBandsModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  equalizer band model.
     *
     */
    typedef EqualizerBandModel::IdentifierType IdentifierType;

public:
    /**
     *  The maximum number of allowed equalizer bands in the model.
     *
     */
    static constexpr size_t kEqualizerBandsMax = 10;

public:
    EqualizerBandsModel(void) = default;
    EqualizerBandsModel(const EqualizerBandsModel &aEqualizerBandsModel);
    virtual ~EqualizerBandsModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const EqualizerBandsModel &aEqualizerBandsModel);

    EqualizerBandsModel &operator =(const EqualizerBandsModel &aEqualizerBandsModel);

    static Common::Status ValidateIdentifier(const IdentifierType &aEqualizerBandIdentifier);
    static bool IsValidIdentifier(const IdentifierType &aEqualizerBandIdentifier);

    Common::Status GetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel);
    Common::Status GetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const;

    Common::Status SetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel &aEqualizerBandModel);

    bool operator ==(const EqualizerBandsModel &aEqualizerBandsModel) const;

private:
    typedef std::map<IdentifierType, EqualizerBandModel> EqualizerBands;

    EqualizerBands  mEqualizerBands;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELEQUALIZERBANDSMODEL_HPP
