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
 *      This file defines a derived object for composing delimited
 *      (that is, initiated and terminated by a delimiting pair of
 *      characters) HLX command buffers.
 *
 */

#ifndef HLXCOMMONCOMMANDDELIMITEDBUFFER_HPP
#define HLXCOMMONCOMMANDDELIMITEDBUFFER_HPP

#include <OpenHLX/Common/CommandBuffer.hpp>
#include <CommandDelimiters.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing delimited HLX command buffers.
 *
 *  A derived object for composing delimited (that is, initiated and
 *  terminated by a delimiting pair of characters) HLX command
 *  buffers.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class DelimitedBuffer :
    public Buffer
{
protected:
    DelimitedBuffer(void) = default;
    virtual ~DelimitedBuffer(void) = default;

    Common::Status Init(const Delimiters &inDelimiters, const char *inBuffer);
    Common::Status Init(const Delimiters &inDelimiters, const char *inBuffer, const size_t &inSize);
    Common::Status Init(const Delimiters &inDelimiters, const char *inStart, const char *inEnd);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDDELIMITEDBUFFER_HPP
