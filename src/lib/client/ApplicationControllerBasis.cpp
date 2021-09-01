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
 *      This file implements an object for...
 *
 */

#include "ApplicationControllerBasis.hpp"

#include <LogUtilities/LogUtilities.hpp>

#include <OpenHLX/Client/ApplicationControllerRefreshDelegate.hpp>
#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Utilities/Percentage.hpp>


using namespace HLX::Common;
using namespace HLX::Utilities;
using namespace Nuovations;


namespace HLX
{

namespace Client
{

namespace Application
{

// MARK: Client Controller Basis

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ControllerBasis :: ControllerBasis(void) :
    ClientObjectControllerContainer(),
    ObjectControllerBasisRefreshDelegate(),
    mConnectionManager(),
    mCommandManager(),
    mControllersDidRefreshCount(0),
    mRefreshDelegate(nullptr)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 */
ControllerBasis :: ~ControllerBasis(void)
{
    return;
}

// MARK: Initializer(s)

/**
 *  @brief
 *    This is a class initializer.
 *
 *  This initializes the client controller with the specified run loop
 *  parameters.
 *
 *  @param[in]  aRunLoopParameters  An immutable reference to the run
 *                                  loop parameters to initialize the
 *                                  client controller with.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENOMEM                  Resources could not be allocated.
 *  @retval  kStatus_ValueAlreadySet  If the client controller was already
 *                                    added as a delegate to the
 *                                    connection manager, command
 *                                    manager, or child controllers.
 *
 */
Status
ControllerBasis :: Init(const Common::RunLoopParameters &aRunLoopParameters)
{
    Status lRetval = kStatus_Success;


    lRetval = ClientObjectControllerContainer::Init();
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mConnectionManager.Init(aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

    lRetval = mCommandManager.Init(mConnectionManager, aRunLoopParameters);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Accessors

const Client::CommandManager &
ControllerBasis :: GetCommandManager(void) const
{
    return (mCommandManager);
}

Client::CommandManager &
ControllerBasis :: GetCommandManager(void)
{
    return (mCommandManager);
}

const Client::ConnectionManager &
ControllerBasis :: GetConnectionManager(void) const
{
    return (mConnectionManager);
}

Client::ConnectionManager &
ControllerBasis :: GetConnectionManager(void)
{
    return (mConnectionManager);
}

/**
 *  @brief
 *    Refresh the state of the client controller.
 *
 *  This should be called on first-time client start-up or whenever
 *  the client controller state needs to be forcibly refreshed.
 *
 *  This iterates through each of the sub-controllers, tasking each
 *  with taking care of the refresh activity appropriate for its scope
 *  of concern.
 *
 *  @retval  kStatus_Success              If successful.
 *  @retval  -ENOMEM                      If memory could not be allocated
 *                                        by a controller to perform the
 *                                        refresh.
 *
 */
Status
ControllerBasis :: Refresh(void)
{
    Status                lRetval = kStatus_Success;
    Controllers::iterator begin, end;

    if (mRefreshDelegate != nullptr)
    {
        mRefreshDelegate->ControllerWillRefresh(*this);
    }

    // Reset the overall refresh count.

    mControllersDidRefreshCount = 0;

    // Begin refreshing each controller.

    begin = GetControllers().begin();
    end = GetControllers().end();

    while (begin != end)
    {
        lRetval = begin->second.mController->Refresh();
        nlREQUIRE_SUCCESS(lRetval, done);

        begin++;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the controller is in the middle of a refresh.
 *
 *  This returns a Boolean indicating whether (true) or not (false)
 *  the controller is in the middle of a refresh operation with the
 *  peer server controller for up-to-date state.
 *
 *  @returns
 *    True if the controller is refreshing; otherwise, false.
 *
 */
bool
ControllerBasis :: IsRefreshing(void) const
{
    return (mControllersDidRefreshCount != GetControllers().size());
}

/**
 *  @brief
 *    Return the delegate for the client controller.
 *
 *  @returns
 *    A pointer to the delegate for the client controller.
 *
 */
Client::Application::ControllerRefreshDelegate *
ControllerBasis :: GetRefreshDelegate(void) const
{
    return (mRefreshDelegate);
}

/**
 *  @brief
 *    Set the delegate for the client controller.
 *
 *  This attempts to set a delegate for the client controller.
 *
 *  @param[in]  aRefreshDelegate  A pointer to the delegate to set.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  kStatus_ValueAlreadySet  If the delegate was already set to
 *                                    the specified value.
 *
 */
Status
ControllerBasis :: SetRefreshDelegate(Client::Application::ControllerRefreshDelegate *aRefreshDelegate)
{
    Status lRetval = kStatus_Success;

    if (aRefreshDelegate == mRefreshDelegate)
    {
        lRetval = kStatus_ValueAlreadySet;
        goto done;
    }

    mRefreshDelegate = aRefreshDelegate;

 done:
    return (lRetval);
}

// MARK: Connection Management

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the default timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL)
{
    Status lRetval = kStatus_Success;

    lRetval = Connect(aMaybeURL, kTimeoutDefault);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL with the provided timeout.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL,
                           const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Connect(aMaybeURL, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Connect to a HLX server peer.
 *
 *  This attempts to asynchronously connect to the HLX server peer at
 *  the specified URL, host name, or host name and port with the
 *  provided timeout using IPv4 or IPv6 resolved addresses as
 *  specified.
 *
 *  @param[in]  aMaybeURL  A pointer to a null-terminated C string
 *                         containing the URL, host name, or host name
 *                         and port of the HLX server peer to connect
 *                         to. The URL or host name may be a name to
 *                         be resolved or a literal IP address.
 *                         HLX server peer to connect to.
 *  @param[in]  aVersions  An immutable references to those IP address
 *                         versions that should be used for resolving
 *                         the host name.
 *  @param[in]  aTimeout   An immutable reference to the timeout by
 *                         which the connection should complete.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -EALREADY                The client connection is already
 *                                    connected.
 *  @retval  -EINPROGRESS             The client connection is in
 *                                    progress.
 *  @retval  -EINVAL                  The port number for the URL was
 *                                    invalid.
 *  @retval  -EPROTONOSUPPORT         The protocol scheme associated
 *                                    with the specified URL is not
 *                                    supported.
 *  @retval  kStatus_ValueAlreadySet  This manager is already the delegate
 *                                    for the connection.
 *
 */
Status
ControllerBasis :: Connect(const char *aMaybeURL,
                           const ConnectionManager::Versions &aVersions,
                           const Timeout &aTimeout)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Connect(aMaybeURL, aVersions, aTimeout);
    nlREQUIRE_SUCCESS(lRetval, done);

done:
    return (lRetval);
}

/**
 *  @brief
 *    Returns whether or not the client controller is connected to a
 *    peer.
 *
 *  @returns
 *    True if the client controller is connected to a peer; otherwise,
 *    false.
 *
 */
bool
ControllerBasis :: IsConnected(void) const
{
    return (GetConnectionManager().IsConnected());
}

/**
 *  @brief
 *    Disconnect from the HLX server peer.
 *
 *  This attempts to disconnect from the connected HLX server peer, if
 *  any.
 *
 *  @retval  kStatus_Success          If successful.
 *  @retval  -ENXIO                   If there is no allocated connection.
 *  @retval  -EALREADY                If the connection is already
 *                                    disconnected.
 *  @retval  -EINPROGRESS             If the connection is already in
 *                                    the process of disconnecting.
 *
 */
Status
ControllerBasis :: Disconnect(void)
{
    Status lRetval = kStatus_Success;

    lRetval = GetConnectionManager().Disconnect();
    nlEXPECT_SUCCESS(lRetval, done);

 done:
    return (lRetval);
}

// MARK: Controller Basis Refresh Delegate Methods

void
ControllerBasis :: ControllerIsRefreshing(Client::ObjectControllerBasis &aController, const uint8_t &aPercentComplete)
{
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = GetControllers().find(&aController);

    if (lControllerIterator != GetControllers().end())
    {
        static const Percentage kPercentCompletePerController    = CalculatePercentage(1,
                                                                                       static_cast<uint8_t>(GetControllers().size()));
        const Percentage        lOtherControllersPercentComplete = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                                       static_cast<uint8_t>(GetControllers().size()));
        const Percentage        lThisControllerPercentComplete   = ((kPercentCompletePerController * aPercentComplete) / 100);
        const Percentage        lPercentComplete                 = (lOtherControllersPercentComplete + lThisControllerPercentComplete);

        if (mRefreshDelegate != nullptr)
        {
            mRefreshDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }
    }
}

/**
 *  @brief
 *    Delegation from a controller that the specified controller is
 *    done refreshing.
 *
 *  On the refresh completion of any one controller, this refreshes
 *  the overall refresh state of the parent client controller.
 *
 *  @param[in]  aController       A reference to the controller
 *                                that issued the delegation.
 *
 */
void
ControllerBasis :: ControllerDidRefresh(Client::ObjectControllerBasis &aController)
{
    Controllers::const_iterator lControllerIterator;

    lControllerIterator = GetControllers().find(&aController);

    if (lControllerIterator != GetControllers().end())
    {
        mControllersDidRefreshCount++;

        if (mRefreshDelegate != nullptr)
        {
            const Percentage lPercentComplete = CalculatePercentage(static_cast<uint8_t>(mControllersDidRefreshCount),
                                                                    static_cast<uint8_t>(GetControllers().size()));

            mRefreshDelegate->ControllerIsRefreshing(*this, lPercentComplete);
        }

        if (mControllersDidRefreshCount == GetControllers().size())
        {
            // At this point, all controllers have asynchronously
            // completed their refresh requests. Before notifying the
            // delegate of that fact, derive any necessary group
            // state, dispatching state change notifications in the
            // process such that it appears to the delegate as though
            // that group state came with and was bookended by the
            // overall refresh request.

            DeriveGroupState();

            // Now that group state has been derived and state change
            // notifications dispatched, notify the client that the
            // refresh request is complete.

            if (mRefreshDelegate != nullptr)
            {
                mRefreshDelegate->ControllerDidRefresh(*this);
            }
        }
    }

    return;
}

}; // namespace Application

}; // namespace Client

}; // namespace HLX
