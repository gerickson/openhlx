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
 *      This file implements derivable objects for HLX client equalizer
 *      band data model properties mutation command request buffers.
 *
 */

#include "CommandEqualizerBandRequestBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/CommandBuffer.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer band level mutation request
 *  operation (for example, increase) against specific equalizer and
 *  equalizer band identifiers.
 *
 *  @param[in]  aObject                   A pointer to a null-terminated
 *                                        C string representing the
 *                                        object for which the
 *                                        equalizer band mutation
 *                                        request is to be made
 *                                        against. For example, "O"
 *                                        for a zone object.
 *  @param[in]  aEqualizerIdentifier      A reference to the specific
 *                                        equalizer identifier which
 *                                        the band level mutation
 *                                        request is to be made
 *                                        against.
 *  @param[in]  aEqualizerBandIdentifier  A reference to the specific
 *                                        equalizer band identifier
 *                                        which the band level
 *                                        mutation request is to be
 *                                        made against.
 *  @param[in]  aOperation                A pointer to a null-terminated
 *                                        C string representing the
 *                                        operation to perform for the
 *                                        mutation request. This might
 *                                        be decrease, increase, or
 *                                        set to a specific level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandRequestBasis :: Init(const char *aObject,
                                  const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                  const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                  const char *aOperation)
{
    return (EqualizerBandBufferBasis::Init(*this,
                                           aObject,
                                           aEqualizerIdentifier,
                                           aEqualizerBandIdentifier,
                                           aOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer band level increase request
 *  operation against specific equalizer and equalizer band
 *  identifiers.
 *
 *  @param[in]  aObject                   A pointer to a null-terminated
 *                                        C string representing the
 *                                        object for which the
 *                                        equalizer band increase
 *                                        request is to be made
 *                                        against. For example, "O"
 *                                        for a zone object.
 *  @param[in]  aEqualizerIdentifier      A reference to the specific
 *                                        equalizer identifier which
 *                                        the band level increase
 *                                        request is to be made
 *                                        against.
 *  @param[in]  aEqualizerBandIdentifier  A reference to the specific
 *                                        equalizer band identifier
 *                                        which the band level
 *                                        increase request is to be
 *                                        made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandIncreaseRequestBasis :: Init(const char *aObject,
                                          const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                          const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    static const char * const kEqualizerBandIncreaseOperation = "U";

    return (EqualizerBandRequestBasis::Init(aObject,
                                            aEqualizerIdentifier,
                                            aEqualizerBandIdentifier,
                                            kEqualizerBandIncreaseOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer band level decrease request
 *  operation against specific equalizer and equalizer band
 *  identifiers.
 *
 *  @param[in]  aObject                   A pointer to a null-terminated
 *                                        C string representing the
 *                                        object for which the
 *                                        equalizer band level
 *                                        decrease request is to be
 *                                        made against. For example,
 *                                        "O" for a zone object.
 *  @param[in]  aEqualizerIdentifier      A reference to the specific
 *                                        equalizer identifier which
 *                                        the band level decrease
 *                                        request is to be made
 *                                        against.
 *  @param[in]  aEqualizerBandIdentifier  A reference to the specific
 *                                        equalizer band identifier
 *                                        which the band level
 *                                        decrease request is to be
 *                                        made against.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandDecreaseRequestBasis :: Init(const char *aObject,
                                          const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                          const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    static const char * const kEqualizerBandDecreaseOperation = "D";

    return (EqualizerBandRequestBasis::Init(aObject,
                                            aEqualizerIdentifier,
                                            aEqualizerBandIdentifier,
                                            kEqualizerBandDecreaseOperation));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer band level set request operation
 *  against specific equalizer and equalizer band identifiers.
 *
 *  @param[in]  aObject                   A pointer to a null-terminated
 *                                        C string representing the
 *                                        object for which the
 *                                        equalizer band level set
 *                                        request is to be made
 *                                        against. For example, "O"
 *                                        for a zone object.
 *  @param[in]  aEqualizerIdentifier      A reference to the specific
 *                                        equalizer identifier which
 *                                        the band level set request
 *                                        is to be made against.
 *  @param[in]  aEqualizerBandIdentifier  A reference to the specific
 *                                        equalizer band identifier
 *                                        which the band level set
 *                                        request is to be made
 *                                        against.
 *  @param[in]   aLevel                   An immutable reference to the
 *                                        equalizer band level to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandSetRequestBasis :: Init(const char *aObject,
                                     const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                     const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                     const EqualizerBandModel::LevelType &aLevel)
{
    return (EqualizerBandBufferBasis::Init(*this,
                                           aObject,
                                           aEqualizerIdentifier,
                                           aEqualizerBandIdentifier,
                                           aLevel));
}

}; // namespace Command

}; // namespace Client

}; // namespace HLX
