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
 *      This file defines a delegate interface for the server
 *      controller basis object.
 *
 */

#ifndef OPENHLXSIMULATOROBJECTCONTROLLERBASISDELEGATE_HPP
#define OPENHLXSIMULATOROBJECTCONTROLLERBASISDELEGATE_HPP


namespace HLX
{

namespace Simulator
{

class ObjectControllerBasis;

/**
 *  @brief
 *    A delegate interface for the server controller basis object.
 *
 *  @ingroup server
 *
 */
class ObjectControllerBasisDelegate
{
public:
    ObjectControllerBasisDelegate(void) = default;
    virtual ~ObjectControllerBasisDelegate(void) = default;

    virtual void ControllerConfigurationIsDirty(Simulator::ObjectControllerBasis &aController) = 0;
};

}; // namespace Simulator

}; // namespace HLX

#endif // OPENHLXSIMULATOROBJECTCONTROLLERBASISDELEGATE_HPP
