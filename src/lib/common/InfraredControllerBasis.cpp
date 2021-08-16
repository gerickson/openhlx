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
 *      This file implements...
 *
 */

#include "InfraredControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace Nuovations;


namespace HLX
{

namespace Common
{

// MARK: Initializer(s)

Status
InfraredControllerBasis :: Init(void)
{
    Status lRetval = kStatus_Success;

    lRetval = mInfraredModel.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observers

Model::InfraredModel &
InfraredControllerBasis :: GetModel(void)
{
    return (mInfraredModel);
}

const Model::InfraredModel &
InfraredControllerBasis :: GetModel(void) const
{
    return (mInfraredModel);
}

}; // namespace Common

}; // namespace HLX
