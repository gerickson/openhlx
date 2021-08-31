/*
 *    Copyright (c) 2021 Grant Erickson
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
 *      This file defines an object for...
 *
 */

#ifndef OPENHLXSERVERFRONTPANELCONTROLLERBASIS_HPP
#define OPENHLXSERVERFRONTPANELCONTROLLERBASIS_HPP

#include <OpenHLX/Model/FrontPanelModel.hpp>
#include <OpenHLX/Server/FrontPanelControllerCommands.hpp>
#include <OpenHLX/Server/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup frontpanel
 *
 */
class FrontPanelControllerBasis :
    public Server::ObjectControllerBasis
{
public:
    virtual ~FrontPanelControllerBasis(void);

protected:
    FrontPanelControllerBasis(Model::FrontPanelModel &aFrontPanelModel);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager);

private:
    // Implementation

    Common::Status RequestInit(void);

private:
    Model::FrontPanelModel & mFrontPanelModel;

protected:
    // Observation (Query) Command Request Handlers

    // Observation (Query) Command Request Instance Handlers

    Common::Status HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const;

protected:
    // Observation (Query) Command Request Class (Static) Handlers

protected:
    // Command Response Handlers

    // Command Response Class (Static) Handlers

    static Common::Status HandleBrightnessResponse(const Model::FrontPanelModel::BrightnessType &aBrightness, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);
    static Common::Status HandleLockedResponse(const Model::FrontPanelModel::LockedType &aLocked, Common::ConnectionBuffer::MutableCountedPointer &aBuffer);

protected:
    static Server::Command::FrontPanel::QueryRequest          kQueryRequest;
    static Server::Command::FrontPanel::SetBrightnessRequest  kSetBrightnessRequest;
    static Server::Command::FrontPanel::SetLockedRequest      kSetLockedRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERFRONTPANELCONTROLLERBASIS_HPP
