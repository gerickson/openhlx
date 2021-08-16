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
 *      This file defines an object for representing default data for
 *      a HLX object name data model.
 *
 */

#ifndef OPENHLXSERVERNAMEMODELDEFAULTS_HPP
#define OPENHLXSERVERNAMEMODELDEFAULTS_HPP

#include <OpenHLX/Model/NameModel.hpp>


namespace HLX
{

namespace Server
{

namespace Defaults
{

/**
 *  @brief
 *    An object for representing default data for a HLX object name
 *    data model.
 *
 *  @private
 *
 */
struct NameModel
{
    const char * const mName;
};

}; // namespace Defaults

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERNAMEMODELDEFAULTS_HPP
