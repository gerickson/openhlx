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

#ifndef OPENHLXCLIENTCONFIGURATIONCONTROLLERBASIS_HPP
#define OPENHLXCLIENTCONFIGURATIONCONTROLLERBASIS_HPP

#include <OpenHLX/Client/ConfigurationControllerCommands.hpp>
#include <OpenHLX/Client/ObjectControllerBasis.hpp>


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup configuration
 *
 */
class ConfigurationControllerBasis :
    public Client::ObjectControllerBasis
{
public:
    virtual ~ConfigurationControllerBasis(void);

protected:
    ConfigurationControllerBasis(void);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status QueryCurrent(void);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void LoadFromBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SaveToBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ResetToDefaultsCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

protected:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void LoadFromBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SaveToBackupCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void ResetToDefaultsCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void SaveToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SavingToBackupNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status ResponseInit(void);

protected:
    static Command::Configuration::SaveToBackupResponse   kSaveToBackupResponse;
    static Command::Configuration::SavingToBackupResponse kSavingToBackupResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTCONFIGURATIONCONTROLLERBASIS_HPP
