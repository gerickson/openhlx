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

#ifndef OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP
#define OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP


#include <map>

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/HLXCommonControllerBasis.hpp>
#include <OpenHLX/Server/ControllerBasis.hpp>


namespace HLX
{

namespace Server
{

class Controller;

namespace Application
{

class Controller;

/**
 *  @brief
 *    An object for effecting an HLX server application controller for
 *    any HLX server application.
 *
 *  @ingroup server
 *
 */
class ControllerBasis :
    public Common::Application::Foo<Server::ControllerBasis>
{
public:
    virtual ~ControllerBasis(void);

    Common::Status Init(void);

protected:
    ControllerBasis(Server::Application::Controller &aController);

private:
    Controller &                                      mController;
};

}; // namespace Application

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERAPPLICATIONCONTROLLERBASIS_HPP
