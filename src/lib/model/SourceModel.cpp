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
 *      This file implements an object for managing a HLX source (input).
 *
 */

#include "SourceModel.hpp"

#include <errno.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Model
{

/**
 *  The maximum allowed length, in bytes, of a source name.
 *
 */
const size_t SourceModel::kNameLengthMax = NameModel::kNameLengthMax;

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
SourceModel :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init();
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
 *                           to initialze the source name with.
 *  @param[in]  aIdentifier  An immutable reference for the source
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
SourceModel :: Init(const char *aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
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
 *  @param[in]  aIdentifier  An immutable reference for the source
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aNameLength was too long.
 *
 */
Status
SourceModel :: Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName, aNameLength);
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
 *                           source name with.
 *  @param[in]  aIdentifier  An immutable reference for the source
 *                           identifier to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aIdentifier was invalid or if @a
 *                            aName was null.
 *  @retval  -ENAMETOOLONG    If @a aName was too long.
 *
 */
Status
SourceModel :: Init(const std::string &aName, const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified source model.
 *
 *  @param[in]  aSourceModel  An immutable reference to the
 *                            source model to initialize
 *                            with.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceModel :: Init(const SourceModel &aSourceModel)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.Init(aSourceModel.mIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mName.Init(aSourceModel.mName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified source model to this one.
 *
 *  @param[in]  aSourceModel  An immutable reference to the
 *                            source model to assign (copy) to this
 *                            one.
 *
 *  @returns
 *    A reference to this source model after the assignment (copy)
 *    is complete.
 *
 */
SourceModel &
SourceModel :: operator =(const SourceModel &aSourceModel)
{
    mIdentifier = aSourceModel.mIdentifier;
    mName       = aSourceModel.mName;

    return (*this);
}

/**
 *  @brief
 *    Attempt to get the source identifier.
 *
 *  This attempts to get the source identifier, if it has been
 *  previously initialized or set.
 *
 *  @param[out]  aIdentifier  A mutable reference to storage for the
 *                            source identifier, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the source identifier
 *                                  value has not been initialized
 *                                  with a known value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 */
Status
SourceModel :: GetIdentifier(IdentifierType &aIdentifier) const
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.GetIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the source name
 *
 *  This attempts to get the source name, if it has been previously
 *  initialized or set.
 *
 *  @param[out]  aName  A reference to pointer to an immutable
 *                      null-terminated C string for the source
 *                      name, if successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the source name value has
 *                                  not been initialized with a known
 *                                  value.
 *
 *  @sa Init
 *  @sa SetIdentifier
 *
 *  @ingroup name
 *
 */
Status
SourceModel :: GetName(const char *&aName) const
{
    Status lRetval = kStatus_Success;

    lRetval = mName.GetName(aName);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model source identifier.
 *
 *  This attempts to set the model with the source identifier.
 *
 *  @param[in]  aIdentifier  An immutable reference to the source
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
SourceModel :: SetIdentifier(const IdentifierType &aIdentifier)
{
    Status lRetval = kStatus_Success;

    lRetval = mIdentifier.SetIdentifier(aIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model source name.
 *
 *  This attempts to set the model with the specified source name.
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
SourceModel :: SetName(const char *aName)
{
    Status lRetval = kStatus_Success;

    lRetval = mName.SetName(aName);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model source name.
 *
 *  This attempts to set the model with the specified source name
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
SourceModel :: SetName(const char *aName, const size_t &aNameLength)
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
 *  This compares the provided source model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aSourceModel  An immutable reference to the source
 *                            model to compare for equality.
 *
 *  @returns
 *    True if this source model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool SourceModel :: operator ==(const SourceModel &aSourceModel) const
{
    return ((mName       == aSourceModel.mName      ) &&
            (mIdentifier == aSourceModel.mIdentifier));
}

}; // namespace Model

}; // namespace HLX
