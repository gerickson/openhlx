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
 *      This file defines a derivable object for realizing a HLX
 *      zones controller, in a client.
 *
 */

#ifndef OPENHLXCLIENTZONESCONTROLLERBASIS_HPP
#define OPENHLXCLIENTZONESCONTROLLERBASIS_HPP

#include <OpenHLX/Client/ObjectControllerBasis.hpp>
#include <OpenHLX/Client/ZonesControllerCommands.hpp>
#include <OpenHLX/Model/ZoneModel.hpp>
#include <OpenHLX/Model/ZonesModel.hpp>


namespace HLX
{

namespace Client
{

namespace Application
{

    class Controller;

}; // namespace Application

/**
 *  @brief
 *    An object for...
 *
 *  @ingroup client
 *  @ingroup zone
 *
 */
class ZonesControllerBasis :
    public Client::ObjectControllerBasis
{
public:
    virtual ~ZonesControllerBasis(void);

protected:
    /**
     *  @brief
     *    This is a class constructor.
     *
     *  This constructs the zones controller with the specified zones
     *  collection model and the maximum number of allowed zones.
     *
     *  @param[in]  aZonesModel  A mutable reference to the zones
     *                           collection model to construct the
     *                           controller with. This is retained by a
     *                           weak pointer reference and, consequently,
     *                           must remain in scope for the lifetime of
     *                           the controller.
     *  @param[in]  aZonesMax    An immutable reference to the maximum
     *                           number of allowed zones managed by the
     *                           controller.
     *
     */
    ZonesControllerBasis(Model::ZonesModel &aZonesModel,
                         const Model::ZoneModel::IdentifierType &aZonesMax);

    // Initializer(s)

    virtual Common::Status Init(CommandManager &aCommandManager, const Common::Timeout &aTimeout);

    Common::Status Refresh(const Common::Timeout &aTimeout) final;

    // Observer Methods

    Common::Status Query(void);
    Common::Status Query(const Model::ZoneModel::IdentifierType &aZoneIdentifier);

    // Command Completion Handler Trampolines

    static void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetSourceAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches, void *aContext);

    static void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError, void *aContext);

    // Notification Handler Trampolines

    static void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);
    static void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches, void *aContext);

protected:
    // Implementation

    Common::Status DoNotificationHandlers(const bool &aRegister);

private:
    // Command Completion Handlers

    void QueryCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetBalanceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerBandCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetEqualizerPresetCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetHighpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetLowpassCrossoverCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetMuteCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetNameCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSoundModeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetSourceAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetToneCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeAllCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);
    void SetVolumeFixedCompleteHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::RegularExpression::Matches &aMatches);

    void CommandErrorHandler(Command::ExchangeBasis::MutableCountedPointer &aExchange, const Common::Error &aError);

    // Notification Handlers

    void BalanceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerBandNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void EqualizerPresetNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void HighpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void LowpassCrossoverNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void MuteNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void NameNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SoundModeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void SourceAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void ToneNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeAllNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);
    void VolumeFixedNotificationReceivedHandler(const uint8_t *aBuffer, const size_t &aSize, const Common::RegularExpression::Matches &aMatches);

private:
    // Implementation

    Common::Status ResponseInit(void);

private:
    //!< Ensure the application controller can access the Handle*Change
    //!< methods for cross zone-to-group and group-to-zone state
    //!< synthesis.

    friend class Application::Controller;

    void HandleMuteChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::MuteType &aMute);
    void HandleSourceChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::SourceModel::IdentifierType &aSourceIdentifier);
    void HandleVolumeChange(const Model::ZoneModel::IdentifierType &aZoneIdentifier, const Model::VolumeModel::LevelType &aVolume);

private:
    Model::ZonesModel &                               mZonesModel;
    const Model::ZoneModel::IdentifierType &          mZonesMax;
    size_t                                            mZonesDidRefreshCount;

protected:
    static Command::Zones::BalanceResponse            kBalanceResponse;
    static Command::Zones::EqualizerBandResponse      kEqualizerBandResponse;
    static Command::Zones::EqualizerPresetResponse    kEqualizerPresetResponse;
    static Command::Zones::HighpassCrossoverResponse  kHighpassCrossoverResponse;
    static Command::Zones::LowpassCrossoverResponse   kLowpassCrossoverResponse;
    static Command::Zones::MuteResponse               kMuteResponse;
    static Command::Zones::NameResponse               kNameResponse;
    static Command::Zones::QueryResponse              kQueryResponse;
    static Command::Zones::SoundModeResponse          kSoundModeResponse;
    static Command::Zones::SourceResponse             kSourceResponse;
    static Command::Zones::SourceAllResponse          kSourceAllResponse;
    static Command::Zones::ToneResponse               kToneResponse;
    static Command::Zones::VolumeResponse             kVolumeResponse;
    static Command::Zones::VolumeAllResponse          kVolumeAllResponse;
    static Command::Zones::VolumeFixedResponse        kVolumeFixedResponse;
};

}; // namespace Client

}; // namespace HLX

#endif // OPENHLXCLIENTZONESCONTROLLERBASIS_HPP
