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
 *      This file defines a base object for realizing a HLX network
 *      controller, either in a client or server context.
 *
 */

#ifndef OPENHLXCOMMONNETWORKCONTROLLERBASIS_HPP
#define OPENHLXCOMMONNETWORKCONTROLLERBASIS_HPP

#include <OpenHLX/Model/NetworkModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX network controller, either in a
 *    client or server context.
 *
 *  @ingroup common
 *
 */
class NetworkControllerBasis
{
public:
    virtual ~NetworkControllerBasis(void) = default;

protected:
    NetworkControllerBasis(void) = default;

    // Initializer(s)

    Common::Status Init(void);

    // Observers

    Model::NetworkModel &GetModel(void);
    const Model::NetworkModel &GetModel(void) const;

protected:
    Model::NetworkModel  mNetworkModel;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONNETWORKCONTROLLERBASIS_HPP
