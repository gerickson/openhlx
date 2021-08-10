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

#ifndef OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP
#define OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP

#include <OpenHLX/Server/FavoritesControllerCommands.hpp>


namespace HLX
{

namespace Server
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup server
 *  @ingroup favorites
 *
 */
class FavoritesControllerBasis
{
public:
    virtual ~FavoritesControllerBasis(void);

protected:
    FavoritesControllerBasis(void);

    Common::Status Init(void);

private:
    Common::Status RequestInit(void);

private:
    static Server::Command::Favorites::QueryRequest    kQueryRequest;
    static Server::Command::Favorites::SetNameRequest  kSetNameRequest;
};

}; // namespace Server

}; // namespace HLX

#endif // OPENHLXSERVERFAVORITESCONTROLLERBASIS_HPP