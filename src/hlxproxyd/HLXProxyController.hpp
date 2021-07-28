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
 *      This file defines an object for effecting a HLX proxy
 *      controller.
 *
 */

#ifndef OPENHLXPROXYCONTROLLER_HPP
#define OPENHLXPROXYCONTROLLER_HPP

#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Common/RunLoopParameters.hpp>

#include "HLXProxyControllerDelegate.hpp"


namespace HLX
{

namespace Proxy
{

/**
 *  @brief
 *    An object for effecting a HLX proxy controller.
 *
 *  @ingroup proxy
 *
 */
class Controller
{
public:
    Controller(void);
    ~Controller(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters);

    ControllerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(ControllerDelegate *aDelegate, void *aContext);

    Common::Status Connect(void);
    Common::Status Listen(void);

private:
    Common::RunLoopParameters       mRunLoopParameters;
    ControllerDelegate *            mDelegate;
    void *                          mDelegateContext;
};
}; // namespace Proxy

}; // namespace HLX

#endif // OPENHLXPROXYCONTROLLER_HPP
