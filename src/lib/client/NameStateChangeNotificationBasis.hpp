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
 *      model name property state change notification (SCN).
 *
 */

#ifndef OPENHLXCLIENTNAMESTATECHANGENOTIFICATIONBASIS_HPP
#define OPENHLXCLIENTNAMESTATECHANGENOTIFICATIONBASIS_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/NameModel.hpp>

namespace HLX
{

namespace Client
{

namespace StateChange
{

/**
 *  @brief
 *    A base object for a HLX client object data model name property
 *    state change notification (SCN).
 *
 *  @ingroup client
 *  @ingroup state-change
 *
 */
class NameNotificationBasis
{
public:
    virtual ~NameNotificationBasis(void) = default;

    const char *GetName(void) const;

protected:
    NameNotificationBasis(void) = default;

    Common::Status Init(const char *aName, const size_t &aNameLength);
    Common::Status Init(const char *aName);
    Common::Status Init(const std::string &aName);

private:
    std::string mName;
};

}; // namespace StateChange

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTNAMESTATECHANGENOTIFICATIONBASIS_HPP
