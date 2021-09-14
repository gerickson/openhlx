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
 *      This file implements a collection object for managing HLX
 *      equalizer preset objects.
 *
 */

#include "EqualizerPresetsModel.hpp"

#include <utility>

#include <errno.h>
#include <string.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace Nuovations;


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aEqualizerPresetsModel  An immutable reference to the
 *                                      equalizer presets model to
 *                                      copy from.
 *
 */
EqualizerPresetsModel :: EqualizerPresetsModel(const EqualizerPresetsModel &aEqualizerPresetsModel) :
	mEqualizerPresetsMax(aEqualizerPresetsModel.mEqualizerPresetsMax),
    mEqualizerPresets(aEqualizerPresetsModel.mEqualizerPresets)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified number of default
 *  presets.
 *
 *  @param[in]  aEqualizerPresetsMax  An immutable reference to the
 *                                    maximum number of equalizer
 *                                    presets to allow in the
 *                                    collection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetsModel :: Init(const IdentifierType &aEqualizerPresetsMax)
{
    EqualizerPresetModel lEqualizerPresetModel;
    Status               lRetval = kStatus_Success;


    mEqualizerPresetsMax = aEqualizerPresetsMax;

    lRetval = lEqualizerPresetModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    for (IdentifierType lEqualizerPreset = IdentifierModel::kIdentifierMin; (lEqualizerPreset <= aEqualizerPresetsMax); lEqualizerPreset++)
    {
        lRetval = lEqualizerPresetModel.SetIdentifier(lEqualizerPreset);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetEqualizerPreset(lEqualizerPreset, lEqualizerPresetModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified equalizer presets
 *  model.
 *
 *  @param[in]  aEqualizerPresetsModel  An immutable reference to the
 *                                      equalizer presets model to
 *                                      initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of presets in @a
 *                            aEqualizerPresetsModel exceeds
 *                            kEqualizerPresetsMax.
 *
 */
Status
EqualizerPresetsModel :: Init(const EqualizerPresetsModel &aEqualizerPresetsModel)
{
    Status lRetval = kStatus_Success;


    *this = aEqualizerPresetsModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified equalizer presets model to this
 *  one.
 *
 *  @param[in]  aEqualizerPresetsModel  An immutable reference to the
 *                                      equalizer presets model to assign
 *                                      (copy) to this one.
 *
 *
 *  @returns
 *    A reference to this equalizer presets model after the assignment
 *    (copy) is complete.
 *
 */
EqualizerPresetsModel &
EqualizerPresetsModel :: operator =(const EqualizerPresetsModel &aEqualizerPresetsModel)
{
	mEqualizerPresetsMax = aEqualizerPresetsModel.mEqualizerPresetsMax;
    mEqualizerPresets    = aEqualizerPresetsModel.mEqualizerPresets;

    return (*this);
}

/**
 *  @brief
 *    Validate an equalizer preset identifier.
 *
 *  This validates the specified equalizer preset identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to
 *                                          the equalizer preset
 *                                          identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the equalizer preset identifier is
 *                            smaller or larger than supported.
 *
 */
Status
EqualizerPresetsModel :: ValidateIdentifier(const IdentifierType &aEqualizerPresetIdentifier) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aEqualizerPresetIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aEqualizerPresetIdentifier <= mEqualizerPresetsMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer preset model associated with the
 *    specified equalizer preset identifier.
 *
 *  This attempts to get the equalizer preset model associated with the
 *  specified equalizer preset identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerPresetIdentifier  An immutable reference to
 *                                           the equalizer preset
 *                                           identifier corresponding
 *                                           to the equalizer preset
 *                                           model to get.
 *  @param[out]  aEqualizerPresetModel       A reference to a pointer
 *                                           for a mutable equalizer
 *                                           preset model, set to the
 *                                           requested preset model,
 *                                           if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer presets model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
EqualizerPresetsModel :: GetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, EqualizerPresetModel *&aEqualizerPresetModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aEqualizerPresetModel = &mEqualizerPresets.at(aEqualizerPresetIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer preset model associated with the
 *    specified equalizer preset identifier.
 *
 *  This attempts to get the equalizer preset model associated with the
 *  specified equalizer preset identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerPresetIdentifier  An immutable reference to
 *                                           the equalizer preset
 *                                           identifier corresponding
 *                                           to the equalizer preset
 *                                           model to get.
 *  @param[out]  aEqualizerPresetModel       A reference to a pointer for
 *                                           for an imutable equalizer
 *                                           preset model, set to the
 *                                           requested preset model,
 *                                           if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer presets model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
EqualizerPresetsModel :: GetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerPresetModel *&aEqualizerPresetModel) const
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aEqualizerPresetModel = &mEqualizerPresets.at(aEqualizerPresetIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset identifier with the specified name.
 *
 *  This attempts to get the equalizer preset model for the equalizer
 *  preset with the specified name.
 *
 *  @param[in]   aName                  A pointer to a null-terminated
 *                                      C string of the name of the
 *                                      equalizer preset to get.
 *  @param[out]  aEqualizerPresetModel  A reference to storage by which
 *                                      to return a pointer to the model,
 *                                      if successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No equalizer preset could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetsModel :: GetEqualizerPreset(const char *aName, const EqualizerPresetModel *&aEqualizerPresetModel) const
{
    Status lRetval = -ENOENT;
    EqualizerPresets::const_iterator current, end;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    current = mEqualizerPresets.begin();
    end     = mEqualizerPresets.end();

    while (current != end)
    {
        const char *  lName;
        Status        lStatus;


        lStatus = current->second.GetName(lName);
        nlREQUIRE_SUCCESS(lStatus, next);

        if (strcmp(lName, aName) == 0)
        {
            aEqualizerPresetModel = &current->second;
            lRetval               = kStatus_Success;
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
 *    This sets the model equalizer preset for the specified identifier.
 *
 *  This initializes the model with the specified equalizer preset model
 *  at the specified identifier.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference to
 *                                          the equalizer preset
 *                                          identifier corresponding
 *                                          to the equalizer preset
 *                                          model to set.
 *  @param[in]  aEqualizerPresetModel       An immutable reference to
 *                                          the equalizer preset model
 *                                          to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aEqualizerPresetModel
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aEqualizerPresetIdentifier value
 *                                    is out of range.
 *
 */
Status
EqualizerPresetsModel :: SetEqualizerPreset(const IdentifierType &aEqualizerPresetIdentifier, const EqualizerPresetModel &aEqualizerPresetModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mEqualizerPresets[aEqualizerPresetIdentifier] == aEqualizerPresetModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mEqualizerPresets[aEqualizerPresetIdentifier] = aEqualizerPresetModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided equalizer presets model against this one
 *  to determine if they are equal to one another.
 *
 *  @param[in]  aEqualizerPresetsModel  An immutable reference to the
 *                                      equalizer presets model to
 *                                      compare for equality.
 *
 *  @returns
 *    True if this equalizer presets model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
EqualizerPresetsModel :: operator ==(const EqualizerPresetsModel &aEqualizerPresetsModel) const
{
    return ((mEqualizerPresetsMax == aEqualizerPresetsModel.mEqualizerPresetsMax) &&
			(mEqualizerPresets    == aEqualizerPresetsModel.mEqualizerPresets   ));
}

}; // namespace Model

}; // namespace HLX
