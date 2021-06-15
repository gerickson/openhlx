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
#include <string.h>

#include <nlunit-test.h>

#include <OpenHLX/Model/ZoneModel.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;


// Global Variables

static constexpr ZoneModel::IdentifierType     lIdentifierConstant = 13;
static const char *                            lNameConstant       = "Test Name";

static constexpr CrossoverModel::FrequencyType kFrequencyMin      = 1;
static constexpr CrossoverModel::FrequencyType kFrequencyMax      = 20011;
static constexpr CrossoverModel::FrequencyType kLowpassFrequency  = 239;
static constexpr CrossoverModel::FrequencyType kHighpassFrequency = 9973;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    ZoneModel                        lZoneModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                        lZoneModel;
    Status                           lStatus;

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestValueInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *               lNameConstant_1 = nullptr;
    const char *               lNameConstant_2 = "0123456789ABCDEF0";
    const std::string          lNameConstant_3(lNameConstant_2);
    const char *               lNameConstant_4 = "Test Name";
    const std::string          lNameConstant_5(lNameConstant_4);
    const char *               lNameConstant_6 = "0123456789ABCDEF";
    const std::string          lNameConstant_7(lNameConstant_6);
    ZoneModel::IdentifierType  lIdentifier;
    ZoneModel                  lZoneModel_1;
    ZoneModel                  lZoneModel_2;
    ZoneModel                  lZoneModel_3;
    ZoneModel                  lZoneModel_4;
    ZoneModel                  lZoneModel_5;
    ZoneModel                  lZoneModel_6;
    Status                     lStatus;

    // Test 1: Test invalid names and valid identifiers

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 1.1: Test a null value

    lStatus = lZoneModel_1.Init(lNameConstant_1, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZoneModel_2.Init(lNameConstant_1, 0, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test a too-long value with a null-terminated C string.

    lStatus = lZoneModel_1.Init(lNameConstant_2, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.3: Test a too-long value with a pointer and a length.

    lStatus = lZoneModel_2.Init(lNameConstant_2, strlen(lNameConstant_2), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.4: Test a too-long value with a C++ Standard Library string.

    lStatus = lZoneModel_3.Init(lNameConstant_3, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 2: Test valid names and invalid identifiers

    lIdentifier = IdentifierModel::kIdentifierInvalid;

    // Test 2.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lZoneModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lZoneModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lZoneModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lZoneModel_1.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lZoneModel_2.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lZoneModel_3.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 3: Test valid names and valid identifiers

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 3.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lZoneModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lZoneModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lZoneModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lZoneModel_4.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lZoneModel_5.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lZoneModel_6.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                        lZoneModel_1;
    ZoneModel                        lZoneModel_2;
    Status                           lStatus;

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.Init(lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestNameObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel    lZoneModel_1;
    ZoneModel    lZoneModel_2;
    ZoneModel    lZoneModel_3;
    ZoneModel    lZoneModel_4;
    const char * lNameValue;
    Status       lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    // Test 1.1: Test name observation

    lStatus = lZoneModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Test name observation

    lStatus = lZoneModel_2.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.1: Test name observation

    lStatus = lZoneModel_3.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);

    // Test 4: Ensure that an object initialized model returns success
    //         and the initialized value on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4.1: Test identifier observation

    lStatus = lZoneModel_4.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);
}

static void TestIdentifierObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                 lZoneModel_1;
    ZoneModel                 lZoneModel_2;
    ZoneModel                 lZoneModel_3;
    ZoneModel                 lZoneModel_4;
    ZoneModel::IdentifierType lIdentifierValue;
    Status                    lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    // Test 1.1: Test identifier observation

    lStatus = lZoneModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Test identifier observation

    lStatus = lZoneModel_2.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.1: Test identifier observation

    lStatus = lZoneModel_3.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 4: Ensure that an object initialized model returns success
    //         and the initialized value on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 4.1: Test identifier observation

    lStatus = lZoneModel_4.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);
}

static void TestVolumeLevelObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel               lZoneModel_1;
    ZoneModel               lZoneModel_2;
    ZoneModel               lZoneModel_3;
    ZoneModel               lZoneModel_4;
    VolumeModel::LevelType lVolumeValue;
    Status                  lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestVolumeMuteObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel             lZoneModel_1;
    ZoneModel             lZoneModel_2;
    ZoneModel             lZoneModel_3;
    ZoneModel             lZoneModel_4;
    VolumeModel::MuteType lMuteValue;
    Status                lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestVolumeFixedObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel              lZoneModel_1;
    ZoneModel              lZoneModel_2;
    ZoneModel              lZoneModel_3;
    ZoneModel              lZoneModel_4;
    VolumeModel::FixedType lFixedValue;
    Status                 lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetVolumeFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetVolumeFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // There is no initializer for the fixed property; this should
    // still return an error on observation.

    lStatus = lZoneModel_3.GetVolumeFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // The fixed property should still be uninitialized and should
    // still return an error on observation.

    lStatus = lZoneModel_4.GetVolumeFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestVolumeObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    TestVolumeLevelObservation(inSuite, inContext);
    TestVolumeMuteObservation(inSuite, inContext);
    TestVolumeFixedObservation(inSuite, inContext);
}

static void TestSourceObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                       lZoneModel_1;
    ZoneModel                       lZoneModel_2;
    ZoneModel                       lZoneModel_3;
    ZoneModel                       lZoneModel_4;
    SourceModel::IdentifierType     lSourceIdentifierValue;
    Status                          lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetSource(lSourceIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetSource(lSourceIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetSource(lSourceIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetSource(lSourceIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestBalanceObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                 lZoneModel_1;
    ZoneModel                 lZoneModel_2;
    ZoneModel                 lZoneModel_3;
    ZoneModel                 lZoneModel_4;
    BalanceModel::BalanceType lBalanceValue;
    Status                    lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestSoundModeObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel               lZoneModel_1;
    ZoneModel               lZoneModel_2;
    ZoneModel               lZoneModel_3;
    ZoneModel               lZoneModel_4;
    SoundModel::ChannelMode lChannelModeValue;
    SoundModel::SoundMode   lSoundModeValue;
    Status                  lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_2.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_3.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_4.Init(lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_4.GetChannelMode(lChannelModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestEqualizerBandObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                          lZoneModel_1;
    ZoneModel                          lZoneModel_2;
    EqualizerBandModel::IdentifierType  lEqualizerBandIdentifier;
    const EqualizerBandModel *          lImmutableEqualizerBandModel;
    EqualizerBandModel *                lMutableEqualizerBandModel;
    Status                              lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 2: Ensure that a default initialized model returns success
    //         on observation.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 3: Ensure that a value initialized model returns success
    //         on observation.

    lStatus = lZoneModel_1.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 4: Ensure that an object initialized model returns success
    //         and on observation.

    lStatus = lZoneModel_2.Init(lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);

    // Test 5: Ensure that other in-range band identifiers return
    //         success on observation.

    for (lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin; lEqualizerBandIdentifier <= EqualizerBandsModel::kEqualizerBandsMax; lEqualizerBandIdentifier++)
    {
        lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lImmutableEqualizerBandModel != nullptr);

        lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lMutableEqualizerBandModel != nullptr);
    }

    // Test 6: Ensure that out of range band identifiers return an
    //         error on observation.

    lEqualizerBandIdentifier = IdentifierModel::kIdentifierMin - 1;

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lEqualizerBandIdentifier = EqualizerBandsModel::kEqualizerBandsMax + 1;

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lImmutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel_1.GetEqualizerBand(lEqualizerBandIdentifier, lMutableEqualizerBandModel);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);
}

static void TestEqualizerPresetObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                            lZoneModel_1;
    ZoneModel                            lZoneModel_2;
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3: Ensure that a value initialized model returns an error
    //         on observation.

    lStatus = lZoneModel_1.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an error
    //         on observation.

    lStatus = lZoneModel_2.Init(lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestToneObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                  lZoneModel_1;
    ZoneModel                  lZoneModel_2;
    ToneModel::LevelType        lBassLevel;
    ToneModel::LevelType        lTrebleLevel;
    Status                      lStatus;


    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    lStatus = lZoneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4: Ensure that an object initialized model returns an
    //         error on observation.

    lStatus = lZoneModel_2.Init(lZoneModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBass(lBassLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTreble(lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel_1.GetTone(lBassLevel, lTrebleLevel);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestLowpassCrossoverObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                     lZoneModel_1;
    ZoneModel                     lZoneModel_2;
    ZoneModel                     lZoneModel_3;
    ZoneModel                     lZoneModel_4;
    CrossoverModel::FrequencyType lFrequency;
    CrossoverModel *              lMutableLowpassCrossoverModel;
    const CrossoverModel *        lImmutableLowpassCrossoverModel;
    Status                        lStatus;

    // Test 1.1: Ensure that an uninitialized model returns an error on
    //           frequency observation.

    lStatus = lZoneModel_1.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Ensure that an uninitialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_1.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lZoneModel_1.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 2.1: Ensure that a default initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Ensure that a default initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_2.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lZoneModel_2.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 3.1: Ensure that a value initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3.2: Ensure that a value initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_3.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lZoneModel_3.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);

    // Test 4.1: Ensure that an object initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that an object initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_4.GetLowpassCrossover(lMutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableLowpassCrossoverModel != nullptr);

    lStatus = lZoneModel_4.GetLowpassCrossover(lImmutableLowpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableLowpassCrossoverModel != nullptr);
}

static void TestHighpassCrossoverObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                     lZoneModel_1;
    ZoneModel                     lZoneModel_2;
    ZoneModel                     lZoneModel_3;
    ZoneModel                     lZoneModel_4;
    CrossoverModel::FrequencyType lFrequency;
    CrossoverModel *              lMutableHighpassCrossoverModel;
    const CrossoverModel *        lImmutableHighpassCrossoverModel;
    Status                        lStatus;

    // Test 1.1: Ensure that an uninitialized model returns an error on
    //           frequency observation.

    lStatus = lZoneModel_1.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Ensure that an uninitialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_1.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lZoneModel_1.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 2.1: Ensure that a default initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.2: Ensure that a default initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_2.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lZoneModel_2.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 3.1: Ensure that a value initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 3.2: Ensure that a value initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_3.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lZoneModel_3.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);

    // Test 4.1: Ensure that an object initialized model returns an
    //           error on frequency observation.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that an object initialized model returns success
    //           on mutable and immutable crossover model observation.

    lStatus = lZoneModel_4.GetHighpassCrossover(lMutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMutableHighpassCrossoverModel != nullptr);

    lStatus = lZoneModel_4.GetHighpassCrossover(lImmutableHighpassCrossoverModel);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lImmutableHighpassCrossoverModel != nullptr);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext)
{
    TestNameObservation(inSuite, inContext);
    TestIdentifierObservation(inSuite, inContext);
    TestVolumeObservation(inSuite, inContext);
    TestSourceObservation(inSuite, inContext);
    TestBalanceObservation(inSuite, inContext);
    TestSoundModeObservation(inSuite, inContext);
    TestEqualizerBandObservation(inSuite, inContext);
    TestEqualizerPresetObservation(inSuite, inContext);
    TestToneObservation(inSuite, inContext);
    TestLowpassCrossoverObservation(inSuite, inContext);
    TestHighpassCrossoverObservation(inSuite, inContext);
}

static void TestNameMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                          lZoneModel;
    const char *                       lNameConstant_1 = nullptr;
    const char *                       lNameConstant_2 = "0123456789ABCDEF0";
    const char *                       lNameConstant_3 = "Test Name 1";
    const char *                       lNameConstant_4 = "Test Name 2";
    const char *                       lNameValue;
    ZoneModel::IdentifierType          lIdentifierValue;
    Status                             lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an invalid value returns an error on
    //         mutation.

    // Test 1.1: Ensure that an invalid name value returns an error on
    //           mutation.

    // Test 1.1.1: Test NULL-terminated C string and an identifier.

    // Test 1.1.1.1: Test a NULL value.

    lStatus = lZoneModel.SetName(lNameConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.1.2: Test a too-long value.

    lStatus = lZoneModel.SetName(lNameConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.1.2: Test a C string, length, and an identifier.

    // Test 1.1.2.1: Test a NULL value.

    lStatus = lZoneModel.SetName(lNameConstant_1, 0);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.2.2: Test a too-long value.

    lStatus = lZoneModel.SetName(lNameConstant_2, strlen(lNameConstant_2));
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.2: Ensure that an invalid identifier value returns an error on
    //           mutation.

    lStatus = lZoneModel.SetIdentifier(IdentifierModel::kIdentifierInvalid);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    // Test 2.1: Ensure that an in range name value can be successfully set
    //           and observed.

    lStatus = lZoneModel.SetName(lNameConstant_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    lStatus = lZoneModel.SetName(lNameConstant_4, strlen(lNameConstant_4));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 2.2: Ensure that an in range identifier value can be successfully
    //           set and observed.

    lStatus = lZoneModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    // Test 3.1: Ensure that setting a name value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3.2: Ensure that setting an identifier value twice results
    //           in non-error / non-success status.

    lStatus = lZoneModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestIdentifierMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                          lZoneModel;
    const char *                       lNameConstant_1 = nullptr;
    const char *                       lNameConstant_2 = "0123456789ABCDEF0";
    const char *                       lNameConstant_3 = "Test Name 1";
    const char *                       lNameConstant_4 = "Test Name 2";
    const char *                       lNameValue;
    ZoneModel::IdentifierType          lIdentifierValue;
    Status                             lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an invalid value returns an error on
    //         mutation.

    // Test 1.1: Ensure that an invalid name value returns an error on
    //           mutation.

    // Test 1.1.1: Test NULL-terminated C string and an identifier.

    // Test 1.1.1.1: Test a NULL value.

    lStatus = lZoneModel.SetName(lNameConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.1.2: Test a too-long value.

    lStatus = lZoneModel.SetName(lNameConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.1.2: Test a C string, length, and an identifier.

    // Test 1.1.2.1: Test a NULL value.

    lStatus = lZoneModel.SetName(lNameConstant_1, 0);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.2.2: Test a too-long value.

    lStatus = lZoneModel.SetName(lNameConstant_2, strlen(lNameConstant_2));
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.2: Ensure that an invalid identifier value returns an error on
    //           mutation.

    lStatus = lZoneModel.SetIdentifier(IdentifierModel::kIdentifierInvalid);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    // Test 2.1: Ensure that an in range name value can be successfully set
    //           and observed.

    lStatus = lZoneModel.SetName(lNameConstant_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    lStatus = lZoneModel.SetName(lNameConstant_4, strlen(lNameConstant_4));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 2.2: Ensure that an in range identifier value can be successfully
    //           set and observed.

    lStatus = lZoneModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    // Test 3.1: Ensure that setting a name value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3.2: Ensure that setting an identifier value twice results
    //           in non-error / non-success status.

    lStatus = lZoneModel.SetIdentifier(lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestVolumeLevelMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    ZoneModel                    lZoneModel;
    VolumeModel::LevelType        lVolumeValue;
    Status                         lStatus;

    // Test adjustment on an uninitialized model

    lStatus = lZoneModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lStatus = lZoneModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetVolume(VolumeModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetVolume(VolumeModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lZoneModel.SetVolume(lVolumeConstant + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lZoneModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lZoneModel.SetVolume(lVolumeConstant - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    lStatus = lZoneModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    // Test 5: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lZoneModel.SetVolume(VolumeModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMax);

    lStatus = lZoneModel.SetVolume(VolumeModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMin);
}

static void TestVolumeMuteMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::MuteType    lMuteConstant = true;
    ZoneModel                    lZoneModel;
    VolumeModel::MuteType          lMuteValue;
    Status                         lStatus;


    // Test toggling on an uninitialized model

    lStatus = lZoneModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 3: Ensure that mute toggling works.

    lStatus = lZoneModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lZoneModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lZoneModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lZoneModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lZoneModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lZoneModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);
}

static void TestVolumeFixedMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::FixedType lFixedConstant = true;
    ZoneModel                    lZoneModel;
    VolumeModel::FixedType       lFixedValue;
    Status                       lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetVolumeFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetVolumeFixed(lFixedValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFixedValue == lFixedConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetVolumeFixed(lFixedConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestVolumeMutation(nlTestSuite *inSuite, void *inContext)
{
    TestVolumeLevelMutation(inSuite, inContext);
    TestVolumeMuteMutation(inSuite, inContext);
    TestVolumeFixedMutation(inSuite, inContext);
}

static void TestSourceMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SourceModel::IdentifierType lSourceIdentifierConstant = 13;
    ZoneModel                         lZoneModel;
    SourceModel::IdentifierType       lSourceIdentifierValue;
    Status                            lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetSource(lSourceIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetSource(lSourceIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSourceIdentifierValue == lSourceIdentifierConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetSource(lSourceIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestBalanceMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const BalanceModel::BalanceType lBalanceConstant = BalanceModel::kBalanceMax - 1;
    ZoneModel                       lZoneModel_1;
    ZoneModel                       lZoneModel_2;
    BalanceModel::BalanceType       lBalanceValue;
    Status                          lStatus;

    // Initialize the model.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel_1.SetBalance(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == lBalanceConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel_1.SetBalance(lBalanceConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that increase balance left/right work

    // Test 4.1: Ensure that increase balance right works

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceCenter);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter);

    lStatus = lZoneModel_1.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 1);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 1);

    lStatus = lZoneModel_1.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 2);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter + 2);

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMax);

    lStatus = lZoneModel_1.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMax);

    lStatus = lZoneModel_2.IncreaseBalanceRight(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 4.2: Ensure that increase balance left works

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceCenter);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter);

    lStatus = lZoneModel_1.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 1);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 1);

    lStatus = lZoneModel_1.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 2);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceCenter - 2);

    lStatus = lZoneModel_1.SetBalance(BalanceModel::kBalanceMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMin);

    lStatus = lZoneModel_1.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel_1.GetBalance(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBalanceValue == BalanceModel::kBalanceMin);

    lStatus = lZoneModel_2.IncreaseBalanceLeft(lBalanceValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestSoundModeMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SoundModel::SoundMode kSoundMode = SoundModel::kSoundModeDisabled;
    ZoneModel                   lZoneModel;
    ZoneModel::SoundMode        lSoundModeValue;
    Status                      lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetSoundMode(static_cast<SoundModel::SoundMode>(SoundModel::kSoundModeMax + 1));
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZoneModel.SetSoundMode(static_cast<SoundModel::SoundMode>(SoundModel::kSoundModeMin - 1));
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetSoundMode(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetSoundMode(lSoundModeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lSoundModeValue == kSoundMode);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetSoundMode(kSoundMode);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestEqualizerPresetMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                            lZoneModel;
    EqualizerPresetModel::IdentifierType  lEqualizerPresetIdentifier;
    Status                                lStatus;


    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetSoundMode(SoundModel::kSoundModePresetEqualizer);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetEqualizerPreset(IdentifierModel::kIdentifierInvalid);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lZoneModel.SetEqualizerPreset(IdentifierModel::kIdentifierMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lEqualizerPresetIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lZoneModel.SetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lEqualizerPresetIdentifier == IdentifierModel::kIdentifierMin);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetEqualizerPreset(lEqualizerPresetIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestToneMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const ToneModel::LevelType  kBassConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kTrebleConstant_1 = ToneModel::kLevelMax - 1;
    const ToneModel::LevelType  kBassConstant_2 = kBassConstant_1 + 1;
    const ToneModel::LevelType  kTrebleConstant_2 = kBassConstant_1 + 1;
    ZoneModel                  lZoneModel;
    ToneModel::LevelType        lBassValue;
    ToneModel::LevelType        lTrebleValue;
    Status                      lStatus;

    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetSoundMode(SoundModel::kSoundModeTone);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetBass(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetBass(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(ToneModel::kLevelMin - 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(kBassConstant_1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(ToneModel::kLevelMin - 1, ToneModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(ToneModel::kLevelMax + 1, kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(kBassConstant_1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetTone(ToneModel::kLevelMax + 1, ToneModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetBass(kBassConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_1);

    lStatus = lZoneModel.SetTreble(kTrebleConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_1);

    // Careful to avoid setting the same value for SetTone as SetBass
    // and SetTreble above as we do not want to test for
    // 'kStatus_ValueAlreadySet' until later.

    lStatus = lZoneModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetTone(lBassValue, lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lTrebleValue == kTrebleConstant_2);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetBass(kBassConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lZoneModel.SetTreble(kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lZoneModel.SetTone(kBassConstant_2, kTrebleConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    // Test 4: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lZoneModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lZoneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat + 1);

    lStatus = lZoneModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lZoneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat + 1);

    lStatus = lZoneModel.SetBass(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelFlat);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lZoneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelFlat - 1);

    lStatus = lZoneModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    lStatus = lZoneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelFlat - 1);

    // Test 5: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lZoneModel.SetBass(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.IncreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMax);

    lStatus = lZoneModel.IncreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMax);

    lStatus = lZoneModel.SetBass(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetTreble(ToneModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.DecreaseBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetBass(lBassValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lBassValue == ToneModel::kLevelMin);

    lStatus = lZoneModel.DecreaseTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.GetTreble(lTrebleValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lTrebleValue == ToneModel::kLevelMin);
}

static void TestLowpassCrossoverMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                     lZoneModel;
    CrossoverModel::FrequencyType  lFrequency;
    Status                         lStatus;

    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetSoundMode(SoundModel::kSoundModeLowpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetLowpassFrequency(kFrequencyMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetLowpassFrequency(kFrequencyMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetLowpassFrequency(kLowpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetLowpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kLowpassFrequency);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetLowpassFrequency(kLowpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestHighpassCrossoverMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                     lZoneModel;
    CrossoverModel::FrequencyType  lFrequency;
    Status                         lStatus;

    // Initialize the model.

    lStatus = lZoneModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.SetSoundMode(SoundModel::kSoundModeHighpass);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lZoneModel.SetHighpassFrequency(kFrequencyMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lZoneModel.SetHighpassFrequency(kFrequencyMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lZoneModel.SetHighpassFrequency(kHighpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel.GetHighpassFrequency(lFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lFrequency == kHighpassFrequency);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lZoneModel.SetHighpassFrequency(kHighpassFrequency);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestNameMutation(inSuite, inContext);
    TestIdentifierMutation(inSuite, inContext);
    TestVolumeMutation(inSuite, inContext);
    TestSourceMutation(inSuite, inContext);
    TestBalanceMutation(inSuite, inContext);
    TestSoundModeMutation(inSuite, inContext);
    TestEqualizerPresetMutation(inSuite, inContext);
    TestToneMutation(inSuite, inContext);
    TestLowpassCrossoverMutation(inSuite, inContext);
    TestHighpassCrossoverMutation(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *                       lNameConstant_1 = "Test Name";
    const std::string                  lNameConstant_2(lNameConstant_1);
    ZoneModel                          lZoneModel_1;
    ZoneModel                          lZoneModel_2;
    ZoneModel                          lZoneModel_3;
    ZoneModel                          lZoneModel_4;
    ZoneModel                          lZoneModel_5;
    ZoneModel                          lZoneModel_6;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_1 == lZoneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lZoneModel_3.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_3 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lZoneModel_5.Init(lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_5 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test than an unintialized object is not equivalent to
    //         an initialized object.

    lAreEqual = (lZoneModel_6 == lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that two zones with the same name and identifier
    //         but different sources are not equal.

    lStatus = lZoneModel_3.SetSource(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.SetSource(11);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_3 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that two zones with the same name, identifier,
    //         and source but different volumes are not equal.

    lStatus = lZoneModel_4.SetSource(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.SetVolume(-37);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.SetVolume(-43);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_3 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 7: Test that two zones with the same name, identifier,
    //         source, and volume but different balances are not
    //         equal.

    lStatus = lZoneModel_4.SetVolume(-37);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_3.SetBalance(BalanceModel::kBalanceMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lZoneModel_4.SetBalance(BalanceModel::kBalanceMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_3 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    ZoneModel                          lZoneModel_1;
    ZoneModel                          lZoneModel_2;
    ZoneModel                          lZoneModel_3;
    ZoneModel                          lZoneModel_4;
    ZoneModel                          lZoneModel_5;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lZoneModel_2 = lZoneModel_1;

    lAreEqual = (lZoneModel_1 == lZoneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lZoneModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lZoneModel_2 = lZoneModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_1 == lZoneModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lZoneModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lZoneModel_4 = lZoneModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_3 == lZoneModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lZoneModel_4.Init(lZoneModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lZoneModel_5 = lZoneModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lZoneModel_4 == lZoneModel_5);
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
        "Zone Model",
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
