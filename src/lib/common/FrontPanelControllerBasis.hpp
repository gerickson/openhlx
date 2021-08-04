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
 *      This file defines a base object for realizing a HLX physical
 *      front panel interface controller, either in a client or server
 *      context.
 *
 */

#ifndef OPENHLXCOMMONFRONTPANELCONTROLLERBASIS_HPP
#define OPENHLXCOMMONFRONTPANELCONTROLLERBASIS_HPP

#include <OpenHLX/Model/FrontPanelModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX physical front panel interface
 *    controller, either in a client or server context.
 *
 *  @ingroup common
 *
 */
class FrontPanelControllerBasis
{
public:
    virtual ~FrontPanelControllerBasis(void) = default;

protected:
    FrontPanelControllerBasis(void) = default;

protected:
    Model::FrontPanelModel  mFrontPanelModel;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONFRONTPANELCONTROLLERBASIS_HPP
