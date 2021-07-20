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
 *      This file...
 *
 */

#ifndef HLXCOMMONVERSION_HPP
#define HLXCOMMONVERSION_HPP

 #include <stdint.h>

namespace HLX
{

namespace Common
{

struct Version
{
    typedef uint32_t Code;
    typdef  uint8_t  Component;
};

extern const char *       GetCopyrightString(void);
extern const char *       GetVersionString(void);
extern Version::Code      GetVersionCode(void);
extern Version::Component GetVersionMajor(const Version::Code &aCode);
extern Version::Component GetVersionMinor(const Version::Code &aCode);
extern Version::Component GetVersionPatch(const Version::Code &aCode);

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONVERSION_HPP
