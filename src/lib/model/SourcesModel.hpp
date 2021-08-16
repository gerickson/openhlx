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
 *      This file defines a collection object for managing HLX source
 *      (input) objects.
 *
 */

#ifndef OPENHLXMMODELSOURCESMODEL_HPP
#define OPENHLXMMODELSOURCESMODEL_HPP

#include <map>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/SourceModel.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    A collection object for managing HLX source (input) objects.
 *
 *  @ingroup model
 *
 */
class SourcesModel
{
public:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  source model.
     *
     */
    typedef SourceModel::IdentifierType IdentifierType;

public:
    SourcesModel(void);
    SourcesModel(const SourcesModel &aSourcesModel);
    virtual ~SourcesModel(void) = default;

    Common::Status Init(const size_t &aSourcesMax);
    Common::Status Init(const SourcesModel &aSourcesModel);

    SourcesModel &operator =(const SourcesModel &aSourcesModel);

    Common::Status GetSource(const IdentifierType &aSource, SourceModel *&aSourceModel);
    Common::Status GetSource(const IdentifierType &aSource, const SourceModel *&aSourceModel) const;
    Common::Status GetSource(const char *aName, const SourceModel *&aSourceModel) const;

    Common::Status SetSource(const IdentifierType &aSource, const SourceModel &aSourceModel);

    bool operator ==(const SourcesModel &aSourcesModel) const;

private:
    Common::Status ValidateIdentifier(const IdentifierType &aSourceIdentifier) const;

private:
    typedef std::map<IdentifierType, SourceModel> Sources;

    size_t    mSourcesMax;
    Sources   mSources;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELSOURCESMODEL_HPP
