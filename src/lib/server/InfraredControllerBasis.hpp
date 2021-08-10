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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXSERVERINFRAREDCONTROLLERBASIS_HPP
#define OPENHLXSERVERINFRAREDCONTROLLERBASIS_HPP

#include <OpenHLX/Server/InfraredControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup infrared
 *
 */
class InfraredControllerBasis
{
public:
    virtual ~InfraredControllerBasis(void);

protected:
    InfraredControllerBasis(void);

    Common::Status Init(void);

private:
    Common::Status RequestInit(void);

private:
    static Server::Command::Infrared::QueryRequest          kQueryRequest;
    static Server::Command::Infrared::SetDisabledRequest    kSetDisabledRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERINFRAREDCONTROLLERBASIS_HPP