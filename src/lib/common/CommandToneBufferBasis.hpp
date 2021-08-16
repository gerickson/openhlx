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
 *      This file defines a derived object for composing HLX commands
 *      that observe or mutate the sound data model tone properties.
 *
 */

#ifndef OPENHLXCOMMONCOMMANDTONEBUFFERBASIS_HPP
#define OPENHLXCOMMONCOMMANDTONEBUFFERBASIS_HPP

#include <OpenHLX/Common/CommandPropertyBufferBases.hpp>
#include <OpenHLX/Common/Errors.hpp>
#include <OpenHLX/Model/IdentifierModel.hpp>
#include <OpenHLX/Model/ToneModel.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

/**
 *  @brief
 *    A derived object for composing HLX commands that observe or
 *    mutate the sound data model tone properties.
 *
 *  @ingroup common
 *  @ingroup command
 *
 */
class ToneBufferBasis :
    public PropertyBufferBasis
{
protected:
    /**
     *  Convenience type redeclaring @a IdentifierType from the
     *  identifier model.
     *
     */
    typedef Model::IdentifierModel::IdentifierType  IdentifierType;

    /**
     *  Convenience type redeclaring @a LevelType from the tone model.
     *
     */
    typedef Model::ToneModel::LevelType             LevelType;

protected:
    ToneBufferBasis(void) = default;
    virtual ~ToneBufferBasis(void) = default;

    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const LevelType &aBass, const LevelType &aTreble);
    static Common::Status Init(BufferBasis &aBuffer, const char *aObject, const IdentifierType &aIdentifier, const char *aOperation);
};

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCOMMANDTONEBUFFERBASIS_HPP
