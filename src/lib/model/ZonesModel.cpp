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
 *      This file implements a collection object for managing HLX zone
 *      objects.
 *
 */

#include "ZonesModel.hpp"

#include <utility>

#include <errno.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>


using namespace HLX::Common;
using namespace Nuovations;

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ZonesModel :: ZonesModel(void) :
    mZonesMax(0),
    mZones()
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aZonesModel  An immutable reference to the
 *                           zones model to copy from.
 *
 */
ZonesModel :: ZonesModel(const ZonesModel &aZonesModel) :
    mZones(aZonesModel.mZones)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified number of default
 *  zones.
 *
 *  @param[in]  aZonesMax  An immutable reference to the maximum
 *                             number of zones to allow in the
 *                             collection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ZonesModel :: Init(const size_t &aZonesMax)
{
    ZoneModel    lZoneModel;
    Status       lRetval = kStatus_Success;


    mZonesMax = aZonesMax;

    lRetval = lZoneModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    for (IdentifierType lZone = IdentifierModel::kIdentifierMin; (lZone <= aZonesMax); lZone++)
    {
        lRetval = lZoneModel.SetIdentifier(lZone);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetZone(lZone, lZoneModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified zones
 *  model.
 *
 *  @param[in]  aZonesModel  An immutable reference to the
 *                               zones model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of zones in @a
 *                            aZonesModel exceeds kZonesMax.
 *
 */
Status
ZonesModel :: Init(const ZonesModel &aZonesModel)
{
    Status lRetval = kStatus_Success;


    *this = aZonesModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified zones model to this
 *  one.
 *
 *  @param[in]  aZonesModel  An immutable reference to the
 *                               zones model to assign (copy) to
 *                               this one.
 *
 *
 *  @returns
 *    A reference to this zones model after the assignment
 *    (copy) is complete.
 *
 */
ZonesModel &
ZonesModel :: operator =(const ZonesModel &aZonesModel)
{
    mZones = aZonesModel.mZones;

    return (*this);
}

/**
 *  @brief
 *    Validate a zone identifier.
 *
 *  This validates the specified zone identifier.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to the
 *                               zone identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the zone identifier is
 *                            smaller or larger than supported.
 *
 */
Status
ZonesModel :: ValidateIdentifier(const IdentifierType &aZoneIdentifier) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aZoneIdentifier != IdentifierModel::kIdentifierInvalid, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aZoneIdentifier <= mZonesMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the zone model associated with the specified
 *    zone identifier.
 *
 *  This attempts to get the zone model associated with the
 *  specified zone identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aZoneIdentifier  An immutable reference to
 *                                    the zone identifier
 *                                    corresponding to the zone
 *                                    model to get.
 *  @param[out]  aZoneModel       A reference to a pointer
 *                                    for a mutable zone model,
 *                                    set to the requested zone
 *                                    model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
ZonesModel :: GetZone(const IdentifierType &aZoneIdentifier, ZoneModel *&aZoneModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aZoneModel = &mZones.at(aZoneIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the zone model associated with the specified
 *    zone identifier.
 *
 *  This attempts to get the zone model associated with the
 *  specified zone identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aZoneIdentifier  An immutable reference to
 *                                    the zone identifier
 *                                    corresponding to the zone
 *                                    model to get.
 *  @param[out]  aZoneModel       A reference to a pointer for
 *                                    for an imutable zone model,
 *                                    set to the requested zone
 *                                    model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the zones model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
ZonesModel :: GetZone(const IdentifierType &aZoneIdentifier, const ZoneModel *&aZoneModel) const
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aZoneModel = &mZones.at(aZoneIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the zone identifier with the specified name.
 *
 *  This attempts to get the zone model for the zone with the
 *  specified name.
 *
 *  @param[in]   aName           A pointer to a null-terminated C
 *                               string of the name of the zone to
 *                               get.
 *  @param[out]  aZoneModel  A reference to storage by which to
 *                               return a pointer to the model, if
 *                               successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No zone could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
ZonesModel :: GetZone(const char *aName, const ZoneModel *&aZoneModel) const
{
    Status lRetval = -ENOENT;
    Zones::const_iterator current, end;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    current = mZones.begin();
    end     = mZones.end();

    while (current != end)
    {
        const char *  lName;
        Status        lStatus;


        lStatus = current->second.GetName(lName);
        nlREQUIRE_SUCCESS(lStatus, next);

        if (strcmp(lName, aName) == 0)
        {
            aZoneModel = &current->second;
            lRetval    = kStatus_Success;
            break;
        }

    next:
        current++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model zone for the specified identifier.
 *
 *  This initializes the model with the specified zone model
 *  at the specified identifier.
 *
 *  @param[in]  aZoneIdentifier  An immutable reference to
 *                                   the zone identifier
 *                                   corresponding to the zone
 *                                   model to set.
 *  @param[in]  aZoneModel       An immutable reference to
 *                                   the zone model to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aZone
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aZoneIdentifier value
 *                                    is out of range.
 *
 */
Status
ZonesModel :: SetZone(const IdentifierType &aZoneIdentifier, const ZoneModel &aZoneModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aZoneIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mZones[aZoneIdentifier] == aZoneModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mZones[aZoneIdentifier] = aZoneModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided zones model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aZonesModel  An immutable reference to the zones
 *                           model to compare for equality.
 *
 *  @returns
 *    True if this zones model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool ZonesModel :: operator ==(const ZonesModel &aZonesModel) const
{
    return (mZones == aZonesModel.mZones);
}

}; // namespace Model

}; // namespace HLX
