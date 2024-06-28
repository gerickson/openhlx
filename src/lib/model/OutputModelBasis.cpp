/*
 *    Copyright (c) 2024 Grant Erickson
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
 *      This file implements an object for managing a HLX output data
 *      model.
 *
 */

#include "OutputModelBasis.hpp"

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The maximum allowed length, in bytes, of an output name.
 *
 */
const size_t OutputModelBasis::kNameLengthMax = NameModel::kNameLengthMax;

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with a null name and identifier.
 *
 *  @retval  kStatus_Success  If successful.
 *
 *  @sa SetIdentifier
 *  @sa SetName
 *
 */
Status
OutputModelBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer to the null-terminated C string
 *                           to initialze the output name with.
 *  @param[in]  aIdentifier  An immutable reference for the output
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
OutputModelBasis :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name extent and
 *  identifier.
 *
 *  @param[in]  aName        A pointer to the start of the string
 *                           name to set.
 *  @param[in]  aNameLength  An immutable reference to the length,
 *                           in bytes, of @a aName.
 *  @param[in]  aIdentifier  An immutable reference for the output
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
OutputModelBasis :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName, aNameLength);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified name and identifier.
 *
 *  @param[in]  aName        A pointer the string to initialze the
 *                           output name with.
 *  @param[in]  aIdentifier  An immutable reference for the output
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
OutputModelBasis :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified output model.
 *
 *  @param[in]  aOutputModelBasis  An immutable reference to the
 *                                 output model to initialize with.
 *
 *  @retval  kStatus_Success  Unconditionally.
 *
 */
Status
OutputModelBasis :: Init(const OutputModelBasis &aOutputModelBasis)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aOutputModelBasis.mIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aOutputModelBasis.mName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mVolume.Init(aOutputModelBasis.mVolume);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified output model to this one.
 *
 *  @param[in]  aOutputModelBasis  An immutable reference to the
 *                                 output model to assign (copy)
 *                                 to this one.
 *
 *
 *  @returns
 *    A reference to this output model after the assignment (copy)
 *    is complete.
 *
 */
OutputModelBasis &
OutputModelBasis :: operator =(const OutputModelBasis &aOutputModelBasis)
{
    mIdentifier = aOutputModelBasis.mIdentifier;
    mName       = aOutputModelBasis.mName;
    mVolume     = aOutputModelBasis.mVolume;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the output identifier.
 *
 *  This attempts to get the output identifier, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            output identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output identifier
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
OutputModelBasis :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.GetIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the output name
 *
 *  This attempts to get the output name, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the output
 *                      name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output name value has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetName
 *
 *  @ingroup name
 *
 */
Status
OutputModelBasis :: GetName(const char *&aName) const
{
    Status lRetval = kStatus_Success;

    lRetval = mName.GetName(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model output volume mute state.
 *
 *  This attempts to get the model output volume mute state, if it has
 *  been previously initialized or set.
 *
 *  @param[out]  aMute  A mutable reference to storage for the
 *                      output volume mute state, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa SetMute
 *  @sa ToggleMute
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: GetMute(MuteType &aMute) const
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.GetMute(aMute);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the model output volume level.
 *
 *  This attempts to get the model output volume level, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aLevel  A mutable reference to storage for the
 *                       output volume level, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: GetVolume(LevelType &aLevel) const
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.GetVolume(aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the model output volume level by one (1) unit.
 *
 *  This attempts to decrease the model output volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting output volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  minimum output volume level.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: DecreaseVolume(LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.DecreaseVolume(aOutLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the model output volume level by one (1) unit.
 *
 *  This attempts to increase the model output volume level by one (1)
 *  unit.
 *
 *  @param[out]  aOutLevel  A mutable reference to storage for the
 *                          resulting output volume level, if
 *                          successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output volume level value
 *                                  has not been initialized with a
 *                                  known value.
 *  @retval  -ERANGE                The attempted adjustment would result
 *                                  in a level that would exceed the
 *                                  maximum output volume level.
 *
 *  @sa SetVolume
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: IncreaseVolume(LevelType &aOutLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.IncreaseVolume(aOutLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model output identifier.
 *
 *  This attempts to set the model with the output identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference to the output
 *                           identifier to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aIdentifier value
 *                                    has already been set.
 *  @retval  -EINVAL                  The specified @a aIdentifier value
 *                                    is invalid.
 *
 */
Status
OutputModelBasis :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.SetIdentifier(aIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model output name.
 *
 *  This attempts to set the model with the specified output name.
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
OutputModelBasis :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model output name.
 *
 *  This attempts to set the model with the specified output name
 *  extent.
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
OutputModelBasis :: SetName(const char *aName, const size_t &aNameLength)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName, aNameLength);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model output volume mute state.
 *
 *  This attempts to set the model with the specified output volume
 *  mute state.
 *
 *  @param[in]  aMute  An immutable reference to the output
 *                     volume mute state to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aMute value
 *                                    has already been set.
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: SetMute(const MuteType &aMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.SetMute(aMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model output volume level.
 *
 *  This attempts to set the model with the specified output volume
 *  level.
 *
 *  @param[in]  aLevel  An immutable reference to the output volume
 *                      level to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aLevel value
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a aLevel value
 *                                    is out of range.
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: SetVolume(const LevelType &aLevel)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.SetVolume(aLevel);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to toggle (flip) the model output volume mute state.
 *
 *  This attempts to toggle (flip) the model output volume mute state.
 *
 *  @param[out]  aOutMute  A mutable reference to storage for the
 *                         resulting output volume mute state, if
 *                         successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the output volume mute state
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @ingroup volume
 *
 */
Status
OutputModelBasis :: ToggleMute(MuteType &aOutMute)
{
    Status lRetval = kStatus_Success;

    lRetval = mVolume.ToggleMute(aOutMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided output model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aOutputModelBasis  An immutable reference to the output
 *                           model to compare for equality.
 *
 *  @returns
 *    True if this output model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
OutputModelBasis :: operator ==(const OutputModelBasis &aOutputModelBasis) const
{
    return ((mIdentifier == aOutputModelBasis.mIdentifier) &&
            (mName       == aOutputModelBasis.mName      ) &&
            (mVolume     == aOutputModelBasis.mVolume    ));
}

}; // namespace Model

}; // namespace HLX
