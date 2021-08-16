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
 *      This file defines a base object for realizing a HLX sources
 *      (inputs) controller, either in a client or server context.
 *
 */

#ifndef OPENHLXCOMMONSOURCESCONTROLLERBASIS_HPP
#define OPENHLXCOMMONSOURCESCONTROLLERBASIS_HPP

#include <OpenHLX/Model/SourceModel.hpp>
#include <OpenHLX/Model/SourcesModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX sources (inputs) controller,
 *    either in a client or server context.
 *
 *  @ingroup common
 *
 */
class SourcesControllerBasis
{
public:
    /**
     *  A locally-scoped convenience type for a source identifier.
     *
     */
    typedef Model::SourceModel::IdentifierType IdentifierType;

public:
    virtual ~SourcesControllerBasis(void) = default;

    // Observer Methods

    static Common::Status GetSourcesMax(size_t &aSources);
    static size_t         GetSourcesMax(void);

    static bool           IsValidIdentifier(const IdentifierType &aSourceIdentifier);
    static Common::Status ValidateIdentifier(const IdentifierType &aSourceIdentifier);

protected:
    SourcesControllerBasis(void) = default;

    // Initializer(s)

    Common::Status Init(void);

protected:
    Model::SourcesModel          mSources;

protected:
    static const IdentifierType  kSourcesMax;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONSOURCESCONTROLLERBASIS_HPP
