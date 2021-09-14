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
 *      This file defines an object for managing an HLX stereophonic
 *      sound mode channel balance data model.
 *
 */

#ifndef OPENHLXMMODELBALANCEMODEL_HPP
#define OPENHLXMMODELBALANCEMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing an HLX stereophonic sound mode channel
 *    balance data model.
 *
 *  @note
 *    This model represents the balance as a non-tagged, continuous
 *    L:{-80, 80}:R model rather than the HLX native tagged,
 *    discontinous model L:{80, 0} to {0, 80}:R as it is an easier
 *    representation for clients to deal with (for example,
 *    representing a balance slider or dial on UI/UX).
 *
 *  @ingroup balance
 *  @ingroup model
 *
 */
class BalanceModel
{
public:
    /**
     *  A type for the stereophonic channel balance bias.
     *
     */
    typedef int8_t BalanceType;

    /**
     *  A type for the stereophonic channel.
     *
     */
    typedef char   ChannelType;

    static const BalanceType    kBalanceMax;
    static const BalanceType    kBalanceMin;
    static const BalanceType    kBalanceCenter;

    static const ChannelType    kChannelLeft;
    static const ChannelType    kChannelRight;

public:
    BalanceModel(void);
    virtual ~BalanceModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const BalanceType &aBalance);
    Common::Status Init(const BalanceModel &aBalanceModel);

    BalanceModel &operator =(const BalanceModel &aBalanceModel);

    Common::Status GetBalance(BalanceType &aBalance) const;

    Common::Status IncreaseBalanceLeft(BalanceType &aOutBalance);
    Common::Status IncreaseBalanceRight(BalanceType &aOutBalance);
    Common::Status SetBalance(const BalanceType &aBalance);

    bool operator ==(const BalanceModel &aBalanceModel) const;

private:
    Common::Status AdjustBalance(const BalanceType &aAdjustment, BalanceType &aOutBalance);

private:
    bool        mBalanceIsNull;
    BalanceType mBalance;
};

}; // namespace Model

}; // namespace HLX

#endif // OPENHLXMMODELBALANCEMODEL_HPP
