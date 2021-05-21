/*
 *    Copyright (c) 2020-2021 Grant Erickson
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
 *      This file...
 *
 */

#include "FrontPanelControllerCommands.hpp"

#include <string>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/OutputStringStream.hpp>


using namespace HLX::Common;


namespace HLX
{

namespace Server
{

namespace Command
{

namespace FrontPanel
{

const char * const QueryRequest::kRequestRegexp         = "QFPL";
const size_t       QueryRequest::kExpectedMatches       = 1;

// MARK: Observer Requests, Responses, and Commands

Status QueryRequest :: Init(void)
{
    Status       lRetval = kStatus_Success;

    lRetval = RequestBasis::Init(kRequestRegexp, kExpectedMatches);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Front Panel Mutator Requests, Responses, and Commands

Common::Status SetBrightnessRequest :: Init(void)
{
    return (BrightnessRegularExpressionBasis::Init(*this));
}

Common::Status BrightnessResponse :: Init(const Model::FrontPanelModel::BrightnessType &aBrightness)
{
    return (BrightnessBufferBasis::Init(*this, aBrightness));
}

Common::Status SetLockedRequest :: Init(void)
{
    return (LockedRegularExpressionBasis::Init(*this));
}

Common::Status LockedResponse :: Init(const Model::FrontPanelModel::LockedType &aLocked)
{
    return (LockedBufferBasis::Init(*this, aLocked));
}

}; // namespace FrontPanel

}; // namespace Command

}; // namespace Server

}; // namespace HLX
