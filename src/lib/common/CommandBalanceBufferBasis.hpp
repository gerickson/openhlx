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
 *      This file defines a derived object for composing HLX command
 *      buffers that observe or set the stereophonic channel balance
 *      data model property.
 *
 */

#ifndef HLXCOMMONCOMMANDBALANCEBUFFERBASIS_HPP
#define HLXCOMMONCOMMANDBALANCEBUFFERBASIS_HPP

#include <stddef.h>

#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/BalanceModel.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX command buffers that observe
 *    or set the stereophonic channel balance data model property.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup balance
 *
 */
class BalanceBufferBasis :
    public PropertyBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a BalanceType from the balance model.
     *
     */
    typedef Model::BalanceModel::BalanceType BalanceType;

    /**
     *  Convenience type redeclaring @a ChannelType from the balance model.
     *
     */
    typedef Model::BalanceModel::ChannelType ChannelType;

protected:
    BalanceBufferBasis(void) = default;
    ~BalanceBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const ChannelType &aChannel, const BalanceType &aBalance);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const BalanceType &aBalance);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDBALANCEBUFFERBASIS_HPP
