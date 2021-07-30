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
 *      This file defines an object for....
 *
 */

#ifndef OPENHLXPROXYZONESCONTROLLER_HPP
#define OPENHLXPROXYZONESCONTROLLER_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>
#include <OpenHLX/Common/ZonesControllerBasis.hpp>

#include "ControllerBasis.hpp"


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    An object for....
 *
 *  @ingroup proxy
 *  @ingroup zone
 *
 */
class ZonesController :
    public ControllerBasis,
    public Common::ZonesControllerBasis
{
public:
    ZonesController(void);
    virtual ~ZonesController(void);

    Common::Status Init(void) final;

    Common::Status Refresh(const Common::Timeout &aTimeout) /* final */;

};

}; // namespace Proxy

}; // namespace HLX

#endif // HLXPROXYZONESCONTROLLER_HPP
