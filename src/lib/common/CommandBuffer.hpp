/*
 *    Copyright (c) 2018-2021 Grant Erickson
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
 *      This file defines a buffer object for composing HLX commands.
 *
 */

#ifndef HLXCOMMONCOMMANDBUFFER_HPP
#define HLXCOMMONCOMMANDBUFFER_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A buffer object for composing HLX commands.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class Buffer
{
protected:
    Buffer(void) = default;
    virtual ~Buffer(void) = default;

    Common::Status Init(const char *inBuffer);
    Common::Status Init(const char *inBuffer, const size_t &inSize);
    Common::Status Init(const char *inStart, const char *inEnd);

public:
    const uint8_t *GetBuffer(void) const;
    size_t GetSize(void) const;

private:
    void AppendBuffer(const char *inStart, const char *inEnd);

private:
    std::vector<char>  mBuffer;
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDBUFFER_HPP
