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
 *      This file defines derivable objects for HLX client equalizer
 *      band data model properties mutation command request buffers.
 *
 */

#ifndef OPENHLXCLIENTCOMMANDEQUALIZERBANDREQUESTBASIS_HPP
#define OPENHLXCLIENTCOMMANDEQUALIZERBANDREQUESTBASIS_HPP

#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Common/CommandEqualizerBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    A derivable object for a HLX client equalizer band level data
 *    model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandRequestBasis :
    public RequestBasis,
    public Common::Command::EqualizerBandBufferBasis
{
protected:
    EqualizerBandRequestBasis(void) = default;
    virtual ~EqualizerBandRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const char *aOperation);

private:
    // Explicitly hide base class initializers

    using RequestBasis::Init;
};

/**
 *  @brief
 *    A derivable object for a HLX client equalizer band level
 *    increase data model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandIncreaseRequestBasis :
    public EqualizerBandRequestBasis
{
protected:
    EqualizerBandIncreaseRequestBasis(void) = default;
    virtual ~EqualizerBandIncreaseRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client equalizer band level
 *    decrease data model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandDecreaseRequestBasis :
    public EqualizerBandRequestBasis
{
protected:
    EqualizerBandDecreaseRequestBasis(void) = default;
    virtual ~EqualizerBandDecreaseRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier);
};

/**
 *  @brief
 *    A derivable object for a HLX client equalizer band level set
 *    data model property mutation command request buffers.
 *
 *  @ingroup client
 *  @ingroup command
 *  @ingroup equalizer-band
 *
 */
class EqualizerBandSetRequestBasis :
    public EqualizerBandRequestBasis
{
protected:
    EqualizerBandSetRequestBasis(void) = default;
    virtual ~EqualizerBandSetRequestBasis(void) = default;

    Common::Status Init(const char *aObject, const Model::EqualizerBandsModel::IdentifierType &aEqualizerIdentifier, const Model::EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const Model::EqualizerBandModel::LevelType &aLevel);
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCOMMANDEQUALIZERBANDREQUESTBASIS_HPP
