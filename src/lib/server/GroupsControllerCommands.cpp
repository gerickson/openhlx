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
 *      This file implements objects for HLX server group data model
 *      commands and their constituent requests and responses.
 *
 */

#include "GroupsControllerCommands.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace Groups
{

static const char * const kGroupObject = "G";

/**
 *  Group add zone command request regular expression pattern.
 *
 */
const char * const AddZoneRequest::kRequestRegexp        = "G([[:digit:]]+)AO([[:digit:]]+)";

/**
 *  Group remove (clear) all zones command request regular expression
 *  pattern.
 *
 */
const char * const ClearZonesRequest::kRequestRegexp     = "GAR";

/**
 *  Group remove zone command request regular expression pattern.
 *
 */
const char * const RemoveZoneRequest::kRequestRegexp     = "G([[:digit:]]+)RO([[:digit:]]+)";

/**
 *  Expected number of group add zone command request regular
 *  expression pattern substring matches.
 *
 */
const size_t AddZoneRequest::kExpectedMatches            = 3;

/**
 *  Expected number of group remove (clear) all zones command request
 *  regular expression pattern substring matches.
 *
 */
const size_t ClearZonesRequest::kExpectedMatches         = 1;

/**
 *  Expected number of group remove zone command request regular
 *  expression pattern substring matches.
 *
 */
const size_t RemoveZoneRequest::kExpectedMatches         = 3;

// MARK: Observer Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the query command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
QueryRequest :: Init(void)
{
    return (QueryRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group query command response buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the
 *                                identifier of the group queried.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
QueryResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier)
{
    return (QueryResponseBasis::Init(kGroupObject,
                                     aGroupIdentifier));
}

// MARK: Mutator Requests, Responses, and Commands

// MARK: (Volume) Mute Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set volume mute state command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
MuteRequest :: Init(void)
{
    return (MuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the toggle volume mute state command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ToggleMuteRequest :: Init(void)
{
    return (ToggleMuteRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group set volume mute state command response
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which the volume mute
 *                                state was set.
 *  @param[in]  aMute             An immutable reference to the volume
 *                                mute state that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetMuteResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                        const Model::VolumeModel::MuteType &aMute)
{
    return (VolumeMuteResponseBasis::Init(kGroupObject,
                                          aGroupIdentifier,
                                          aMute));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group toggle volume mute state command
 *  response buffer.
 *
 *  @param[in]  aBuffer  An pointer to an immutable buffer containing
 *                       the toggle volume mute state content to put
 *                       into the response buffer.
 *  @param[in]  aSize    The size, in bytes, of the content pointed to
 *                       by @a aBuffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ToggleMuteResponse :: Init(const uint8_t *aBuffer, const size_t &aSize)
{
    return (ResponseBasis::Init(reinterpret_cast<const char *>(aBuffer), aSize));
}

// MARK: Name Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set name command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group name command response
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference
 *                                for the group
 *                                identifier for which to
 *                                form the name response.
 *  @param[in]  aName             A pointer to the null-
 *                                terminated C string of the
 *                                group name for
 *                                which to form the
 *                                response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                     const char * aName)
{
    return (NameSetResponseBasis::Init(kGroupObject,
                                       aGroupIdentifier,
                                       aName));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group name command response
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference
 *                                for the group
 *                                identifier for which to
 *                                form the name response.
 *  @param[in]  aName             A pointer to the null-
 *                                terminated C string of the
 *                                group name for
 *                                which to form the
 *                                response.
 *  @param[in]  aNameLength       An immutable reference to
 *                                the length, in bytes, of
 *                                @a aName.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
NameResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                     const char * aName,
                     const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kGroupObject,
                                       aGroupIdentifier,
                                       aName,
                                       aNameLength));
}

// MARK: Source Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set source (input) command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetSourceRequest :: Init(void)
{
    return (SourceRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group source (input) command response buffer.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group identifier for which to form
 *                                 the response.
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source (input) identifier for which
 *                                 to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SourceResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier, 
                       const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    return (SourceSetResponseBasis::Init(kGroupObject,
                                         aGroupIdentifier,
                                         aSourceIdentifier));
}

// MARK: Volume Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the decrease volume level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
DecreaseVolumeRequest :: Init(void)
{
    return (DecreaseVolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the increase volume level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
IncreaseVolumeRequest :: Init(void)
{
    return (IncreaseVolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the set volume level command request regular
 *  expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SetVolumeRequest :: Init(void)
{
    return (VolumeRegularExpressionBasis::Init(*this));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group adjust volume level command response
 *  buffer.
 *
 *  @param[in]  aBuffer  An pointer to an immutable buffer containing
 *                       the adjust volume level content to put into
 *                       the response buffer.
 *  @param[in]  aSize    The size, in bytes, of the content pointed to
 *                       by @a aBuffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
AdjustVolumeResponse :: Init(const uint8_t *aBuffer, const size_t &aSize)
{
    return (ResponseBasis::Init(reinterpret_cast<const char *>(aBuffer), aSize));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group set volume level command response
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference for the
 *                                group for which the volume level
 *                                was set.
 *  @param[in]  aVolume           An immutable reference to the volume
 *                                level that was set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
SetVolumeResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                          const Model::VolumeModel::LevelType &aVolume)
{
    return (VolumeResponseBasis::Init(kGroupObject, aGroupIdentifier, aVolume));
}

// MARK: Zone (Membership) Mutator Requests, Responses, and Commands

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the add zone command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
AddZoneRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the remove zone command request regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
RemoveZoneRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group zone membership change command response
 *  buffer.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group identifier for which to form
 *                                 the response.
 *  @param[in]  aOperation         The operation (add or remove) that
 *                                 was performed on the group and zone
 *                                 for which to form the response.
 *  @param[in]  aZoneIdentifier    An immutable reference to the zone
 *                                 identifier which was added or
 *                                 removed for which to form the
 *                                 response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ZoneResponseBasis :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
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

    return (ResponseBasis::Init(lBuffer.c_str(), lBuffer.size()));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group add zone command response buffer.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group identifier for which to form
 *                                 the response.
 *  @param[in]  aZoneIdentifier    An immutable reference to the zone
 *                                 identifier which was added for
 *                                 which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
AddZoneResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                        const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char kAddOperation = 'A';

    return (ZoneResponseBasis::Init(aGroupIdentifier,
                                    kAddOperation,
                                    aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the group remove zone command response buffer.
 *
 *  @param[in]  aGroupIdentifier   An immutable reference for the
 *                                 group identifier for which to form
 *                                 the response.
 *  @param[in]  aZoneIdentifier    An immutable reference to the zone
 *                                 identifier which was removed for
 *                                 which to form the response.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
RemoveZoneResponse :: Init(const Model::GroupModel::IdentifierType &aGroupIdentifier,
                           const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    static const char kRemoveOperation = 'R';

    return (ZoneResponseBasis::Init(aGroupIdentifier,
                                    kRemoveOperation,
                                    aZoneIdentifier));
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the remove (clear) all zones command request
 *  regular expression.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ClearZonesRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the group remove (clear) all zone command
 *  response buffer.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
ClearZonesResponse :: Init(void)
{
    const std::string lBuffer = std::string(kGroupObject) + "AR";

    return (ResponseBasis::Init(lBuffer.c_str()));
}

}; // namespace Groups

}; // namespace Command

}; // namespace Server

}; // namespace HLX
