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
 *      This file defines a base object for a HLX client object data
 *      model identifier property state change notification (SCN).
 *
 */

#ifndef HLXCLIENTIDENTIFIERSTATECHANGENOTIFICATIONBASIS_HPP
#define HLXCLIENTIDENTIFIERSTATECHANGENOTIFICATIONBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A base object for a HLX client object data model identifier
 *    property state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *
 */
class IdentifierNotificationBasis
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType IdentifierType;

public:
    virtual ~IdentifierNotificationBasis(void) = default;

    IdentifierType GetIdentifier(void) const;

protected:
    IdentifierNotificationBasis(void);

    Common::Status Init(const IdentifierType &aIdentifier);

private:
    IdentifierType mIdentifier;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTIDENTIFIERSTATECHANGENOTIFICATIONBASIS_HPP
