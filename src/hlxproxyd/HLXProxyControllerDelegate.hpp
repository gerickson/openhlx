/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file defines a delegate interface for the top-level HLX
 *      proxy controller object.
 *
 */

#ifndef OPENHLXPROXYCONTROLLERDELEGATE_HPP
#define OPENHLXPROXYCONTROLLERDELEGATE_HPP

namespace HLX
{

namespace Proxy
{

class Controller;

/**
 *  @brief
 *    A delegate interface for the top-level HLX proxy controller
 *    object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of the proxy as it
 *  moves through its lifetime.
 *
 *  @ingroup proxy
 *
 */
class ControllerDelegate
{
public:
    ControllerDelegate(void) = default;
    virtual ~ControllerDelegate(void) = default;

};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLERDELEGATE_HPP
