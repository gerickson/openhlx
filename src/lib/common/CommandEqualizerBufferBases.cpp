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
 *      This file implements base and derived objects for composing HLX
 *      commands that observe or set HLX DSP equalizer data model
 *      properties.
 *
 */

#include "CommandEqualizerBufferBases.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Common
{

namespace Command
{

static const char kEqualizerProperty = 'E';

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an equalizer band level operation (for example,
 *  increase) against specific equalizer and equalizer band
 *  identifiers for a specific object into the specified command
 *  buffer.
 *
 *  @param[in,out]  aBuffer                   A mutable reference to the
 *                                            command buffer to compose
 *                                            the operation into.
 *  @param[in]      aObject                   A pointer to a null-terminated
 *                                            C string representing the
 *                                            object for which the
 *                                            equalizer band operation
 *                                            is to be made against.
 *  @param[in]      aEqualizerIdentifier      A reference to the specific
 *                                            equalizer identifier which
 *                                            the band level command
 *                                            buffer is to be made for.
 *  @param[in]      aEqualizerBandIdentifier  A reference to the specific
 *                                            equalizer band identifier
 *                                            which the band level
 *                                            command buffer is to be
 *                                            formed for.
 *  @param[in]      aOperation                A pointer to a null-terminated
 *                                            C string representing the
 *                                            operation to perform or
 *                                            performed for the command.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandBufferBasis :: Init(BufferBasis &aBuffer,
                                 const char *aObject,
                                 const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                 const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                 const char *aOperation)
{
    static const char   kEqualizerBandObject = 'B';
    OutputStringStream  lEqualizerBandIdentifierStream;
    std::string         lBuffer;


    lEqualizerBandIdentifierStream << aEqualizerBandIdentifier;

    // Compose the buffer.

    lBuffer = kEqualizerBandObject;
    lBuffer += lEqualizerBandIdentifierStream.str();
    lBuffer += aOperation;

    return (PropertyBufferBasis::Init(aBuffer,
                                      kEqualizerProperty,
                                      aObject,
                                      aEqualizerIdentifier,
                                      lBuffer.c_str()));
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes an equalizer band level operation against specific
 *  equalizer and equalizer band identifiers and level for a specific
 *  object into the specified command buffer.
 *
 *  @param[in,out]  aBuffer                   A mutable reference to the
 *                                            command buffer to compose
 *                                            the operation into.
 *  @param[in]      aObject                   A pointer to a null-terminated
 *                                            C string representing the
 *                                            object for which the
 *                                            equalizer band operation
 *                                            is to be made against.
 *  @param[in]      aEqualizerIdentifier      A reference to the specific
 *                                            equalizer identifier which
 *                                            the band level command
 *                                            buffer is to be formed for.
 *  @param[in]      aEqualizerBandIdentifier  A reference to the specific
 *                                            equalizer band identifier
 *                                            which the band level
 *                                            command buffer is to be
 *                                            formed for.
 *  @param[in]      aLevel                    An immutable reference to the
 *                                            equalizer band level.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandBufferBasis :: Init(BufferBasis &aBuffer,
                                 const char *aObject,
                                 const EqualizerBandsModel::IdentifierType &aEqualizerIdentifier,
                                 const IdentifierModel::IdentifierType &aEqualizerBandIdentifier,
                                 const LevelType &aLevel)
{
    static const char   kEqualizerBandLevelOperation = 'L';
    OutputStringStream  lLevelStream;
    std::string         lBuffer;

    // Upcast to an integer to ensure it is interpretted as something
    // to be converted rather than a character literal.

    lLevelStream << static_cast<int>(aLevel);

    // Compose the buffer.

    lBuffer = kEqualizerBandLevelOperation + lLevelStream.str();

    return (EqualizerBandBufferBasis::Init(aBuffer,
                                           aObject,
                                           aEqualizerIdentifier,
                                           aEqualizerBandIdentifier,
                                           lBuffer.c_str()));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer crossover filter operation against a
 *  specific object and identifier and crossover frequency into the
 *  specified command buffer.
 *
 *  @param[in,out]  aBuffer                   A mutable reference to the
 *                                            command buffer to compose
 *                                            the operation into.
 *  @param[in]      aObject                   A pointer to a null-terminated
 *                                            C string representing the
 *                                            object for which the
 *                                            crossover filter
 *                                            frequency operation is
 *                                            to be made against.
 *  @param[in]      aObjectIdentifier         A reference to the specific
 *                                            object identifier which
 *                                            the crossover filter
 *                                            frequency command buffer
 *                                            is to be formed for.
 *  @param[in]      aOperation                A pointer to a null-terminated
 *                                            C string representing
 *                                            the operation to perform
 *                                            for the command.
 *  @param[in]      aFrequency                An immutable reference to the
 *                                            crossover frequency.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerCrossoverBufferBasis :: Init(BufferBasis &aBuffer,
                                      const char *aObject,
                                      const IdentifierType &aObjectIdentifier,
                                      const char *aOperation,
                                      const FrequencyType &aFrequency)
{
    return (PropertySetBufferBasis::Init(aBuffer,
                                         kEqualizerProperty,
                                         aObject,
                                         aObjectIdentifier,
                                         aOperation,
                                         aFrequency));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer highpass crossover filter operation
 *  against a specific object and identifier and crossover frequency
 *  into the specified command buffer.
 *
 *  @param[in,out]  aBuffer                   A mutable reference to the
 *                                            command buffer to compose
 *                                            the operation into.
 *  @param[in]      aObject                   A pointer to a null-terminated
 *                                            C string representing the
 *                                            object for which the
 *                                            highpass crossover
 *                                            filter frequency
 *                                            operation is to be made
 *                                            against.
 *  @param[in]      aObjectIdentifier         A reference to the specific
 *                                            object identifier which
 *                                            the highpass crossover
 *                                            filter frequency command
 *                                            buffer is to be formed
 *                                            for.
 *  @param[in]      aFrequency                An immutable reference to the
 *                                            highpass crossover
 *                                            frequency.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerHighpassCrossoverBufferBasis :: Init(BufferBasis &aBuffer,
                                              const char *aObject,
                                              const IdentifierType &aObjectIdentifier,
                                              const FrequencyType &aFrequency)
{
    static const char * const kEqualizerHighpassCrossoverOperation = "HP";

    return (EqualizerCrossoverBufferBasis::Init(aBuffer,
                                                aObject,
                                                aObjectIdentifier,
                                                kEqualizerHighpassCrossoverOperation,
                                                aFrequency));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer lowpass crossover filter operation
 *  against a specific object and identifier and crossover frequency
 *  into the specified command buffer.
 *
 *  @param[in,out]  aBuffer                   A mutable reference to the
 *                                            command buffer to compose
 *                                            the operation into.
 *  @param[in]      aObject                   A pointer to a null-terminated
 *                                            C string representing the
 *                                            object for which the
 *                                            lowpass crossover
 *                                            filter frequency
 *                                            operation is to be made
 *                                            against.
 *  @param[in]      aObjectIdentifier         A reference to the specific
 *                                            object identifier which
 *                                            the lowpass crossover
 *                                            filter frequency command
 *                                            buffer is to be formed
 *                                            for.
 *  @param[in]      aFrequency                An immutable reference to the
 *                                            lowpass crossover
 *                                            frequency.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerLowpassCrossoverBufferBasis :: Init(BufferBasis &aBuffer,
                                             const char *aObject,
                                             const IdentifierType &aObjectIdentifier,
                                             const FrequencyType &aFrequency)
{
    static const char * const kEqualizerLowpassCrossoverOperation = "LP";

    return (EqualizerCrossoverBufferBasis::Init(aBuffer,
                                                aObject,
                                                aObjectIdentifier,
                                                kEqualizerLowpassCrossoverOperation,
                                                aFrequency));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer preset operation against a specific
 *  object and identifier and preset into the specified command
 *  buffer.
 *
 *  @param[in,out]  aBuffer                     A mutable reference to the
 *                                              command buffer to compose
 *                                              the operation into.
 *  @param[in]      aObject                     A pointer to a null-terminated
 *                                              C string representing
 *                                              the object for which the
 *                                              preset operation is to
 *                                              be made against.
 *  @param[in]      aObjectIdentifier           A reference to the specific
 *                                              object identifier which
 *                                              the preset command
 *                                              buffer is to be formed
 *                                              for.
 *  @param[in]      aEqualizerPresetIdentifier  An immutable reference
 *                                              to the equalizer preset
 *                                              identifier to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetBufferBasis :: Init(BufferBasis &aBuffer,
                                   const char *aObject,
                                   const IdentifierType &aObjectIdentifier,
                                   const IdentifierType &aEqualizerPresetIdentifier)
{
    static const char kEqualizerPresetOperation = 'P';

    return (PropertySetBufferBasis::Init(aBuffer,
                                         kEqualizerProperty,
                                         aObject,
                                         aObjectIdentifier,
                                         kEqualizerPresetOperation,
                                         aEqualizerPresetIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes an equalizer sound mode operation against a
 *  specific object and identifier and sound mode into the specified
 *  command buffer.
 *
 *  @param[in,out]  aBuffer                     A mutable reference to the
 *                                              command buffer to compose
 *                                              the operation into.
 *  @param[in]      aObject                     A pointer to a null-terminated
 *                                              C string representing
 *                                              the object for which the
 *                                              sound mode operation is to
 *                                              be made against.
 *  @param[in]      aObjectIdentifier           A reference to the specific
 *                                              object identifier which
 *                                              the sound mode command
 *                                              buffer is to be formed
 *                                              for.
 *  @param[in]      aSoundMode                  An immutable reference to
 *                                              the equalizer sound
 *                                              mode to set.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerSoundModeBufferBasis :: Init(BufferBasis &aBuffer,
                                      const char *aObject,
                                      const IdentifierType &aObjectIdentifier,
                                      const SoundMode &aSoundMode)
{
    static const char kEqualizerSoundModeOperation = 'M';

    return (PropertySetBufferBasis::Init(aBuffer,
                                         kEqualizerProperty,
                                         aObject,
                                         aObjectIdentifier,
                                         kEqualizerSoundModeOperation,
                                         aSoundMode));
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
