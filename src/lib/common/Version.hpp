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
 *      This file defines types and interfaces for accessing version
 *      and copyright information for OpenHLX package.
 *
 */

#ifndef OPENHLXCOMMONVERSION_HPP
#define OPENHLXCOMMONVERSION_HPP

 #include <stdint.h>

namespace HLX
{

namespace Common
{

/**
 *  Types for encoding and decoding package version information.
 *
 */
struct Version
{
    typedef uint32_t Code;       //!< Encoded version type, including major,
                                 //!< minor, and patch components.
    typedef uint8_t  Component;  //!< Decoded version component type, for
                                 //!< major, minor, or patch component.
};

extern const char *       GetCopyrightString(void);
extern const char *       GetVersionString(void);
extern Version::Code      GetVersionCode(void);
extern Version::Component GetVersionMajor(const Version::Code &aCode);
extern Version::Component GetVersionMinor(const Version::Code &aCode);
extern Version::Component GetVersionPatch(const Version::Code &aCode);

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONVERSION_HPP
