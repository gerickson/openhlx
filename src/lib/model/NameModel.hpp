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
 *      This file defines an object for managing a HLX object name
 *      data model.
 *
 */

#ifndef OPENHLXMMODELNAMEMODEL_HPP
#define OPENHLXMMODELNAMEMODEL_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX object name data model.
 *
 *  This defines an object for managing HLX object names such as those
 *  used for equalizer presets, favorites, groups, sources, and zones.
 *
 *  @ingroup model
 *
 */
class NameModel
{
public:
    static const size_t kNameLengthMax;

public:
    NameModel(void);
    virtual ~NameModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const char *aName);
    Common::Status Init(const char *aName, const size_t &aNameLength);
    Common::Status Init(const std::string &aName);
    Common::Status Init(const NameModel &aNameModel);

    NameModel &operator =(const NameModel &aNameModel);

    Common::Status GetName(const char *&aName) const;
    Common::Status SetName(const char *aName);
    Common::Status SetName(const char *aName, const size_t &aNameLength);

    bool operator ==(const char *aName) const;
    bool operator ==(const std::string &aName) const;
    bool operator ==(const NameModel &aNameModel) const;

private:
    bool        mNameIsNull;
    std::string mName;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELNAMEMODEL_HPP
