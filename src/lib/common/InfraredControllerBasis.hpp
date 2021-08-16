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
 *      This file defines a base object for realizing a HLX infrared
 *      remote control interface controller, either in a client or
 *      server context.
 *
 */

#ifndef OPENHLXCOMMONINFRAREDCONTROLLERBASIS_HPP
#define OPENHLXCOMMONINFRAREDCONTROLLERBASIS_HPP

#include <OpenHLX/Model/InfraredModel.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    A base object for realizing a HLX infrared remote control
 *    interface controller, either in a client or server context.
 *
 *  @ingroup common
 *
 */
class InfraredControllerBasis
{
public:
    virtual ~InfraredControllerBasis(void) = default;

protected:
    InfraredControllerBasis(void) = default;

    // Initializer(s)

    Common::Status Init(void);

    // Observers

    Model::InfraredModel &GetModel(void);
    const Model::InfraredModel &GetModel(void) const;

protected:
    Model::InfraredModel  mInfraredModel;
};

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONINFRAREDCONTROLLERBASIS_HPP
