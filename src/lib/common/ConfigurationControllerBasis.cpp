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
 *      This file implements a base object for realizing a HLX
 *      configuration controller, either in a client or server
 *      context.
 *
 */

#include "ConfigurationControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is the class default initializer.
 *
 *  This initializes the configuration controller basis.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ConfigurationControllerBasis :: Init(void)
{
    return (kStatus_Success);
}

}; // namespace Common

}; // namespace HLX
