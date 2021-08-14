/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines...
 *
 */

#ifndef HLXPROXYCOMMAND_HPP
#define HLXPROXYCOMMAND_HPP

#include <OpenHLX/Client/CommandExchangeBasis.hpp>
#include <OpenHLX/Client/CommandRequestBasis.hpp>
#include <OpenHLX/Client/CommandResponseBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Proxy
{

namespace Command
{

class Proxy :
    public Client::Command::ExchangeBasis
{
public:
    Proxy(void) = default;
    virtual ~Proxy(void) = default;

    Common::Status Init(const uint8_t *aBuffer,
                        const size_t &aSize,
                        const Client::Command::ResponseBasis &aResponse);

private:
    Client::Command::RequestBasis   mRequest;
    Client::Command::ResponseBasis  mResponse;
};

}; // namespace Command

}; // namespace Server

}; // namespace HLX

#endif // HLXPROXYCOMMAND_HPP
