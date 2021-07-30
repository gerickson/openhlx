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
 *      This file defines a delegate interface for the HLX server
 *      peer-to-peer network connection listener basis object.
 *
 */

#ifndef HLXSERVERLISTENERBASISDELEGATE_HPP
#define HLXSERVERLISTENERBASISDELEGATE_HPP

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/Timeout.hpp>

#include <CoreFoundation/CFURL.h>


namespace HLX
{

namespace Server
{

class ListenerBasis;

/**
 *  @brief
 *    A delegate interface for the HLX server peer-to-peer network
 *    connection listener basis object.
 *
 *  @ingroup server
 *
 */
class ListenerBasisDelegate
{
public:
    ListenerBasisDelegate(void);
    ~ListenerBasisDelegate(void);

    // Listen

    virtual void ListenerWillListen(ListenerBasis &aListener) = 0;
    virtual void ListenerIsListening(ListenerBasis &aListener) = 0;
    virtual void ListenerDidListen(ListenerBasis &aListener) = 0;
    virtual void ListenerDidNotListen(ListenerBasis &aListener, const Common::Error &aError) = 0;
    // Error

    virtual void ListenerError(ListenerBasis &aListener, const Common::Error &aError) = 0;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERLISTENERBASISDELEGATE_HPP
