/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file implements an object for managing a HLX equalizer preset.
 *
 */

#include "EqualizerPresetModel.hpp"

#include <errno.h>

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
 *  The maximum allowed length, in bytes, of an equalizer preset name.
 *
 */
const size_t EqualizerPresetModel::kNameLengthMax = NameModel::kNameLengthMax;

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name, identifier, and
 *  default equalizer bands.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *  @sa SetName
 *
 */
Status
EqualizerPresetModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerBands.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name, identifier,
 *  and default equalizer bands.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the equalizer preset name
 *                           with.
 *  @param[in]  aIdentifier  An immutable reference for the equalizer
 *                           preset identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
EqualizerPresetModel :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerBands.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent,
 *  identifier, and default equalizer bands.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *  @param[in]  aIdentifier  An immutable reference for the equalizer
 *                           preset identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
EqualizerPresetModel :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName, aNameLength);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerBands.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name, identifier,
 *  and default equalizer bands.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           equalizer preset name with.
 *  @param[in]  aIdentifier  An immutable reference for the equalizer
 *                           preset identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
EqualizerPresetModel :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerBands.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified equalizer preset
 *  model.
 *
 *  @param[in]  aEqualizerPresetModel  An immutable reference to the
 *                                     equalizer preset model to
 *                                     initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerPresetModel :: Init(const EqualizerPresetModel &aEqualizerPresetModel)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aEqualizerPresetModel.mIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aEqualizerPresetModel.mName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerBands.Init(aEqualizerPresetModel.mEqualizerBands);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified equalizer preset model to this
 *  one.
 *
 *  @param[in]  aEqualizerPresetModel  An immutable reference to the
 *                                     equalizer preset model to assign
 *                                     (copy) to this one.
 *
 *
 *  @returns
 *    A reference to this equalizer preset model after the assignment
 *    (copy) is complete.
 *
 */
EqualizerPresetModel &
EqualizerPresetModel :: operator =(const EqualizerPresetModel &aEqualizerPresetModel)
{
    mIdentifier     = aEqualizerPresetModel.mIdentifier;
    mName           = aEqualizerPresetModel.mName;
    mEqualizerBands = aEqualizerPresetModel.mEqualizerBands;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the equalizer preset identifier.
 *
 *  This attempts to get the equalizer preset identifier, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            equalizer preset identifier, if
 *                            successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer preset identifier
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
EqualizerPresetModel :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.GetIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer preset name
 *
 *  This attempts to get the equalizer preset name, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the
 *                      equalizer preset name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer preset name
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetModel :: GetName(const char *&aName) const
{
    Status lRetval = kStatus_Success;

    lRetval = mName.GetName(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer band model associated with the
 *    specified equalizer band identifier.
 *
 *  This attempts to get the equalizer band model associated with the
 *  specified equalizer band identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerBandIdentifier  An immutable reference to the
 *                                         equalizer band identifier
 *                                         corresponding to the
 *                                         equalizer band model to get.
 *  @param[out]  aEqualizerBandModel       A reference to a pointer for
 *                                         a mutable equalizer band
 *                                         model, set to the requested
 *                                         band model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer bands model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
EqualizerPresetModel :: GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel)
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerBands.GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the equalizer band model associated with the
 *    specified equalizer band identifier.
 *
 *  This attempts to get the equalizer band model associated with the
 *  specified equalizer band identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aEqualizerBandIdentifier  An immutable reference to the
 *                                         equalizer band identifier
 *                                         corresponding to the
 *                                         equalizer band model to get.
 *  @param[out]  aEqualizerBandModel       A reference to a pointer for
 *                                         an imutable equalizer band
 *                                         model, set to the requested
 *                                         band model, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the equalizer bands model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
EqualizerPresetModel :: GetEqualizerBand(const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const
{
    Status lRetval = kStatus_Success;

    lRetval = mEqualizerBands.GetEqualizerBand(aEqualizerBandIdentifier, aEqualizerBandModel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer preset identifier.
 *
 *  This attempts to set the model with the specified equalizer preset
 *  identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference to the equalizer
 *                           preset identifier to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aIdentifier value
 *                                    has already been set.
 *  @retval  -EINVAL                  The specified @a aIdentifier value
 *                                    is invalid.
 *
 */
Status
EqualizerPresetModel :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.SetIdentifier(aIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer preset name.
 *
 *  This attempts to set the model with the specified equalizer preset
 *  name.
 *
 *  @param[in]  aName        A pointer to the start of the null-
 *                           terminated C string name to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aName was too long.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetModel :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer preset name.
 *
 *  This attempts to set the model with the specified equalizer preset
 *  name extent.
 *
 *  @param[in]  aName        A pointer to the start of the string name
 *                           to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified name has already
 *                                    been set.
 *  @retval  -EINVAL                  If @a aName was null.
 *  @retval  -ENAMETOOLONG            If @a aNameLength was too long.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetModel :: SetName(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName, aNameLength);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided equalizer preset model against this one
 *  to determine if they are equal to one another.
 *
 *  @param[in]  aEqualizerPresetModel  An immutable reference to the
 *                                     equalizer preset model to
 *                                     compare for equality.
 *
 *  @returns
 *    True if this equalizer preset model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool EqualizerPresetModel :: operator ==(const EqualizerPresetModel &aEqualizerPresetModel) const
{
    return ((mName           == aEqualizerPresetModel.mName          ) &&
            (mIdentifier     == aEqualizerPresetModel.mIdentifier    ) &&
            (mEqualizerBands == aEqualizerPresetModel.mEqualizerBands));
}

}; // namespace Model

}; // namespace HLX
