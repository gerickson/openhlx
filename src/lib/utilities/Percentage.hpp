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
 *    express or implied.  See the License for the specific language
 *    IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *    governing permissions and limitations under the License.
 *
 */

/**
 *    @file
 *      This file defines types and interfaces for calculating a fixed
 *      pointer percentage from a fixed pointer numerator and
 *      denominator.
 *
 */

#ifndef OPENHLXPERCENTAGE_HPP
#define OPENHLXPERCENTAGE_HPP

#include <stdint.h>


namespace HLX
{

namespace Utilities
{

/**
 *  A type for a fixed point percentage in the range 0 to 100,
 *  inclusive.
 *
 */
typedef uint8_t Percentage;

extern Percentage CalculatePercentage(const uint8_t &aNumerator,
                                      const uint8_t &aDenominator);

}; // namespace Utilities

}; // namespace HLX

#endif // OPENHLXPERCENTAGE_HPP
