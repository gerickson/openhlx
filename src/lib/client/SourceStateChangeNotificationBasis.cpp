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
 *      This file implements a base object for a HLX client object data
 *      model source property state change notification (SCN).
 *
 */

#include "SourceStateChangeNotificationBasis.hpp"


using namespace HLX::Common;
using namespace HLX::Model;


namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
SourceNotificationBasis :: SourceNotificationBasis(void) :
    mSource(IdentifierModel::kIdentifierInvalid)
{
    return;
}

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the state change notification with the specified
 *  source identifier.
 *
 *  @param[in]  aSourceIdentifier           An immutable reference
 *                                          to the identifier of the
 *                                          source object whose state
 *                                          changed.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
SourceNotificationBasis :: Init(const IdentifierType &aSourceIdentifier)
{
    Status lRetval = kStatus_Success;

    mSource = aSourceIdentifier;

    return (lRetval);
}

/**
 *  @brief
 *    Return the state change class object identifier.
 *
 *  @returns
 *    The identifier of the class object whose state changed.
 *
 */
SourceNotificationBasis::IdentifierType
SourceNotificationBasis :: GetSource(void) const
{
    return (mSource);
}

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX
