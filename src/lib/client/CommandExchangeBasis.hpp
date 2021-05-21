/*
 *    Copyright (c) 2019-2021 Grant Erickson
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
 *      This file defines a base, derivable object for an HLX client
 *      command request / response pair.
 *
 */

#ifndef HLXCLIENTCOMMANDEXCHANGE_HPP
#define HLXCLIENTCOMMANDEXCHANGE_HPP

#include <memory>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Client
{

namespace Command
{

class RequestBasis;
class ResponseBasis;

/**
 *  @brief
 *    A base, derivable object for an HLX client command request /
 *    response pair.
 *
 *  All HLX client commands are expressed as an exchange, consisting
 *  of a request buffer / response regular expression pair.
 *
 *  Since the HLX control protocol is a single-command-at-a-time
 *  protocol, the exchange is an implicit, logical rather than
 *  explicit, over-the-wire concept.
 *
 *  @ingroup client
 *  @ingroup command
 *
 */
class ExchangeBasis
{
public:
    /**
     *  Smart or shared pointer to a mutable @a ExchangeBasis in which
     *  the associated memory is released when there are no further
     *  owners of the underlying pointer.
     *
     */
    typedef std::shared_ptr<ExchangeBasis> MutableCountedPointer;

public:
    ExchangeBasis(void);
    virtual ~ExchangeBasis(void) = default;

public:
    Common::Status Init(RequestBasis &aRequest, ResponseBasis &aResponse);

    RequestBasis *   GetRequest(void) const;
    ResponseBasis *  GetResponse(void) const;

private:
    RequestBasis *   mRequest;
    ResponseBasis *  mResponse;
};

}; // namespace Command

}; // namespace Client

}; // namespace HLX

#endif // HLXCLIENTCOMMANDEXCHANGE_HPP
