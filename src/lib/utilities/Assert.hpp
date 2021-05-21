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
 *      This file defines an Open HLX project-specific wrapper header
 *      for the Nest Labs Assert package that defines desired default
 *      behavior of that package for this project.
 *
 */

#ifndef HLXASSERT_HPP
#define HLXASSERT_HPP

/**
 *  @def NL_ASSERT_USE_FLAGS_DEFAULT
 *
 *  Indicate that Open HLX desires the default Nest Labs Assert
 *  behavior flags.
 *
 */
#define NL_ASSERT_USE_FLAGS_DEFAULT 1

/**
 *  @def NL_ASSERT_LOG
 *
 *  Indicate that Open HLX will use the default Nest Labs Assert
 *  logging function.
 *
 */
#define NL_ASSERT_LOG               NL_ASSERT_LOG_DEFAULT

#include <nlassert.h>

#endif // HLXASSERT_HPP
