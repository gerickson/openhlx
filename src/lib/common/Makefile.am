#
#    Copyright (c) 2020-2021 Grant Erickson
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
#      This file is the makefile for the OpenHLX common, shared
#      implementation (that is, for both clients and servers)
#      library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                                               = \
    libopenhlx-common.a                                       \
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
# libopenhlx_common_a_include_HEADERS will get installed in
# libopenhlx_common_a_includedir.

libopenhlx_common_a_includedir                              = \
    ${includedir}/OpenHLX/Common

# Public library headers to distribute and install.

libopenhlx_common_a_include_HEADERS                         = \
    ApplicationControllerBasis.hpp                            \
    ApplicationControllerErrorDelegate.hpp                    \
    ApplicationObjectControllerContainerTemplate.hpp          \
    CommandBalanceBufferBasis.hpp                             \
    CommandBuffer.hpp                                         \
    CommandBufferBasis.hpp                                    \
    CommandConfigurationRegularExpressionBases.hpp            \
    CommandDelimitedBuffer.hpp                                \
    CommandDelimitedRegularExpression.hpp                     \
    CommandDelimiters.hpp                                     \
    CommandEqualizerBufferBases.hpp                           \
    CommandEqualizerPresetsRegularExpressionBases.hpp         \
    CommandFavoritesRegularExpressionBases.hpp                \
    CommandFrontPanelBufferBases.hpp                          \
    CommandFrontPanelRegularExpressionBases.hpp               \
    CommandGroupsRegularExpressionBases.hpp                   \
    CommandInfraredBufferBases.hpp                            \
    CommandInfraredRegularExpressionBases.hpp                 \
    CommandNameSetBufferBasis.hpp                             \
    CommandNetworkRegularExpressionBases.hpp                  \
    CommandPropertyBufferBases.hpp                            \
    CommandQueryBufferBasis.hpp                               \
    CommandRegularExpression.hpp                              \
    CommandRegularExpressionBasis.hpp                         \
    CommandRole.hpp                                           \
    CommandRoleDelimitedBuffer.hpp                            \
    CommandRoleDelimitedRegularExpression.hpp                 \
    CommandRoleDelimiters.hpp                                 \
    CommandSourceBufferBases.hpp                              \
    CommandSourcesRegularExpressionBases.hpp                  \
    CommandToneBufferBasis.hpp                                \
    CommandVolumeBufferBases.hpp                              \
    CommandZonesRegularExpressionBases.hpp                    \
    ConfigurationControllerBasis.hpp                          \
    ConnectionBasis.hpp                                       \
    ConnectionBuffer.hpp                                      \
    ConnectionManagerApplicationDataDelegate.hpp              \
    ConnectionManagerBasis.hpp                                \
    ConnectionManagerDelegateBasis.hpp                        \
    EqualizerPresetsControllerBasis.hpp                       \
    Errors.hpp                                                \
    FavoritesControllerBasis.hpp                              \
    FrontPanelControllerBasis.hpp                             \
    GroupsControllerBasis.hpp                                 \
    HostURL.hpp                                               \
    HostURLAddress.hpp                                        \
    InfraredControllerBasis.hpp                               \
    IPAddress.hpp                                             \
    NetworkControllerBasis.hpp                                \
    OutputStringStream.hpp                                    \
    RegularExpression.hpp                                     \
    RunLoopParameters.hpp                                     \
    RunLoopQueue.hpp                                          \
    RunLoopQueueDelegate.hpp                                  \
    SocketAddress.hpp                                         \
    SourcesControllerBasis.hpp                                \
    Timeout.hpp                                               \
    Timer.hpp                                                 \
    TimerDelegate.hpp                                         \
    Version.hpp                                               \
    ZonesControllerBasis.hpp                                  \
    $(NULL)

libopenhlx_common_a_CPPFLAGS                                = \
    -I$(top_srcdir)/third_party/CFUtilities/repo/include      \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include     \
    -I$(top_srcdir)/third_party/libtelnet/repo                \
    -I$(top_srcdir)/src/lib/model                             \
    -I$(top_srcdir)/src/lib/utilities                         \
    $(NULL)

libopenhlx_common_a_SOURCES                                 = \
    ApplicationControllerBasis.cpp                            \
    CommandBalanceBufferBasis.cpp                             \
    CommandBuffer.cpp                                         \
    CommandConfigurationRegularExpressionBases.cpp            \
    CommandDelimitedBuffer.cpp                                \
    CommandDelimitedRegularExpression.cpp                     \
    CommandEqualizerBufferBases.cpp                           \
    CommandEqualizerPresetsRegularExpressionBases.cpp         \
    CommandFavoritesRegularExpressionBases.cpp                \
    CommandFrontPanelBufferBases.cpp                          \
    CommandFrontPanelRegularExpressionBases.cpp               \
    CommandGroupsRegularExpressionBases.cpp                   \
    CommandInfraredBufferBases.cpp                            \
    CommandInfraredRegularExpressionBases.cpp                 \
    CommandNameSetBufferBasis.cpp                             \
    CommandNetworkRegularExpressionBases.cpp                  \
    CommandPropertyBufferBases.cpp                            \
    CommandQueryBufferBasis.cpp                               \
    CommandRegularExpression.cpp                              \
    CommandRoleDelimitedBuffer.cpp                            \
    CommandRoleDelimitedRegularExpression.cpp                 \
    CommandRoleDelimiters.cpp                                 \
    CommandSourceBufferBases.cpp                              \
    CommandSourcesRegularExpressionBases.cpp                  \
    CommandToneBufferBasis.cpp                                \
    CommandVolumeBufferBases.cpp                              \
    CommandZonesRegularExpressionBases.cpp                    \
    ConfigurationControllerBasis.cpp                          \
    ConnectionBasis.cpp                                       \
    ConnectionBuffer.cpp                                      \
    ConnectionManagerBasis.cpp                                \
    EqualizerPresetsControllerBasis.cpp                       \
    FavoritesControllerBasis.cpp                              \
    FrontPanelControllerBasis.cpp                             \
    GroupsControllerBasis.cpp                                 \
    HostURL.cpp                                               \
    HostURLAddress.cpp                                        \
    InfraredControllerBasis.cpp                               \
    IPAddress.cpp                                             \
    NetworkControllerBasis.cpp                                \
    OutputStringStream.cpp                                    \
    RegularExpression.cpp                                     \
    RunLoopParameters.cpp                                     \
    RunLoopQueue.cpp                                          \
    SocketAddress.cpp                                         \
    SourcesControllerBasis.cpp                                \
    Timeout.cpp                                               \
    Timer.cpp                                                 \
    Version.cpp                                               \
    ZonesControllerBasis.cpp                                  \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
