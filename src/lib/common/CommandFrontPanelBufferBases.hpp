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
 *      commands that observe or set HLX physical front panel control
 *      data model properties.
 *
 */

#ifndef HLXCOMMONCOMMANDFRONTPANELBUFFERBASES_HPP
#define HLXCOMMONCOMMANDFRONTPANELBUFFERBASES_HPP

#include <OpenHLX/Common/CommandQueryBufferBasis.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/FrontPanelModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

namespace FrontPanel
{

/**
 *  @brief
 *    A derived object for composing a HLX query command for the front
 *    panel data model locked property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class QueryLockedBufferBasis :
    public QueryBufferBasis
{
protected:
    QueryLockedBufferBasis(void) = default;
    virtual ~QueryLockedBufferBasis(void) = default;

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
 *    HLX physical front panel data model display brightness property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class BrightnessBufferBasis :
    public BufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a BrightnessType from the
     *  front panel model.
     *
     */
    typedef Model::FrontPanelModel::BrightnessType  BrightnessType;

protected:
    BrightnessBufferBasis(void) = default;
    virtual ~BrightnessBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const BrightnessType &aBrightness);
};

/**
 *  @brief
 *    A derived object for composing a HLX command for mutating the
 *    HLX physical front panel data model locked property.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class LockedBufferBasis :
    public BufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a LockedType from the
     *  front panel model.
     *
     */
    typedef Model::FrontPanelModel::LockedType  LockedType;

protected:
    LockedBufferBasis(void) = default;
    virtual ~LockedBufferBasis(void) = default;

    static Common::Status Init(Common::Command::BufferBasis &aBuffer, const LockedType &aLocked);
};

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDFRONTPANELBUFFERBASES_HPP
