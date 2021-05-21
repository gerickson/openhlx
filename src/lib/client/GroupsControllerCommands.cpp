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
 *      This file implements objects for HLX client group data model
 *      commands and their constituent requests and responses.
 *
 */

#include "GroupsControllerCommands.hpp"

#include <string>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/OutputStringStream.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace Command
{

namespace Groups
{

static const char * const kGroupObject = "G";

/**
 *  The group zone membership command regular expression pattern
 *  string.
 *
 */
const char * const ZoneResponse::kResponseRegexp = "G([[:digit:]]+)([AR])O([[:digit:]]+)";

/**
 *  The group zone membership command regular expression pattern
 *  expected substring matches.
 *
 */
const size_t ZoneResponse::kExpectedMatches      = 4;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group query command request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the identifier
 *                                of the group to query.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (QueryRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group query command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group query command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the identifier
 *                                of the group to query.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
Query :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute set (assert/enable) command
 *  request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set
 *                                (assert/enable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMuteRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (VolumeMuteSetRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group volume mute command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMuteResponse :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute set (assert/enable) command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set
 *                                (assert/enable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMute :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute clear (deassert/disable)
 *  command request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to clear
 *                                (deassert/disable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ClearMuteRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (VolumeMuteClearRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute clear (deassert/disable)
 *  command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to clear
 *                                (deassert/disable) volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ClearMute :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute toggle (flip) command
 *  request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to toggle (flip)
 *                                volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMuteRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (VolumeMuteToggleRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group volume toggle mute command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMuteResponse :: Init(void)
{
    return (ToggleMuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume mute toggle (flip) command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to toggle (flip)
 *                                volume mute.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMute :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group set name command request
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the
 *                                name.
 *  @param[in]  aName             A pointer to the null-terminated
 *                                C string to set the group name
 *                                to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetNameRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                       const char *aName)
{
    return (NameSetRequestBasis::Init(kGroupObject, aGroupIdentifier, aName));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group name command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group set name command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the
 *                                name.
 *  @param[in]  aName             A pointer to the null-terminated
 *                                C string to set the group name
 *                                to.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetName :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group source (input) set command request
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSourceRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                         const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetRequestBasis::Init(kGroupObject, aGroupIdentifier, aSourceIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group source (input) command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceResponse :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group source (input) set command.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group for which to set the
 *                                 source (input) state.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetSource :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                  const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier, aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level set command request
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                         const Model::VolumeModel::LevelType &aLevel)
{
    return (VolumeSetRequestBasis::Init(kGroupObject, aGroupIdentifier, aLevel));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group volume command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeResponse :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level set command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to set the volume
 *                                level state.
 *  @param[in]  aLevel            An immutable reference to the
 *                                volume level state to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolume :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                  const Model::VolumeModel::LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier, aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level increase by one (1) unit
 *  command request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseVolumeRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (VolumeIncreaseRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group increase volume command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseVolumeResponse :: Init(void)
{
    return (IncreaseVolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level increase by one (1) unit
 *  command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to increase the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
IncreaseVolume :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level decrease by one (1) unit
 *  command request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseVolumeRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (VolumeDecreaseRequestBasis::Init(kGroupObject, aGroupIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group decrease volume command response regular
 *  expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseVolumeResponse :: Init(void)
{
    return (DecreaseVolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group volume level decrease by one (1) unit
 *  command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to decrease the
 *                                volume level state.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
DecreaseVolume :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mRequest.Init(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Zone (Membership) Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership mutation command
 *  request buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to add or remove the
 *                                zone.
 *  @param[in]  aOperation        The operation to perform on the group
 *                                and zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to add to or remove
 *                                from the group.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ZoneRequestBasis :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                         char aOperation,
                         const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char   kZoneObject = 'O';
    OutputStringStream  lGroupIdentifierStream;
    OutputStringStream  lZoneIdentifierStream;
    std::string         lBuffer;


    lGroupIdentifierStream << aGroupIdentifier;
    lZoneIdentifierStream << aZoneIdentifier;

    // Compose the buffer.

    lBuffer = kGroupObject;
    lBuffer += lGroupIdentifierStream.str();
    lBuffer += aOperation;
    lBuffer += kZoneObject;
    lBuffer += lZoneIdentifierStream.str();

    return (RequestBasis::Init(lBuffer.c_str(), lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership add command request
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to add the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to add to the group.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
AddZoneRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                       const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char kAddOperation = 'A';
    Status lRetval = kStatus_Success;


    lRetval = ZoneRequestBasis::Init(aGroupIdentifier, kAddOperation, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership remove command request
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to remove the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to remove to the group.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RemoveZoneRequest :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                          const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char kRemoveOperation = 'R';
    Status lRetval = kStatus_Success;


    lRetval = ZoneRequestBasis::Init(aGroupIdentifier, kRemoveOperation, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group zone membership command response
 *  regular expression.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ZoneResponse :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = ResponseBasis::Init(kResponseRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership add command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to add the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to add to the group.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
AddZone :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;


    lRetval = mRequest.Init(aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership remove command.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which to remove the zone.
 *  @param[in]  aZoneIdentifier   An immutable reference for the
 *                                zone identifier to remove to the group.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RemoveZone :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, const ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status lRetval = kStatus_Success;


    lRetval = mRequest.Init(aGroupIdentifier, aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mResponse.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = ExchangeBasis::Init(mRequest, mResponse);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Groups

}; // namespace Command

}; // namespace Client

}; // namespace HLX
