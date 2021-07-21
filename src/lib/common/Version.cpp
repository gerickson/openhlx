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
 *      This file implements interfaces for accessing version and
 *      copyright information for OpenHLX package.
 *
 */

#include <OpenHLX/Version.h>
#include <OpenHLX/Common/Version.hpp>

namespace HLX
{

namespace Common
{

/**
 *  Get the package copyright string as a null-terminated C string.
 *
 *  @returns
 *    A pointer to the package copyright string.
 *
 */
const char *
GetCopyrightString(void)
{
    static const char * const retval = OPENHLX_COPYRIGHT_STRING;

    return (retval);
}

/**
 *  Get the package version string as a null-terminated C string.
 *
 *  @returns
 *    A pointer to the package version string.
 *
 */
const char *
GetVersionString(void)
{
    static const char * const retval = OPENHLX_VERSION_STRING;

    return (retval);
}

/**
 *  @brief
 *    Get the package version code.
 *
 *  This returns an encoded version for the package, including major,
 *  minor, and patch components.
 *
 *  These encoded version can be decomposed into its constituent components
 *  using the functions #GetVersionMajor, #GetVersionMinor, and
 *  #GetVersionPatch or using the preprocessor macros
 *  #OPENHLX_VERSION_CODE_DECODE_MAJOR, #OPENHLX_VERSION_CODE_DECODE_MINOR,
 *  and #OPENHLX_VERSION_CODE_DECODE_PATCH.
 *
 *  @returns
 *    The package version code.
 *
 *  @sa GetVersionMajor
 *  @sa GetVersionMinor
 *  @sa GetVersionPatch
 *  @sa OPENHLX_VERSION_CODE_DECODE_MAJOR
 *  @sa OPENHLX_VERSION_CODE_DECODE_MINOR
 *  @sa OPENHLX_VERSION_CODE_DECODE_PATCH
 *
 */
Version::Code
GetVersionCode(void)
{
    static constexpr Version::Code retval = OPENHLX_VERSION_CODE;

    return (retval);
}

/**
 *  Get the package major subversion component from the specified
 *  package version code.
 *
 *  @param[in]  aCode  An immutable reference to the encoded package
 *                     version code.
 *
 *  @returns
 *    The package version code major subversion component.
 */
Version::Component
GetVersionMajor(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_MAJOR(aCode);

    return (retval);
}

/**
 *  Get the package minor subversion component from the specified
 *  package version code.
 *
 *  @param[in]  aCode  An immutable reference to the encoded package
 *                     version code.
 *
 *  @returns
 *    The package version code minor subversion component.
 */
Version::Component
GetVersionMinor(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_MINOR(aCode);

    return (retval);
}

/**
 *  Get the package patch subversion component from the specified
 *  package version code.
 *
 *  @param[in]  aCode  An immutable reference to the encoded package
 *                     version code.
 *
 *  @returns
 *    The package version code patch subversion component.
 */
Version::Component
GetVersionPatch(const Version::Code &aCode)
{
    const Version::Component retval = OPENHLX_VERSION_CODE_DECODE_PATCH(aCode);

    return (retval);
}

}; // namespace Common

}; // namespace HLX
