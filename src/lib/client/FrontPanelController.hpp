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
 *      This file defines an object for managing the client-side
 *      observation and mutation of a HLX physical front panel.
 *
 */

#ifndef OPENHLXCLIENTFRONTPANELCONTROLLER_HPP
#define OPENHLXCLIENTFRONTPANELCONTROLLER_HPP

#include <OpenHLX/Client/FrontPanelControllerBasis.hpp>
#include <OpenHLX/Client/FrontPanelControllerCommands.hpp>
#include <OpenHLX/Common/FrontPanelControllerBasis.hpp>
#include <OpenHLX/Model/FrontPanelModel.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for managing the client-side observation and mutation
 *    of a HLX physical front panel.
 *
 *  @ingroup client
 *  @ingroup front-panel
 *
 */
class FrontPanelController :
    public Common::FrontPanelControllerBasis,
    public Client::FrontPanelControllerBasis
{
public:
    FrontPanelController(void);
    virtual ~FrontPanelController(void);

    // Initializer(s)

    Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout) final;

    // Mutator Methods

    Common::Status SetBrightness(const Model::FrontPanelModel::BrightnessType &aBrightness);
    Common::Status SetLocked(const Model::FrontPanelModel::LockedType &aLocked);

};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTFRONTPANELCONTROLLER_HPP
