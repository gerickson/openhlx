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
 *      This file defines an object for representing default data for
 *      a HLX DSP equalizer band data model.
 *
 */

#ifndef OPENHLXSIMULATOREQUALIZERBANDMODELDEFAULTS_HPP
#define OPENHLXSIMULATOREQUALIZERBANDMODELDEFAULTS_HPP

#include <OpenHLX/Model/EqualizerBandModel.hpp>


namespace HLX
{

namespace Simulator
{

namespace Defaults
{

/**
 *  @brief
 *    An object for representing default data for a HLX DSP equalizer
 *    band data model.
 *
 *  @private
 *
 */
struct EqualizerBandModel
{
    Model::EqualizerBandModel::LevelType mLevel;
};

}; // namespace Defaults

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATOREQUALIZERBANDMODELDEFAULTS_HPP
