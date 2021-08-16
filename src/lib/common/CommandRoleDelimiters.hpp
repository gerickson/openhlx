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
 *      This file defines interfaces for obtaining HLX control
 *      protocol command buffer and regular expression delimiters
 *      based on the role of the command initiator or recipient.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDROLEDELIMITERS_HPP
#define OPENHLXCOMMONCOMMANDROLEDELIMITERS_HPP

#include <OpenHLX/Common/CommandDelimiters.hpp>
#include <OpenHLX/Common/CommandRole.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

const Delimiters &GetRoleBufferDelimiters(const Role &inRole);
const Delimiters &GetRoleRegularExpressionDelimiters(const Role &inRole);

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDROLEDELIMITERS_HPP
