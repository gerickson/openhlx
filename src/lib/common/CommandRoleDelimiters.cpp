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
 *      This file implements interfaces for obtaining HLX control
 *      protocol command buffer and regular expression delimiters
 *      based on the role of the command initiator or recipient.
 *
 */

#include "CommandRoleDelimiters.hpp"

namespace HLX
{

namespace Common
{

namespace Command
{

static Delimiters::DelimiterType kRequestBufferStart = "[";
static Delimiters::DelimiterType kRequestBufferEnd   = "]\r\n";

static const Delimiters sRequestBufferDelimiters =
{
    kRequestBufferStart,
    kRequestBufferEnd
};

static Delimiters::DelimiterType kResponseBufferStart = "(";
static Delimiters::DelimiterType kResponseBufferEnd   = ")\r\n";

static const Delimiters sResponseBufferDelimiters =
{
    kResponseBufferStart,
    kResponseBufferEnd
};

static Delimiters::DelimiterType kRequestRegularExpressionStart = "\\[";
static Delimiters::DelimiterType kRequestRegularExpressionEnd   = "\\](\r\n)*$";

static const Delimiters sRequestRegularExpressionDelimiters =
{
    kRequestRegularExpressionStart,
    kRequestRegularExpressionEnd
};

static Delimiters::DelimiterType kResponseRegularExpressionStart = "\\(";
static Delimiters::DelimiterType kResponseRegularExpressionEnd   = "\\)(\r\n)*$";

static const Delimiters sResponseRegularExpressionDelimiters =
{
    kResponseRegularExpressionStart,
    kResponseRegularExpressionEnd
};

/**
 *  @brief
 *    Return the command buffer delimiters for the specified role.
 *
 *  @param[in]  inRole  The command role for which to return the
 *                      command buffer delimiters.
 *
 *  @returns An immutable reference to the command buffer delimiters
 *           for the specified role.
 *
 */
const Delimiters &GetRoleBufferDelimiters(const Role &inRole)
{
    switch (inRole)
    {

    case Role::kRequestor:
        return (sRequestBufferDelimiters);

    case Role::kResponder:
        return (sResponseBufferDelimiters);

    }
}

/**
 *  @brief
 *    Return the command regular expression delimiters for the
 *    specified role.
 *
 *  @param[in]  inRole  The command role for which to return the
 *                      command regular expression delimiters.
 *
 *  @returns An immutable reference to the command regular expression
 *           delimiters for the specified role.
 *
 */
const Delimiters &GetRoleRegularExpressionDelimiters(const Role &inRole)
{
    switch (inRole)
    {

    case Role::kRequestor:
        return (sRequestRegularExpressionDelimiters);

    case Role::kResponder:
        return (sResponseRegularExpressionDelimiters);

    }
}

}; // namespace Command

}; // namespace Common

}; // namespace HLX
