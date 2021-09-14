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
 *      This file implements interfaces for calculating a fixed
 *      pointer percentage from a fixed pointer numerator and
 *      denominator.
 *
 */

#include "Percentage.hpp"

#include <stdint.h>


namespace HLX
{

namespace Utilities
{

/**
 *  @brief
 *    Calculate a fixed point percentage.
 *
 *  This calculates a fixed point percentage, from 0 to 100, from a
 *  fixed point numerator and denominator.
 *
 *  @param[in]  aNumerator    An immutable reference to the numerator.
 *  @param[in]  aDenominator  An immutable reference to the denominator.
 *
 *  @returns
 *     A percentage, in the range 0 to 100.
 *
 */
Percentage
CalculatePercentage(const uint8_t &aNumerator, const uint8_t &aDenominator)
{
    const Percentage lRetval = static_cast<Percentage>(((static_cast<uint16_t>(aNumerator) * 100) / aDenominator));

    return (lRetval);
}

}; // namespace Utilities

}; // namespace HLX
