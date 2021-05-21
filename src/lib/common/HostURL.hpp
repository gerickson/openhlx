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
 *      This file defines an object for working with and managing an
 *      IETF RFC 1738-style Internet host URL.
 *
 */

#ifndef HLXCOMMONHOSTURL_HPP
#define HLXCOMMONHOSTURL_HPP

#include <CoreFoundation/CFURL.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for working with and managing an IETF RFC 1738-style
 *    Internet host URL.
 *
 *  @ingroup common
 *
 */
class HostURL
{
public:
    HostURL(void);
    ~HostURL(void);

    Status Init(const char *aURL);
    Status Init(CFStringRef aURLStringRef);
    Status Init(CFURLRef aURLRef);

    bool operator ==(const HostURL &aHostURL) const;

    HostURL & operator =(const HostURL &aHostURL);
    HostURL & operator =(HostURL &&aHostURL);

    CFURLRef GetURL(void) const;

private:
    HostURL(const HostURL &aHostURL) = delete;
    HostURL(HostURL &&aHostURL) = delete;

private:
    CFURLRef  mURLRef;
};

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONHOSTURL_HPP
