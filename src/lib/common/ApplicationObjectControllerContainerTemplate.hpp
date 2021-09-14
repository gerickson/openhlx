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
 *      This file defines a templated object for managing a collection
 *      of HLX object controllers from an application controller.
 *
 */

#ifndef OPENHLXCOMMONAPPLICATIONOBJECTCONTROLLERCONTAINERTEMPLATE_HPP
#define OPENHLXCOMMONAPPLICATIONOBJECTCONTROLLERCONTAINERTEMPLATE_HPP


#include <map>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Application
{

/**
 *  @brief
 *    A template object for managing a collection of HLX object
 *    controllers from an application controller.
 *
 *  @tparam  T  The basis type of the object controllers to manage.
 *
 */
template <typename T>
class ObjectControllerContainerTemplate
{
public:
    /**
     *  A local convenience type for the template parameter, @a T.
     *
     */
    typedef T ObjectControllerBasisType;

public:
    virtual ~ObjectControllerContainerTemplate(void) = default;

protected:
    ObjectControllerContainerTemplate(void) = default;

    /**
     *  @brief
     *    This is a class initializer.
     *
     *  @retval  kStatus_Success  If successful.
     *
     */
    Common::Status Init(void)
    {
        return (kStatus_Success);
    }

protected:
    /**
     *  @brief
     *    A wrapper type for the object controller basis type
     *    maintained by this object.
     *
     */
    struct ObjectControllerState
    {
        /**
         *  A pointer to the object controller basis type.
         *
         */
        ObjectControllerBasisType * mController;
    };

    /**
     *  A local convenience type for the container of controllers
     *  managed by this object.
     *
     */
    typedef std::map<ObjectControllerBasisType *, ObjectControllerState> Controllers;

protected:
    // Accessors

    /**
     *  @brief
     *    Return the container of object controllers.
     *
     *  @returns
     *    An immutable reference to the container of object controllers.
     *
     */
    const Controllers &  GetControllers(void) const
    {
        return (mObjectControllers);
    }

    /**
     *  @brief
     *    Return the container of object controllers.
     *
     *  @returns
     *    A mutable reference to the container of object controllers.
     *
     */
    Controllers &        GetControllers(void)
    {
        return (mObjectControllers);
    }

    // Mutators

    /**
     *  @brief
     *    Add a controller to the container.
     *
     *  This adds the specified controller to the container of object
     *  controllers under management.
     *
     *  @param[in]  aController  A reference to the controller to add.
     *
     */
    void AddController(ObjectControllerBasisType &aController)
    {
        const ObjectControllerState lObjectControllerState = { &aController };

        mObjectControllers[&aController] = lObjectControllerState;
    }

private:
    Controllers  mObjectControllers;
};

}; // namespace Application

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONAPPLICATIONOBJECTCONTROLLERCONTAINERTEMPLATE_HPP
