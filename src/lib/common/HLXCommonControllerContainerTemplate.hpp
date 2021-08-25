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

#ifndef OPENHLXCOMMONAPPLICATIONCONTROLLERCONTAINERTEMPLATE_HPP
#define OPENHLXCOMMONAPPLICATIONCONTROLLERCONTAINERTEMPLATE_HPP


#include <map>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Application
{

template <typename T>
class ControllerContainerTemplate
{
public:
    typedef T ControllerBasisType;

public:
    virtual ~ControllerContainerTemplate(void) = default;

protected:
    ControllerContainerTemplate(void) = default;

    Common::Status Init(void)
    {
        return (kStatus_Success);
    }

protected:
    struct ControllerState
    {
        ControllerBasisType * mController;
    };

    typedef std::map<ControllerBasisType *, ControllerState> Controllers;

protected:
    // Accessors

    const Controllers &  GetControllers(void) const
    {
        return (mControllers);
    }

    Controllers &        GetControllers(void)
    {
        return (mControllers);
    }

    // Mutators

    void AddController(ControllerBasisType &aController)
    {
        const ControllerState lControllerState = { &aController };

        mControllers[&aController] = lControllerState;
    }

private:
    Controllers  mControllers;
};

}; // namespace Application

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONAPPLICATIONCONTROLLERCONTAINERTEMPLATE_HPP
