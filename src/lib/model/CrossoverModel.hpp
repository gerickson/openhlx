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
 *      This file defines an object for managing a HLX zone DSP sound
 *      model high- or lowpass crossover data model.
 *
 */

#ifndef OPENHLXMMODELCROSSOVERMODEL_HPP
#define OPENHLXMMODELCROSSOVERMODEL_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX zone DSP sound model high- or
 *    lowpass crossover data model.
 *
 *  @ingroup model
 *
 */
class CrossoverModel
{
public:
    /**
     *  A type for the filter crossover frequency, in Hertz.
     *
     */
    typedef uint16_t                         FrequencyType;

    /**
     *  @brief
     *    An object for specifying the lower and upper frequency limits of a HLX
     *    DSP sound model crossover filter.
     *
     *  @ingroup model
     *
     */
    struct FrequencyLimits
    {
        FrequencyType  mMin;  //!< The filter frequency minimum range, in Hertz.
        FrequencyType  mMax;  //!< The filter frequency maximum range, in Hertz.
    };

public:
    CrossoverModel(const FrequencyLimits &aFrequencyLimits);
    virtual ~CrossoverModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const FrequencyType &aFrequency);
    Common::Status Init(const CrossoverModel &aCrossoverModel);

    CrossoverModel &operator =(const CrossoverModel &aCrossoverModel);

    Common::Status GetFrequency(FrequencyType &aFrequency) const;
    Common::Status GetFrequencyLimits(FrequencyLimits &aFrequencyLimits) const;

    Common::Status SetFrequency(const FrequencyType &aFrequency);

    bool operator ==(const CrossoverModel &aCrossoverModel) const;

private:
    const FrequencyLimits  mFrequencyLimits;
    bool                   mFrequencyIsNull;
    FrequencyType          mFrequency;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELCROSSOVERMODEL_HPP
