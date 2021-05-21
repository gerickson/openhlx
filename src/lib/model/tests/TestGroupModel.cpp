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

#include <array>

#include <errno.h>

#include <nlunit-test.h>

#include <OpenHLX/Model/GroupModel.hpp>
#include <OpenHLX/Utilities/ElementsOf.hpp>


using namespace HLX;
using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;


static void TestConstruction(nlTestSuite *inSuite __attribute__((unused)),
                             void *inContext __attribute__((unused)))
{
    GroupModel lGroupModel;
}

static void TestDefaultInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel lGroupModel;
    Status     lStatus;

    lStatus = lGroupModel.Init();
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
    GroupModel::IdentifierType lIdentifier;
    GroupModel                 lGroupModel_1;
    GroupModel                 lGroupModel_2;
    GroupModel                 lGroupModel_3;
    GroupModel                 lGroupModel_4;
    GroupModel                 lGroupModel_5;
    GroupModel                 lGroupModel_6;
    Status                     lStatus;

    // Test 1: Test invalid names and valid identifiers

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 1.1: Test a null value

    lStatus = lGroupModel_1.Init(lNameConstant_1, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lGroupModel_2.Init(lNameConstant_1, 0, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test a too-long value with a null-terminated C string.

    lStatus = lGroupModel_1.Init(lNameConstant_2, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.3: Test a too-long value with a pointer and a length.

    lStatus = lGroupModel_2.Init(lNameConstant_2, strlen(lNameConstant_2), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.4: Test a too-long value with a C++ Standard Library string.

    lStatus = lGroupModel_3.Init(lNameConstant_3, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 2: Test valid names and invalid identifiers

    lIdentifier = IdentifierModel::kIdentifierInvalid;

    // Test 2.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lGroupModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lGroupModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lGroupModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lGroupModel_1.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lGroupModel_2.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 2.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lGroupModel_3.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 3: Test valid names and valid identifiers

    lIdentifier = IdentifierModel::kIdentifierMin;

    // Test 3.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lGroupModel_1.Init(lNameConstant_4, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lGroupModel_2.Init(lNameConstant_4, strlen(lNameConstant_4), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.3: Test a C++ Standard Library string that is under the
    //           maximum length.

    lStatus = lGroupModel_3.Init(lNameConstant_5, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.4: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lGroupModel_4.Init(lNameConstant_6, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.5: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lGroupModel_5.Init(lNameConstant_6, strlen(lNameConstant_6), lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 3.6: Test a C++ Standard Library string that is exactly the
    //           maximum length.

    lStatus = lGroupModel_6.Init(lNameConstant_7, lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestObjectInitialization(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                       lGroupModel_1;
    GroupModel                       lGroupModel_2;
    Status                           lStatus;

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.Init(lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
}

static void TestInitialization(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitialization(inSuite, inContext);
    TestValueInitialization(inSuite, inContext);
    TestObjectInitialization(inSuite, inContext);
}

static void TestUninitializedObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                                 lGroupModel_1;
    const char *                               lNameValue;
    GroupModel::IdentifierType                 lIdentifierValue;
    GroupModel::MuteType                       lMute;
    GroupModel::LevelType                     lVolume;
    size_t                                     lCount;
    IdentifiersCollection                      lSourcesCollection;
    std::array<SourceModel::IdentifierType, 1> lSources;
    std::array<ZoneModel::IdentifierType, 1>   lZones;
    bool                                       lContainsZone;
    Status                                     lStatus;

    // Test 1: Ensure that an uninitialized model returns an error on
    //         observation.

    // Test 1.1: Test name observation

    lStatus = lGroupModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Test identifier observation

    lStatus = lGroupModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.3: Test (volume) mute observation

    lStatus = lGroupModel_1.GetMute(lMute);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.4: Test volume observation

    lStatus = lGroupModel_1.GetVolume(lVolume);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.5: Test sources observation

    lStatus = lGroupModel_1.GetSources(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lCount = lSources.size();

    lStatus = lGroupModel_1.GetSources(&lSources[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // This instance of GetSources is, intentionally, an odd expection
    // in that whether or not the object is initialized, the status is
    // always successful and the returned data always 'sane' and
    // meaningful--in this case, not initialized.

    lStatus = lGroupModel_1.GetSources(lSourcesCollection);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesCollection.GetCount(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.6: Test zones observation

    lContainsZone = lGroupModel_1.ContainsZone(IdentifierModel::kIdentifierMin);
    NL_TEST_ASSERT(inSuite, lContainsZone == false);

    lStatus = lGroupModel_1.GetZones(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    lCount = lZones.size();

    lStatus = lGroupModel_1.GetZones(&lZones[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);
}

static void TestDefaultInitializedObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                                 lGroupModel_1;
    const char *                               lNameValue;
    GroupModel::IdentifierType                 lIdentifierValue;
    GroupModel::MuteType                       lMute;
    GroupModel::LevelType                     lVolume;
    size_t                                     lCount;
    IdentifiersCollection                      lSourcesCollection;
    std::array<SourceModel::IdentifierType, 1> lSources;
    std::array<ZoneModel::IdentifierType, 1>   lZones;
    bool                                       lContainsZone;
    Status                                     lStatus;

    // Test 1: Ensure that a default initialized model returns an
    //         error on observation.

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1.1: Test name observation

    lStatus = lGroupModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.2: Test identifier observation

    lStatus = lGroupModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.3: Test (volume) mute observation

    lStatus = lGroupModel_1.GetMute(lMute);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.4: Test volume observation

    lStatus = lGroupModel_1.GetVolume(lVolume);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.5: Test sources observation

    lStatus = lGroupModel_1.GetSources(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lSources.size();

    lStatus = lGroupModel_1.GetSources(&lSources[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lStatus = lGroupModel_1.GetSources(lSourcesCollection);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesCollection.GetCount(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    // Test 1.6: Test zones observation

    lContainsZone = lGroupModel_1.ContainsZone(IdentifierModel::kIdentifierMin);
    NL_TEST_ASSERT(inSuite, lContainsZone == false);

    lStatus = lGroupModel_1.GetZones(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lZones.size();

    lStatus = lGroupModel_1.GetZones(&lZones[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);
}

static void TestValueInitializedObservation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const GroupModel::IdentifierType           lIdentifierConstant = 13;
    const char *                               lNameConstant = "Test Name";
    GroupModel                                 lGroupModel_1;
    GroupModel                                 lGroupModel_2;
    const char *                               lNameValue;
    GroupModel::IdentifierType                 lIdentifierValue;
    GroupModel::MuteType                       lMute;
    GroupModel::LevelType                     lVolume;
    size_t                                     lCount;
    IdentifiersCollection                      lSourcesCollection;
    std::array<SourceModel::IdentifierType, 1> lSources;
    std::array<ZoneModel::IdentifierType, 1>   lZones;
    bool                                       lContainsZone;
    Status                                     lStatus;

    // Test 1: Ensure that a value initialized model returns success
    //         and the initialized value on observation.

    lStatus = lGroupModel_1.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1.1: Test name observation

    lStatus = lGroupModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);

    // Test 1.2: Test identifier observation

    lStatus = lGroupModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 1.3: Test (volume) mute observation

    lStatus = lGroupModel_1.GetMute(lMute);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.4: Test volume observation

    lStatus = lGroupModel_1.GetVolume(lVolume);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 1.5: Test sources observation

    lStatus = lGroupModel_1.GetSources(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lSources.size();

    lStatus = lGroupModel_1.GetSources(&lSources[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lStatus = lGroupModel_1.GetSources(lSourcesCollection);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesCollection.GetCount(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    // Test 1.6: Test zones observation

    lContainsZone = lGroupModel_1.ContainsZone(IdentifierModel::kIdentifierMin);
    NL_TEST_ASSERT(inSuite, lContainsZone == false);

    lStatus = lGroupModel_1.GetZones(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lZones.size();

    lStatus = lGroupModel_1.GetZones(&lZones[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    // Test 2: Ensure that an object initialized model returns success
    //         and the original initializing object value on observation.

    lStatus = lGroupModel_2.Init(lGroupModel_1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 2.1: Test name observation

    lStatus = lGroupModel_2.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant) == 0);

    // Test 2.2: Test identifier observation

    lStatus = lGroupModel_2.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifierValue == lIdentifierConstant);

    // Test 2.3: Test (volume) mute observation

    lStatus = lGroupModel_2.GetMute(lMute);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.4: Test volume observation

    lStatus = lGroupModel_2.GetVolume(lVolume);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2.5: Test sources observation

    lStatus = lGroupModel_2.GetSources(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lSources.size();

    lStatus = lGroupModel_2.GetSources(&lSources[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lStatus = lGroupModel_2.GetSources(lSourcesCollection);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lSourcesCollection.GetCount(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    // Test 2.6: Test zones observation

    lContainsZone = lGroupModel_2.ContainsZone(IdentifierModel::kIdentifierMin);
    NL_TEST_ASSERT(inSuite, lContainsZone == false);

    lStatus = lGroupModel_2.GetZones(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);

    lCount = lZones.size();

    lStatus = lGroupModel_2.GetZones(&lZones[0], lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lCount == 0);
}

static void TestInitializedObservation(nlTestSuite *inSuite, void *inContext)
{
    TestDefaultInitializedObservation(inSuite, inContext);
    TestValueInitializedObservation(inSuite, inContext);
}

static void TestObservation(nlTestSuite *inSuite, void *inContext)
{
    TestUninitializedObservation(inSuite, inContext);
    TestInitializedObservation(inSuite, inContext);
}

static void TestNameMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const char *                  lNameConstant_1 = nullptr;
    const char *                  lNameConstant_2 = "0123456789ABCDEF0";
    const char *                  lNameConstant_3 = "Test Name";
    const char *                  lNameConstant_4 = "0123456789ABCDEF";
    const char *                  lNameValue;
    GroupModel                    lGroupModel_1;
    GroupModel                    lGroupModel_2;
    GroupModel                    lGroupModel_3;
    GroupModel                    lGroupModel_4;
    Status                        lStatus;


    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_3.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test name mutation

    // Test 1.1: Test invalid values

    // Test 1.1.1: Test a null value

    lStatus = lGroupModel_1.SetName(lNameConstant_1);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    lStatus = lGroupModel_2.SetName(lNameConstant_1, 0);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.1.2: Test a too-long value with a null-terminated C string.

    lStatus = lGroupModel_1.SetName(lNameConstant_2);
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.1.3: Test a too-long value with a pointer and a length.

    lStatus = lGroupModel_2.SetName(lNameConstant_2, strlen(lNameConstant_2));
    NL_TEST_ASSERT(inSuite, lStatus == -ENAMETOOLONG);

    // Test 1.2: Test valid values

    // Test 1.2.1: Test a null-terminated C string that is under the
    //           maximum length.

    lStatus = lGroupModel_1.SetName(lNameConstant_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    // Test 1.2.2: Test a pointer and a length that is under the maximum
    //           length.

    lStatus = lGroupModel_2.SetName(lNameConstant_3, strlen(lNameConstant_3));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_3) == 0);

    // Test 1.2.3: Test a null-terminated C string that is exactly the
    //           maximum length.

    lStatus = lGroupModel_3.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_3.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 1.2.4: Test a pointer and a length that is exactly the maximum
    //           length.

    lStatus = lGroupModel_4.SetName(lNameConstant_4, strlen(lNameConstant_4));
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);

    // Test 1.3: Ensure that setting a name value twice results in non-error
    //           / non-success status.

    lStatus = lGroupModel_4.SetName(lNameConstant_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lGroupModel_4.GetName(lNameValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, strcmp(lNameValue, lNameConstant_4) == 0);
}

static void TestIdentifierMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel::IdentifierType lIdentifier;
    GroupModel::IdentifierType lIdentifierValue;
    GroupModel                 lGroupModel_1;
    Status                     lStatus;


    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Test identifier mutation

    // Test 1.1: Test an invalid identifier

    lIdentifier = IdentifierModel::kIdentifierInvalid;

    lStatus = lGroupModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == -EINVAL);

    // Test 1.2: Test a valid identifier

    lIdentifier = IdentifierModel::kIdentifierMin;

    lStatus = lGroupModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifier == lIdentifierValue);

    // Test 1.3: Ensure that setting an identifier value twice results
    //           in non-error / non-success status.

    lStatus = lGroupModel_1.SetIdentifier(lIdentifier);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lGroupModel_1.GetIdentifier(lIdentifierValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lIdentifier == lIdentifierValue);
}

static void TestVolumeMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::LevelType  lVolumeConstant = (((VolumeModel::kLevelMax - VolumeModel::kLevelMin) / 2) + VolumeModel::kLevelMin);
    GroupModel                     lGroupModel;
    VolumeModel::LevelType        lVolumeValue;
    Status                         lStatus;


    // Initialize the model.

    lStatus = lGroupModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that out of range values return an error on
    //         mutation.

    lStatus = lGroupModel.SetVolume(VolumeModel::kLevelMax + 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lGroupModel.SetVolume(VolumeModel::kLevelMin - 1);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    // Test 2: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lGroupModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);

    // Test 3: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lGroupModel.SetVolume(lVolumeConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant);

    // Test 5: Ensure that decrement and increment work correctly in
    //         range.

    lStatus = lGroupModel.SetVolume(lVolumeConstant + 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant + 2);

    lStatus = lGroupModel.SetVolume(lVolumeConstant - 1);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == lVolumeConstant - 2);

    // Test 6: Ensure that decrement and increment work correctly out of
    //         range.

    lStatus = lGroupModel.SetVolume(VolumeModel::kLevelMax);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.IncreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMax);

    lStatus = lGroupModel.SetVolume(VolumeModel::kLevelMin);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.DecreaseVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == -ERANGE);

    lStatus = lGroupModel.GetVolume(lVolumeValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lVolumeValue == VolumeModel::kLevelMin);
}

static void TestMuteMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const VolumeModel::MuteType    lMuteConstant = true;
    GroupModel                     lGroupModel;
    VolumeModel::MuteType          lMuteValue;
    Status                         lStatus;


    // Initialize the model.

    lStatus = lGroupModel.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 1: Ensure that an in range value can be successfully set
    //         and observed.

    lStatus = lGroupModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    // Test 2: Ensure that setting a value twice results in non-error
    //         / non-success status.

    lStatus = lGroupModel.SetMute(lMuteConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lGroupModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    // Test 3: Ensure that mute toggling works.

    lStatus = lGroupModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lGroupModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lGroupModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lGroupModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == lMuteConstant);

    lStatus = lGroupModel.ToggleMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);

    lStatus = lGroupModel.GetMute(lMuteValue);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lMuteValue == !lMuteConstant);
}

static void TestSourceAdd(nlTestSuite *inSuite, void *inContext __attribute__((unused)), const SourceModel::IdentifierType *aInitialIdentifiers, const size_t &aInitialCount, GroupModel &aGroupModel)
{
    Status                             lStatus;
    size_t                             lActualCount;
    size_t                             lExpectedCount;
    size_t                             lCount;
    const SourceModel::IdentifierType  lAddedIdentifiers[] = { 5, 8, 13, 21 };

    // Test 1: Test adding identifiers known to be present in the
    //         group. Verify non-success status and that the size
    //         remains constant.

    if ((aInitialIdentifiers != nullptr) && (aInitialCount > 0))
    {
        for (size_t i = 0; i < aInitialCount; i++)
        {
            lStatus = aGroupModel.AddSource(aInitialIdentifiers[i]);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

            lStatus = aGroupModel.GetSources(lActualCount);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
            NL_TEST_ASSERT(inSuite, lActualCount == aInitialCount);
        }
    }

    // Test 2: Test adding an identifier known to be absent from the
    //         collection. Verify that the size increases.

    lCount = ElementsOf(lAddedIdentifiers);
    lExpectedCount = aInitialCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const SourceModel::IdentifierType  lIdentifier = lAddedIdentifiers[i];


        lStatus = aGroupModel.AddSource(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount++;

        lStatus = aGroupModel.GetSources(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);
    }
}

static void TestSourceAdd(nlTestSuite *inSuite, void *inContext)
{
    GroupModel                         lGroupModel_1;
    GroupModel                         lGroupModel_2;
    GroupModel                         lGroupModel_3;
    GroupModel                         lGroupModel_4;
    Status                             lStatus;


    // Test 1: Test adding to an uninitialized object

    lStatus = lGroupModel_1.AddSource(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test adding to a default initialized object

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestSourceAdd(inSuite, inContext, nullptr, 0, lGroupModel_1);
}

static void TestSourceSet(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const SourceModel::IdentifierType   lInitialIdentifiers[] = { 1, 2, 3 };
    const SourceModel::IdentifierType   lReplacementIdentifiers[] = { 5, 8, 13, 21 };
    GroupModel::Sources                 lInitialSources;
    GroupModel::Sources                 lReplacementSources;
    GroupModel                          lGroupModel_1;
    GroupModel                          lGroupModel_2;
    GroupModel                          lGroupModel_3;
    GroupModel                          lGroupModel_4;
    GroupModel                          lGroupModel_5;
    GroupModel                          lGroupModel_6;
    GroupModel                          lGroupModel_7;
    Status                              lStatus;
    const SourceModel::IdentifierType * lIdentifiers;
    size_t                              lActualCount;
    size_t                              lExpectedCount;
    size_t                              lCount;


    // Test 1: Test setting by value to an uninitialized object

    lIdentifiers = nullptr;
    lCount = 0;

    lStatus = lGroupModel_1.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test setting to a default initialized object

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_2.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    lStatus = lGroupModel_2.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 3: Test setting by value to a value initialized object
    //         that is equivalent.

    lStatus = lGroupModel_3.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_3.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_3.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_3.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lExpectedCount = lCount;

    lStatus = lGroupModel_3.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 4: Test setting by value to a value initialized object
    //         that is not equivalent.

    lStatus = lGroupModel_4.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_4.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lIdentifiers = lReplacementIdentifiers;
    lCount = ElementsOf(lReplacementIdentifiers);

    lStatus = lGroupModel_4.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    lStatus = lGroupModel_4.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 5: Test setting a single source

    // Test 5.1: Test setting by value to an uninitialized object

    lStatus = lGroupModel_5.SetSource(lInitialIdentifiers[0]);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 5.2: Test setting by value to an initialized object

    lStatus = lGroupModel_5.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_5.SetSource(lInitialIdentifiers[0]);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = 1;

    lStatus = lGroupModel_5.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 5.3: Test setting by value to an initialized object with
    //           multiple sources replaces all of those sources.

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_5.SetSources(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_5.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lStatus = lGroupModel_5.SetSource(lReplacementIdentifiers[0]);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = 1;

    lStatus = lGroupModel_5.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 6: Test using source collection arguments

    lIdentifiers = lInitialIdentifiers;
    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lInitialSources.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lIdentifiers = lReplacementIdentifiers;
    lCount = ElementsOf(lReplacementIdentifiers);

    lStatus = lReplacementSources.Init(lIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    // Test 6.1: Test setting by collection to an uninitialized object.

    lStatus = lGroupModel_6.SetSources(lInitialSources);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 6.2: Test setting by collection to a default initialized
    //           object.

    lStatus = lInitialSources.GetCount(lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_6.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_6.SetSources(lInitialSources);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_6.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    // Test 6.3: Test setting by collection to an object that already
    //           has that collection.

    lStatus = lInitialSources.GetCount(lExpectedCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_6.SetSources(lInitialSources);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

    lStatus = lGroupModel_6.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    // Test 6.4: Test setting by collection to an object already
    //           containing a collection that is not equivalent.

    lStatus = lGroupModel_7.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lInitialSources.GetCount(lExpectedCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_7.SetSources(lInitialSources);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_7.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

    lStatus = lReplacementSources.GetCount(lExpectedCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_7.SetSources(lReplacementSources);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.GetSources(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);
}

static void TestSourceRemove(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                        lGroupModel_1;
    GroupModel                        lGroupModel_2;
    GroupModel                        lGroupModel_3;
    GroupModel                        lGroupModel_4;
    Status                            lStatus;
    const SourceModel::IdentifierType lInitialIdentifiers[] = { 1, 2, 3 };
    size_t                            lActualCount;
    size_t                            lExpectedCount;
    size_t                            lCount;


    // Test 1: Test removing from an uninitialized object

    lStatus = lGroupModel_1.RemoveSource(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test removing from a default initialized object

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.RemoveSource(0);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test removing from a value initialized object

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCount = ElementsOf(lInitialIdentifiers);

    lStatus = lGroupModel_2.SetSources(lInitialIdentifiers, lCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lExpectedCount = lCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const SourceModel::IdentifierType  lIdentifier = lInitialIdentifiers[i];


        lStatus = lGroupModel_2.RemoveSource(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount--;

        lStatus = lGroupModel_2.GetSources(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);
    }
}

static void TestSourceMutation(nlTestSuite *inSuite, void *inContext)
{
    TestSourceAdd(inSuite, inContext);
    TestSourceSet(inSuite, inContext);
    TestSourceRemove(inSuite, inContext);
}

static void TestZoneAdd(nlTestSuite *inSuite, void *inContext __attribute__((unused)), const ZoneModel::IdentifierType *aInitialIdentifiers, const size_t &aInitialCount, GroupModel &aCollection)
{
    Status                                       lStatus;
    size_t                                       lActualCount;
    size_t                                       lExpectedCount;
    size_t                                       lCount;
    const ZoneModel::IdentifierType  lAddedIdentifiers[] = { 5, 8, 13, 21 };

    // Test 1: Test adding identifiers known to be present in the
    //         collection. Verify non-success status and that the size
    //         remains constant.

    if ((aInitialIdentifiers != nullptr) && (aInitialCount > 0))
    {
        for (size_t i = 0; i < aInitialCount; i++)
        {
            lStatus = aCollection.AddZone(aInitialIdentifiers[i]);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_ValueAlreadySet);

            lStatus = aCollection.GetZones(lActualCount);
            NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
            NL_TEST_ASSERT(inSuite, lActualCount == aInitialCount);
        }
    }

    // Test 2: Test adding an identifier known to be absent from the
    //         collection. Verify that the size increases.

    lCount = ElementsOf(lAddedIdentifiers);
    lExpectedCount = aInitialCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const ZoneModel::IdentifierType  lIdentifier = lAddedIdentifiers[i];
        bool                                         lDoesContainIdentifier;


        lStatus = aCollection.AddZone(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount++;

        lStatus = aCollection.GetZones(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

        lDoesContainIdentifier = aCollection.ContainsZone(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == true);
    }
}

static void TestZoneAdd(nlTestSuite *inSuite, void *inContext)
{
    GroupModel                    lGroupModel_1;
    GroupModel                    lGroupModel_2;
    GroupModel                    lGroupModel_3;
    GroupModel                    lGroupModel_4;
    Status                        lStatus;


    // Test 1: Test adding to an uninitialized object

    lStatus = lGroupModel_1.AddZone(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test adding to a default initialized object

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    TestZoneAdd(inSuite, inContext, nullptr, 0, lGroupModel_1);
}

static void TestZoneRemove(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                       lGroupModel_1;
    GroupModel                       lGroupModel_2;
    GroupModel                       lGroupModel_3;
    GroupModel                       lGroupModel_4;
    Status                           lStatus;
    const ZoneModel::IdentifierType  lInitialIdentifiers[] = { 1, 2, 3 };
    size_t                           lActualCount;
    size_t                           lExpectedCount;
    size_t                           lCount;


    // Test 1: Test removing from an uninitialized object

    lStatus = lGroupModel_1.RemoveZone(0);
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test removing from a default initialized object

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.RemoveZone(0);
    NL_TEST_ASSERT(inSuite, lStatus == -ENOENT);

    // Test 2: Test removing from a value initialized object

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCount = ElementsOf(lInitialIdentifiers);

    for (size_t i = 0; i < lCount; i++)
    {
        lStatus = lGroupModel_2.AddZone(lInitialIdentifiers[i]);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    }

    lExpectedCount = lCount;

    for (size_t i = 0; i < lCount; i++)
    {
        const ZoneModel::IdentifierType  lIdentifier = lInitialIdentifiers[i];
        bool                                         lDoesContainIdentifier;

        lStatus = lGroupModel_2.RemoveZone(lIdentifier);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

        lExpectedCount--;

        lStatus = lGroupModel_2.GetZones(lActualCount);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
        NL_TEST_ASSERT(inSuite, lActualCount == lExpectedCount);

        lDoesContainIdentifier = lGroupModel_2.ContainsZone(lIdentifier);
        NL_TEST_ASSERT(inSuite, lDoesContainIdentifier == false);
    }
}

static void TestZoneClear(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    GroupModel                       lGroupModel_1;
    GroupModel                       lGroupModel_2;
    Status                           lStatus;
    const ZoneModel::IdentifierType  lInitialIdentifiers[] = { 1, 2, 3 };
    size_t                           lActualCount;
    size_t                           lCount;

    // Test 1: Test clearing from an uninitialized object

    lStatus = lGroupModel_1.ClearZones();
    NL_TEST_ASSERT(inSuite, lStatus == kError_NotInitialized);

    // Test 2: Test clearing from a default initialized object

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.GetZones(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    lStatus = lGroupModel_1.ClearZones();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_1.GetZones(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);

    // Test 3: Test clearing from a value initialized object

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lCount = ElementsOf(lInitialIdentifiers);

    for (size_t i = 0; i < lCount; i++)
    {
        lStatus = lGroupModel_2.AddZone(lInitialIdentifiers[i]);
        NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    }

    lStatus = lGroupModel_2.GetZones(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == lCount);

    lStatus = lGroupModel_2.ClearZones();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.GetZones(lActualCount);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);
    NL_TEST_ASSERT(inSuite, lActualCount == 0);
}

static void TestZoneMutation(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    TestZoneAdd(inSuite, inContext);
    TestZoneRemove(inSuite, inContext);
    TestZoneClear(inSuite, inContext);
}

static void TestMutation(nlTestSuite *inSuite, void *inContext)
{
    TestNameMutation(inSuite, inContext);
    TestIdentifierMutation(inSuite, inContext);
    TestVolumeMutation(inSuite, inContext);
    TestMuteMutation(inSuite, inContext);
    TestSourceMutation(inSuite, inContext);
    TestZoneMutation(inSuite, inContext);
}

static void TestEquality(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const GroupModel::IdentifierType   lIdentifierConstant = 13;
    const char *                       lNameConstant_1 = "Test Name";
    const std::string                  lNameConstant_2(lNameConstant_1);
    GroupModel                         lGroupModel_1;
    GroupModel                         lGroupModel_2;
    GroupModel                         lGroupModel_3;
    GroupModel                         lGroupModel_4;
    GroupModel                         lGroupModel_5;
    GroupModel                         lGroupModel_6;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: Test that two default initialized objects are
    //         equivalent.

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_2.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_1 == lGroupModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that two value initialized objects are equivalent.

    lStatus = lGroupModel_3.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.Init(lNameConstant_1, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_3 == lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that two object initialized objects are
    //         equivalent.

    lStatus = lGroupModel_5.Init(lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_5 == lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test than an unintialized object is not equivalent to
    //         an initialized object.

    lAreEqual = (lGroupModel_6 == lGroupModel_3);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 5: Test that two groups with the same name and identifier
    //         but different sources are not equal.

    lStatus = lGroupModel_3.SetSource(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.SetSource(11);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_3 == lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);

    // Test 6: Test that two groups with the same name, identifier,
    //         and source but different volumes are not equal.

    lStatus = lGroupModel_4.SetSource(7);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_3.SetVolume(-37);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lStatus = lGroupModel_4.SetVolume(-43);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_3 == lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == false);
}

static void TestAssignment(nlTestSuite *inSuite, void *inContext __attribute__((unused)))
{
    const GroupModel::IdentifierType   lIdentifierConstant = 13;
    const char *                       lNameConstant = "Test Name";
    GroupModel                         lGroupModel_1;
    GroupModel                         lGroupModel_2;
    GroupModel                         lGroupModel_3;
    GroupModel                         lGroupModel_4;
    GroupModel                         lGroupModel_5;
    Status                             lStatus;
    bool                               lAreEqual;

    // Test 1: There is one uninitialized object and that object
    //         assigned to another object are equivalent.

    lGroupModel_2 = lGroupModel_1;

    lAreEqual = (lGroupModel_1 == lGroupModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 2: Test that one default initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lGroupModel_1.Init();
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lGroupModel_2 = lGroupModel_1;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_1 == lGroupModel_2);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 3: Test that one value initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lGroupModel_3.Init(lNameConstant, lIdentifierConstant);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lGroupModel_4 = lGroupModel_3;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_3 == lGroupModel_4);
    NL_TEST_ASSERT(inSuite, lAreEqual == true);

    // Test 4: Test that one object initialized object and that
    //         object assigned to another object are equivalent.

    lStatus = lGroupModel_4.Init(lGroupModel_3);
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lGroupModel_5 = lGroupModel_4;
    NL_TEST_ASSERT(inSuite, lStatus == kStatus_Success);

    lAreEqual = (lGroupModel_4 == lGroupModel_5);
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
        "Group Model",
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
