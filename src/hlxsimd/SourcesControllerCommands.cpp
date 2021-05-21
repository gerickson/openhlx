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
 *      This file...
 *
 */

#include "SourcesControllerCommands.hpp"

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;

namespace HLX
{

namespace Server
{

namespace Command
{

namespace Sources
{

static const char * const kSourceObject = "I";

// MARK: Mutator Requests, Responses, and Commands

// MARK: Name Mutator Requests, Responses, and Commands

Status SetNameRequest :: Init(void)
{
    return (NameRegularExpressionBasis::Init(*this));
}

Status NameResponse :: Init(const Model::SourceModel::IdentifierType &aSource,
                            const char * aName)
{
    return (NameSetResponseBasis::Init(kSourceObject, aSource, aName));
}

Status NameResponse :: Init(const Model::SourceModel::IdentifierType &aSource,
                            const char * aName,
                            const size_t &aNameLength)
{
    return (NameSetResponseBasis::Init(kSourceObject, aSource, aName, aNameLength));
}

}; // namespace Sources

}; // namespace Command

}; // namespace Server

}; // namespace HLX
