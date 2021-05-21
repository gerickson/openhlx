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
 *    Description:
 *      This file...
 */

#include <errno.h>

#include <nlunit-test.h>

#include <OpenHLX/Model/SoundModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


// Global Variables

static constexpr CrossoverModel::FrequencyType  kFrequencyMin = 1;
static constexpr CrossoverModel::FrequencyType  kFrequencyMax = 20011;
static constexpr CrossoverModel::FrequencyType  kLowpassFrequency = 239;
static constexpr CrossoverModel::FrequencyType  kHighpassFrequency = 9973;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    SoundModel  lSoundModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel  lSoundModel;
    Status      lStatus;


    lStatus = lSoundModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeDisabled;
    SoundModel                          lSoundModel;
    Status                              lStatus;


    lStatus = lSoundModel.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel  lSoundModel_1;
    SoundModel  lSoundModel_2;
    Status      lStatus;


    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestSoundModeObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                     lSoundModel_1;
    SoundModel                     lSoundModel_2;
    SoundModel::ChannelMode        lChannelModeValue;
    SoundModel::SoundMode          lSoundModeValue;
    Status                         lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lSoundModel_1.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lSoundModel_1.Init(SoundModel::kSoundModeTone);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeTone);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeTone);

    // Test 5: Check channel modes against sound modes

    // Disabled == Stereo

    lStatus = lSoundModel_2.Init(SoundModel::kSoundModeDisabled);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeDisabled);

    lStatus = lSoundModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lChannelModeValue == SoundModel::kChannelModeStereo);

    // ZoneEqualizer == Stereo

    lStatus = lSoundModel_2.Init(SoundModel::kSoundModeZoneEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeZoneEqualizer);

    lStatus = lSoundModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lChannelModeValue == SoundModel::kChannelModeStereo);

    // PresetEqualizer == Stereo

    lStatus = lSoundModel_2.Init(SoundModel::kSoundModePresetEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModePresetEqualizer);

    lStatus = lSoundModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lChannelModeValue == SoundModel::kChannelModeStereo);

    // Lowpass == Mono

    lStatus = lSoundModel_2.Init(SoundModel::kSoundModeLowpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeLowpass);

    lStatus = lSoundModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lChannelModeValue == SoundModel::kChannelModeMono);

    // Highpass == Stereo

    lStatus = lSoundModel_2.Init(SoundModel::kSoundModeHighpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == SoundModel::kSoundModeHighpass);

    lStatus = lSoundModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lChannelModeValue == SoundModel::kChannelModeStereo);
}

static void TestEqualizerBandObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                          lSoundModel_1;
    SoundModel                          lSoundModel_2;
    EqualizerBandModel::IdentifierType  lEqualizerBandIdentifier;
    const EqualizerBandModel *          lImmutableEqualizerBandModel;
    EqualizerBandModel *                lMutableEqualizerBandModel;
    Status                              lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 2: Ensure that a default initialized model returns success
    //         on observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 3: Ensure that a value initialized model returns success
    //         on observation.

    lStatus = lSoundModel_1.Init(SoundModel::kSoundModeZoneEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 4: Ensure that an object initialized model returns success
    //         and on observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 5: Ensure that other in-range band identifiers return
    //         success on observation.

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

        lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);
    }

    // Test 6: Ensure that out of range band identifiers return an
    //         error on observation.

    lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin - 1;

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lEqualizerBandIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);
}

static void TestEqualizerPresetObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                            lSoundModel_1;
    SoundModel                            lSoundModel_2;
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lSoundModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an error
    //         on observation.

    lStatus = lSoundModel_1.Init(SoundModel::kSoundModePresetEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an error
    //         on observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestToneObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                  lSoundModel_1;
    SoundModel                  lSoundModel_2;
    ToneModel::LevelType        lBassLevel;
    ToneModel::LevelType        lTrebleLevel;
    Status                      lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lSoundModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lSoundModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestLowpassCrossoverObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeLowpass;
    SoundModel                          lSoundModel_1;
    SoundModel                          lSoundModel_2;
    CrossoverModel::FrequencyType       lFrequency;
    CrossoverModel *                    lMutableLowpassCrossoverModel;
    const CrossoverModel *              lImmutableLowpassCrossoverModel;
    Status                              lStatus;

    // Test 1.1: Ensure that an uninitialized model returns an error on
    //           frequency observation.

    lStatus = lSoundModel_1.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Ensure that an uninitialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 2.1: Ensure that a default initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Ensure that a default initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 3.1: Ensure that a value initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_1.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3.2: Ensure that a value initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 4.1: Ensure that an object initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that an object initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);
}

static void TestHighpassCrossoverObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeHighpass;
    SoundModel                          lSoundModel_1;
    SoundModel                          lSoundModel_2;
    CrossoverModel::FrequencyType       lFrequency;
    CrossoverModel *                    lMutableHighpassCrossoverModel;
    const CrossoverModel *              lImmutableHighpassCrossoverModel;
    Status                              lStatus;

    // Test 1.1: Ensure that an uninitialized model returns an error on
    //           frequency observation.

    lStatus = lSoundModel_1.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Ensure that an uninitialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 2.1: Ensure that a default initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Ensure that a default initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 3.1: Ensure that a value initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_1.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_1.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3.2: Ensure that a value initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 4.1: Ensure that an object initialized model returns an
    //           error on frequency observation.

    lStatus = lSoundModel_2.Init(lSoundModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that an object initialized model returns success
    //           mutable and immutable crossover model observation.

    lStatus = lSoundModel_1.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lSoundModel_1.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext)
{
    TestSoundModeObservation(inSuite, inContext);
    TestEqualizerBandObservation(inSuite, inContext);
    TestEqualizerPresetObservation(inSuite, inContext);
    TestToneObservation(inSuite, inContext);
    TestLowpassCrossoverObservation(inSuite, inContext);
    TestHighpassCrossoverObservation(inSuite, inContext);
}

static void TestSoundModeMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeDisabled;
    SoundModel                   lSoundModel;
    SoundModel::SoundMode        lSoundModeValue;
    Status                       lStatus;


    // Initialize the model.

    lStatus = lSoundModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lSoundModel.SetSoundMode(static_cast<SoundModel::SoundMode>(SoundModel::kSoundModeMax + 1));
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSoundModel.SetSoundMode(static_cast<SoundModel::SoundMode>(SoundModel::kSoundModeMin - 1));
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lSoundModel.SetSoundMode(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == kSoundMode);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lSoundModel.SetSoundMode(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEqualizerPresetMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                            lSoundModel;
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lStatus;


    // Initialize the model.

    lStatus = lSoundModel.Init(SoundModel::kSoundModePresetEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lSoundModel.SetEqualizerPreset(IdentifierModel::kIdentifierInvalid);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lSoundModel.SetEqualizerPreset(IdentifierModel::kIdentifierMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lSoundModel.SetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lEqualizerPresetIdentifier == IdentifierModel::kIdentifierMin);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lSoundModel.SetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestToneMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ToneModel::LevelType  kBassConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kTrebleConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kBassConstant_2 = kBassConstant_1 + 1;
    const ToneModel::LevelType  kTrebleConstant_2 = kBassConstant_1 + 1;
    SoundModel                  lSoundModel;
    ToneModel::LevelType        lBassValue;
    ToneModel::LevelType        lTrebleValue;
    Status                      lStatus;

    // Initialize the model.

    lStatus = lSoundModel.Init(SoundModel::kSoundModeTone);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lSoundModel.SetBass(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetBass(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(ToneModel::kLevelMin - 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(kBassConstant_1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(ToneModel::kLevelMin - 1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(ToneModel::kLevelMax + 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(kBassConstant_1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetTone(ToneModel::kLevelMax + 1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lSoundModel.SetBass(kBassConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_1);

    lStatus = lSoundModel.SetTreble(kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_1);

    // Careful to avoid setting the same value for SetTone as SetBass
    // and SetTreble above as we do not want to test for
    // 'kStatus_ValueAlreadySet' until later.

    lStatus = lSoundModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetTone(lBassValue, lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_2);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lSoundModel.SetBass(kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lSoundModel.SetTreble(kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lSoundModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lSoundModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lSoundModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lSoundModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lSoundModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lSoundModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lSoundModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lSoundModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    lStatus = lSoundModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    // Test 5: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lSoundModel.SetBass(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMax);

    lStatus = lSoundModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMax);

    lStatus = lSoundModel.SetBass(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.SetTreble(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMin);

    lStatus = lSoundModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMin);
}

static void TestLowpassCrossoverMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                     lSoundModel;
    CrossoverModel::FrequencyType  lFrequency;
    Status                         lStatus;

    // Initialize the model.

    lStatus = lSoundModel.Init(SoundModel::kSoundModeLowpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lSoundModel.SetLowpassFrequency(kFrequencyMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetLowpassFrequency(kFrequencyMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lSoundModel.SetLowpassFrequency(kLowpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kLowpassFrequency);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lSoundModel.SetLowpassFrequency(kLowpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestHighpassCrossoverMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    SoundModel                     lSoundModel;
    CrossoverModel::FrequencyType  lFrequency;
    Status                         lStatus;

    // Initialize the model.

    lStatus = lSoundModel.Init(SoundModel::kSoundModeHighpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lSoundModel.SetHighpassFrequency(kFrequencyMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lSoundModel.SetHighpassFrequency(kFrequencyMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lSoundModel.SetHighpassFrequency(kHighpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kHighpassFrequency);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lSoundModel.SetHighpassFrequency(kHighpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestSoundModeMutation(inSuite, inContext);
    TestEqualizerPresetMutation(inSuite, inContext);
    TestToneMutation(inSuite, inContext);
    TestLowpassCrossoverMutation(inSuite, inContext);
    TestHighpassCrossoverMutation(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeDisabled;
    SoundModel                          lSoundModel_1;
    SoundModel                          lSoundModel_2;
    SoundModel                          lSoundModel_3;
    SoundModel                          lSoundModel_4;
    SoundModel                          lSoundModel_5;
    Status                              lStatus;
    bool                                lAreEqual;


    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_1 == lSoundModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lSoundModel_3.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSoundModel_4.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_3 == lSoundModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lSoundModel_5.Init(lSoundModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_5 == lSoundModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    static const SoundModel::SoundMode  kSoundMode = SoundModel::kSoundModeDisabled;
    SoundModel                          lSoundModel_1;
    SoundModel                          lSoundModel_2;
    SoundModel                          lSoundModel_3;
    SoundModel                          lSoundModel_4;
    SoundModel                          lSoundModel_5;
    Status                              lStatus;
    bool                                lAreEqual;


    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lSoundModel_2 = lSoundModel_1;

    lAreEqual = (lSoundModel_1 == lSoundModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSoundModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSoundModel_2 = lSoundModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_1 == lSoundModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSoundModel_3.Init(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSoundModel_4 = lSoundModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_3 == lSoundModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lSoundModel_4.Init(lSoundModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lSoundModel_5 = lSoundModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lSoundModel_4 == lSoundModel_5);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);
}

/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("Construction",   TestConstruction),
    NL_TEST_DEF("Initialization", TestInitialization),
    NL_TEST_DEF("Observation",    TestObservation),
    NL_TEST_DEF("Mutation",       TestMutation),
    NL_TEST_DEF("Equality",       TestEquality),
    NL_TEST_DEF("Assignment",     TestAssignment),

    NL_TEST_SENTINEL()
};

int main(void)
{
    nlTestSuite theSuite = {
        "Sound Model",
        &sTests[0],
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        0,
        0,
        0,
        0,
        0
    };

    // Generate human-readable output.
    nlTestSetOutputStyle(OUTPUT_DEF);

    // Run test suit againt one context.
    nlTestRunner(&theSuite, nullptr);

    return nlTestRunnerStats(&theSuite);
}
