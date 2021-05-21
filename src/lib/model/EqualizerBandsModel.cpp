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
 *      This file implements a collection object for managing HLX
 *      equalizer band objects.
 *
 */

#include "EqualizerBandsModel.hpp"

#include <utility>

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

static const EqualizerBandModel::FrequencyType kFrequencyMap[EqualizerBandsModel::kEqualizerBandsMax] =
{
       32,
       64,
      125,
      250,
      500,
     1000,
     2000,
     4000,
     8000,
    16000
};

/**
 *  @brief
 *    This is a class copy constructor.
 *
 *  @param[in]  aEqualizerBandsModel  An immutable reference to the
 *                                    equalizer bands model to copy
 *                                    from.
 *
 */
EqualizerBandsModel :: EqualizerBandsModel(const EqualizerBandsModel &aEqualizerBandsModel) :
    mEqualizerBands(aEqualizerBandsModel.mEqualizerBands)
{
    return;
}

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the model with bands of the default center
 *  frequencies and null levels.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
EqualizerBandsModel :: Init(void)
{
    Status              lRetval = kStatus_Success;


    for (IdentifierType lEqualizerBand = IdentifierModel::kIdentifierMin; (lEqualizerBand <= kEqualizerBandsMax); lEqualizerBand++)
    {
        EqualizerBandModel  lEqualizerBandModel;

        lRetval = lEqualizerBandModel.Init(kFrequencyMap[lEqualizerBand - 1]);
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = SetEqualizerBand(lEqualizerBand, lEqualizerBandModel);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class copy initializer.
 *
 *  This initializes the class with the specified equalizer bands
 *  model.
 *
 *  @param[in]  aEqualizerBandsModel  An immutable reference to the
 *                                    equalizer bands model to
 *                                    initialize with.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          The number of bands in @a aEqualizerBandsModel
 *                            exceeds kEqualizerBandsMax.
 *
 */
Status
EqualizerBandsModel :: Init(const EqualizerBandsModel &aEqualizerBandsModel)
{
    Status lRetval = kStatus_Success;

    nlREQUIRE_ACTION(aEqualizerBandsModel.mEqualizerBands.size() <= kEqualizerBandsMax, done, lRetval = -EINVAL);

    *this = aEqualizerBandsModel;

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class assignment (copy) operator.
 *
 *  This assigns (copies) the specified equalizer bands model to this
 *  one.
 *
 *  @param[in]  aEqualizerBandsModel  An immutable reference to the
 *                                    equalizer bands model to assign
 *                                    (copy) to this one.
 *
 *
 *  @returns
 *    A reference to this equalizer bands model after the assignment
 *    (copy) is complete.
 *
 */
EqualizerBandsModel &
EqualizerBandsModel :: operator =(const EqualizerBandsModel &aEqualizerBandsModel)
{
    mEqualizerBands = aEqualizerBandsModel.mEqualizerBands;

    return (*this);
}

/**
 *  @brief
 *    Validate an equalizer band identifier.
 *
 *  This validates the specified equalizer band identifier.
 *
 *  @param[in]  aEqualizerBandIdentifier  An immutable reference to the
 *                                        equalizer band identifer
 *                                        to validate.
 *
 *  @retval  kStatus_Success  If valid.
 *  @retval  -ERANGE          If the equalizer band identifier is
 *                            smaller or larger than supported.
 *
 */
Status
EqualizerBandsModel :: ValidateIdentifier(const IdentifierType &aEqualizerBandIdentifier)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aEqualizerBandIdentifier >= IdentifierModel::kIdentifierMin, done, lRetval = -ERANGE);
    nlREQUIRE_ACTION(aEqualizerBandIdentifier <= kEqualizerBandsMax, done, lRetval = -ERANGE);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Determine whether or not an equalizer band identifier is valid.
 *
 *  This determines whether or not the specified equalizer band
 *  identifier is valid.
 *
 *  @param[in]  aEqualizerBandIdentifier  An immutable reference to the
 *                                        equalizer band identifer
 *                                        to validate.
 *
 *  @returns
 *    True if the identifier is valid; otherwise, false.
 *
 */
bool
EqualizerBandsModel :: IsValidIdentifier(const IdentifierType &aEqualizerBandIdentifier)
{
    const bool lIsValid = (ValidateIdentifier(aEqualizerBandIdentifier) == kStatus_Success);

    return (lIsValid);
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
EqualizerBandsModel :: GetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, EqualizerBandModel *&aEqualizerBandModel)
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(mEqualizerBands.size() == kEqualizerBandsMax, done, lRetval = kError_NotInitialized);

    lRetval = ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aEqualizerBandModel = &mEqualizerBands.at(aEqualizerBandIdentifier);

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
 *                                         an immutable equalizer band
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
EqualizerBandsModel :: GetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel *&aEqualizerBandModel) const
{
    Status lRetval = kStatus_Success;


    nlREQUIRE_ACTION(mEqualizerBands.size() == kEqualizerBandsMax, done, lRetval = kError_NotInitialized);

    lRetval = ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    aEqualizerBandModel = &mEqualizerBands.at(aEqualizerBandIdentifier);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This sets the model equalizer band for the specified identifier.
 *
 *  This initializes the model with the specified equalizer band model
 *  at the specified identifier.
 *
 *  @param[in]  aEqualizerBandIdentifier  An immutable reference to the
 *                                        equalizer band identifier
 *                                        corresponding to the
 *                                        equalizer band model to set.
 *  @param[in]  aEqualizerBandModel       An immutable reference to the
 *                                        equalizer band model to
 *                                        set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  The specified @a aEqualizerBandModel
 *                                    has already been set.
 *  @retval  -ERANGE                  The specified @a
 *                                    aEqualizerBandIdentifier value
 *                                    is out of range.
 *
 */
Status
EqualizerBandsModel :: SetEqualizerBand(const IdentifierType &aEqualizerBandIdentifier, const EqualizerBandModel &aEqualizerBandModel)
{
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    if (mEqualizerBands[aEqualizerBandIdentifier] == aEqualizerBandModel)
    {
        lRetval = kStatus_ValueAlreadySet;
    }
    else
    {
        mEqualizerBands[aEqualizerBandIdentifier] = aEqualizerBandModel;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    This is a class equality operator.
 *
 *  This compares the provided equalizer bands model against this one
 *  to determine if they are equal to one another.
 *
 *  @param[in]  aEqualizerBandsModel  An immutable reference to the
 *                                    equalizer bands model to compare
 *                                    for equality.
 *
 *  @returns
 *    True if this equalizer bands model is equal to the specified one;
 *    otherwise, false.
 *
 */
bool EqualizerBandsModel :: operator ==(const EqualizerBandsModel &aEqualizerBandsModel) const
{
    return (mEqualizerBands == aEqualizerBandsModel.mEqualizerBands);
}

}; // namespace Model

}; // namespace HLX
