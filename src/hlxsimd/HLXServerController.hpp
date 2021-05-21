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
 *      This file defines an object for effecting a HLX server
 *      controller.
 *
 */

#ifndef HLXSERVERCONTROLLER_HPP
#define HLXSERVERCONTROLLER_HPP

#include <map>

#include <boost/filesystem.hpp>

#include <CoreFoundation/CFRunLoop.h>

#include <CommandManager.hpp>
#include <CommandManagerDelegate.hpp>
#include <ConnectionManagerDelegate.hpp>
#include <ConnectionManager.hpp>
#include <HLXServerControllerDelegate.hpp>
#include <OpenHLX/Common/Errors.hpp>

#include <ConfigurationController.hpp>
#include <ConfigurationControllerDelegate.hpp>
#include <EqualizerPresetsController.hpp>
#include <FavoritesController.hpp>
#include <FrontPanelController.hpp>
#include <GroupsController.hpp>
#include <GroupsControllerDelegate.hpp>
#include <InfraredController.hpp>
#include <NetworkController.hpp>
#include <SourcesController.hpp>
#include <ZonesController.hpp>

namespace HLX
{

namespace Common
{

    class RunLoopParameters;

}; // namespace Common

namespace Server
{

/**
 *  @brief
 *    An object for effecting a HLX server controller.
 *
 *  @ingroup server
 *
 */
class Controller :
    public CommandManagerDelegate,
    public ConfigurationControllerDelegate,
    public ControllerBasisDelegate,
    public ConnectionManagerDelegate,
    public GroupsControllerDelegate
{
public:
    Controller(void);
    ~Controller(void);

    Common::Status Init(const Common::RunLoopParameters &aRunLoopParameters, const boost::filesystem::path &aConfigurationPath);

    ControllerDelegate *GetDelegate(void) const;

    Common::Status SetDelegate(ControllerDelegate *aDelegate, void *aContext);

    Common::Status Listen(void);
    Common::Status Listen(const ConnectionManager::Versions &aVersions);
    Common::Status Listen(const char *aMaybeURL);
    Common::Status Listen(const char *aMaybeURL, const ConnectionManager::Versions &aVersions);

    Common::Status RegisterRequestHandler(Server::Command::RequestBasis &aRequest, void *aContext, CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler);

    // Configuration Controller Delegate Methods

    Common::Status LoadFromBackupConfiguration(ConfigurationController &aController, CFDictionaryRef aBackupDictionary) final;
    Common::Status LoadFromBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef &aBackupDictionary) final;
    void QueryCurrentConfiguration(ConfigurationController &aController, ConnectionBasis &aConnection, Common::ConnectionBuffer::MutableCountedPointer &aBuffer) final;
    void ResetToDefaultConfiguration(ConfigurationController &aController) final;
    void SaveToBackupConfiguration(ConfigurationController &aController, CFMutableDictionaryRef aBackupDictionary) final;
    Common::Status SaveToBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef aBackupDictionary) final;

    // Controller Delegate Methods

    void ControllerConfigurationIsDirty(ControllerBasis &aController) final;

    // Connection Manager Delegate Methods

    // Resolve

    void ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost) final;
    void ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::IPAddress &aIPAddress) final;
    void ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const char *aHost, const Common::Error &aError) final;

    // Listen

    void ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Accept

    void ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Disconnect

    void ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef) final;
    void ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;
    void ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError) final;

    // Error

    void ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const Common::Error &aError) final;

    // Groups Controller Delegate Methods

    Common::Status ShouldAdjustVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment) final;
    Common::Status ShouldSetMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute) final;
    Common::Status ShouldSetSource(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier) final;
    Common::Status ShouldSetVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume) final;
    Common::Status ShouldToggleMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel) final;

    // Timer Trampoline

    static void TimerCallBack(CFRunLoopTimerRef aTimerRef, void *aContext);

private:
    class ShouldDoForGroupZonesFunctorBasis
    {
    protected:
        ShouldDoForGroupZonesFunctorBasis(ZonesController &aZonesController);

    public:
        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) = 0;

    protected:
        ZonesController &                           mZonesController;
    };

    class ShouldAdjustVolumeFunctor :
        public ShouldDoForGroupZonesFunctorBasis
    {
    public:
        ShouldAdjustVolumeFunctor(ZonesController &aZonesController, const Model::VolumeModel::LevelType &aAdjustment);

        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) final;

    private:
        const Model::VolumeModel::LevelType &      mAdjustment;
    };

    class ShouldSetMuteFunctor :
        public ShouldDoForGroupZonesFunctorBasis
    {
    public:
        ShouldSetMuteFunctor(ZonesController &aZonesController, const Model::VolumeModel::MuteType &aMute);

        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) final;

    private:
        const Model::VolumeModel::MuteType &        mMute;
    };

    class ShouldSetSourceFunctor :
        public ShouldDoForGroupZonesFunctorBasis
    {
    public:
        ShouldSetSourceFunctor(ZonesController &aZonesController, const Model::SourceModel::IdentifierType &aSourceIdentifier);

        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) final;

    private:
        const Model::SourceModel::IdentifierType &  mSourceIdentifier;
    };

    class ShouldSetVolumeFunctor :
        public ShouldDoForGroupZonesFunctorBasis
    {
    public:
        ShouldSetVolumeFunctor(ZonesController &aZonesController, const Model::VolumeModel::LevelType &aVolume);

        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) final;

    private:
        const Model::VolumeModel::LevelType &      mVolume;
    };

    class ShouldToggleMuteFunctor :
        public ShouldDoForGroupZonesFunctorBasis
    {
    public:
        ShouldToggleMuteFunctor(ZonesController &aZonesController);

        virtual Common::Status operator ()(const ZonesController::IdentifierType &aZoneIdentifier) final;
    };

    Common::Status ShouldDoForGroupZones(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, ShouldDoForGroupZonesFunctorBasis &aFunctorBasis);

private:
    Common::Status InitConnectionManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitCommandManager(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitControllers(const Common::RunLoopParameters &aRunLoopParameters);
    Common::Status InitConfiguration(const Common::RunLoopParameters &aRunLoopParameters, const boost::filesystem::path &aPath);

    void AddController(ControllerBasis &aController);

    void TimerCallBack(CFRunLoopTimerRef aTimerRef);

private:
    struct ControllerState
    {
        ControllerBasis * mController;
    };

    typedef std::map<ControllerBasis *, ControllerState> Controllers;

    // Sub-controller order is important since this is the order that
    // most closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command.

    Common::RunLoopParameters       mRunLoopParameters;
    boost::filesystem::path         mConfigurationPath;
    ConnectionManager               mConnectionManager;
    CommandManager                  mCommandManager;
    ConfigurationController         mConfigurationController;
    NetworkController               mNetworkController;
    FavoritesController             mFavoritesController;
    GroupsController                mGroupsController;
    FrontPanelController            mFrontPanelController;
    InfraredController              mInfraredController;
    EqualizerPresetsController      mEqualizerPresetsController;
    SourcesController               mSourcesController;
    ZonesController                 mZonesController;
    Controllers                     mControllers;
    ControllerDelegate *            mDelegate;
    void *                          mDelegateContext;
    CFRunLoopTimerRef               mConfigurationAutoSaveTimer;
    bool                            mConfigurationIsDirty;
};

}; // namespace Server

}; // namespace HLX

#endif // HLXSERVERCONTROLLER_HPP
