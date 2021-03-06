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
 *      This file defines a delegate interface for the HLX client
 *      command manager object.
 *
 */

#ifndef OPENHLXCLIENTCOMMANDMANAGERDELEGATE_HPP
#define OPENHLXCLIENTCOMMANDMANAGERDELEGATE_HPP

namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    A delegate interface for the HLX client command manager object.
 *
 *  This delegate interface allows interested delegates to receive
 *  notifications regarding the state of server commands as they
 *  move through their lifetime.
 *
 *  @note
 *    At this time, no such delegations or events are defined.
 *
 *  @ingroup client
 *
 */
class CommandManagerDelegate
{
public:
    CommandManagerDelegate(void) = default;
    ~CommandManagerDelegate(void) = default;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCOMMANDMANAGERDELEGATE_HPP
