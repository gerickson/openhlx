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
 *      This file defines an object for managing a HLX source (input)
 *      data model.
 *
 */

#ifndef OPENHLXMMODELSOURCEMODEL_HPP
#define OPENHLXMMODELSOURCEMODEL_HPP

#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/NameModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing a HLX source (input) data model.
 *
 *  @ingroup model
 *  @ingroup source
 *
 */
class SourceModel
{
public:
    static const size_t kNameLengthMax;

    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef IdentifierModel::IdentifierType IdentifierType;

public:
    SourceModel(void) = default;
    virtual ~SourceModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const char *aName, const IdentifierType &aIdentifier);
    Common::Status Init(const char *aName, const size_t &aNameLength, const IdentifierType &aIdentifier);
    Common::Status Init(const std::string &aName, const IdentifierType &aIdentifier);
    Common::Status Init(const SourceModel &aSourceModel);

    SourceModel &operator =(const SourceModel &aSourceModel);

    Common::Status GetName(const char *&aName) const;
    Common::Status GetIdentifier(IdentifierType &aIdentifier) const;

    Common::Status SetName(const char *aName);
    Common::Status SetName(const char *aName, const size_t &aNameLength);
    Common::Status SetIdentifier(const IdentifierType &aIdentifier);

    bool operator ==(const SourceModel &aSourceModel) const;

private:
    IdentifierModel mIdentifier;
    NameModel       mName;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELSOURCEMODEL_HPP
