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
 *      This file implements a base object for all HLX application
 *      controllers.
 *
 */

#include "ApplicationControllerBasis.hpp"


using namespace HLX::Common;


namespace HLX
{

namespace Common
{

namespace Application
{

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the application controller basis.
 *
 *  @retval  kStatus_Success  If successful.
 *
 */
Status
ControllerBasis :: Init(void)
{
    return (kStatus_Success);
}

}; // namespace Application

}; // namespace Common

}; // namespace HLX
