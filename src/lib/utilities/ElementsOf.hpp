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
 *      This file defines a template for determining the number of
 *      elements in a C-style array.
 *
 */

#ifndef OPENHLXELEMENTSOF_HPP
#define OPENHLXELEMENTSOF_HPP

#include <stddef.h>

namespace HLX
{

namespace Utilities
{

/**
 *  @brief
 *    Return the number of elements in a C-style array.
 *
 *  @tparam  T  The type of the elements in the array.
 *  @tparam  N  The number of elements in the array.
 *
 *  @param[in]  aArray  A reference to the array.
 *
 *  @returns
 *    The number of elements in the C-style array.
 *
 */
template <typename T, size_t N>
inline constexpr size_t
ElementsOf(__attribute__((unused)) const T (&aArray)[N])
{
    return (N);
}

}; // namespace Utilities

}; // namespace HLX

#endif // OPENHLXELEMENTSOF_HPP
