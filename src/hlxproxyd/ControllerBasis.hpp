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
 *      This file defines a base object for....
 *
 */

#ifndef OPENHLXPROXYCONTROLLERBASIS_HPP
#define OPENHLXPROXYCONTROLLERBASIS_HPP

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    A base object for....
 *
 *  @ingroup proxy
 *
 */
class ControllerBasis
{
public:
    virtual Common::Status Init(void);

protected:
    ControllerBasis(void);
    ~ControllerBasis(void);

};

}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLERBASIS_HPP
