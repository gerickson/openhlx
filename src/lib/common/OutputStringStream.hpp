/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file defines an object for serializing a plain old data
 *      (POD) type as decimal-formatted value via a C++ Standard
 *      (Template) Library output string stream.
 *
 */

#ifndef OPENHLXCOMMONOUTPUTSTRINGSTREAM_HPP
#define OPENHLXCOMMONOUTPUTSTRINGSTREAM_HPP

#include <iomanip>
#include <ios>
#include <sstream>
#include <string>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for serializing a plain old data (POD) type as
 *    decimal-formatted value.
 *
 *  With this object, a plain old data (POD) type can be serialized as
 *  decimal-formatted value via a C++ Standard (Template) Library
 *  output string stream.
 *
 *  @ingroup common
 *
 */
class OutputStringStream
{
public:
    OutputStringStream(void) = default;
    ~OutputStringStream(void) = default;

    /**
     *  @brief
     *    This is a class insertion operator.
     *
     *  This generates a sequence of characters with the
     *  representation of the specified value, properly formatted for
     *  a decimal base and internal padding, and inserts them into the
     *  output stream.
     *
     *  @tparam  T  The type of the value to format and insert.
     *
     *  @param[in]  aValue  An immutable reference to the value to
     *                      format and insert.
     *
     *  @returns
     *    A reference to the output string stream, modified with the
     *    inserted characters.
     *
     */
    template <class T>
    OutputStringStream & operator << (T const &aValue)
    {
        m_oss << std::internal << std::dec << aValue;

        return (*this);
    }

    std::string str(void) const;

private:
    std::ostringstream m_oss;
};

template<>
OutputStringStream & OutputStringStream::operator << (Model::IdentifierModel::IdentifierType const &aIdentifier);

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONOUTPUTSTRINGSTREAM_HPP
