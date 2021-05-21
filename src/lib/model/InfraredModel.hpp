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
 *      This file defines an object for managing the HLX infrared
 *      remote control data model.
 *
 */

#ifndef HLXMMODELINFRAREDMODEL_HPP
#define HLXMMODELINFRAREDMODEL_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Model
{

/**
 *  @brief
 *    An object for managing the HLX infrared remote control data
 *    model.

 *  @ingroup model
 *  @ingroup infrared
 *
 */
class InfraredModel
{
public:
    /**
     *  A type for the infrared remote control disabled state.
     *
     */
    typedef bool    DisabledType;

public:
    InfraredModel(void);
    virtual ~InfraredModel(void) = default;

    Common::Status Init(void);
    Common::Status Init(const DisabledType &aDisabled);
    Common::Status Init(const InfraredModel &aInfraredModel);

    InfraredModel &operator =(const InfraredModel &aInfraredModel);

    Common::Status GetDisabled(DisabledType &aDisabled) const;

    Common::Status SetDisabled(const DisabledType &aDisabled);

    bool operator ==(const InfraredModel &aInfraredModel) const;

private:
    bool            mDisabledIsNull;
    DisabledType    mDisabled;
};

}; // namespace Model

}; // namespace HLX

#endif // HLXMMODELINFRAREDMODEL_HPP
