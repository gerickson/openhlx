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
 *      This file implements a collection object for managing HLX source
 *      (input) objects.
 *
 */

#include "SourcesModel.hpp"

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
SourcesModel :: SourcesModel(void) :
    mSourcesMax(0),
    mSources()
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aSourcesModel  An immutable reference to the
 *                             sources model to copy from.
 *
 */
SourcesModel :: SourcesModel(const SourcesModel &aSourcesModel) :
    mSources(aSourcesModel.mSources)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified number of default
 *  sources.
 *
 *  @param[in]  aSourcesMax  An immutable reference to the maximum
 *                           number of sources to allow in the
 *                           collection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourcesModel :: Init(const size_t &aSourcesMax)
{
    SourceModel  lSourceModel;
    Status       lRetval = kStatus_Success;


    mSourcesMax = aSourcesMax;

    lRetval = lSourceModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    for (IdentifierType lSource = IdentifierModel::kIdentifierMin; (lSource <= aSourcesMax); lSource++)
    {
        lRetval = lSourceModel.SetIdentifier(lSource);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetSource(lSource, lSourceModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified sources model.
 *
 *  @param[in]  aSourcesModel  An immutable reference to the
 *                             sources model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of sources in @a
 *                            aSourcesModel exceeds kSourcesMax.
 *
 */
Status
SourcesModel :: Init(const SourcesModel &aSourcesModel)
{
    Status lRetval = kStatus_Success;


    *this = aSourcesModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified sources model to this one.
 *
 *  @param[in]  aSourcesModel  An immutable reference to the
 *                             sources model to assign (copy) to this
 *                             one.
 *
 *  @returns
 *    A reference to this sources model after the assignment
 *    (copy) is complete.
 *
 */
SourcesModel &
SourcesModel :: operator =(const SourcesModel &aSourcesModel)
{
    mSources = aSourcesModel.mSources;

    return (*this);
}

/**
 *  @brief
 *    Validate an source identifier.
 *
 *  This validates the specified source identifier.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to the
 *                                 source identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the source identifier is
 *                            smaller or larger than supported.
 *
 */
Status
SourcesModel :: ValidateIdentifier(const IdentifierType &aSourceIdentifier) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aSourceIdentifier != IdentifierModel::kIdentifierInvalid, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aSourceIdentifier <= mSourcesMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the source model associated with the specified
 *    source identifier.
 *
 *  This attempts to get the source model associated with the
 *  specified source identifier, if it has been previously initialized
 *  or set.
 *
 *  @param[in]   aSourceIdentifier  An immutable reference to
 *                                  the source identifier
 *                                  corresponding to the source model
 *                                  to get.
 *  @param[out]  aSourceModel       A reference to a pointer
 *                                  for a mutable source model, set to
 *                                  the requested source model, if
 *                                  successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the sources model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
SourcesModel :: GetSource(const IdentifierType &aSourceIdentifier, SourceModel *&aSourceModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aSourceModel = &mSources.at(aSourceIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the source model associated with the specified
 *    source identifier.
 *
 *  This attempts to get the source model associated with the
 *  specified source identifier, if it has been previously initialized
 *  or set.
 *
 *  @param[in]   aSourceIdentifier  An immutable reference to
 *                                  the source identifier
 *                                  corresponding to the source model
 *                                  to get.
 *  @param[out]  aSourceModel       A reference to a pointer for
 *                                  for an imutable source model, set
 *                                  to the requested source model, if
 *                                  successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the sources model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
SourcesModel :: GetSource(const IdentifierType &aSourceIdentifier, const SourceModel *&aSourceModel) const
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aSourceModel = &mSources.at(aSourceIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the source identifier with the specified name.
 *
 *  This attempts to get the source model for the source with the
 *  specified name.
 *
 *  @param[in]   aName         A pointer to a null-terminated C
 *                             string of the name of the source to
 *                             get.
 *  @param[out]  aSourceModel  A reference to storage by which to
 *                             return a pointer to the model, if
 *                             successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No source could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
SourcesModel :: GetSource(const char *aName, const SourceModel *&aSourceModel) const
{
    Status lRetval = -ENOENT;
    Sources::const_iterator current, end;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    current = mSources.begin();
    end     = mSources.end();

    while (current != end)
    {
        const char *  lName;
        Status        lStatus;


        lStatus = current->second.GetName(lName);
        nlREQUIRE_SUCCESS(lStatus, next);

        if (strcmp(lName, aName) == 0)
        {
            aSourceModel = &current->second;
            lRetval      = kStatus_Success;
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
 *    This sets the model source for the specified identifier.
 *
 *  This initializes the model with the specified source model
 *  at the specified identifier.
 *
 *  @param[in]  aSourceIdentifier  An immutable reference to
 *                                 the source identifier
 *                                 corresponding to the source
 *                                 model to set.
 *  @param[in]  aSourceModel       An immutable reference to
 *                                 the source model to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aSource
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aSourceIdentifier value
 *                                    is out of range.
 *
 */
Status
SourcesModel :: SetSource(const IdentifierType &aSourceIdentifier, const SourceModel &aSourceModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aSourceIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mSources[aSourceIdentifier] == aSourceModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mSources[aSourceIdentifier] = aSourceModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided sources model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aSourcesModel  An immutable reference to the sources
 *                             model to compare for equality.
 *
 *  @returns
 *    True if this sources model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool SourcesModel :: operator ==(const SourcesModel &aSourcesModel) const
{
    return (mSources == aSourcesModel.mSources);
}

}; // namespace Model

}; // namespace HLX
