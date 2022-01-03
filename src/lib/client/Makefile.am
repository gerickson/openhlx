#
#    Copyright (c) 2019-2022 Grant Erickson
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing,
#    software distributed under the License is distributed on an "AS
#    IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
#    express or implied.  See the License for the specific language
#    governing permissions and limitations under the License.
#

#
#    Description:
#      This file is the makefile for the OpenHLX client library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                                               = \
    libopenhlx-client.a                                       \
    $(NULL)

# Ensure that libraries are built before trying to run any recursive
# targets from SUBDIRS that might depend on them.

AM_RECURSIVE_TARGETS: $(LIBRARIES)

SUBDIRS                                                     = \
    tests                                                     \
    $(NULL)

noinst_HEADERS                                              = \
    $(NULL)

# The 'install' target directory transform. Headers in
# libopenhlx_client_a_include_HEADERS will get installed in
# libopenhlx_client_a_includedir.

libopenhlx_client_a_includedir                              = \
    ${includedir}/OpenHLX/Client

# Public library headers to distribute and install.

libopenhlx_client_a_include_HEADERS                         = \
    ApplicationController.hpp                                 \
    ApplicationControllerBasis.hpp                            \
    ApplicationControllerDelegate.hpp                         \
    ApplicationControllerRefreshDelegate.hpp                  \
    ApplicationControllerStateChangeDelegate.hpp              \
    CommandBalanceRequestBases.hpp                            \
    CommandEqualizerBandRequestBases.hpp                      \
    CommandErrorResponse.hpp                                  \
    CommandExchangeBasis.hpp                                  \
    CommandManager.hpp                                        \
    CommandManagerDelegate.hpp                                \
    CommandNameSetRequestBasis.hpp                            \
    CommandPropertyRequestBases.hpp                           \
    CommandQueryRequestBasis.hpp                              \
    CommandRequestBasis.hpp                                   \
    CommandResponseBasis.hpp                                  \
    CommandSourceRequestBases.hpp                             \
    CommandVolumeRequestBases.hpp                             \
    ConfigurationController.hpp                               \
    ConfigurationControllerBasis.hpp                          \
    ConfigurationControllerCommands.hpp                       \
    ConfigurationStateChangeNotifications.hpp                 \
    ConnectionBasis.hpp                                       \
    ConnectionBasisDelegate.hpp                               \
    ConnectionFactory.hpp                                     \
    ConnectionManager.hpp                                     \
    ConnectionManagerDelegate.hpp                             \
    ConnectionTelnet.hpp                                      \
    EqualizerBandStateChangeNotificationBasis.hpp             \
    EqualizerPresetsController.hpp                            \
    EqualizerPresetsControllerBasis.hpp                       \
    EqualizerPresetsControllerCommands.hpp                    \
    EqualizerPresetsStateChangeNotifications.hpp              \
    FavoritesController.hpp                                   \
    FavoritesControllerBasis.hpp                              \
    FavoritesControllerCommands.hpp                           \
    FavoritesStateChangeNotifications.hpp                     \
    FrontPanelController.hpp                                  \
    FrontPanelControllerBasis.hpp                             \
    FrontPanelControllerCommands.hpp                          \
    FrontPanelStateChangeNotifications.hpp                    \
    GroupsController.hpp                                      \
    GroupsControllerBasis.hpp                                 \
    GroupsControllerCommands.hpp                              \
    GroupsStateChangeNotifications.hpp                        \
    IdentifierStateChangeNotificationBasis.hpp                \
    InfraredController.hpp                                    \
    InfraredControllerBasis.hpp                               \
    InfraredControllerCommands.hpp                            \
    InfraredStateChangeNotifications.hpp                      \
    NameStateChangeNotificationBasis.hpp                      \
    NetworkController.hpp                                     \
    NetworkControllerBasis.hpp                                \
    NetworkControllerCommands.hpp                             \
    NetworkStateChangeNotifications.hpp                       \
    ObjectControllerBasis.hpp                                 \
    ObjectControllerBasisDelegate.hpp                         \
    ObjectControllerBasisErrorDelegate.hpp                    \
    ObjectControllerBasisRefreshDelegate.hpp                  \
    ObjectControllerBasisStateChangeDelegate.hpp              \
    SourceStateChangeNotificationBasis.hpp                    \
    SourcesController.hpp                                     \
    SourcesControllerBasis.hpp                                \
    SourcesControllerCommands.hpp                             \
    SourcesStateChangeNotifications.hpp                       \
    StateChangeNotificationBasis.hpp                          \
    StateChangeNotificationTypes.hpp                          \
    VolumeStateChangeNotificationBasis.hpp                    \
    ZonesController.hpp                                       \
    ZonesControllerBasis.hpp                                  \
    ZonesControllerCommands.hpp                               \
    ZonesStateChangeNotifications.hpp                         \
    $(NULL)

libopenhlx_client_a_CPPFLAGS                                = \
    -I$(top_srcdir)/third_party/CFUtilities/repo/include      \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include     \
    -I$(top_srcdir)/third_party/libtelnet/repo                \
    -I$(top_srcdir)/src/lib/common                            \
    -I$(top_srcdir)/src/lib/model                             \
    -I$(top_srcdir)/src/lib/utilities                         \
    $(NULL)

libopenhlx_client_a_SOURCES                                 = \
    ApplicationController.cpp                                 \
    ApplicationControllerBasis.cpp                            \
    CommandBalanceRequestBases.cpp                            \
    CommandEqualizerBandRequestBases.cpp                      \
    CommandErrorResponse.cpp                                  \
    CommandExchangeBasis.cpp                                  \
    CommandManager.cpp                                        \
    CommandNameSetRequestBasis.cpp                            \
    CommandPropertyRequestBases.cpp                           \
    CommandQueryRequestBasis.cpp                              \
    CommandRequestBasis.cpp                                   \
    CommandResponseBasis.cpp                                  \
    CommandSourceRequestBases.cpp                             \
    CommandVolumeRequestBases.cpp                             \
    ConfigurationController.cpp                               \
    ConfigurationControllerBasis.cpp                          \
    ConfigurationControllerCommands.cpp                       \
    ConfigurationStateChangeNotifications.cpp                 \
    ConnectionBasis.cpp                                       \
    ConnectionFactory.cpp                                     \
    ConnectionManager.cpp                                     \
    ConnectionTelnet.cpp                                      \
    EqualizerBandStateChangeNotificationBasis.cpp             \
    EqualizerPresetsController.cpp                            \
    EqualizerPresetsControllerBasis.cpp                       \
    EqualizerPresetsControllerCommands.cpp                    \
    EqualizerPresetsStateChangeNotifications.cpp              \
    FavoritesController.cpp                                   \
    FavoritesControllerBasis.cpp                              \
    FavoritesControllerCommands.cpp                           \
    FavoritesStateChangeNotifications.cpp                     \
    FrontPanelController.cpp                                  \
    FrontPanelControllerBasis.cpp                             \
    FrontPanelControllerCommands.cpp                          \
    FrontPanelStateChangeNotifications.cpp                    \
    GroupsController.cpp                                      \
    GroupsControllerBasis.cpp                                 \
    GroupsControllerCommands.cpp                              \
    GroupsStateChangeNotifications.cpp                        \
    IdentifierStateChangeNotificationBasis.cpp                \
    InfraredController.cpp                                    \
    InfraredControllerBasis.cpp                               \
    InfraredControllerCommands.cpp                            \
    InfraredStateChangeNotifications.cpp                      \
    NameStateChangeNotificationBasis.cpp                      \
    NetworkController.cpp                                     \
    NetworkControllerBasis.cpp                                \
    NetworkControllerCommands.cpp                             \
    NetworkStateChangeNotifications.cpp                       \
    ObjectControllerBasis.cpp                                 \
    SourceStateChangeNotificationBasis.cpp                    \
    SourcesController.cpp                                     \
    SourcesControllerBasis.cpp                                \
    SourcesControllerCommands.cpp                             \
    SourcesStateChangeNotifications.cpp                       \
    StateChangeNotificationBasis.cpp                          \
    VolumeStateChangeNotificationBasis.cpp                    \
    ZonesController.cpp                                       \
    ZonesControllerBasis.cpp                                  \
    ZonesControllerCommands.cpp                               \
    ZonesStateChangeNotifications.cpp                         \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
