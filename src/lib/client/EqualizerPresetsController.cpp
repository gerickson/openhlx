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
 *      This file implements an object for managing the client-side
 *      observation and mutation of a collection of HLX equalizer
 *      presets.
 *
 */

#include "EqualizerPresetsController.hpp"

#include <memory>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/CommandManager.hpp>
#include <OpenHLX/Client/EqualizerPresetsControllerCommands.hpp>
#include <OpenHLX/Client/EqualizerPresetsStateChangeNotifications.hpp>
#include <OpenHLX/Model/EqualizerPresetModel.hpp>
#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
EqualizerPresetsController :: EqualizerPresetsController(void) :
    Common::EqualizerPresetsControllerBasis(),
    Client::EqualizerPresetsControllerBasis(Common::EqualizerPresetsControllerBasis::mEqualizerPresets,
                                            Common::EqualizerPresetsControllerBasis::kEqualizerPresetsMax)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
EqualizerPresetsController :: ~EqualizerPresetsController(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is the class initializer.
 *
 *  This initializes the class with the specified command manager and
 *  timeout.
 *
 *  @param[in]  aCommandManager  A reference to the command manager
 *                               instance to initialize the controller
 *                               with.
 *  @param[in]  aTimeout         The timeout to initialize the controller
 *                               with that will serve as the timeout for
 *                               future operations with the peer server.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -EINVAL                      If an internal parameter was
 *                                        invalid.
 *  @retval  -ENOMEM                      If memory could not be allocated.
 *  @retval  kError_NotInitialized        The base class was not properly
 *                                        initialized.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: Init(CommandManager &aCommandManager, const Timeout &aTimeout)
{
    DeclareScopedFunctionTracer(lTracer);
    constexpr bool  kRegister = true;
    Status          lRetval = kStatus_Success;


    lRetval = Common::EqualizerPresetsControllerBasis::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = Client::EqualizerPresetsControllerBasis::Init(aCommandManager, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

    // This MUST come AFTER the base class initialization due to a
    // dependency on the command manager instance.

    lRetval = DoNotificationHandlers(kRegister);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Observer Methods

/**
 *  @brief
 *    Get the equalizer preset model associated with specified
 *    equalizer preset identifier.
 *
 *  @param[in]   aIdentifier  An immutable reference to the equalizer
 *                            preset model to obtain.
 *  @param[out]  aModel       A reference to an immutable pointer
 *                            by which to return the equalizer preset
 *                            model.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than supported.
 *
 */
Status
EqualizerPresetsController :: GetEqualizerPreset(const IdentifierType &aIdentifier, const EqualizerPresetModel *&aModel) const
{
    Status  lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mEqualizerPresets.GetEqualizerPreset(aIdentifier, aModel);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Get the equalizer preset identifier with the specified name.
 *
 *  This attempts to lookup the equalizer preset identifier for the
 *  equalizer preset with the specified name.
 *
 *  @param[in]   aName                       A pointer to a null-
 *                                           terminated C string of
 *                                           the name of the equalizer
 *                                           preset to find an
 *                                           identifier for.
 *  @param[out]  aEqualizerPresetIdentifier  A reference to storage by
 *                                           which to return the
 *                                           identifier if successful.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ENOENT          No equalizer preset could be found with
 *                            the specified name.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetsController :: LookupIdentifier(const char *aName, IdentifierType &aEqualizerPresetIdentifier) const
{
    const EqualizerPresetModel * lEqualizerPresetModel;
    Status                       lRetval;


    lRetval = mEqualizerPresets.GetEqualizerPreset(aName, lEqualizerPresetModel);
    nlEXPECT_SUCCESS(lRetval, done);

    lRetval = lEqualizerPresetModel->GetIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

// MARK: Mutator Methods

// MARK: Equalizer Band Mutator Commands

/**
 *  @brief
 *    Set the level of an equalizer band of an equalizer preset.
 *
 *  This attempts to set the level of the specified equalizer band of
 *  the specified equalizer preset to the provided value on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          set the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to set the band
 *                                          level.
 *  @param[in]  aLevel                      An immutable reference to
 *                                          band level to set.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: SetEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                               const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier,
                                               const EqualizerBandModel::LevelType &aLevel)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::SetEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::SetEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                    aEqualizerBandIdentifier,
                                                                                                    aLevel);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                          static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Increase the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to increase the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          increase the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to increase the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: IncreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::IncreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::IncreaseEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                         aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                          static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Decrease the level of an equalizer band of an equalizer preset
 *    by one (1) unit.
 *
 *  This attempts to decrease the level of the specified equalizer
 *  band of the specified equalizer preset by one (1) unit on the peer
 *  HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          to the equalizer preset
 *                                          identifier for which to
 *                                          decrease the band level.
 *  @param[in]  aEqualizerBandIdentifier    An immutable reference
 *                                          to the equalizer band on
 *                                          the equalizer preset for
 *                                          which to decrease the band
 *                                          level.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ERANGE                      If the equalizer preset or
 *                                        equalizer band identifier is
 *                                        smaller or larger than
 *                                        supported.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        for the command exchange or
 *                                        exchange state.
 *  @retval  kError_InitializationFailed  If initialization otherwise failed.
 *
 */
Status
EqualizerPresetsController :: DecreaseEqualizerBand(const IdentifierType &aEqualizerPresetIdentifier,
                                                    const EqualizerBandModel::IdentifierType &aEqualizerBandIdentifier)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status lRetval = kStatus_Success;


    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = EqualizerBandsModel::ValidateIdentifier(aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::DecreaseEqualizerBand());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::DecreaseEqualizerBand>(lCommand)->Init(aEqualizerPresetIdentifier,
                                                                                                         aEqualizerBandIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::EqualizerPresetsControllerBasis::SetEqualizerBandCompleteHandler,
                          Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                          static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Name Mutator Commands

/**
 *  @brief
 *    Set the equalizer preset to the specified name.
 *
 *  This attempts to set the equalizer preset with the provided identifier
 *  to the specified name on the peer HLX server controller.
 *
 *  @param[in]  aEqualizerPresetIdentifier  An immutable reference
 *                                          for the equalizer preset
 *                                          for which to set the name.
 *  @param[in]  aName                       A pointer to the null-
 *                                          terminated C string to set
 *                                          the equalizer preset name
 *                                          to.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EINVAL          If @a aName was null.
 *  @retval  -ERANGE          If the equalizer preset identifier
 *                            is smaller or larger than
 *                            supported.
 *  @retval  -ENOMEM          If memory could not be allocated
 *                            for the command exchange or
 *                            exchange state.
 *
 *  @ingroup name
 *
 */
Status
EqualizerPresetsController :: SetName(const IdentifierType &aEqualizerPresetIdentifier,
                                      const char *aName)
{
    Command::ExchangeBasis::MutableCountedPointer lCommand;
    Status                                        lRetval = kStatus_Success;


    nlREQUIRE_ACTION(aName != nullptr, done, lRetval = -EINVAL);

    lRetval = ValidateIdentifier(aEqualizerPresetIdentifier);
    nlREQUIRE_SUCCESS(lRetval, done);

    lCommand.reset(new Command::EqualizerPresets::SetName());
    nlREQUIRE_ACTION(lCommand, done, lRetval = -ENOMEM);

    lRetval = std::static_pointer_cast<Command::EqualizerPresets::SetName>(lCommand)->Init(aEqualizerPresetIdentifier, aName);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = SendCommand(lCommand,
                          Client::EqualizerPresetsControllerBasis::SetNameCompleteHandler,
                          Client::EqualizerPresetsControllerBasis::CommandErrorHandler,
                          static_cast<Client::EqualizerPresetsControllerBasis *>(this));
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

}; // namespace Client

}; // namespace HLX
