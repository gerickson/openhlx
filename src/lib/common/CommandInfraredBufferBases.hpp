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
 *      This file defines base and derived objects for composing HLX
 *      commands that observe or set infrared remote control data
 *      model properties.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDINFRAREDBUFFERBASES_HPP
#define OPENHLXCOMMONCOMMANDINFRAREDBUFFERBASES_HPP

#include <OpenHLX/Common/CommandQueryBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/InfraredModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace Infrared
{

/**
 *  @brief
 *    A derived object for composing a HLX query command for the
 *    infrared remote control data model disabled property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class QueryDisabledBufferBasis :
    public QueryBufferBasis
{
protected:
    QueryDisabledBufferBasis(void) = default;
    virtual ~QueryDisabledBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer);
};

/**
 *  @brief
 *    A base object for composing a HLX command for mutating a
 *    property with an unsigned 8-bit value.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class BufferBasis
{
protected:
    BufferBasis(void) = default;
    virtual ~BufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const char *aOperation, const uint8_t &aValue);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    infrared remote control data model disabled property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class DisabledBufferBasis :
    public BufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a DisabledType from the
     *  infrared model.
     *
     */
    typedef Model::InfraredModel::DisabledType  DisabledType;

protected:
    DisabledBufferBasis(void) = default;
    virtual ~DisabledBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const DisabledType &aDisabled);
};

}; // namespace Infrared

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDINFRAREDBUFFERBASES_HPP
