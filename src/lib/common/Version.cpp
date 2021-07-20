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

#include <OpenHLX/Version.h>
#include <OpenHLX/Common/Version.hpp>

namespace HLX
{

namespace Common
{

const char *
GetCopyrightString(void)
{
    static const char * const retval = OPENHLX_COPYRIGHT_STRING;

    return (retval);
}

const char *
GetVersionString(void)
{
    static const char * const retval = OPENHLX_VERSION_STRING;

    return (retval);
}

Version::Code
GetVersionCode(void)
{
    static constexpr Version::Code retval = OPENHLX_VERSION_CODE;

    return (retval);
}

Version::Component
GetVersionMajor(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_MAJOR(aCode);

    return (retval);
}

Version::Component
GetVersionMinor(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_MINOR(aCode);

    return (retval);
}

Version::Component
GetVersionPatch(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_PATCH(aCode);

    return (retval);
}

}; // namespace Common

}; // namespace HLX
