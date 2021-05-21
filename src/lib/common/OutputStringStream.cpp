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
 *      This file implements an object for serializing a plain old data
 *      (POD) type as decimal-formatted value via a C++ Standard
 *      (Template) Library output string stream.
 *
 */

#include "OutputStringStream.hpp"


using namespace HLX::Model;


namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    Return a string object with a copy of the current contents of
 *    the stream.
 *
 *  @returns
 *    A string object with a copy of the current contents of the
 *    output string stream.
 *
 */
std::string
OutputStringStream :: str(void) const
{
    return (m_oss.str());
}

/**
 *  @brief
 *    This is a class insertion operator template specialization.
 *
 *  This is a template specialization for the HLX identifier model
 *  #HLX::Model::IdentifierModel::IdentifierType, which generates a
 *  sequence of characters with the representation of the identifier,
 *  properly formatted for a decimal base and internal padding, and
 *  inserts them into the output stream.
 *
 *  @param[in]  aIdentifier  An immutable reference to the object
 *                           identifier to format and insert.
 *
 *  @returns
 *    A reference to the output string stream, modified with the
 *    inserted characters for the provided identifier.
 *
 */
template<>
OutputStringStream &
OutputStringStream :: operator << (IdentifierModel::IdentifierType const &aIdentifier)
{
    return (operator <<(static_cast<unsigned int>(aIdentifier)));
}

}; // namespace Common

}; // namespace HLX
