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
 *      This file implements a collection object for managing HLX group
 *      objects.
 *
 */

#include "GroupsModel.hpp"

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
GroupsModel :: GroupsModel(void) :
    mGroupsMax(0),
    mGroups()
{
    return;
}

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aGroupsModel  An immutable reference to the
 *                            groups model to copy from.
 *
 */
GroupsModel :: GroupsModel(const GroupsModel &aGroupsModel) :
    mGroups(aGroupsModel.mGroups)
{
    return;
}

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the model with the specified number of default
 *  groups.
 *
 *  @param[in]  aGroupsMax  An immutable reference to the maximum
 *                          number of groups to allow in the
 *                          collection.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
GroupsModel :: Init(const size_t &aGroupsMax)
{
    GroupModel lGroupModel;
    Status     lRetval = kStatus_Success;


    mGroupsMax = aGroupsMax;

    lRetval = lGroupModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    for (IdentifierType lGroup = IdentifierModel::kIdentifierMin; (lGroup <= aGroupsMax); lGroup++)
    {
        lRetval = lGroupModel.SetIdentifier(lGroup);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetGroup(lGroup, lGroupModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified groups
 *  model.
 *
 *  @param[in]  aGroupsModel  An immutable reference to the
 *                            groups model to initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of groups in @a
 *                            aGroupsModel exceeds kGroupsMax.
 *
 */
Status
GroupsModel :: Init(const GroupsModel &aGroupsModel)
{
    Status lRetval = kStatus_Success;


    *this = aGroupsModel;

    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified groups model to this
 *  one.
 *
 *  @param[in]  aGroupsModel  An immutable reference to the
 *                            groups model to assign (copy) to this
 *                            one.
 *
 *  @returns
 *    A reference to this groups model after the assignment
 *    (copy) is complete.
 *
 */
GroupsModel &
GroupsModel :: operator =(const GroupsModel &aGroupsModel)
{
    mGroups = aGroupsModel.mGroups;

    return (*this);
}

/**
 *  @brief
 *    Validate a group identifier.
 *
 *  This validates the specified group identifier.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to the
 *                                group identifer to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the group identifier is
 *                            smaller or larger than supported.
 *
 */
Status
GroupsModel :: ValidateIdentifier(const IdentifierType &aGroupIdentifier) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aGroupIdentifier != IdentifierModel::kIdentifierInvalid, done, lRetval = -EINVAL);
    nlREQUIRE_ACTION(aGroupIdentifier <= mGroupsMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the group model associated with the specified
 *    group identifier.
 *
 *  This attempts to get the group model associated with the specified
 *  group identifier, if it has been previously initialized or set.
 *
 *  @param[in]   aGroupIdentifier  An immutable reference to
 *                                 the group identifier corresponding
 *                                 to the group model to get.
 *  @param[out]  aGroupModel       A reference to a pointer for a
 *                                 mutable group model, set to the
 *                                 requested group model, if
 *                                 successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the groups model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
GroupsModel :: GetGroup(const IdentifierType &aGroupIdentifier, GroupModel *&aGroupModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aGroupModel = &mGroups.at(aGroupIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Attempt to get the group model associated with the specified
 *    group identifier.
 *
 *  This attempts to get the group model associated with the
 *  specified group identifier, if it has been previously
 *  initialized or set.
 *
 *  @param[in]   aGroupIdentifier  An immutable reference to the
 *                                 group identifier corresponding to
 *                                 the group model to get.

 *  @param[out]  aGroupModel       A reference to a pointer for for an
 *                                 imutable group model, set to the
 *                                 requested group model, if
 *                                 successful.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_NotInitialized  If the groups model has not
 *                                  been completely and successfully
 *                                  initialized.
 *
 *  @sa Init
 *
 */
Status
GroupsModel :: GetGroup(const IdentifierType &aGroupIdentifier, const GroupModel *&aGroupModel) const
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aGroupModel = &mGroups.at(aGroupIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the group identifier with the specified name.
 *
 *  This attempts to get the group model for the group with the
 *  specified name.
 *
 *  @param[in]   aName        A pointer to a null-terminated C
 *                            string of the name of the group to
 *                            get.
 *  @param[out]  aGroupModel  A reference to storage by which to
 *                            return a pointer to the model, if
 *                            successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No group could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
GroupsModel :: GetGroup(const char *aName, const GroupModel *&aGroupModel) const
{
    Status lRetval = -ENOENT;
    Groups::const_iterator current, end;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    current = mGroups.begin();
    end     = mGroups.end();

    while (current != end)
    {
        const char *  lName;
        Status        lStatus;


        lStatus = current->second.GetName(lName);
        nlREQUIRE_SUCCESS(lStatus, next);

        if (strcmp(lName, aName) == 0)
        {
            aGroupModel = &current->second;
            lRetval     = kStatus_Success;
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
 *    This sets the model group for the specified identifier.
 *
 *  This initializes the model with the specified group model
 *  at the specified identifier.
 *
 *  @param[in]  aGroupIdentifier  An immutable reference to
 *                                the group identifier
 *                                corresponding to the group
 *                                model to set.
 *  @param[in]  aGroupModel       An immutable reference to
 *                                the group model to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aGroup
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aGroupIdentifier value
 *                                    is out of range.
 *
 */
Status
GroupsModel :: SetGroup(const IdentifierType &aGroupIdentifier, const GroupModel &aGroupModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aGroupIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mGroups[aGroupIdentifier] == aGroupModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mGroups[aGroupIdentifier] = aGroupModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided groups model against this one to
 *  determine if they are equal to one another.
 *
 *  @param[in]  aGroupsModel  An immutable reference to the groups
 *                            model to compare for equality.
 *
 *  @returns
 *    True if this groups model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool
GroupsModel :: operator ==(const GroupsModel &aGroupsModel) const
{
    return (mGroups == aGroupsModel.mGroups);
}

}; // namespace Model

}; // namespace HLX
