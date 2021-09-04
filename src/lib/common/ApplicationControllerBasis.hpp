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
 *      This file defines a base object for all HLX application
 *      controllers.
 *
 */

#ifndef OPENHLXCOMMONAPPLICATIONCONTROLLERBASIS_HPP
#define OPENHLXCOMMONAPPLICATIONCONTROLLERBASIS_HPP


#include <map>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Application
{

/**
 *  @brief
 *    A base object for all HLX application controllers.
 *
 *  @ingroup common
 *  @ingroup application
 *
 */
class ControllerBasis
{
public:
    virtual ~ControllerBasis(void) = default;

protected:
    ControllerBasis(void) = default;
    ControllerBasis(const ControllerBasis &aControllerBasis) = delete;

    ControllerBasis & operator =(const ControllerBasis &aControllerBasis) = delete;

    Common::Status Init(void);
};

}; // namespace Application

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONAPPLICATIONCONTROLLERBASIS_HPP
