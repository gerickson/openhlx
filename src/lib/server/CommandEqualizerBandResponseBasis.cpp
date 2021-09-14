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
 *      This file implements a derived object for composing HLX server
 *      command response buffers for the equalizer band level data
 *      model property.
 *
 */

#include "CommandEqualizerBandResponseBasis.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBuffer.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

/**
 *  Initialize a HLX server command response buffer for an operation
 *  on an equalizer band data model that is a subset of a parent data
 *  model object, such as an equalizer preset or zone.
 *
 *  @param[in]  aObject                   A null-terminated C string
 *                                        representing the parent
 *                                        equalizer object class to
 *                                        which the equalizer band
 *                                        belongs.
 *  @param[in]  aEqualizerIdentifier      The identifier of the parent
 *                                        equalizer object to which
 *                                        the equalizer band belongs.
 *  @param[in]  aEqualizerBandIdentifier  The identifier of the equalizer
 *                                        band the operation is to be
 *                                        performed on.
 *  @param[in]  aOperation                The operation to perform on the
 *                                        equalizer band.
 *
 *  @retval kStatus_Success if successful.
 *  @retval -ENOMEM if there was insufficient memory.
 *
 */
Status
EqualizerBandResponseBasis :: Init(const char * aObject,
                                   const Model::EqualizerBandsModel::IdentifierType & aEqualizerIdentifier,
                                   const Model::EqualizerBandModel::IdentifierType & aEqualizerBandIdentifier,
                                   const char * aOperation)
{
    return (EqualizerBandBufferBasis::Init(*this,
                                           aObject,
                                           aEqualizerIdentifier,
                                           aEqualizerBandIdentifier,
                                           aOperation));
}

/**
 *  Initialize a HLX server command response buffer for an operation
 *  on an equalizer band data model level property that is a subset of
 *  a parent data model object, such as an equalizer preset or zone.
 *
 *  @param[in]  aObject                   A null-terminated C string
 *                                        representing the parent
 *                                        equalizer object class to
 *                                        which the equalizer band
 *                                        belongs.
 *  @param[in]  aEqualizerIdentifier      The identifier of the parent
 *                                        equalizer object to which
 *                                        the equalizer band belongs.
 *  @param[in]  aEqualizerBandIdentifier  The identifier of the equalizer
 *                                        band the operation is to be
 *                                        performed on.
 *  @param[in]  aLevel                    The equalizer band level.
 *
 *  @retval kStatus_Success if successful.
 *  @retval -ENOMEM if there was insufficient memory.
 *
 */
Status
EqualizerBandResponseBasis :: Init(const char * aObject,
                                   const Model::EqualizerBandsModel::IdentifierType & aEqualizerIdentifier,
                                   const Model::EqualizerBandModel::IdentifierType &  aEqualizerBandIdentifier,
                                   const LevelType &       aLevel)
{
    return (EqualizerBandBufferBasis::Init(*this,
                                           aObject,
                                           aEqualizerIdentifier,
                                           aEqualizerBandIdentifier,
                                           aLevel));
}

}; // namespace Command

}; // namespace Server

}; // namespace HLX
