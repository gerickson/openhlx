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
 *      This file defines an object for....
 *
 */

#ifndef OPENHLXCLIENTFRONTPANELCONTROLLERBASIS_HPP
#define OPENHLXCLIENTFRONTPANELCONTROLLERBASIS_HPP

#include <OpenHLX/Client/FrontPanelControllerCommands.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup front-panel
 *
 */
class FrontPanelControllerBasis
{
public:
    virtual ~FrontPanelControllerBasis(void);

protected:
    FrontPanelControllerBasis(void);

    Common::Status Init(void);

private:
    Common::Status ResponseInit(void);

protected:
    static Command::FrontPanel::BrightnessResponse kBrightnessResponse;
    static Command::FrontPanel::LockedResponse     kLockedResponse;
    static Command::FrontPanel::QueryResponse      kQueryResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFRONTPANELCONTROLLERBASIS_HPP
