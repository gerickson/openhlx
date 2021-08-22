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
 *      This file implements an object for effecting a HLX simulated
 *      server controller.
 *
 */

#include "HLXSimulatorController.hpp"

#include <vector>

#include <errno.h>

#include <boost/filesystem.hpp>

#include <CFUtilities/CFString.hpp>
#include <CFUtilities/CFUtilities.hpp>
#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Utilities/Assert.hpp>


using namespace HLX::Common;
using namespace HLX::Model;
using namespace HLX::Server;
using namespace Nuovations;


namespace HLX
{

namespace Simulator
{

namespace Application
{

namespace Detail
{

typedef std::vector<ZoneModel::IdentifierType>   ZoneIdentifiers;

};

Controller :: Controller(void) :
    ConnectionManagerDelegate(),
    CommandManagerDelegate(),
    ConfigurationControllerDelegate(),
    Simulator::ControllerBasisDelegate(),
    GroupsControllerDelegate(),
    mRunLoopParameters(),
    mConfigurationPath(),
    mConnectionManager(),
    mCommandManager(),
    mConfigurationController(),
    mNetworkController(),
    mFavoritesController(),
    mGroupsController(),
    mFrontPanelController(),
    mInfraredController(),
    mEqualizerPresetsController(),
    mSourcesController(),
    mZonesController(),
    mControllers(),
    mDelegate(nullptr),
    mConfigurationAutoSaveTimer(nullptr),
    mConfigurationIsDirty(false)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
Controller :: ~Controller(void)
{
    DeclareScopedFunctionTracer(lTracer);

    if (mConfigurationAutoSaveTimer != nullptr)
    {
        CFRunLoopTimerInvalidate(mConfigurationAutoSaveTimer);

        CFRelease(mConfigurationAutoSaveTimer);

        mConfigurationAutoSaveTimer = nullptr;
    }
}

Status
Controller :: Init(const RunLoopParameters &aRunLoopParameters, const boost::filesystem::path &aConfigurationPath)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;


    // Initialize the connection manager

    lRetval = InitConnectionManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Initialize the command manager

    lRetval = InitCommandManager(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Initialize the controllers

    lRetval = InitControllers(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Bootstrap the server configuration settings and state

    lRetval = InitConfiguration(aRunLoopParameters, aConfigurationPath);
    nlREQUIRE_SUCCESS(lRetval, done);

    mRunLoopParameters = aRunLoopParameters;

done:
    return (lRetval);
}

Status
Controller :: InitConnectionManager(const RunLoopParameters &aRunLoopParameters)
{
    Status  lRetval;


    lRetval = mConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionManager.AddDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status Controller :: InitCommandManager(const RunLoopParameters &aRunLoopParameters)
{
    Status  lRetval;


    lRetval = mCommandManager.Init(mConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mCommandManager.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: InitControllers(const RunLoopParameters &aRunLoopParameters)
{
    Controllers::iterator  lCurrent, lEnd;
    Status                 lRetval;


    (void)aRunLoopParameters;

    // Place the various controllers into the controller
    // container. Order is important since this is the order that most
    // closely matches the order in which the actual HLX hardware
    // responds to for the 'query current configuration' command.

    AddController(mConfigurationController);
    AddController(mNetworkController);
    AddController(mFavoritesController);
    AddController(mGroupsController);
    AddController(mFrontPanelController);
    AddController(mInfraredController);
    AddController(mEqualizerPresetsController);
    AddController(mSourcesController);
    AddController(mZonesController);

    // Explicitly handle the configuration controller, since it has a
    // unique initialization signature and delegate override.

    lRetval = mConfigurationController.Init(mCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Explicitly set this parent controller to be the delegate for
    // fanout of any configuration controller delegations.

    lRetval = mConfigurationController.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Explicitly handle the groups controller, since it has a
    // unique initialization signature and delegate override.

    lRetval = mGroupsController.Init(mCommandManager);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Explicitly set this parent controller to be the delegate for
    // fanout of any groups controller delegations.

    lRetval = mGroupsController.SetDelegate(this);
    nlREQUIRE_SUCCESS(lRetval, done);

    // Intialize the controllers (skipping the configuration
    // controller as we just handled that).

    lCurrent = mControllers.begin();
    lEnd     = mControllers.end();

    while (lCurrent != lEnd)
    {
        // Skip the configuration and groups controllers, since those
        // were already handled above.

        if ((lCurrent->second.mController != &mConfigurationController) &&
            (lCurrent->second.mController != &mGroupsController))
        {
            lRetval = lCurrent->second.mController->Init(mCommandManager);
            nlREQUIRE_SUCCESS(lRetval, done);
        }

        // Unconditionally set the delegate (including the
        // configuration and groups controllers since here, we are
        // handling their upcast class).

        lRetval = lCurrent->second.mController->SetDelegate(this);
        nlREQUIRE_SUCCESS(lRetval, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

Status
Controller :: InitConfiguration(const RunLoopParameters &aRunLoopParameters, const boost::filesystem::path &aPath)
{
    DeclareScopedFunctionTracer(lTracer);
    Status  lRetval;


    // Fail fast: at this point, at least attempt to validate that the
    // path components up to the configuration file exist and will
    // allowing reading (load) or writing (save) the configuration
    // data.

    mConfigurationPath = aPath;

    // Attempt to load the backup configuration.
    //
    // We will be called back on LoadFromBackupConfigurationStorage
    // where there exists an opportunity to do input from stable
    // storage, based on the path that is specified.

    lRetval = mConfigurationController.LoadFromBackup();

    // If that fails, reset to defaults and then save the backup
    // configuration.

    if (lRetval != kStatus_Success)
    {
        lRetval = mConfigurationController.ResetToDefaults();
        nlREQUIRE_SUCCESS(lRetval, done);

        lRetval = mConfigurationController.SaveToBackup();
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    // Establish the backup configuration auto-save timer.

    {
        static const CFOptionFlags  lFlags = 0;
        static const CFIndex        lOrder = 0;
        const CFTimeInterval        lAutoSaveIntervalSeconds = 30;
        const CFAbsoluteTime        lAutoSaveFirstFireDate = CFAbsoluteTimeGetCurrent() + lAutoSaveIntervalSeconds;
        CFRunLoopTimerContext       lTimerContext = { 0, this, 0, 0, 0 };


        mConfigurationAutoSaveTimer = CFRunLoopTimerCreate(kCFAllocatorDefault,
                                              lAutoSaveFirstFireDate,
                                              lAutoSaveIntervalSeconds,
                                              lFlags,
                                              lOrder,
                                              TimerCallBack,
                                              &lTimerContext);
        nlREQUIRE_ACTION(mConfigurationAutoSaveTimer != nullptr, done, lRetval = -ENOMEM);

        CFRunLoopAddTimer(aRunLoopParameters.GetRunLoop(),
                          mConfigurationAutoSaveTimer,
                          aRunLoopParameters.GetRunLoopMode());
    }

    mConfigurationIsDirty = false;

 done:
    return (lRetval);
}

void
Controller :: AddController(Simulator::ControllerBasis &aController)
{
    const ControllerState lControllerState = { &aController };


    mControllers[&aController] = lControllerState;

    return;
}

Status Controller :: Listen(void)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mConnectionManager.Listen();
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
Controller :: Listen(const Common::ConnectionManagerBasis::Versions &aVersions)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mConnectionManager.Listen(aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

Status
Controller :: Listen(const char *aMaybeURL)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mConnectionManager.Listen(aMaybeURL);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

Status
Controller :: Listen(const char *aMaybeURL, const Common::ConnectionManagerBasis::Versions &aVersions)
{
    DeclareScopedFunctionTracer(lTracer);
    Status lRetval = kStatus_Success;

    lRetval = mConnectionManager.Listen(aMaybeURL, aVersions);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

ControllerDelegate *
Controller :: GetDelegate(void) const
{
    return (mDelegate);
}

Status
Controller :: SetDelegate(ControllerDelegate *aDelegate)
{
    Status lRetval = kStatus_Success;

    if (aDelegate == mDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mDelegate        = aDelegate;

 done:
    return (lRetval);
}

Status Controller :: RegisterRequestHandler(Server::Command::RequestBasis &aRequest, void *aContext, CommandManager::OnRequestReceivedFunc aOnRequestReceivedHandler)
{
    return (mCommandManager.RegisterRequestHandler(aRequest, aContext, aOnRequestReceivedHandler));
}

// MARK: Configuration Controller Delegate Methods

Status Controller :: LoadFromBackupConfiguration(ConfigurationController &aController, CFDictionaryRef aBackupDictionary)
{
    DeclareScopedFunctionTracer(lTracer);
    Controllers::iterator lCurrent;
    Controllers::iterator lLast;
    Status                lRetval = kStatus_Success;


    (void)aController;

    nlREQUIRE_ACTION(aBackupDictionary != nullptr, done, lRetval = -EINVAL);

    lCurrent = mControllers.begin();
    lLast = mControllers.end();

    while (lCurrent != lLast)
    {
        lRetval = lCurrent->second.mController->LoadFromBackupConfiguration(aBackupDictionary);
        nlREQUIRE(lRetval >= kStatus_Success, done);

        lCurrent++;
    }

 done:
    return (lRetval);
}

Status Controller :: LoadFromBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef &aBackupDictionary)
{
    const CFPropertyListMutabilityOptions kMutability   = kCFPropertyListImmutable;
    CFPropertyListRef                     lPropertyList;
    CFStringRef                           lError = nullptr;
    bool                                  lStatus;
    Status                                lRetval = kStatus_Success;


    (void)aController;

    // Attempt to load the backup configuration from file at the
    // initialized path. It is entirely possible that the file is not
    // there or is empty, so expect that failure is likely to occur
    // here.

    lStatus = CFUPropertyListReadFromFile(mConfigurationPath.c_str(),
                                          kMutability,
                                          &lPropertyList,
                                          &lError);
    nlEXPECT(lStatus, done);

    // At this point, the file exists, was read, and was successfully
    // parsed into property list data. Cast the data into the
    // immutable backup dictionary.

    aBackupDictionary = static_cast<CFDictionaryRef>(lPropertyList);

 done:
    if (lStatus != true)
    {
        lRetval = -EIO;

        if (lError != nullptr)
        {
            Log::Error().Write("Failed to load configuration from '%s': %s\n",
                               mConfigurationPath.c_str(),
                               CFString(lError).GetCString());

            CFURelease(lError);
        }
    }

    return (lRetval);
}

void Controller :: QueryCurrentConfiguration(ConfigurationController &aController, Server::ConnectionBasis &aConnection, ConnectionBuffer::MutableCountedPointer &aBuffer)
{
    Controllers::iterator lCurrent;
    Controllers::iterator lLast;


    (void)aController;

    lCurrent = mControllers.begin();
    lLast = mControllers.end();

    while (lCurrent != lLast)
    {
        lCurrent->second.mController->QueryCurrentConfiguration(aConnection, aBuffer);

        lCurrent++;
    }
}

void Controller :: ResetToDefaultConfiguration(ConfigurationController &aController)
{
    Controllers::iterator lCurrent;
    Controllers::iterator lLast;


    (void)aController;

    lCurrent = mControllers.begin();
    lLast = mControllers.end();

    while (lCurrent != lLast)
    {
        lCurrent->second.mController->ResetToDefaultConfiguration();

        lCurrent++;
    }
}

void Controller :: SaveToBackupConfiguration(ConfigurationController &aController, CFMutableDictionaryRef aBackupDictionary)
{
    DeclareScopedFunctionTracer(lTracer);
    Controllers::iterator lCurrent;
    Controllers::iterator lLast;


    (void)aController;

    // First, allow all controllers to serialize their configuration
    // into the back-up dictionary.

    lCurrent = mControllers.begin();
    lLast = mControllers.end();

    while (lCurrent != lLast)
    {
        lCurrent->second.mController->SaveToBackupConfiguration(aBackupDictionary);

        lCurrent++;
    }
}

Status Controller :: SaveToBackupConfigurationStorage(ConfigurationController &aController, CFDictionaryRef aBackupDictionary)
{
    const CFPropertyListFormat kFormat       = kCFPropertyListBinaryFormat_v1_0;
    const bool                 kWritable     = true;
    CFPropertyListRef          lPropertyList = aBackupDictionary;
    CFStringRef                lError        = nullptr;
    bool                       lStatus;
    Status                     lRetval       = kStatus_Success;


    (void)aController;

    lStatus = CFUPropertyListWriteToFile(mConfigurationPath.c_str(),
                                         kWritable,
                                         kFormat,
                                         lPropertyList,
                                         &lError);
    nlREQUIRE(lStatus == true, done);

 done:
    if (lStatus != true)
    {
        lRetval = -EIO;

        if (lError != nullptr)
        {
            Log::Error().Write("Failed to save configuration to '%s': %s\n",
                               mConfigurationPath.c_str(),
                               CFString(lError).GetCString());

            CFURelease(lError);
        }
    }

    return (lRetval);
}

// MARK: Controller Delegate Methods

void Controller :: ControllerConfigurationIsDirty(Simulator::ControllerBasis &aController)
{
    (void)aController;

    mConfigurationIsDirty = true;
}

// MARK: Connection Manager Delegate Methods

// Resolve

void Controller :: ConnectionManagerWillResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillResolve(*this, aHost);
    }
}

void Controller :: ConnectionManagerIsResolving(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsResolving(*this, aHost);
    }
}

void Controller :: ConnectionManagerDidResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::IPAddress &aIPAddress)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidResolve(*this, aHost, aIPAddress);
    }
}

void Controller :: ConnectionManagerDidNotResolve(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const char *aHost, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotResolve(*this, aHost, aError);
    }
}

// Listen

void Controller :: ConnectionManagerWillListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillListen(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerIsListening(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsListening(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidListen(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidNotListen(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotListen(*this, aURLRef, aError);
    }
}

// Accept

void Controller :: ConnectionManagerWillAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillAccept(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerIsAccepting(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerIsAccepting(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidAccept(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidNotAccept(ConnectionManager &aConnectionManager, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotAccept(*this, aURLRef, aError);
    }
}

// Disconnect

void Controller :: ConnectionManagerWillDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerWillDisconnect(*this, aURLRef);
    }
}

void Controller :: ConnectionManagerDidDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidDisconnect(*this, aURLRef, aError);
    }
}

void Controller :: ConnectionManagerDidNotDisconnect(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, CFURLRef aURLRef, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerDidNotDisconnect(*this, aURLRef, aError);
    }
}

// Error

void Controller :: ConnectionManagerError(Common::ConnectionManagerBasis &aConnectionManager, const ConnectionManagerBasis::Roles &aRoles, const Common::Error &aError)
{
    (void)aConnectionManager;
    (void)aRoles;

    if (mDelegate != nullptr)
    {
        mDelegate->ControllerError(*this, aError);
    }
}

// MARK: Groups Controller Delegate Methods

Controller :: ShouldDoForGroupZonesFunctorBasis :: ShouldDoForGroupZonesFunctorBasis(ZonesController &aZonesController) :
    mZonesController(aZonesController)
{
    return;
}

Controller :: ShouldAdjustVolumeFunctor :: ShouldAdjustVolumeFunctor(ZonesController &aZonesController, const Model::VolumeModel::LevelType &aAdjustment) :
    ShouldDoForGroupZonesFunctorBasis(aZonesController),
    mAdjustment(aAdjustment)
{
    return;
}

Common::Status Controller :: ShouldAdjustVolumeFunctor :: operator ()(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    VolumeModel::LevelType       lVolume;
    Status                        lRetval;

    // At some point, an decrease or increase in volume may
    // desaturate or saturate at the lower and upper
    // limits. Ignore such errors.

    lRetval = mZonesController.AdjustVolume(aZoneIdentifier, mAdjustment, lVolume);
    nlREQUIRE(lRetval >= kStatus_Success || lRetval == -ERANGE, done);

 done:
    if (lRetval == -ERANGE)
    {
        lRetval = kStatus_ValueAlreadySet;
    }

    return (lRetval);
}

Controller :: ShouldSetMuteFunctor :: ShouldSetMuteFunctor(ZonesController &aZonesController, const Model::VolumeModel::MuteType &aMute) :
    ShouldDoForGroupZonesFunctorBasis(aZonesController),
    mMute(aMute)
{
    return;
}

Status Controller :: ShouldSetMuteFunctor :: operator ()(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status                        lRetval;

    lRetval = mZonesController.SetMute(aZoneIdentifier, mMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Controller :: ShouldSetSourceFunctor :: ShouldSetSourceFunctor(ZonesController &aZonesController, const Model::SourceModel::IdentifierType &aSourceIdentifier) :
    ShouldDoForGroupZonesFunctorBasis(aZonesController),
    mSourceIdentifier(aSourceIdentifier)
{
    return;
}

Status Controller :: ShouldSetSourceFunctor :: operator ()(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status                        lRetval;

    lRetval = mZonesController.SetSource(aZoneIdentifier, mSourceIdentifier);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Controller :: ShouldSetVolumeFunctor :: ShouldSetVolumeFunctor(ZonesController &aZonesController, const Model::VolumeModel::LevelType &aVolume) :
    ShouldDoForGroupZonesFunctorBasis(aZonesController),
    mVolume(aVolume)
{
    return;
}

Status Controller :: ShouldSetVolumeFunctor :: operator ()(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    Status                        lRetval;

    lRetval = mZonesController.SetVolume(aZoneIdentifier, mVolume);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Controller :: ShouldToggleMuteFunctor :: ShouldToggleMuteFunctor(ZonesController &aZonesController) :
    ShouldDoForGroupZonesFunctorBasis(aZonesController)
{
    return;
}

Status Controller :: ShouldToggleMuteFunctor :: operator ()(const Model::ZoneModel::IdentifierType &aZoneIdentifier)
{
    VolumeModel::MuteType         lMute;
    Status                        lRetval;

    lRetval = mZonesController.ToggleMute(aZoneIdentifier, lMute);
    nlREQUIRE(lRetval >= kStatus_Success, done);

 done:
    return (lRetval);
}

Status Controller :: ShouldDoForGroupZones(const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, ShouldDoForGroupZonesFunctorBasis &aFunctorBasis)
{
    size_t  lZoneCount;
    Status  lRetval;


    (void)aGroupIdentifier;

    lRetval = aGroupModel.GetZones(lZoneCount);
    nlEXPECT_SUCCESS(lRetval, done);

    if (lZoneCount > 0)
    {
        Detail::ZoneIdentifiers                  lZoneIdentifiers;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierCurrent;
        Detail::ZoneIdentifiers::const_iterator  lZoneIdentifierEnd;

        lZoneIdentifiers.resize(lZoneCount);

        lRetval = aGroupModel.GetZones(&lZoneIdentifiers[0], lZoneCount);
        nlREQUIRE_SUCCESS(lRetval, done);

        lZoneIdentifierCurrent = lZoneIdentifiers.begin();
        lZoneIdentifierEnd     = lZoneIdentifiers.end();

        while (lZoneIdentifierCurrent != lZoneIdentifierEnd)
        {
            lRetval = aFunctorBasis(*lZoneIdentifierCurrent);
            nlREQUIRE(lRetval >= kStatus_Success, done);

            lZoneIdentifierCurrent++;
        }
    }

 done:
    return (lRetval);
}

Status Controller :: ShouldAdjustVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aAdjustment)
{
    ShouldAdjustVolumeFunctor lShouldAdjustVolumeFunctor(mZonesController, aAdjustment);


    (void)aController;

    return (ShouldDoForGroupZones(aGroupIdentifier, aGroupModel, lShouldAdjustVolumeFunctor));
}

Status Controller :: ShouldSetMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::MuteType &aMute)
{
    ShouldSetMuteFunctor lShouldSetMuteFunctor(mZonesController, aMute);


    (void)aController;

    return (ShouldDoForGroupZones(aGroupIdentifier, aGroupModel, lShouldSetMuteFunctor));
}

Status Controller :: ShouldSetSource(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::SourceModel::IdentifierType &aSourceIdentifier)
{
    ShouldSetSourceFunctor lShouldSetSourceFunctor(mZonesController, aSourceIdentifier);


    (void)aController;

    return (ShouldDoForGroupZones(aGroupIdentifier, aGroupModel, lShouldSetSourceFunctor));
}

Status Controller :: ShouldSetVolume(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel, const Model::VolumeModel::LevelType &aVolume)
{
    ShouldSetVolumeFunctor lShouldSetVolumeFunctor(mZonesController, aVolume);


    (void)aController;

    return (ShouldDoForGroupZones(aGroupIdentifier, aGroupModel, lShouldSetVolumeFunctor));
}

Status Controller :: ShouldToggleMute(GroupsController &aController, const Model::GroupModel::IdentifierType &aGroupIdentifier, const Model::GroupModel &aGroupModel)
{
    ShouldToggleMuteFunctor lShouldToggleMuteFunctor(mZonesController);


    (void)aController;

    return (ShouldDoForGroupZones(aGroupIdentifier, aGroupModel, lShouldToggleMuteFunctor));
}

void Controller :: TimerCallBack(CFRunLoopTimerRef aTimerRef)
{
    Status lStatus;

    if (aTimerRef == mConfigurationAutoSaveTimer)
    {
        Log::Debug().Write("Auto-save timer fired!\n");

        if (mConfigurationIsDirty)
        {
            lStatus = mConfigurationController.SaveToBackup();
            nlREQUIRE_SUCCESS(lStatus, done);

            mConfigurationIsDirty = false;
        }
    }

 done:
    return;
}

void Controller :: TimerCallBack(CFRunLoopTimerRef aTimerRef, void *aContext)
{
    Controller *lController = static_cast<Controller *>(aContext);

    if (lController != nullptr)
    {
        lController->TimerCallBack(aTimerRef);
    }
}

}; // namespace Application

}; // namespace Simulator

}; // namespace HLX
