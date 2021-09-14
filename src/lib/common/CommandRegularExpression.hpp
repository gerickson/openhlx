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
 *      This file defines an object for managing regular expression
 *      text pattern search and matching used in the context of an HLX
 *      command.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDREGULAREXPRESSION_HPP
#define OPENHLXCOMMONCOMMANDREGULAREXPRESSION_HPP

#include <vector>

#include <stddef.h>

#include <OpenHLX/Common/ConnectionBuffer.hpp>
#include <OpenHLX/Common/RegularExpression.hpp>
#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *     An object for managing regular expression text pattern search
 *     and matching used in the context of an HLX command.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class RegularExpression
{
public:
    virtual ~RegularExpression(void) = default;

    Status Init(const char *aRegexp, const size_t &aExpectedMatchCount);

protected:
    RegularExpression(void) = default;

public:
    const Common::RegularExpression &          GetRegularExpression(void) const;
    Common::RegularExpression::Matches &       GetMatches(void);
    const ConnectionBuffer *                   GetBuffer(void) const;

    Status SetBuffer(ConnectionBuffer::ImmutableCountedPointer &aBuffer);

private:
    Common::RegularExpression                          mRegexp;
    Common::RegularExpression::Matches                 mMatches;
    ConnectionBuffer::ImmutableCountedPointer          mBuffer;
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDREGULAREXPRESSION_HPP
