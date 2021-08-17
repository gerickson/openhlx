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
 *      This file implements an object for...
 *
 */

#include "SourcesControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Server
{

// Request data

Server::Command::Sources::SetNameRequest      SourcesControllerBasis::kSetNameRequest;

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
SourcesControllerBasis :: SourcesControllerBasis(Model::SourcesModel &aSourcesModel,
                                                 const Model::SourceModel::IdentifierType &aSourcesMax) :
    mSourcesModel(aSourcesModel),
    mSourcesMax(aSourcesMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
SourcesControllerBasis :: ~SourcesControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

Status
SourcesControllerBasis :: Init(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = RequestInit();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
SourcesControllerBasis :: RequestInit(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    lRetval = kSetNameRequest.Init();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Handlers

// MARK: Observation (Query) Command Request Instance Handlers

Status
SourcesControllerBasis :: HandleQueryReceived(Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    Status lRetval = kStatus_Success;

    for (auto lSourceIdentifier = IdentifierModel::kIdentifierMin; lSourceIdentifier <= mSourcesMax; lSourceIdentifier++)
    {
        lRetval = HandleQueryReceived(lSourceIdentifier, aBuffer);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

 done:
    return (lRetval);
}

Status
SourcesControllerBasis :: HandleQueryReceived(const Model::SourceModel::IdentifierType &aSourceIdentifier, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) const
{
    const SourceModel *                       lSourceModel;
    const char *                              lName;
    Server::Command::Sources::NameResponse    lResponse;
    const uint8_t *                           lBuffer;
    size_t                                    lSize;
    Status                                    lRetval;


    lRetval = mSourcesModel.GetSource(aSourceIdentifier, lSourceModel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lSourceModel->GetName(lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = lResponse.Init(aSourceIdentifier, lName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lBuffer = lResponse.GetBuffer();
    lSize = lResponse.GetSize();

    lRetval = Common::Utilities::Put(*aBuffer.get(), lBuffer, lSize);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Observation (Query) Command Request Class (Static) Handlers

// MARK: Command Response Handlers

// MARK: Command Response Class (Static) Handlers

}; // namespace Server

}; // namespace HLX
