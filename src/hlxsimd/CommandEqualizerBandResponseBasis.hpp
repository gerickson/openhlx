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
 *      This file defines a derived object for composing HLX server
 *      command response buffers for the equalizer band level data
 *      model property.
 *
 */

#ifndef HLXSERVERCOMMANDEQUALIZERBANDRESPONSEBASIS_HPP
#define HLXSERVERCOMMANDEQUALIZERBANDRESPONSEBASIS_HPP

#include <CommandResponseBasis.hpp>
#include <CommandEqualizerBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX server command response
 *    buffers for the equalizer band level data model property.
 *
 *  @ingroup server
 *  @ingroup command
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandResponseBasis :
    public ResponseBasis,
    public Common::Command::EqualizerBandBufferBasis
{
protected:
    EqualizerBandResponseBasis(void) = default;
    virtual ~EqualizerBandResponseBasis(void) = default;

    // Allow both the base and derived class initializers

    using ResponseBasis::Init;

    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const LevelType &aLevel);

private:
    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const char *aOperation);
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCOMMANDEQUALIZERBANDRESPONSEBASIS_HPP
